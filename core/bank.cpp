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

#include "bank.h"

/*****************************************************************************/
Bank::Bank(const uchar id)
    : Edit(id, (uchar *) malloc(BANK_LEN_SYSEX), BANK_LEN_SYSEX, 0, BANK_OFF_PARAM, EDIT_OBJ_BANK)
{
//Initialise la classe
    if (sysEx == NULL) throw (Memory_ex("Unable to allocate the bank sysex !"));
    memset(voices, 0, sizeof(Bank_voice *) * BANK_NB_VOICES);
    Initialiser();
//Initialise les voices
    for (int i = 0; i < BANK_NB_VOICES; i ++) {
        voices[i] = new Bank_voice(i, &sysEx[BANK_OFF_VOICES + i * BANKVOICE_LEN_SYSEX]);
        if (voices[i] == NULL) throw Memory_ex("Unable to allocate the voices of banks !");
    }
//Initialise l'édition
    voiceSel = 0;
    AutoriserEnvoi(true);
}

Bank::~Bank()
{
//Libère les voices
    for (int i = 0; i < BANK_NB_VOICES; i ++)
        if (voices[i] != NULL) delete voices[i];
//Libère le sysex
    if (sysEx != NULL) free(sysEx);
}

/*****************************************************************************/
Bank_voice * Bank::RecupererVoice(const uint index)
{
    return voices[index];
}

/*****************************************************************************/
void Bank::EnvoyerTout()
{
//Actualise le premier entête
    if (!EnvoiAutorise()) return;
    sysEx[BANK_OFF_CHECK] = CalculerCheckSum(BANK_OFF_PARAM, BANK_LEN_PARAM);
    sysEx[BANK_OFF_PARAM-2] = 0x0;
    sysEx[BANK_OFF_PARAM-1] = 0x40;
//Actualise les sous entêtes
    for (uint i = 0; i < BANK_NB_VOICES; i ++)
        voices[i]->GenererEntete();
//Actualise la destination
    sysEx[3] = MIDI::SysChannel();
    sysEx[6] = id & 0x7;
//Envoie le sysEx
    MIDI::EnvSysEx(sysEx, BANK_LEN_SYSEX);
}

void Bank::RecevoirTout()
{
    uchar recBank[] = {0xF0, 0x43, 0x75, 0x00, 0x20, 0x00, 0x00, 0xF7};
//Prépare la demande
    recBank[3] = MIDI::SysChannel();
    recBank[6] = id & 0x7;
//Attend la réception
    MIDI::EnvSysEx(recBank, 8, true);
    MIDI::RecSysEx(sysEx, BANK_LEN_SYSEX);
//Vérifie le sysEx
    VerifierCheckSum(BANK_OFF_PARAM, BANK_LEN_PARAM, BANK_OFF_CHECK);
//Vérifie les sous-sysEx
    for (uint i = 0; i < BANK_NB_VOICES; i ++)
        voices[i]->VerifierCheckSum(BANKVOICE_OFF_PARAM, BANKVOICE_LEN_PARAM, BANKVOICE_OFF_CHECK);
}

/*****************************************************************************/
void Bank::Initialiser()
{
    uchar entBank[7] = {0xF0, 0x43, 0x75, 0x00, 0x00, 0x00, 0x00};
//Entete du sysEx
    Preparer(entBank, 7);
    EcrireNom((char *) "none");
}

/*****************************************************************************/
void Bank::CopierVoice(CopieStr * copie)
{
    voices[voiceSel]->Copier(copie);
}

void Bank::CollerVoice(CopieStr * copie)
{
    voices[voiceSel]->Coller(copie);
}

void Bank::EchangerVoice(CopieStr * copie)
{
    voices[voiceSel]->Echanger(copie);
}

void Bank::SelectionnerVoice(const uchar index)
{
    voiceSel = index;
}

uchar Bank::VoiceSelectionnee()
{
    return voiceSel;
}

/*****************************************************************************/
char * Bank::LireNom()
{
    static char nom[BANK_LEN_NOM + 1];
    try {
        for (uchar i = 0; i < BANK_LEN_NOM; i++)
            nom[i] = (char) LireParam2Oct(i);
        nom[BANK_LEN_NOM] = 0;
        return nom;
    }catch(MIDI_ex ex) {
        printf("Bank exception : %s\n\r", ex.Info());
        return NULL;
    }
}

void Bank::EcrireNom(char * nom)
{
    char temp[BANK_LEN_NOM];
    try {
        strncpy(temp, nom, BANK_LEN_NOM);
        for (uint i = 0; i < BANK_LEN_NOM; i++) {
            if (temp[i] == 0) EcrireParam2Oct(i, ' ');
            else EcrireParam2Oct(i, temp[i]);
        }
    }catch(MIDI_ex ex) {
        printf("Bank exception : %s\n\r", ex.Info());
    }
}

