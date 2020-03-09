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

#include "voice.h"

/*****************************************************************************/
Voice::Voice()
     : Edit(0, (uchar *) malloc(VOICE_LEN_SYSEX), VOICE_LEN_SYSEX,
            VOICE_NB_PARAM, VOICE_OFF_PARAM, EDIT_OBJ_VOICE)
{
//Vérifie l'allocation
    if (sysEx == NULL) throw(Memory_ex("Unable to allocate the voice sysex !"));
    memset(operateurs, 0, sizeof(Operateur *) * VOICE_NB_OPS);
//Initialise la classe
    Initialiser();
    CreerCallbacks();
//Initialise les opérateurs
    for (int i = 0; i < VOICE_NB_OPS; i++) {
        operateurs[i] = new Operateur(i, &sysEx[VOICE_OFF_OPS + OPERATOR_LEN_SYSEX * (VOICE_NB_OPS - i - 1)]);
        if (operateurs[i] == NULL) throw Memory_ex("Unable to allocate the operators !");
    }
    AutoriserEnvoi(true);
}

Voice::~Voice()
{
//Libère les opérateurs
    for (int i = 0; i < 4; i++)
        delete operateurs[i];
//Libère le sysex
    if (sysEx != NULL) free(sysEx);
}

/*****************************************************************************/
Operateur * Voice::RecupererOP(const uint index)
{
    return operateurs[index];
}

void Voice::AssocierInstrument(const uint index)
{
    this->id = index;
    for (int i=0; i < VOICE_NB_OPS; i++)
        operateurs[i]->AssocierInstrument(index);
}

/*****************************************************************************/
bool Voice::Enregistrer(FILE * fichier)
{
//Sauvegarde des informations
    if (fwrite(auteur, VOICE_LEN_AUTEUR, 1, fichier) == 0) return false;
    if (fwrite(commentaires, VOICE_LEN_COMMENTS, 1, fichier) == 0) return false;
    if (fwrite(LireNom(), VOICE_LEN_NOM, 1, fichier) == 0) return false;
//Sauvegarde la table
    if (!Edit::Enregistrer(fichier)) return false;
//Sauvegarde les opérateurs
    for (uint i = 0; i < VOICE_NB_OPS; i ++)
        if (!operateurs[i]->Enregistrer(fichier))
            return false;
    return true;
}

bool Voice::Charger(FILE * fichier, const short version)
{
    char nom[VOICE_LEN_NOM];
//Récupère des informations
    if (fread(auteur, VOICE_LEN_AUTEUR, 1, fichier) == 0) return false;
    if (fread(commentaires, VOICE_LEN_COMMENTS, 1, fichier) == 0) return false;
    if (fread(nom, VOICE_LEN_NOM, 1, fichier) == 0) return false;
    EcrireNom(nom);
//Récupère la table
    if (!Edit::Charger(fichier, version)) return false;
//Récupère les opérateurs
    for (uint i = 0; i < VOICE_NB_OPS; i ++)
        if (!operateurs[i]->Charger(fichier, version))
            return false;
    return true;
}

/*****************************************************************************/
const uchar initTab[VOICE_NB_PARAM] = {7, 0, 0, 2, 1, 0, 4, 2, 127, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1};
void Voice::Initialiser()
{
    uchar entVoice[7] = {0xF0, 0x43, 0x75, 0x00, 0x08, 0x00, 0x00};
//Entete du sysEx
    Preparer(entVoice, 7);
//Chaines de caractères
    EcrireNom((char *) "none");
    EcrireAuteur((char *) "unknown");
    EcrireCommentaires((char *) "none");
//Paramêtres internes
    for (uchar i = 0; i < VOICE_NB_PARAM; i++)
        EcrireParam(i, initTab[i]);
//Paramêtres opérateurs
    for (uint i = 0; i < VOICE_NB_OPS; i++)
        if (operateurs[i] != NULL) operateurs[i]->Initialiser();
}

void Voice::Randomiser()
{
    Edit::Randomiser();
//Limite la randomisation
    if (LireParam(VOICE_CONTROLLER) > 4)
        EcrireParam(VOICE_CONTROLLER, 4);
}

