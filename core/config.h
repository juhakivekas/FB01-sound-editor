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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types.h"
#include "../excep/memory_ex.h"
#include "../excep/midi_ex.h"

#include "midi.h"
#include "edit.h"

/*****************************************************************************/
class Config : public Edit, public Automated {
public :
//Constantes
    #define CONFIG_NB_PARAM 4
//Paramêtres éditables
    #define CONFIG_SYSCHANNEL 0
    #define CONFIG_MEMORY_PROTECT 1
    #define CONFIG_CONFIG_NUMBER 2
    #define CONFIG_MASTER_DETUNE 3
    #define CONFIG_MASTER_VOLUME 4
//Constructeurs
    Config();
    ~Config();
//Modification des propriétés
    uchar LireParam(const uchar param);
    void  EcrireParam(const uchar param, const uchar valeur);
//Envoi / Reception de l'objet
    void Envoyer(const uchar param, const uchar valeur);
//Gestion des callbacks automation
    void CreerCallbacks();
    void AppelerCallback(const uint index, const uchar valeur);
private :
//Paramêtres temporaires
    uchar masterVolume;
    uchar masterDetune;
};

#endif
