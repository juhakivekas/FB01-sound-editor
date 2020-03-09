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

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include <stdlib.h>
#include <stdio.h>

#include "../types.h"
#include "../excep/memory_ex.h"
#include "../excep/midi_ex.h"
#include "../excep/automation_ex.h"

#include "automation.h"
#include "automated.h"
#include "midi.h"
#include "edit.h"


class Instrument : public Edit, public Automated {
public :
//Constantes
    #define INSTRU_NB_PARAM 15
    #define INSTRU_LEN_SYSEX 0x10
//Paramêtres éditables
    #define INSTRU_NB_NOTES 0
    #define INSTRU_CHANNEL 1
    #define INSTRU_UPPER 2
    #define INSTRU_LOWER 3
    #define INSTRU_BANK 4
    #define INSTRU_VOICE 5
    #define INSTRU_DETUNE 6
    #define INSTRU_TRANS 7
    #define INSTRU_VOLUME 8
    #define INSTRU_PAN 9
    #define INSTRU_LFO 10
    #define INSTRU_PORTAMENTO 11
    #define INSTRU_PITCHBEND 12
    #define INSTRU_POLY 13
    #define INSTRU_CONTROLLER 14
//Constructeurs
    Instrument(const uchar id, uchar * sysEx);
    ~Instrument();
//Edition de l'objet
    void Initialiser();
    void Randomiser();
//Modification des propriétés
    uchar LireParam(const uchar param);
    void  EcrireParam(const uchar param, const uchar valeur);
//Envoi / Reception de l'objet
    void Envoyer(const uchar param);
//Callbacks automation
    void CreerCallbacks();
    void AppelerCallback(const uint index, const uchar valeur);
};

#endif
