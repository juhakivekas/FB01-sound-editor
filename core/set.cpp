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

#include "set.h"

/*****************************************************************************/
Set::Set()
   : Edit(0, (uchar *) malloc(SET_LEN_SYSEX), SET_LEN_SYSEX, SET_NB_PARAM, SET_OFF_PARAM, EDIT_OBJ_SET)
{
//Vérifie l'allocation
    if (sysEx == NULL) throw(Memory_ex("Unable to allocate the set sysex !"));
    memset(instruments, 0, sizeof(Instrument *) * SET_NB_INSTRU);
//Initialise la classe
    Initialiser();
    CreerCallbacks();
//Initialise les instruments
    for (int i = 0; i < SET_NB_INSTRU; i++) {
        instruments[i] = new Instrument(i, &sysEx[SET_OFF_INSTRU + INSTRU_LEN_SYSEX * i]);
        if (instruments[i] == NULL) throw Memory_ex("Unable to allocate the instruments !");
    }
    AutoriserEnvoi(true);
}

Set::~Set()
{
//Libère les instruments
    for (int i = 0; i < SET_NB_INSTRU; i++)
        if (instruments[i] != NULL) delete instruments[i];
//Libère le sysex
    if (sysEx != NULL) free(sysEx);
}

/*****************************************************************************/
Instrument * Set::RecupererInstrument(const uint index)
{
    return instruments[index];
}

/*****************************************************************************/
bool Set::Enregistrer(FILE * fichier)
{
//Sauvegarde le nom
    if (fwrite(LireNom(), SET_LEN_NOM, 1, fichier) == 0) return false;
//Sauvegarde la table
    if(!Edit::Enregistrer(fichier))
        return false;
//Sauvegarde les instruments
    for (int i = 0; i < SET_NB_INSTRU; i++)
        if (!instruments[i]->Enregistrer(fichier))
            return false;
    return true;
}

bool Set::Charger(FILE * fichier, const short version)
{
    char nom[SET_LEN_NOM];
//Récupère le nom
    if (fread(nom, SET_LEN_NOM, 1, fichier) == 0) return false;
    EcrireNom(nom);
//Recupère la table
    if (!Edit::Charger(fichier, version)) return false;
//Charge le set d'instruments
    for (int i = 0; i < SET_NB_INSTRU; i++)
        if (!instruments[i]->Charger(fichier, version))
            return false;
    return true;
}

/*****************************************************************************/
const uchar initTab[SET_NB_PARAM] = {0,};
void Set::Initialiser()
{
    uchar entSet[7] = {0xF0, 0x43, 0x75, 0x00, 0x00, 0x01, 0x00};
//Entete du sysEx
    Preparer(entSet, 7);
//Paramêtres internes
    EcrireNom((char *) "none");
    for (uchar i = 0; i < SET_NB_PARAM; i++)
        EcrireParam(i, initTab[i]);
//Paramêtres instruments
    for (uint i = 0; i < SET_NB_INSTRU; i++)
        if (instruments[i] != NULL) instruments[i]->Initialiser();
}

void Set::Randomiser()
{
    Edit::Randomiser();
//Limite la randomisation
    if (LireParam(SET_RECEPTION_MODE) > 2)
        EcrireParam(SET_RECEPTION_MODE, 2);
}

/*****************************************************************************/
uchar Set::LireParam(const uchar param)
{
    try {
        switch(param) {
        case SET_LFO_SPEED:
            return LireParam1Oct(0x9);
        case SET_LFO_WAVE:
            return LireParam1Oct(0xC) & 0x3;
        case SET_LFO_AMD:
            return LireParam1Oct(0xA);
        case SET_LFO_PMD:
            return LireParam1Oct(0xB);
        case SET_COMBINE_MODE:
            return LireParam1Oct(0x8) & 0x1;
        case SET_RECEPTION_MODE:
            return LireParam1Oct(0xD) & 0x3;
        default: return 0;
        }
    }catch(MIDI_ex ex) {
        printf("Set exception : %s\n\r", ex.Info());
        return 0;
    }
}

