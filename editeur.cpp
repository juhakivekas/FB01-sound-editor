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

#include "editeur.h"

#ifdef LINUX
    #include <QX11Info>
#endif

//Objets principaux editeur
QApplication * application = NULL;
Editeur      * editeur = NULL;

/*****************************************************************************/
int main(int argc, char *argv[])
{
//Démarre le programme
    try {
    //Créé l'application et l'éditeur
        if ((application = new QApplication(argc, argv)) == NULL)
            throw Memory_ex("Unable to create the application !");
        if ((editeur = new Editeur()) == NULL)
            throw Memory_ex("Unable to create the editor !");
    //Démarre le programme
        application->exec();
    }catch(MIDI_ex ex) {
        QMessageBox::critical(NULL, "FB01 SE:", ex.Info());
    }catch (Memory_ex ex) {
        QMessageBox::critical(NULL, "FB01 SE:", ex.Info());
    }catch (...) {
        QMessageBox::critical(NULL, "FB01 SE:", "Unknown exception occured !");
    }
//Libère les ressources
    quit(); return 0;
}

void quit()
{
    if (editeur != NULL) delete editeur;
    if (application != NULL) delete application;
}

/*****************************************************************************/
Editeur::Editeur()
{
//Initialise l'éditeur
    InitialiserInterface();
    InitialiserEditeur();
}

Editeur::~Editeur()
{
//Termine l'éditeur
    TerminerEditeur();
    editeur = NULL;
}

/*****************************************************************************/
void Editeur::InitialiserEditeur()
{
//Alloue les banks
    for (uchar i = 0; i < EDITEUR_NB_BANK; i++) {
        banks[i] = new Bank(i);
        if (banks[i] == NULL) throw(Memory_ex("Unable to allocate bank space !"));
    }
//Attribue les classes à l'interface
    AttribuerConfig();
    AttribuerBanks();
    AttribuerInstruments();
    AttribuerVoice();
    AttribuerOperateurs();
//Initialise les modules
#ifdef WIN32
    Periph::Initialiser(NULL);
#endif
#ifdef LINUX
    Periph::Initialiser((void *) mainWindow.x11Info().display());
#endif
    InitialiserAutomation();
//Sélectionne les pages
    ChoisirPageSet(0);
    ChoisirPageBank(0);
//Sélectionne les objets
    ChoisirBank(0);
    ChoisirInstru(0);
    ChoisirOP(0);
//Initialisations diverses
    //srand(QTime::currentTime().msec());
    copie.objet = Edit::EDIT_OBJ_RIEN;
    copie.sysExTemp = false;
}

void Editeur::InitialiserInterface()
{
//Désactive l'interface
    mainWindow.show();
    ActiverMenus(false);
    ActiverOnglets(false);
    ActualiserEdition(MainWindow::ONGLET_CONFIG);
//Liste les drivers
    mainWindow.on_pshBut_refresh_midi_pressed();
}

void Editeur::TerminerEditeur()
{
//Libère les ressources
    for (uchar i = 0; i < EDITEUR_NB_BANK; i++)
        if (banks[i] != NULL) delete banks[i];
//Libère la table de copie
    if (copie.sysExTemp) free (copie.sysEx);
//Réinitialise les modules
    MIDI::LibererDrivers();
    Periph::Terminer();
}

/*****************************************************************************/
void Editeur::ActiverOnglets(const bool actifs)
{
//Active ou désactive les onglets
    mainWindow.ui->tab_banks->setEnabled(actifs);
    mainWindow.ui->tab_set->setEnabled(actifs);
    mainWindow.ui->tab_voice->setEnabled(actifs);
    mainWindow.ui->tab_operas->setEnabled(actifs);
    mainWindow.ui->widget_config->setEnabled(actifs);
    if (!actifs) mainWindow.ui->tab_automation->setEnabled(false);
}

