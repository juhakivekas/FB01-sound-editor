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

#ifndef VOICE_H
#define VOICE_H

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
#include "operateur.h"

class Voice : public Edit, public Automated {
public :
//Constantes
    #define VOICE_NB_OPS 4
    #define VOICE_NB_PARAM 20
    #define VOICE_OFF_PARAM 0x9
    #define VOICE_OFF_OPS 0x29
    #define VOICE_OFF_CHECK 0x89
    #define VOICE_LEN_PARAM 0x80
    #define VOICE_LEN_SYSEX 0x8B
    #define VOICE_LEN_AUTEUR 129
    #define VOICE_LEN_COMMENTS 129
    #define VOICE_LEN_NOM 7
//Paramêtres éditables
    #define VOICE_ALGORITHM 0
    #define VOICE_USERCODE 1
    #define VOICE_FEEDBACK 2
    #define VOICE_TRANSPOSE 3
    #define VOICE_POLY 4
    #define VOICE_PORTAMENTO 5
    #define VOICE_PITCHBEND 6
    #define VOICE_CONTROLLER 7
    #define VOICE_LFO_SPEED 8
    #define VOICE_LFO_WAVE 9
    #define VOICE_LFO_LOAD 10
    #define VOICE_LFO_SYNC 11
    #define VOICE_LFO_AMD 12
    #define VOICE_LFO_AMS 13
    #define VOICE_LFO_PMD 14
    #define VOICE_LFO_PMS 15
    #define VOICE_ENABLE_OP1 16
    #define VOICE_ENABLE_OP2 17
    #define VOICE_ENABLE_OP3 18
    #define VOICE_ENABLE_OP4 19
//Constructeurs
    Voice();
    ~Voice();
//Récupération des objets
    Operateur * RecupererOP(const uint index);
//Association de l'instrument
    void AssocierInstrument(const uint index);
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
//Modification des informations additionnelles
    char * LireAuteur();
    void   EcrireAuteur(char * auteur);
    char * LireCommentaires();
    void   EcrireCommentaires(char * commentaires);
//Envoi / Reception de l'objet
    void Envoyer(const uchar param);
    void EnvoyerTout();
    void RecevoirTout();
//Gestion des callbacks automation
    void CreerCallbacks();
    void AppelerCallback(const uint index, const uchar valeur);
private :
//Opérateurs intégrés
    Operateur * operateurs[VOICE_NB_OPS];
//Paramêtres spéciaux
    char auteur[VOICE_LEN_AUTEUR];
    char commentaires[VOICE_LEN_COMMENTS];
};

#endif
