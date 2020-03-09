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

#ifndef OPERATOR_H
#define OPERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types.h"
#include "../excep/memory_ex.h"
#include "../excep/midi_ex.h"
#include "../excep/automation_ex.h"

#include "automation.h"
#include "automated.h"
#include "midi.h"
#include "edit.h"

class Operateur : public Edit, public Automated {
public :
//Constantes
    #define OPERATOR_NB_PARAM  16
    #define OPERATOR_LEN_SYSEX 0x10
    #define OPERATOR_OFF_PARAM 0x50
//Paramêtres éditables
    #define OPERATOR_VOLUME 0
    #define OPERATOR_LEVEL_CURB 1
    #define OPERATOR_LEVEL_VELOCITY 2
    #define OPERATOR_LEVEL_DEPTH 3
    #define OPERATOR_ADJUST 4
    #define OPERATOR_FINE 5
    #define OPERATOR_MULTIPLE 6
    #define OPERATOR_RATE_DEPTH 7
    #define OPERATOR_ATTACK 8
    #define OPERATOR_MODULATOR 9
    #define OPERATOR_ATTACK_VELOCITY 10
    #define OPERATOR_DECAY1 11
    #define OPERATOR_COARSE 12
    #define OPERATOR_DECAY2 13
    #define OPERATOR_SUSTAIN 14
    #define OPERATOR_RELEASE 15
//Constructeurs
    Operateur(const uchar id, uchar * sysEx);
    ~Operateur();
//Association de l'instrument
    void AssocierInstrument(const uint index);
//Edition de l'objet
    void Initialiser();
//Modification des propriétés
    uchar LireParam(const uchar param);
    void  EcrireParam(const uchar param, const uchar valeur);
//Envoi / Reception de l'objet
    void Envoyer(const uchar param);
//Gestion des callbacks automation
    void CreerCallbacks();
    void AppelerCallback(const uint index, const uchar valeur);
private :
//Paramêtres globaux
    uchar instru;
};

#endif
