/*
    FB01 : Sound editor
    Copyright Meslin Frédéric 2009 - 2010
    fredericmeslin@hotmail.com

    This file is part of FB01 SE

    FB01 SE is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    FB01 SE is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FB01 SE.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "midi.h"

//Identifiants et informations drivers
int MIDI::indIn = -1, MIDI::indOut = -1, MIDI::indCtrl = -1;
uint MIDI::nbIns = 0; uint MIDI::nbOuts = 0;
MIDI::DriversStr MIDI::ins[MIDI_MAX_DRIVERS];
MIDI::DriversStr MIDI::outs[MIDI_MAX_DRIVERS];

//Parametres du module
uchar MIDI::midiChannel = 0;
uchar MIDI::velocity    = 127;
uchar MIDI::sysChannel  = 0;

//Relai données MIDI
bool  MIDI::relaiIN   = true;
bool  MIDI::relaiCTRL = true;

//Gestion du tampon de réception
uchar MIDI::tampon[MIDI_LEN_TAMPON];
bool MIDI::attente = false;
bool MIDI::prepare = false;

//Spécificités Windows
#ifdef WIN32
    MIDIHDR MIDI::header;
    #define hndIn   (ins[indIn].handleNum)
    #define hndCtrl (ins[indCtrl].handleNum)
    #define hndOut  (outs[indOut].handleNum)
#endif

//Spécificités Linux
#ifdef LINUX
    #define hndIn   ((snd_rawmidi_t *) ins[indIn].handleStruct)
    #define hndCtrl ((snd_rawmidi_t *) ins[indCtrl].handleStruct)
    #define hndOut  ((snd_rawmidi_t *) outs[indOut].handleStruct)
    pthread_t MIDI::threadIn, MIDI::threadCtrl;
    bool MIDI::runIn = false, MIDI::runCtrl = false;
#endif

/*****************************************************************************/
void MIDI::EnumererDrivers()
{
//Libère les anciens drivers
    LibererDrivers();
//Alloue les nouveaux
#ifdef WIN32
//Alloue les entrées MIDI
    uint n = midiInGetNumDevs();
    for (uint i = 0; i < n; i++) {
        MIDIINCAPS * desc = (MIDIINCAPS *) malloc(sizeof(MIDIINCAPS));
        if (desc == NULL) throw Memory_ex("Unable to allocate driver descriptor !");
        if (midiInGetDevCapsA(i, desc, sizeof(MIDIINCAPS)) == MMSYSERR_NOERROR)
            AjouterDriverIn((void *) desc, "");
        else free(desc);
    }
//Alloue les sorties MIDI
    n = midiOutGetNumDevs();
    for (uint i = 0; i < n; i++) {
        MIDIOUTCAPS * desc = (MIDIOUTCAPS *) malloc(sizeof(MIDIOUTCAPS));
        if (desc == NULL) throw Memory_ex("Unable to allocate driver descriptor !");
        if (midiOutGetDevCapsA(i, desc, sizeof(MIDIOUTCAPS)) == MMSYSERR_NOERROR)
            AjouterDriverOut((void *) desc, "");
        else free(desc);
    }
#endif
#ifdef LINUX
//Alloue la structure d'information
    snd_ctl_t * ctl;
    snd_rawmidi_info_t * mainInfo, * info;
//Parcourt l'arborescence
    int iC = -1;
    while (1) {
    //Ajoute un périphérique
        if (snd_card_next(&iC) < 0) break;
        if (iC == -1) break;
    //Ajoute les devices
        char chemin[40];
        sprintf(chemin, "hw:%d", iC);
        if (snd_ctl_open(&ctl, chemin, 0) >= 0) {
            int iD = -1;
            while (1) {
                if (snd_ctl_rawmidi_next_device(ctl, &iD) < 0) break;
                if (iD == -1) break;
            //Configure le périphérique
                snd_rawmidi_info_alloca(&mainInfo);
                snd_rawmidi_info_set_device(mainInfo, iD);
            //Inscrit chaque périphérique
                int iSD = 0;
                while (1) {
                    sprintf(chemin, "hw:%d,%d,%d", iC, iD, iSD);
                    snd_rawmidi_info_set_subdevice(mainInfo, iSD);
                //Test en entrée
                    snd_rawmidi_info_set_stream(mainInfo, SND_RAWMIDI_STREAM_INPUT);
                    if (snd_ctl_rawmidi_info(ctl, mainInfo) == 0) {
                        if (snd_rawmidi_info_malloc(&info) < 0)
                            throw Memory_ex("Unable to allocate driver descriptor !");
                        snd_rawmidi_info_copy(info, mainInfo);
                        AjouterDriverIn((void *) info, chemin);
                    }
                //Test en sortie
                    snd_rawmidi_info_set_stream(mainInfo, SND_RAWMIDI_STREAM_OUTPUT);
                    if (snd_ctl_rawmidi_info(ctl, mainInfo) == 0) {
                        if (snd_rawmidi_info_malloc(&info) < 0)
                            throw Memory_ex("Unable to allocate driver descriptor !");
                        snd_rawmidi_info_copy(info, mainInfo);
                        AjouterDriverOut((void *) info, chemin);
                    }else break;
                    iSD ++;
                }
            }
            snd_ctl_close(ctl);
        }
    }
//Libère la mémoire
    snd_config_update_free_global();
#endif
}