void Editeur::ActiverMenus(const bool actifs)
{
//Menu fichier
    mainWindow.ui->actionLoad_automations->setEnabled(actifs);
    mainWindow.ui->actionSave_automations->setEnabled(actifs);
    mainWindow.ui->actionLoad_bank->setEnabled(actifs);
    mainWindow.ui->actionSave_bank->setEnabled(actifs);
    mainWindow.ui->actionLoad_set->setEnabled(actifs);
    mainWindow.ui->actionSave_set->setEnabled(actifs);
    mainWindow.ui->actionLoad_voice->setEnabled(actifs);
    mainWindow.ui->actionSave_voice->setEnabled(actifs);
    mainWindow.ui->actionImport_bank->setEnabled(actifs);
    mainWindow.ui->actionImport_set->setEnabled(actifs);
    mainWindow.ui->actionImport_voice->setEnabled(actifs);
    mainWindow.ui->actionExport_bank->setEnabled(actifs);
    mainWindow.ui->actionExport_set->setEnabled(actifs);
    mainWindow.ui->actionExport_voice->setEnabled(actifs);
//Menu FB01
    mainWindow.ui->actionGet_current_config->setEnabled(actifs);
    mainWindow.ui->actionGet_current_bank->setEnabled(actifs);
    mainWindow.ui->actionGet_current_set->setEnabled(actifs);
    mainWindow.ui->actionGet_current_voice->setEnabled(actifs);
    mainWindow.ui->actionSend_current_config->setEnabled(actifs);
    mainWindow.ui->actionSend_current_bank->setEnabled(actifs);
    mainWindow.ui->actionSend_current_set->setEnabled(actifs);
    mainWindow.ui->actionSend_current_voice->setEnabled(actifs);
}

/*****************************************************************************/
void Editeur::ActualiserEdition(const int onglet)
{
//Désactive tous les contrôles
    mainWindow.ui->actionInitialize->setEnabled(false);
    mainWindow.ui->actionRandomize->setEnabled(false);
    mainWindow.ui->actionCopy->setEnabled(false);
    mainWindow.ui->actionPaste->setEnabled(false);
    mainWindow.ui->actionExchange->setEnabled(false);
    if (!mainWindow.ui->tabWidget->widget(onglet)->isEnabled()) return;
//Active seulement ceux utilisés
    switch (onglet) {
    case MainWindow::ONGLET_CONFIG :
        if (!mainWindow.ui->widget_config->isEnabled()) return;
    case MainWindow::ONGLET_VOICE :
        mainWindow.ui->actionInitialize->setEnabled(true);
        mainWindow.ui->actionRandomize->setEnabled(true);
    break;
    case MainWindow::ONGLET_BANK :
        mainWindow.ui->actionCopy->setEnabled(true);
        mainWindow.ui->actionPaste->setEnabled(true);
        mainWindow.ui->actionExchange->setEnabled(true);
    break;
    case MainWindow::ONGLET_SET :
    case MainWindow::ONGLET_OPERATEURS :
        mainWindow.ui->actionInitialize->setEnabled(true);
        mainWindow.ui->actionRandomize->setEnabled(true);
        mainWindow.ui->actionCopy->setEnabled(true);
        mainWindow.ui->actionPaste->setEnabled(true);
        mainWindow.ui->actionExchange->setEnabled(true);
    break;
    default: break;
    }
}

/*****************************************************************************/
void Editeur::ChoisirPageSet(const int page)
{
//Sélectionne la page
    pageSetSel = page;
    if (pageSetSel == 0) {
        mainWindow.ui->frame_set_1->show();
        mainWindow.ui->frame_set_2->hide();
    }else{
        mainWindow.ui->frame_set_2->show();
        mainWindow.ui->frame_set_1->hide();
    }
}

void Editeur::ChoisirPageBank(const int page)
{
//Sélectionne la page
    pageBankSel = page;
    if (pageBankSel == 0) {
        mainWindow.ui->frame_bank_1->show();
        mainWindow.ui->frame_bank_2->hide();
    }else{
        mainWindow.ui->frame_bank_2->show();
        mainWindow.ui->frame_bank_1->hide();
    }
}

