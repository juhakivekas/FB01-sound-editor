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

#ifndef SET_H
#define SET_H

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
#include "instrument.h"

class Set : public Edit,  public Automated {
public :
//Constantes
    #define SET_NB_PARAM 6
    #define SET_NB_INSTRU 8
    #define SET_OFF_PARAM 0x9
    #define SET_OFF_INSTRU 0x29
    #define SET_OFF_CHECK 0xA9
    #define SET_LEN_PARAM 0xA0
    #define SET_LEN_NOM 8
    #define SET_LEN_SYSEX 0xAB
//Paramêtres éditables
    #define SET_LFO_SPEED 0
    #define SET_LFO_WAVE 1
    #define SET_LFO_AMD 2
    #define SET_LFO_PMD 3
    #define SET_COMBINE_MODE 4
    #define SET_RECEPTION_MODE 5
//Constructeurs
    Set();
    ~Set();
//Récupération des objets
    Instrument * RecupererInstrument(const uint index);
//Chargement / déchargement
    bool Enregistrer(FILE * fichier);
    bool Charger(FILE * fichier, const short version);
//Edition de l'objet
    void Initialiser();
    void Randomiser();
//Modification des propriétés
    uchar  LireParam(const uchar param);
    void   EcrireParam(const uchar param, const uchar valeur);
    char * LireNom();
    void   EcrireNom(char * nom);
//Envoi / Reception de l'objet
    void Envoyer(const uchar param);
    void EnvoyerTout();
    void RecevoirTout();
//Gestion des callbacks automation
    void CreerCallbacks();
    void AppelerCallback(const uint index, const uchar valeur);
private :
//Instruments du set
    Instrument * instruments[SET_NB_INSTRU];
};

#endif