void MIDI::LibererDrivers()
{
//Ferme les communications
    ActiverIn(-1); ActiverOut(-1); ActiverCtrl(-1);
//Libère les configurations
#ifdef WIN32
    for (uint i = 0; i < nbIns; i++)
        free(ins[i].descr);
    for (uint i = 0; i < nbOuts; i++)
        free(outs[i].descr);
#endif
#ifdef LINUX
    for (uint i = 0; i < nbIns; i++)
        snd_rawmidi_info_free((snd_rawmidi_info_t *) ins[i].descr);
    for (uint i = 0; i < nbOuts; i++)
        snd_rawmidi_info_free((snd_rawmidi_info_t *) outs[i].descr);
#endif
    nbIns = 0; nbOuts = 0;
}

/*****************************************************************************/
void MIDI::AjouterDriverIn(void * descr, const char * chemin)
{
    if (nbIns >= MIDI_MAX_DRIVERS) throw ("Too much drivers added in the list !");
    ins[nbIns].descr = descr;
    strncpy(ins[nbIns].chemin, chemin, MIDI_LEN_CHEMIN);
    ins[nbIns].chemin[MIDI_LEN_CHEMIN - 1] = 0;
    nbIns ++;
}

void MIDI::AjouterDriverOut(void * descr, const char * chemin)
{
    if (nbOuts >= MIDI_MAX_DRIVERS) throw ("Too much drivers added in the list !");
    outs[nbOuts].descr = descr;
    strncpy(outs[nbOuts].chemin, chemin, MIDI_LEN_CHEMIN);
    outs[nbOuts].chemin[MIDI_LEN_CHEMIN - 1] = 0;
    nbOuts ++;
}

/*****************************************************************************/
const char * MIDI::DriverIn(const int index)
{
#ifdef WIN32
    return ((MIDIINCAPS *) ins[index].descr)->szPname;
#endif
#ifdef LINUX
    return snd_rawmidi_info_get_name(((snd_rawmidi_info_t *) ins[index].descr));
#endif
    return NULL;
}

const char * MIDI::DriverOut(const int index)
{
#ifdef WIN32
    return ((MIDIINCAPS *) outs[index].descr)->szPname;
#endif
#ifdef LINUX
    return snd_rawmidi_info_get_name(((snd_rawmidi_info_t *) outs[index].descr));
#endif
    return NULL;
}

/*****************************************************************************/
uint MIDI::NbDriversIn()
{
    return nbIns;
}

uint MIDI::NbDriversOut()
{
    return nbOuts;
}