/*****************************************************************************/
void Editeur::ChoisirBank(const int bank)
{
//Sélectionne la bank
    bankSel = bank;
    mainWindow.ui->pshBut_bank_cur_1->setChecked(bankSel == 0);
    mainWindow.ui->pshBut_bank_cur_2->setChecked(bankSel == 1);
    mainWindow.ui->pshBut_bank_cur_3->setChecked(bankSel == 2);
    mainWindow.ui->pshBut_bank_cur_4->setChecked(bankSel == 3);
    mainWindow.ui->pshBut_bank_cur_5->setChecked(bankSel == 4);
    mainWindow.ui->pshBut_bank_cur_6->setChecked(bankSel == 5);
    mainWindow.ui->pshBut_bank_cur_7->setChecked(bankSel == 6);
}

void Editeur::ChoisirInstru(const int instru)
{
//Sélectionne l'instrument
    instruSel = instru;
    mainWindow.ui->pshBut_inst_cur_1->setChecked(instruSel == 0);
    mainWindow.ui->pshBut_inst_cur_2->setChecked(instruSel == 1);
    mainWindow.ui->pshBut_inst_cur_3->setChecked(instruSel == 2);
    mainWindow.ui->pshBut_inst_cur_4->setChecked(instruSel == 3);
    mainWindow.ui->pshBut_inst_cur_5->setChecked(instruSel == 4);
    mainWindow.ui->pshBut_inst_cur_6->setChecked(instruSel == 5);
    mainWindow.ui->pshBut_inst_cur_7->setChecked(instruSel == 6);
    mainWindow.ui->pshBut_inst_cur_8->setChecked(instruSel == 7);
}

void Editeur::ChoisirOP(const int OP)
{
//Sélectionne l'opérateur
    OPSel = OP;
    mainWindow.ui->pshBut_op_cur_1->setChecked(OPSel == 0);
    mainWindow.ui->pshBut_op_cur_2->setChecked(OPSel == 1);
    mainWindow.ui->pshBut_op_cur_3->setChecked(OPSel == 2);
    mainWindow.ui->pshBut_op_cur_4->setChecked(OPSel == 3);
}

/*****************************************************************************/
bool Editeur::ActualiserSet()
{
    try {
        set.RecevoirTout();
    }catch (MIDI_ex ex) {
        QMessageBox::warning(&mainWindow, "FB01 SE:", ex.Info());
        return false;
    }
    RafraichirConfig();
    RafraichirSet();
    return true;
}

void Editeur::RafraichirSet(const bool visible)
{
    if (visible) {
    //Rafraichit les contrôles visibles
        if (pageSetSel == 0) {
            mainWindow.ui->widget_instru_1->Rafraichir();
            mainWindow.ui->widget_instru_2->Rafraichir();
            mainWindow.ui->widget_instru_3->Rafraichir();
            mainWindow.ui->widget_instru_4->Rafraichir();
        }else{
            mainWindow.ui->widget_instru_5->Rafraichir();
            mainWindow.ui->widget_instru_6->Rafraichir();
            mainWindow.ui->widget_instru_7->Rafraichir();
            mainWindow.ui->widget_instru_8->Rafraichir();
        }
    }else{
    //Rafraichit tous les contrôles
        mainWindow.ui->widget_instru_1->Rafraichir();
        mainWindow.ui->widget_instru_2->Rafraichir();
        mainWindow.ui->widget_instru_3->Rafraichir();
        mainWindow.ui->widget_instru_4->Rafraichir();
        mainWindow.ui->widget_instru_5->Rafraichir();
        mainWindow.ui->widget_instru_6->Rafraichir();
        mainWindow.ui->widget_instru_7->Rafraichir();
        mainWindow.ui->widget_instru_8->Rafraichir();
    }
}

void Editeur::RafraichirConfig(const bool visible)
{
    mainWindow.ui->widget_config->Rafraichir(visible);
}

