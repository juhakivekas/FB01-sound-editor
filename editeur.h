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

#ifndef EDITEUR_H
#define EDITEUR_H

#include <QApplication>
#include "interface/mainwindow.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "excep/memory_ex.h"
#include "excep/midi_ex.h"

#include "core/midi.h"
#include "core/edit.h"
#include "core/voice.h"
#include "core/operateur.h"
#include "core/set.h"
#include "core/instrument.h"
#include "core/bank.h"
#include "core/bank_voice.h"
#include "core/config.h"

//****************************************************************************/
int main(int argc, char *argv[]);
void quit();

//****************************************************************************/
class Editeur {
    friend class MainWindow;
public :
//Constantes générales
    #define EDITEUR_NB_BANK 7
//Types de fichiers
    typedef enum {
        FICHIER_VOICE = 0,
        FICHIER_SET,
        FICHIER_AUTOMATION,
        FICHIER_SYSEX
    }FICHIER_TYPE;
//Constructeurs
    Editeur();
    ~Editeur();
//Chargement et déchargement
    FILE * ChargerFichier(FICHIER_TYPE type, short * version);
    FILE * EnregistrerFichier(FICHIER_TYPE type);
//Actualisation de l'éditeur
    void Actualiser();
    void Rafraichir(const bool visible = false);
    void Reinitialiser();
//Attribution des objets
    void AttribuerConfig();
    void AttribuerBanks();
    void AttribuerInstruments();
    void AttribuerVoice();
    void AttribuerOperateurs();
    void InitialiserAutomation();
//Configuration de l'interface
    void ActiverOnglets(const bool actifs);
    void ActiverMenus(const bool actifs);
    void ActualiserEdition(const int onglet);
//Sélection des pages
    void ChoisirPageSet(const int page);
    void ChoisirPageBank(const int page);
//Sélection des objets
    void ChoisirBank(const int bank);
    void ChoisirInstru(const int instru);
    void ChoisirOP(const int OP);
//Actualisation des contenus
    bool ActualiserBanks();
    bool ActualiserBank(const int bank);
    bool ActualiserSet();
    bool ActualiserVoice();
    void ActualiserAutomation();
//Rafraichissement de l'affichage
    void RafraichirAutomation();
    void RafraichirBanks();
    void RafraichirBank(const int bank);
    void RafraichirSet(const bool visible = false);
    void RafraichirConfig(const bool visible = false);
    void RafraichirVoice(const bool visible = false);
private :
//Fenêtre de l'éditeur
    MainWindow mainWindow;
//Objets de l'éditeur
    Bank * banks[EDITEUR_NB_BANK];
    Set    set;
    Voice  voice;
    Config config;
//Sélection des pages et objets
    int pageSetSel;
    int pageBankSel;
    int instruSel;
    int OPSel;
    int bankSel;
//Mécanisme de copier/coller
    Edit::CopieStr copie;
//Initialisation de l'éditeur
    void InitialiserEditeur();
    void InitialiserInterface();
    void TerminerEditeur();
    void TerminerInterface();    
};

#endif