/*****************************************************************************/
uchar Voice::LireParam(const uchar param)
{
    try {
        switch(param) {
        case VOICE_ALGORITHM :
            return LireParam2Oct(0xC) & 0x7;
        case VOICE_USERCODE :
            return LireParam2Oct(0x7);
        case VOICE_FEEDBACK :
            return (LireParam2Oct(0xC) >> 3) & 0x7;
        case VOICE_TRANSPOSE :
            return LireParam2Oct(0xF);
        case VOICE_POLY :
            return (LireParam2Oct(0x3A) >> 7) & 0x1;
        case VOICE_PORTAMENTO :
            return LireParam2Oct(0x3A) & 0x7F;
        case VOICE_PITCHBEND :
            return LireParam2Oct(0x3B) & 0xF;
        case VOICE_CONTROLLER :
            return (LireParam2Oct(0x3B) >> 4) & 0x7 ;
        case VOICE_LFO_SPEED :
            return LireParam2Oct(0x8);
        case VOICE_LFO_WAVE :
            return (LireParam2Oct(0xE) >> 5) & 0x3;
        case VOICE_LFO_LOAD :
            return (LireParam2Oct(0x9) >> 7) & 0x1;
        case VOICE_LFO_SYNC :
            return (LireParam2Oct(0xA) >> 7) & 0x1;
        case VOICE_LFO_AMD :
            return LireParam2Oct(0x9) & 0x7F;
        case VOICE_LFO_AMS :
            return LireParam2Oct(0xD) & 0x3;
        case VOICE_LFO_PMD :
            return LireParam2Oct(0xA) & 0x7F;
        case VOICE_LFO_PMS :
            return (LireParam2Oct(0xD) >> 4) & 0x7;
        case VOICE_ENABLE_OP1 :
            return (LireParam2Oct(0xB) >> 6) & 0x1;
        case VOICE_ENABLE_OP2 :
            return (LireParam2Oct(0xB) >> 5) & 0x1;
        case VOICE_ENABLE_OP3 :
            return (LireParam2Oct(0xB) >> 4) & 0x1;
        case VOICE_ENABLE_OP4 :
            return (LireParam2Oct(0xB) >> 3) & 0x1;
        default : return 0;
        }
    }catch(MIDI_ex ex) {
        printf("Voice exception : %s\n\r", ex.Info());
        return 0;
    }
}

void Voice::EcrireParam(const uchar param, const uchar valeur)
{
    uchar byte;
    try {
        switch(param) {
        case VOICE_ALGORITHM :
            byte  = LireParam2Oct(0xC) & 0xF8;
            byte += valeur & 0x7;
            EcrireParam2Oct(0xC, byte);
        break;
        case VOICE_USERCODE :
            EcrireParam2Oct(0x7, valeur);
        break;
        case VOICE_FEEDBACK :
            byte  = LireParam2Oct(0xC) & 0xC7;
            byte += (valeur & 0x7) << 3;
            EcrireParam2Oct(0xC, byte);
        break;
        case VOICE_TRANSPOSE :
            EcrireParam2Oct(0xF, valeur);
        break;
        case VOICE_POLY :
            byte  = LireParam2Oct(0x3A) & 0x7F;
            byte += (valeur & 0x1) << 7;
            EcrireParam2Oct(0x3A, byte);
        break;
        case VOICE_PORTAMENTO :
            byte  = LireParam2Oct(0x3A) & 0x80;
            byte += valeur & 0x7F;
            EcrireParam2Oct(0x3A, byte);
        break;
        case VOICE_PITCHBEND :
            byte  = LireParam2Oct(0x3B) & 0xF0;
            byte += valeur & 0xF;
            EcrireParam2Oct(0x3B, byte);
        break;
        case VOICE_CONTROLLER :
            byte  = LireParam2Oct(0x3B) & 0x8F;
            byte += (valeur & 0x7) << 4;
            EcrireParam2Oct(0x3B, byte);
        break;
        case VOICE_LFO_SPEED :
            EcrireParam2Oct(0x8, valeur);
        break;
        case VOICE_LFO_WAVE :
            byte  = LireParam2Oct(0xE) & 0x9F;
            byte += (valeur & 0x3) << 5;
            EcrireParam2Oct(0xE, byte);
        break;
        case VOICE_LFO_LOAD :
            byte  = LireParam2Oct(0x9) & 0x7F;
            byte += (valeur & 0x1) << 7;
            EcrireParam2Oct(0x9, byte);
        break;
        case VOICE_LFO_SYNC :
            byte  = LireParam2Oct(0xA) & 0x7F;
            byte += (valeur & 0x1) << 7;
            EcrireParam2Oct(0xA, byte);
        break;
        case VOICE_LFO_AMD :
            byte  = LireParam2Oct(0x9) & 0x80;
            byte += valeur & 0x7F;
            EcrireParam2Oct(0x9, byte);
        break;
        case VOICE_LFO_AMS :
            byte  = LireParam2Oct(0xD) & 0xFC;
            byte += valeur & 0x3;
            EcrireParam2Oct(0xD, byte);
        break;
        case VOICE_LFO_PMD :
            byte  = LireParam2Oct(0xA) & 0x80;
            byte += valeur & 0x7F;
            EcrireParam2Oct(0xA, byte);
        break;
        case VOICE_LFO_PMS :
            byte  = LireParam2Oct(0xD) & 0x8F;
            byte += (valeur & 0x7) << 4;
            EcrireParam2Oct(0xD, byte);
        break;
        case VOICE_ENABLE_OP1 :
            byte  = LireParam2Oct(0xB) & 0xBF;
            byte += (valeur & 0x1) << 6;
            EcrireParam2Oct(0xB, byte);
        break;
        case VOICE_ENABLE_OP2 :
            byte  = LireParam2Oct(0xB) & 0xDF;
            byte += (valeur & 0x1) << 5;
            EcrireParam2Oct(0xB, byte);
        break;
        case VOICE_ENABLE_OP3 :
            byte  = LireParam2Oct(0xB) & 0xEF;
            byte += (valeur & 0x1) << 4;
            EcrireParam2Oct(0xB, byte);
        break;
        case VOICE_ENABLE_OP4 :
            byte  = LireParam2Oct(0xB) & 0xF7;
            byte += (valeur & 0x1) << 3;
            EcrireParam2Oct(0xB, byte);
        break;
        default : return;
        }
    }catch(MIDI_ex ex) {
        printf("Voice exception : %s\n\r", ex.Info());
    }
}