/*****************************************************************************/
bool Editeur::ActualiserVoice()
{
    voice.AssocierInstrument(instruSel);
    try {
        voice.RecevoirTout();
    }catch (MIDI_ex ex) {
        QMessageBox::warning(&mainWindow, "FB01 SE:", ex.Info());
        return false;
    }
    RafraichirVoice();
    return true;
}

void Editeur::RafraichirVoice(const bool visible)
{
    if (visible) {
    //Rafraichit les contrôles visibles
        if (mainWindow.ui->tabWidget->currentIndex() == MainWindow::ONGLET_VOICE)
            mainWindow.ui->widget_voice->Rafraichir(true);
        if (mainWindow.ui->tabWidget->currentIndex() == MainWindow::ONGLET_OPERATEURS) {
            mainWindow.ui->widget_opera_1->Rafraichir();
            mainWindow.ui->widget_opera_2->Rafraichir();
            mainWindow.ui->widget_opera_3->Rafraichir();
            mainWindow.ui->widget_opera_4->Rafraichir();
            mainWindow.ui->pshBut_OPon_1->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP1));
            mainWindow.ui->pshBut_OPon_2->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP2));
            mainWindow.ui->pshBut_OPon_3->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP3));
            mainWindow.ui->pshBut_OPon_4->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP4));
        }
    }else{
    //Rafraichit tous les contrôles
        mainWindow.ui->widget_voice->Rafraichir();
        mainWindow.ui->widget_opera_1->Rafraichir();
        mainWindow.ui->widget_opera_2->Rafraichir();
        mainWindow.ui->widget_opera_3->Rafraichir();
        mainWindow.ui->widget_opera_4->Rafraichir();
        mainWindow.ui->pshBut_OPon_1->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP1));
        mainWindow.ui->pshBut_OPon_2->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP2));
        mainWindow.ui->pshBut_OPon_3->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP3));
        mainWindow.ui->pshBut_OPon_4->setChecked(editeur->voice.LireParam(VOICE_ENABLE_OP4));
    }
}

/*****************************************************************************/
bool Editeur::ActualiserBanks()
{
    for (uchar i = 0; i < EDITEUR_NB_BANK; i++)
        if (!ActualiserBank(i)) return false;
    return true;
}

void Editeur::RafraichirBanks()
{
    mainWindow.ui->widget_bank_1->Rafraichir();
    mainWindow.ui->widget_bank_2->Rafraichir();
    mainWindow.ui->widget_bank_3->Rafraichir();
    mainWindow.ui->widget_bank_4->Rafraichir();
    mainWindow.ui->widget_bank_5->Rafraichir();
    mainWindow.ui->widget_bank_6->Rafraichir();
    mainWindow.ui->widget_bank_7->Rafraichir();
}

/*****************************************************************************/
bool Editeur::ActualiserBank(const int bank)
{
    try {
        banks[bank]->RecevoirTout();
    }catch (MIDI_ex ex) {
        QMessageBox::warning(&mainWindow, "FB01 SE:", ex.Info());
        return false;
    }
    RafraichirBank(bank);
    return true;
}

void Editeur::RafraichirBank(const int bank)
{
    switch (bank) {
        case 0 : mainWindow.ui->widget_bank_1->Rafraichir(); break;
        case 1 : mainWindow.ui->widget_bank_2->Rafraichir(); break;
        case 2 : mainWindow.ui->widget_bank_3->Rafraichir(); break;
        case 3 : mainWindow.ui->widget_bank_4->Rafraichir(); break;
        case 4 : mainWindow.ui->widget_bank_5->Rafraichir(); break;
        case 5 : mainWindow.ui->widget_bank_6->Rafraichir(); break;
        case 6 : mainWindow.ui->widget_bank_7->Rafraichir(); break;
        default: break;
    }
}

/*****************************************************************************/
void Editeur::InitialiserAutomation()
{
    mainWindow.ui->widget_automation->InitialiserListes();
}

