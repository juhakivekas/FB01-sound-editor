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

#ifndef BLOCK_H
#define BLOCK_H

#include <stdlib.h>
#include <string.h>

#include "../types.h"
#include "midi.h"

class Block {
public :
//Constructeurs
    Block(uchar * sysEx, const uint lenSysEx, const uint nbParam, const uint offParam);
    ~Block();
//Modification des propriétés
    virtual uchar LireParam(const uchar param);
    virtual void  EcrireParam(const uchar param, const uchar valeur);
//Etat des communications
    void AutoriserEnvoi(const bool envoi);
    bool EnvoiAutorise();
//Envoi / Reception de l'objet
    virtual void Envoyer(const uchar param);
    virtual void EnvoyerTout();
    virtual void RecevoirTout();
//Vérification de la transmission
    virtual uchar CalculerCheckSum(const uint debut, const uint longueur);
    virtual void  VerifierCheckSum(const uint debut, const uint longueur, const uint position);
protected :
//Paramêtres du sysEx
    uchar * sysEx;
    uint lenSysEx;
    uint nbParam;
    uint offParam;
//Paramêtres de communication
    bool envoi;
//Modification du sysEx
    uchar LireParam1Oct(const uchar param);
    uchar LireParam2Oct(const uchar param);
    void  EcrireParam1Oct(const uchar param, const uchar valeur);
    void  EcrireParam2Oct(const uchar param, const uchar valeur);
//Utilitaires du SysEx
    virtual void  Preparer(const uchar * entete, const uint lenEntete);
};

#endif