/*****************************************************************************/
char * Voice::LireNom()
{
    static char nom[VOICE_LEN_NOM + 1];
    try {
        for (uchar i = 0; i < VOICE_LEN_NOM; i++)
            nom[i] = (char) LireParam2Oct(i);
        nom[VOICE_LEN_NOM] = 0;
        return nom;
    }catch(MIDI_ex ex) {
        printf("Voice exception : %s\n\r", ex.Info());
        return NULL;
    }
}

void Voice::EcrireNom(char * nom)
{
    char temp[VOICE_LEN_NOM];
    try {
        strncpy(temp, nom, VOICE_LEN_NOM);
        for (uint i = 0; i < VOICE_LEN_NOM; i++) {
            if (temp[i] == 0) EcrireParam2Oct(i, ' ');
            else EcrireParam2Oct(i, temp[i]);
        }
    }catch(MIDI_ex ex) {
        printf("Voice exception : %s\n\r", ex.Info());
    }
}

/*****************************************************************************/
char * Voice::LireAuteur()
{
    return auteur;
}

void Voice::EcrireAuteur(char * auteur)
{
    strncpy(this->auteur, auteur, VOICE_LEN_AUTEUR);
    this->auteur[VOICE_LEN_AUTEUR - 1] = 0;
}

char * Voice::LireCommentaires()
{
    return commentaires;
}

void Voice::EcrireCommentaires(char * commentaires)
{
    strncpy(this->commentaires, commentaires, VOICE_LEN_COMMENTS);
    this->commentaires[VOICE_LEN_COMMENTS - 1] = 0;
}

/*****************************************************************************/    
void Voice::Envoyer(const uchar param)
{
    uchar envVoice[9] = {0xF0, 0x43, 0x75, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF7};
//Construit le message
    envVoice[3] = MIDI::SysChannel();
    envVoice[4] = 0x18 + (id & 0x7);
//Envoi les changements
    envVoice[5] = (param + 0x40) & 0x7F;
    envVoice[6] = sysEx[param * 2 + VOICE_OFF_PARAM];
    envVoice[7] = sysEx[param * 2 + VOICE_OFF_PARAM + 1];
//Envoi le message
    MIDI::EnvSysEx(envVoice, 9);
}

void Voice::EnvoyerTout()
{
//Actualise l'entête
    if (!EnvoiAutorise()) return;
    sysEx[VOICE_OFF_CHECK] = CalculerCheckSum(VOICE_OFF_PARAM, VOICE_LEN_PARAM);
    sysEx[VOICE_OFF_PARAM-2] = 0x1;
    sysEx[VOICE_OFF_PARAM-1] = 0x0;
//Actualise la destination
    sysEx[3] = MIDI::SysChannel();
    sysEx[4] = 0x8 + (id & 0x7);
//Envoie le sysEx
    MIDI::EnvSysEx(sysEx, VOICE_LEN_SYSEX);
}

void Voice::RecevoirTout()
{
    uchar recVoice[8] = {0xF0, 0x43, 0x75, 0x00, 0x00, 0x00, 0x00, 0xF7};
//Prépare la demande
    recVoice[3] = MIDI::SysChannel();
    recVoice[4] = 0x20 + ((id + 0x8) & 0xF);
//Attend la réception
    MIDI::EnvSysEx(recVoice, 8, true);
    MIDI::RecSysEx(sysEx, VOICE_LEN_SYSEX);
//Vérifie le sysex
    VerifierCheckSum(VOICE_OFF_PARAM, VOICE_LEN_PARAM, VOICE_OFF_CHECK);
//Initialise les informations
    EcrireAuteur((char *) "unknown");
    EcrireCommentaires((char *) "none");
}

/*****************************************************************************/
void Voice::CreerCallbacks()
{
    Automation::AjouterCallback(this, VOICE_ALGORITHM, "Voice algorithm");
    Automation::AjouterCallback(this, VOICE_FEEDBACK, "Voice feedback");
}

void Voice::AppelerCallback(const uint index, const uchar valeur)
{
    switch(index) {
    case VOICE_ALGORITHM : EcrireParam(VOICE_ALGORITHM, valeur >> 4); break;
    case VOICE_FEEDBACK  : EcrireParam(VOICE_FEEDBACK, valeur >> 4); break;
    }
}