void Editeur::ActualiserAutomation()
{
    mainWindow.ui->widget_automation->Actualiser();
    RafraichirAutomation();
}

void Editeur::RafraichirAutomation()
{
    mainWindow.ui->widget_automation->Rafraichir();
}

/*****************************************************************************/
void Editeur::AttribuerConfig()
{
//Attribue la configuration
    mainWindow.ui->widget_config->DefinirConfig(&config);
    mainWindow.ui->widget_config->DefinirSet(&set);
}

void Editeur::AttribuerBanks()
{
//Attribue les banks aux contrôles
    mainWindow.ui->widget_bank_1->DefinirBank(banks[0]);
    mainWindow.ui->widget_bank_2->DefinirBank(banks[1]);
    mainWindow.ui->widget_bank_3->DefinirBank(banks[2]);
    mainWindow.ui->widget_bank_4->DefinirBank(banks[3]);
    mainWindow.ui->widget_bank_5->DefinirBank(banks[4]);
    mainWindow.ui->widget_bank_6->DefinirBank(banks[5]);
    mainWindow.ui->widget_bank_7->DefinirBank(banks[6]);
}

void Editeur::AttribuerInstruments()
{
//Attribue les instruments aux contrôles
    mainWindow.ui->widget_instru_1->DefinirInstrument(set.RecupererInstrument(0));
    mainWindow.ui->widget_instru_2->DefinirInstrument(set.RecupererInstrument(1));
    mainWindow.ui->widget_instru_3->DefinirInstrument(set.RecupererInstrument(2));
    mainWindow.ui->widget_instru_4->DefinirInstrument(set.RecupererInstrument(3));
    mainWindow.ui->widget_instru_5->DefinirInstrument(set.RecupererInstrument(4));
    mainWindow.ui->widget_instru_6->DefinirInstrument(set.RecupererInstrument(5));
    mainWindow.ui->widget_instru_7->DefinirInstrument(set.RecupererInstrument(6));
    mainWindow.ui->widget_instru_8->DefinirInstrument(set.RecupererInstrument(7));
}

void Editeur::AttribuerVoice()
{
//Attribue la voice au controle
    mainWindow.ui->widget_voice->DefinirVoice(&voice);
}

void Editeur::AttribuerOperateurs()
{
//Attribue les opérateurs aux contrôles
    mainWindow.ui->widget_opera_1->DefinirOP(voice.RecupererOP(0));
    mainWindow.ui->widget_opera_2->DefinirOP(voice.RecupererOP(1));
    mainWindow.ui->widget_opera_3->DefinirOP(voice.RecupererOP(2));
    mainWindow.ui->widget_opera_4->DefinirOP(voice.RecupererOP(3));
}

/*****************************************************************************/
void Editeur::Actualiser()
{
//Vérifie la configuration
    if (!MIDI::InOk() || !MIDI::OutOk()) return;
    ActualiserEdition(mainWindow.ui->tabWidget->currentIndex());
//Récupère les informations
    if (!ActualiserSet() || !ActualiserVoice()) {
        Reinitialiser();
        return;
    }
//Actualise l'interface
    ActualiserAutomation();
    RafraichirBanks();
//Active l'éditeur
    ActiverMenus(true);
    ActiverOnglets(true);
}

/*****************************************************************************/
void Editeur::Rafraichir(const bool visible)
{
    if (visible) {
        switch (mainWindow.ui->tabWidget->currentIndex()) {
            case MainWindow::ONGLET_CONFIG :     RafraichirConfig(); break;
            case MainWindow::ONGLET_AUTOMATION : RafraichirAutomation(); break;
            case MainWindow::ONGLET_SET :        RafraichirSet(true); break;
            case MainWindow::ONGLET_VOICE :      RafraichirVoice(true); break;
            case MainWindow::ONGLET_OPERATEURS : RafraichirVoice(true); break;
            default: break;
        }
    }else {
        RafraichirAutomation();
        RafraichirBanks();
        RafraichirConfig();
        RafraichirSet();
        RafraichirVoice();
    }
}