void Set::EcrireParam(const uchar param, const uchar valeur)
{
    try {
        switch(param) {
        case SET_LFO_SPEED:
            EcrireParam1Oct(0x9, valeur & 0x7F);
        break;
        case SET_LFO_WAVE:
            EcrireParam1Oct(0xC, valeur & 0x3);
        break;
        case SET_LFO_AMD:
            EcrireParam1Oct(0xA, valeur & 0x7F);
        break;
        case SET_LFO_PMD:
            EcrireParam1Oct(0xB, valeur & 0x7F);
        break;
        case SET_COMBINE_MODE:
            EcrireParam1Oct(0x8, valeur & 0x1);
        break;
        case SET_RECEPTION_MODE:
            EcrireParam1Oct(0xD, valeur & 0x3);
        break;
        default: return;
        }
    }catch(MIDI_ex ex) {
        printf("Set exception : %s\n\r", ex.Info());
    }
}

/*****************************************************************************/
char * Set::LireNom()
{
    static char nom[SET_LEN_NOM + 1];
    try {
        for (uchar i = 0; i < SET_LEN_NOM; i++)
            nom[i] = (char) LireParam1Oct(i);
        nom[SET_LEN_NOM] = 0;
        return nom;
    }catch(MIDI_ex ex) {
        printf("Set exception : %s\n\r", ex.Info());
        return NULL;
    }
}

void Set::EcrireNom(char * nom)
{
    char temp[SET_LEN_NOM];
    try {
        strncpy(temp, nom, SET_LEN_NOM);
        for (uint i = 0; i < SET_LEN_NOM; i++) {
            if (temp[i] == 0) EcrireParam1Oct(i, ' ');
            else EcrireParam1Oct(i, temp[i]);
        }
    }catch(MIDI_ex ex) {
        printf("Set exception : %s\n\r", ex.Info());
    }
}

/*****************************************************************************/
void Set::Envoyer(const uchar param)
{
    uchar envSet[8] = {0xF0, 0x43, 0x75, 0x00, 0x10, 0x00, 0x00, 0xF7};
//Construit le message
    if (!EnvoiAutorise()) return;
    envSet[3] = MIDI::SysChannel();
//Envoi les changements
    envSet[5] = param & 0x7F;
    envSet[6] = sysEx[param + SET_OFF_PARAM];
//Envoi le message
    MIDI::EnvSysEx(envSet, 8);
}

void Set::EnvoyerTout()
{
//Actualise l'entête
    if (!EnvoiAutorise()) return;
    sysEx[SET_OFF_CHECK] = CalculerCheckSum(SET_OFF_PARAM, SET_LEN_PARAM);
    sysEx[SET_OFF_PARAM-2] = 0x01;
    sysEx[SET_OFF_PARAM-1] = 0x20;
//Actualise la destination
    sysEx[3] = MIDI::SysChannel();
//Envoie le sysEx
    MIDI::EnvSysEx(sysEx, SET_LEN_SYSEX);
}

void Set::RecevoirTout()
{
    uchar recSet[] = {0xF0, 0x43, 0x75, 0x00, 0x20, 0x01, 0x00, 0xF7};
//Prépare la demande
    recSet[3] = MIDI::SysChannel();
//Attend la réception
    MIDI::EnvSysEx(recSet, 8, true);
    MIDI::RecSysEx(sysEx, SET_LEN_SYSEX);
//Vérifie le sysex
    VerifierCheckSum(SET_OFF_PARAM, SET_LEN_PARAM, SET_OFF_CHECK);
}

/*****************************************************************************/
void Set::CreerCallbacks()
{
    Automation::AjouterCallback(this, SET_LFO_SPEED, "Set LFO Speed");
    Automation::AjouterCallback(this, SET_LFO_WAVE, "Set LFO Waveform");
    Automation::AjouterCallback(this, SET_LFO_AMD, "Set LFO AMD");
    Automation::AjouterCallback(this, SET_LFO_PMD, "Set LFO PMD");
}

void Set::AppelerCallback(const uint index, const uchar valeur)
{
    switch(index) {
    case SET_LFO_SPEED : EcrireParam(SET_LFO_SPEED, valeur); break;
    case SET_LFO_WAVE  : EcrireParam(SET_LFO_WAVE, valeur >> 5); break;
    case SET_LFO_AMD   : EcrireParam(SET_LFO_AMD, valeur); break;
    case SET_LFO_PMD   : EcrireParam(SET_LFO_PMD, valeur); break;
    }
}