/*****************************************************************************/
void MIDI::ActiverIn(const int index)
{
//Désactive l'ancien driver
    attente = false;
    if (InOk()) {
#ifdef WIN32
    //Vide le tampon
        midiInStop(hndIn);
        midiInReset(hndIn);
        DePreparerTampon();
    //Stoppe l'écoute
        midiInClose(hndIn);
        hndIn = 0;
#endif
#ifdef LINUX
    //Arrete le thread
        runIn = false;
        prepare = false;
        pthread_join(threadIn, NULL);
    //Stoppe l'écoute
        snd_rawmidi_close(hndIn);
        ins[indIn].handleStruct = NULL;
#endif
    }
//Quitte si désactivation
    indIn = -1;
    if (index < 0) return;
//Ouvre le port en entrée
    if (index == indCtrl)
        throw MIDI_ex("Cannot select the same driver as MIDI CTRL !");
#ifdef WIN32
    if (midiInOpen(&ins[index].handleNum, index, (ulong) CallbackIn, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to open MIDI IN !");
    if (midiInStart(ins[index].handleNum) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to start listenning to MIDI IN !");
#endif
#ifdef LINUX
    if (snd_rawmidi_open((snd_rawmidi_t **) &ins[index].handleStruct, NULL, ins[index].chemin, SND_RAWMIDI_NONBLOCK) < 0)
        throw MIDI_ex("Unable to open MIDI IN !");
    runIn = true;
    if (pthread_create(&threadIn, NULL, CallbackIn, NULL) != 0) {
        runIn = false;
        throw MIDI_ex("Unable to start IN thread !");
    }
#endif
//Sauvegarde l'index
    indIn = index;
}

/*****************************************************************************/
void MIDI::ActiverOut(const int index)
{
//Désactive l'ancien driver
    if (OutOk()) {
#ifdef WIN32
        midiOutReset(hndOut);
        midiOutClose(hndOut);
        hndOut = 0;
#endif
#ifdef LINUX
        snd_rawmidi_close(hndOut);
        outs[indOut].handleStruct = NULL;
#endif
    }
//Quitte si désactivation
    indOut = -1;
    if (index < 0) return;
//Ouvre le port en sortie
#ifdef WIN32
    if (midiOutOpen(&outs[index].handleNum, index, 0, 0, 0) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to open MIDI OUT !");
#endif
#ifdef LINUX
    if (snd_rawmidi_open(NULL, (snd_rawmidi_t **) &outs[index].handleStruct, outs[index].chemin,
                         SND_RAWMIDI_APPEND | SND_RAWMIDI_NONBLOCK | SND_RAWMIDI_SYNC) < 0)
        throw MIDI_ex("Unable to open MIDI OUT !");
#endif
//Sauvegarde l'index
    indOut = index;
}

/*****************************************************************************/
void MIDI::ActiverCtrl(const int index)
{
//Désactive l'ancien driver
    if (CtrlOk()) {
#ifdef WIN32
    //Stoppe l'écoute
        midiInReset(hndCtrl);
        midiInStop(hndCtrl);
        midiInClose(hndCtrl);
        hndCtrl = 0;
#endif
#ifdef LINUX
    //Arrete le thread
        runCtrl = false;
        pthread_join(threadCtrl, NULL);
    //Stoppe l'écoute
        snd_rawmidi_close(hndCtrl);
        ins[indCtrl].handleStruct = NULL;
#endif
    }
//Quitte si désactivation
    indCtrl = -1;
    if (index < 0) return;
//Ouvre le port en entrée
    if (index == indIn)
        throw MIDI_ex("Cannot select the same driver as MIDI IN !");
#ifdef WIN32
    if (midiInOpen(&ins[index].handleNum, index, (ulong) CallbackCtrl, 0, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to open MIDI CTRL !");
    if (midiInStart(ins[index].handleNum) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to start listenning to MIDI CTRL !");
#endif
#ifdef LINUX
    if (snd_rawmidi_open((snd_rawmidi_t **) &ins[index].handleStruct, NULL, ins[index].chemin, SND_RAWMIDI_NONBLOCK) < 0)
        throw MIDI_ex("Unable to open MIDI CTRL !");
    runCtrl = true;
    if (pthread_create(&threadCtrl, NULL, CallbackCtrl, NULL) != 0) {
        runCtrl = false;
        throw MIDI_ex("Unable to start CTRL thread !");
    }
#endif
//Sauvegarde l'index
    indCtrl = index;
}

/*****************************************************************************/
bool MIDI::InOk()
{
    return indIn >= 0;
}

bool MIDI::OutOk()
{
    return indOut >= 0;
}

bool MIDI::CtrlOk()
{
    return indCtrl >= 0;
}

/*****************************************************************************/
void MIDI::EnvMsg(uchar * msg)
{
//Vérifie l'ouverture
    if (!OutOk()) throw MIDI_ex("No MIDI OUT port openned !");
//Envoie le message
#ifdef WIN32
    if (midiOutShortMsg(hndOut, * (ulong *) msg) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to send MIDI data !");
#endif
#ifdef LINUX
    if (snd_rawmidi_write(hndOut, msg, 3) != 3)
        throw MIDI_ex("Unable to send MIDI data !");
#endif
//Attente d'émission
    stdSleep(MIDI_PAUSE_MESSAGE);
}

void MIDI::EnvSysEx(uchar * sysEx, const int taille, const bool reponse)
{
//Vérifie l'ouverture
    if (!OutOk()) throw MIDI_ex("No MIDI OUT port openned !");
    if (reponse && !InOk()) throw MIDI_ex("No MIDI IN port openned !");
#ifdef WIN32
//Créé l'entête message
    MIDIHDR head;
    memset(&head, 0, sizeof(MIDIHDR));
    head.dwBufferLength = taille;
    head.lpData = (char *) sysEx;
//Prépare le message
    if (midiOutPrepareHeader(hndOut, &head, sizeof(MIDIHDR)) != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to prepare header !");
//Envoie le message
    if (reponse) PreparerTampon();
    if (midiOutLongMsg(hndOut, &head, sizeof(MIDIHDR)) != MMSYSERR_NOERROR) {
    //Supprime l'attente de réception
        attente = false;
        midiOutUnprepareHeader(hndOut, &head, sizeof(MIDIHDR));
    //Envoie l'exception
        throw MIDI_ex("Unable to send MIDI data !");
    }
    midiOutUnprepareHeader(hndOut, &head, sizeof(MIDIHDR));
#endif
#ifdef LINUX
    if (reponse) {
        attente = true;
        prepare = true;
    }
    if (snd_rawmidi_write(hndOut, sysEx, taille) != taille)
        throw MIDI_ex("Unable to send MIDI data !");
#endif
//Attente d'émission
    stdSleep(MIDI_PAUSE_MESSAGE);
}

void MIDI::RecSysEx(uchar * sysEx, const int taille)
{
    ulong cmpt = 0;
//Vérifie l'ouverture
    if (!InOk()) throw MIDI_ex("No MIDI IN port openned !");
//Attend un message
    while (cmpt < MIDI_ATTENTE) {
    //Recoit un nouveau message
        if (!attente) {
            memcpy(sysEx, tampon, taille);
        #ifdef WIN32
            DePreparerTampon();
        #endif
        #ifdef LINUX
            prepare = false;
        #endif
            return;
        }
    //Attend le message
        stdSleep(MIDI_PAUSE_THREAD);
        cmpt ++;
    }
//Delai d'attente dépassé
#ifdef WIN32
    DePreparerTampon();
#endif
#ifdef LINUX
    prepare = false;
#endif
    throw MIDI_ex("No MIDI data received !");
}

/*****************************************************************************/
void MIDI::ChoisirMidiChannel(const uchar channel)
{
    midiChannel = channel;
}

uchar MIDI::MidiChannel()
{
    return midiChannel;
}

void MIDI::ChoisirVelocity(const uchar velo)
{
    velocity = velo;
}

uchar MIDI::Velocity()
{
    return velocity;
}

void MIDI::ChoisirSysChannel(const uchar channel)
{
    sysChannel = channel;
}

uchar MIDI::SysChannel()
{
    return sysChannel;
}

/*****************************************************************************/
void MIDI::ActiverINToOUT(const bool active)
{
    relaiIN = active;
}

void MIDI::ActiverCTRLToOUT(const bool active)
{
    relaiCTRL = active;
}

/*****************************************************************************/
void MIDI::NoteOn(const uchar note)
{
    uchar msg[4];
//Active une note
    msg[0] = 0x90 + (midiChannel & 0xF);
    msg[1] = note & 0x7F;
    msg[2] = velocity & 0x7F;
    msg[3] = 0;
//Envoie la note
    EnvMsg(msg);
}

void MIDI::NoteOff(const uchar note)
{
    uchar msg[4];
//Active une note
    msg[0] = 0x90 + (midiChannel & 0xF);
    msg[1] = note & 0x7F;
    msg[2] = 0;
    msg[3] = 0;
//Envoie la note
    EnvMsg(msg);
}

void MIDI::AllNotesOff()
{
    uchar msg[4];
//Désactive toutes les notes
    msg[0] = 0xB0 + (midiChannel & 0xF);
    msg[1] = 0x7B;
    msg[2] = 0;
    msg[3] = 0;
//Envoie la commande
    EnvMsg(msg);
}

/*****************************************************************************/
#ifdef WIN32
void MIDI::PreparerTampon()
{
    if (prepare) return;
//Initialise le tampon
    memset(tampon, 0, MIDI_LEN_TAMPON);
    memset(&header, 0, sizeof(MIDIHDR));
//Configure l'entête
    header.dwBufferLength = MIDI_LEN_TAMPON;
    header.lpData = (char *) tampon;
//Prépare le tampon
    if (midiInPrepareHeader(hndIn, &header, sizeof(MIDIHDR))
        != MMSYSERR_NOERROR)
        throw MIDI_ex("Unable to prepare header !");
//Ajoute à la liste
    if (midiInAddBuffer(hndIn, &header, sizeof(MIDIHDR))
        != MMSYSERR_NOERROR) {
        midiInUnprepareHeader(hndIn, &header, sizeof(MIDIHDR));
        throw MIDI_ex("Unable to add buffer to MIDI IN !");
    }
//Attend la réception
    prepare = true;
    attente = true;
}
#endif

#ifdef WIN32
void MIDI::DePreparerTampon()
{
    if (!prepare) return;
//Déprepare le tampon
    attente = false;
    midiInUnprepareHeader(hndIn, &header, sizeof(MIDIHDR));
    prepare = false;
}
#endif

/*****************************************************************************/
bool MIDI::BackupTampon(char * chemin)
{
    FILE * fichier;
//Sauvegarde un backup binaire
    fichier = fopen(chemin, "wb+");
    if (fichier == NULL) return false;
    if (fwrite(tampon, MIDI_LEN_TAMPON, 1, fichier) != 1) return false;
    fclose(fichier);
    return true;
}

/*****************************************************************************/
#ifdef WIN32
void WINAPI MIDI::CallbackIn(uint hmi, uint msg, uint instance, uint param1, uint param2)
{
    switch(msg) {
    case MIM_LONGDATA :
    //Signifie la réception du sysEx
        attente = false;
    break;
    case MIM_DATA :
   //Relai les données
        if (OutOk() && relaiIN)
            try {EnvMsg((uchar *) param1);}
            catch (...) { printf("MIDI IN relay exception !\n\r"); }
    break;
    }
}

void WINAPI MIDI::CallbackCtrl(uint hmi, uint msg, uint instance, uint param1, uint param2)
{
    switch(msg) {
    case MIM_DATA :
    //Gère les automations
        if ((param1 & 0xF0) == 0xB0) {
            uchar CC = (param1 >> 8) & 0xFF;
            uchar valeur = (param1 >> 16) & 0xFF;
            Automation::ReagirCC(CC, valeur);
        }
    //Relai les données
        if (OutOk() && relaiCTRL)
            try {EnvMsg((uchar *) param1);}
            catch (...) { printf("MIDI CTRL relay exception !\n\r"); }
    break;
    }
}
#endif

/*****************************************************************************/

#ifdef LINUX
void * MIDI::CallbackIn(void * param)
{
    ReceptionStr rec;
    bool okSysEx, okNormal;
//Initialise la structure
    rec.recSysEx  = false;
    rec.recNormal = false;
//Attend les messages midi
    while (runIn) {
        RecevoirMessages(hndIn, &rec, &okNormal, &okSysEx);
        if (okNormal) {
        //Relai les données
            if (OutOk() && relaiIN)
                try {EnvMsg(rec.tamponNormal);}
                catch (...) { printf("MIDI IN relay exception !\n\r"); }
        }
        if (okSysEx) {
        //Recopie le sysEx et signale
            memcpy(tampon, rec.tamponSysEx, rec.posSysEx);
            attente = false;
        }
        stdSleep(MIDI_PAUSE_THREAD);
    }
    pthread_exit(NULL);
}

void * MIDI::CallbackCtrl(void * param)
{
    ReceptionStr rec;
    bool okSysEx, okNormal;
//Initialise la structure
    rec.recSysEx  = false;
    rec.recNormal = false;
//Attend les messages midi
    while (runCtrl) {
        RecevoirMessages(hndCtrl, &rec, &okNormal, &okSysEx);
        if (okNormal) {
        //Gère les automations
            if ((rec.tamponNormal[0] & 0xF0) == 0xB0) {
                uchar CC = rec.tamponNormal[1];
                uchar valeur = rec.tamponNormal[2];
                Automation::ReagirCC(CC, valeur);
            }
        //Relai les données
            if (OutOk() && relaiCTRL)
                try {EnvMsg(rec.tamponNormal);}
                catch (...) { printf("MIDI CTRL relay exception !\n\r"); }
        }
        stdSleep(MIDI_PAUSE_THREAD);
    }
    pthread_exit(NULL);
}

/*****************************************************************************/
void MIDI::RecevoirMessages(snd_rawmidi_t * handle, ReceptionStr * rec, bool * okNormal, bool * okSysEx)
{
    uchar octet;
    ssize_t res;
//Vérifie l'handle et reset
    if (handle == NULL) return;
    * okNormal = false; * okSysEx = false;
//Récupère les octets
    if ((res = snd_rawmidi_read(handle, &octet, 1)) == 1) {
    //Recherche un octet de statut
        if (octet & 0x80) {
        //Détermine le type de message
            if (octet == 0xF0) {
            //Récupère un sysEx
                rec->recSysEx = true;
                rec->posSysEx = 0;
            }else if (octet == 0xF7) {
                rec->recSysEx = false;
                * okSysEx = true;
            }else{
            //Supprime les messages temps réél
                if ((octet & 0xF0) == 0xF0) return;
            //Récupère un message standard
                rec->recNormal = true;
                rec->posNormal = 0;
            }
        }else if (res < 0) printf("MIDI data receiving error !\n\r");
    //Recopie les données sysEx
        if (rec->recSysEx) {
        //Remplie le tampon
            rec->tamponSysEx[rec->posSysEx] = octet;
            rec->posSysEx ++;
            if (rec->posSysEx == MIDI_LEN_TAMPON)
                rec->recSysEx = false;
        }
    //Recopie les données normales
        if (rec->recNormal) {
        //Remplie le tampon
            rec->tamponNormal[rec->posNormal] = octet;
            rec->posNormal ++;
            if (rec->posNormal == 3) {
                rec->recNormal = false;
                * okNormal = true;
            }
        }
    }
}

#endif