/*****************************************************************************/
void Editeur::Reinitialiser()
{
//Déselectionne les drivers
    MIDI::ActiverIn(-1);
    MIDI::ActiverOut(-1);
    MIDI::ActiverCtrl(-1);
    mainWindow.ui->cmbBox_MIDIIn->setCurrentIndex(0);
    mainWindow.ui->cmbBox_MIDIOut->setCurrentIndex(0);
    mainWindow.ui->cmbBox_MIDICtrl->setCurrentIndex(0);
//Vérrouille l'interface
    ActiverMenus(false);
    ActiverOnglets(false);
}

/*****************************************************************************/
const char ChargerTitres[4][25] = {"Load a voice :", "Load a set :", "Load the automations :", "Load a sysex file :"};
const char ChargerExts[4][4]= {"fbv", "fbs", "aut", "syx"};
FILE * Editeur::ChargerFichier(FICHIER_TYPE type, short * version)
{
    FILE * fichier;
    char ext[3];
//Ouvre le sélecteur de fichier
    QString filtre = "*.";
    filtre.append(ChargerExts[type]);
    QString nom = QFileDialog::getOpenFileName(&mainWindow, ChargerTitres[type], filtre);
    if (nom.isEmpty()) return NULL;
//Ouvre le fichier
    fichier = fopen(nom.toAscii().data(), "rb");
    if (fichier == NULL) {
        QMessageBox::warning(&mainWindow, "FB01 SE :", "Unable to open the file !");
        return NULL;
    }
//Vérifie l'entète du fichier
    if (type != FICHIER_SYSEX) {
    //Récupère les informations
        if ((fread(ext, 1, 3, fichier) != 3) || (fread(version, 2, 1, fichier) != 1)) {
            QMessageBox::warning(&mainWindow, "FB01 SE :", "Unable to read the file !");
            fclose(fichier);
            return NULL;
        }
        if (*version != VERSION) {
            QMessageBox::warning(&mainWindow, "FB01 SE :", "Sorry, older editor files are not supported yet !");
            fclose(fichier);
            return NULL;
        }
    //Vérifie le format
        if (strncmp(ChargerExts[type], ext, 3)) {
            QMessageBox::warning(&mainWindow, "FB01 SE :", "Bad file format or corrupted file !");
            fclose(fichier);
            return NULL;
        }
    //Vérifie la version
        if (*version > VERSION) {
            QMessageBox::warning(&mainWindow, "FB01 SE :", "New file format, please download the latest version !");
            fclose(fichier);
            return NULL;
        }
    }
//Retourne le pointeur
    return fichier;
}

char EnregTitres[4][25] = {"Save a voice :", "Save a set :", "Save the automations :", "Save a sysex file :"};
FILE * Editeur::EnregistrerFichier(FICHIER_TYPE type)
{
    FILE * fichier;
//Ouvre le sélecteur
    QString filtre = "*.";
    filtre.append(ChargerExts[type]);
    QString nom = QFileDialog::getSaveFileName(&mainWindow, EnregTitres[type], filtre);
    if (nom.isEmpty()) return NULL;
    if (!nom.endsWith(ChargerExts[type], Qt::CaseInsensitive))
        nom.append(filtre.right(filtre.length() - 1));
//Ouvre le fichier
    fichier = fopen(nom.toAscii().data(), "wb+");
    if (fichier == NULL) {
        QMessageBox::warning(&mainWindow, "FB01 SE :", "Unable to open the file !");
        return NULL;
    }
//Ecrit l'entète
    if (type != FICHIER_SYSEX) {
        short version = VERSION;
        if ((fwrite(ChargerExts[type], 1, 3, fichier) != 3) || (fwrite(&version, 2, 1, fichier) != 1)) {
            QMessageBox::warning(&mainWindow, "FB01 SE :", "Unable to write the file !");
            fclose(fichier);
            return NULL;
        }
    }
//Retourne le pointeur
    return fichier;
}
