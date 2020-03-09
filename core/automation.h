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

#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../types.h"
#include "../win32.h"
#include "../linux.h"

#include "../excep/memory_ex.h"
#include "../excep/automation_ex.h"

#include "automated.h"

class Automation
{
public:
//Constantes
    #define AUTO_LEN_NOM  41
    #define AUTO_LEN_DESC 81
//Gestion des callbacks
    static void   AjouterCallback(Automated * automated, const uint index, const char * nom);
    static uint   NbCallbacks();
    static char * NomCallback(const uint index);
//Gestion des automations
    static void   Ajouter(const uchar CC, const uchar inFrom, const uchar inTo, const uint indexCB, const uchar outFrom, const uchar outTo);
    static void   Enlever(const uint indexAuto);
    static void   EnleverTout();
    static uint   NbAutomations();
    static char * DescAutomation(const uint indexAuto);
//Réaction aux MIDI CCs
    static void  ReagirCC(const uchar CC, const uchar valeur);
    static uchar DernierCC();
    static uchar DerniereValeur();
//Chargement / enregistrement
    static bool Enregistrer(FILE * fichier);
    static bool Charger(FILE * fichier, const short version);
private :
//Constantes
    #define AUTO_NB_CCS 128
    #define AUTO_NB_CBS 4
    #define AUTO_MAX_CBS 100
//Structures des callbacks
    typedef struct {
        char nom[AUTO_LEN_NOM];
        uint index;
        Automated * automated;
    }CallBackStr;
    typedef struct {
        uchar CC;
        uchar inFrom, inTo;
        uchar outFrom, outTo;
        uint  indexCB;
        bool  valide;
    }AutomationStr;
//Retrouve une automation
    static AutomationStr * TrouverAuto(const uint indexAuto);
//Liste des callbacks et automations
    static CallBackStr callbacks[AUTO_MAX_CBS];
    static AutomationStr automations[AUTO_NB_CCS][AUTO_NB_CBS];
    static uint nbCallbacks, nbAutomations;
//Statistiques évenements CC
    static uchar dernierCC;
    static uchar derniereValeur;
};

#endif // AUTOMATION_H
