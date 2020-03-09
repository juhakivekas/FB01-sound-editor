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

#include "edit.h"

/*****************************************************************************/
Edit::Edit(const uchar id, uchar * sysEx, const uint lenSysEx,
           const uint nbParam, const uint offParam, const EDIT_OBJET objet)
    : Block(sysEx, lenSysEx, nbParam, offParam)
{
    this->id = id;
    this->objet = objet;
}

Edit::~Edit()
{
}

/*****************************************************************************/
uchar Edit::LireId()
{
    return id;
}

void Edit::EcrireId(uchar id)
{
    this->id = id;
}

/*****************************************************************************/
bool Edit::Enregistrer(FILE * fichier)
{
    uchar sauv[nbParam];
    for (uint i = 0; i < nbParam; i++)
        sauv[i] = LireParam(i);
    if (fwrite(sauv, nbParam, 1, fichier) == 0)
        return false;
    return true;
}

bool Edit::Charger(FILE * fichier, const short version)
{
    uchar sauv[nbParam];
    if (fread(sauv, nbParam, 1, fichier) == 0)
        return false;
    for (uint i = 0; i < nbParam; i++)
        EcrireParam(i, sauv[i]);
    return true;
}

/*****************************************************************************/
bool Edit::Exporte(FILE * fichier)
{
    if (sysEx == NULL) return false;
//Enregistre le sysEx
    if (fwrite(sysEx, lenSysEx, 1, fichier) == 0)
        return false;
    return true;
}

bool Edit::Importe(FILE * fichier)
{
    if (sysEx == NULL) return false;
//Vérifie la taille du sysEx
    fseek(fichier, 0, SEEK_END);
    if (ftell(fichier) != (int) lenSysEx) return false;
    fseek(fichier, 0, SEEK_SET);
//Charge le sysEx
    if (fread(sysEx, lenSysEx, 1, fichier) == 0)
        return false;
    EnvoyerTout();
    return true;
}

/*****************************************************************************/
void Edit::Copier(CopieStr * copie)
{
//Vérifie la présence du sysEx
    if (sysEx == NULL) return;
//Libère un sysEx temporaire
    if (copie->sysExTemp) free (copie->sysEx);
//Copie le sysEx
    copie->objet = objet;
    copie->sysEx = sysEx;
    copie->lenSysEx = lenSysEx;
    copie->sysExTemp = false;
}

void Edit::Coller(CopieStr * copie)
{
//Vérifie la présence du sysEx
    if (sysEx == NULL) return;
//Colle le sysEx
    if (copie->objet != objet || copie->objet == EDIT_OBJ_RIEN) return;
    memcpy(sysEx, copie->sysEx, lenSysEx);
//Actualise l'objet
    EnvoyerTout();
}

void Edit::Echanger(CopieStr * copie)
{
    uchar * tempSysEx;
//Vérifie la présence du sysEx
    if (sysEx == NULL) return;
//Alloue la table temporaire
    if (copie->objet != objet || copie->objet == EDIT_OBJ_RIEN) return;
    tempSysEx = (uchar *) malloc(lenSysEx);
    if (tempSysEx == NULL) throw Memory_ex("Unable to allocate the exchange table !");
//Echange les sysEx
    memcpy(tempSysEx, sysEx, lenSysEx);
    memcpy(sysEx, copie->sysEx, lenSysEx);
    if (copie->sysExTemp) free (copie->sysEx);
//Actualise la structure de copie
    copie->sysEx = tempSysEx;
    copie->sysExTemp = true;
//Actualise l'objet
    EnvoyerTout();
}

/*****************************************************************************/
void Edit::Initialiser()
{
    if (sysEx == NULL) return;
    for (uint i = 0; i < nbParam; i++)
        EcrireParam(i, 0);
}

void Edit::Randomiser()
{
    if (sysEx == NULL) return;
    for (uint i = 0; i < nbParam; i++)
        EcrireParam(i, RAND(0, 255));
}
