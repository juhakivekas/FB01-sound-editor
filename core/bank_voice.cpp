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

#include "bank_voice.h"

/*****************************************************************************/
Bank_voice::Bank_voice(const uchar id, uchar * sysEx)
    : Edit(id, sysEx, BANKVOICE_LEN_SYSEX, 0, BANKVOICE_OFF_PARAM, EDIT_OBJ_BANK_VOICE)
{
    AutoriserEnvoi(true);
}

Bank_voice::~Bank_voice()
{

}

/*****************************************************************************/
char * Bank_voice::LireNom()
{
    static char nom[BANKVOICE_LEN_NOM + 1];
    try {
        for (uchar i = 0; i < BANKVOICE_LEN_NOM; i++)
            nom[i] = (char) LireParam2Oct(i);
        nom[BANKVOICE_LEN_NOM] = 0;
        return nom;
    }catch(MIDI_ex ex) {
        printf("Bank_voice exception : %s\n\r", ex.Info());
        return NULL;
    }
}

void Bank_voice::EcrireNom(const char * nom)
{
    char temp[BANKVOICE_LEN_NOM];
    try {
        strncpy(temp, nom, BANKVOICE_LEN_NOM);
        for (uint i = 0; i < BANKVOICE_LEN_NOM; i++) {
            if (temp[i] == 0) EcrireParam2Oct(i, ' ');
            else EcrireParam2Oct(i, temp[i]);
        }
    }catch(MIDI_ex ex) {
        printf("Bank_voice exception : %s\n\r", ex.Info());
    }
}

/*****************************************************************************/
uchar Bank_voice::LireParam(const uchar param)
{
    try {
        switch(param) {
        case BANKVOICE_STYLE :
            return LireParam2Oct(0x7) & 0x7F;
        default : return 0;
        }
    }catch(MIDI_ex ex) {
        printf("Bank_voice exception : %s\n\r", ex.Info());
        return 0;
    }
}

void Bank_voice::EcrireParam(const uchar param, const uchar valeur)
{
    try {
        switch(param) {
        case BANKVOICE_STYLE :
            EcrireParam2Oct(0x7, valeur & 0x7F);
        break;
        default : return;
        }
    }catch(MIDI_ex ex) {
        printf("Bank_voice exception : %s\n\r", ex.Info());
    }
}

/*****************************************************************************/
void Bank_voice::GenererEntete()
{
//Créé l'entête du sysEx
    sysEx[0x0] = 0x1;
    sysEx[0x1] = 0x0;
    sysEx[BANKVOICE_OFF_CHECK] = CalculerCheckSum(BANKVOICE_OFF_PARAM, BANKVOICE_LEN_PARAM);
}
