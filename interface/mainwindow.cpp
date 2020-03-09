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
#include "mainwindow.h"

extern QApplication * application;
extern Editeur * editeur;

/*****************************************************************************/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
//Initialise l'interface
    ui->setupUi(this);
    ui->but_kybchan->setValue(1);
    ui->but_kybvelo->setValue(100);
//Initialise certaines valeurs par défaut
    MIDI::ActiverINToOUT(true);
    MIDI::ActiverCTRLToOUT(true);
    MIDI::ChoisirSysChannel(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*****************************************************************************/
void MainWindow::on_actionLoad_automations_triggered(bool checked)
{
    short version;
//Ouvre le fichier
    FILE * fichier = editeur->ChargerFichier(Editeur::FICHIER_AUTOMATION, &version);
    if (fichier == NULL) return;
//Charge les automations
    if (!Automation::Charger(fichier, version))
        QMessageBox::warning(this, "FB01 SE :", "Error reading file !");
    editeur->ActualiserAutomation();
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionSave_automations_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->EnregistrerFichier(Editeur::FICHIER_AUTOMATION);
    if (fichier == NULL) return;
//Enregistre les automations
    if (!Automation::Enregistrer(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error writing file !");
//Ferme le fichier
    fclose(fichier);
}

/*****************************************************************************/
void MainWindow::on_actionLoad_set_triggered(bool checked)
{
    short version;
//Ouvre le fichier
    FILE * fichier = editeur->ChargerFichier(Editeur::FICHIER_SET, &version);
    if (fichier == NULL) return;
//Charge les propriétés du set
    if (!editeur->set.Charger(fichier, version))
        QMessageBox::warning(this, "FB01 SE :", "Error reading file !");
    editeur->RafraichirConfig();
    editeur->RafraichirSet();
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionSave_set_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->EnregistrerFichier(Editeur::FICHIER_SET);
    if (fichier == NULL) return;
//Enregistre les propriétés du set
    if (!editeur->set.Enregistrer(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error writing file !");
//Ferme le fichier
    fclose(fichier);
}

/*****************************************************************************/
void MainWindow::on_actionLoad_voice_triggered(bool checked)
{
    short version;
//Ouvre le fichier
    FILE * fichier = editeur->ChargerFichier(Editeur::FICHIER_VOICE, &version);
    if (fichier == NULL) return;
//Charge la voice
    if (!editeur->voice.Charger(fichier, version))
        QMessageBox::warning(this, "FB01 SE :", "Error reading file !");
    editeur->RafraichirVoice();
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionSave_voice_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->EnregistrerFichier(Editeur::FICHIER_VOICE);
    if (fichier == NULL) return;
//Charge la voice
    if (!editeur->voice.Enregistrer(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error writing file !");
//Ferme le fichier
    fclose(fichier);
}

/*****************************************************************************/
void MainWindow::on_actionImport_bank_triggered(bool checked)
{
//Vérifie la possibilité
    if (editeur->bankSel > 1) {
        QMessageBox::warning(this, "FB01 SE :", "The editor cannot overwrite the rom banks !");
        return;
    }
//Ouvre le fichier
    FILE * fichier = editeur->ChargerFichier(Editeur::FICHIER_SYSEX, NULL);
    if (fichier == NULL) return;
//Importe la bank
    if (!editeur->banks[editeur->bankSel]->Importe(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error reading file !");
    editeur->RafraichirBank(editeur->bankSel);
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionExport_bank_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->EnregistrerFichier(Editeur::FICHIER_SYSEX);
    if (fichier == NULL) return;
//Exporte la bank
    if (!editeur->banks[editeur->bankSel]->Exporte(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error writing file !");
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionImport_set_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->ChargerFichier(Editeur::FICHIER_SYSEX, NULL);
    if (fichier == NULL) return;
//Importe le set
    if (!editeur->set.Importe(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error reading file !");
    editeur->RafraichirConfig();
    editeur->RafraichirSet();
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionExport_set_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->EnregistrerFichier(Editeur::FICHIER_SYSEX);
    if (fichier == NULL) return;
//Exporte le set
    if (!editeur->set.Exporte(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error writing file !");
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionImport_voice_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->ChargerFichier(Editeur::FICHIER_SYSEX, NULL);
    if (fichier == NULL) return;
//Importe la voice
    if (!editeur->voice.Importe(fichier))
            QMessageBox::warning(this, "FB01 SE :", "Error reading file !");
    editeur->RafraichirVoice();
//Ferme le fichier
    fclose(fichier);
}

void MainWindow::on_actionExport_voice_triggered(bool checked)
{
//Ouvre le fichier
    FILE * fichier = editeur->EnregistrerFichier(Editeur::FICHIER_SYSEX);
    if (fichier == NULL) return;
//Exporte la voice
    if (!editeur->voice.Exporte(fichier))
        QMessageBox::warning(this, "FB01 SE :", "Error writing file !");
//Ferme le fichier
    fclose(fichier);
}

/*****************************************************************************/
void MainWindow::on_actionInitialize_triggered(bool checked)
{
    switch(ui->tabWidget->currentIndex()) {
    case ONGLET_CONFIG :
        editeur->set.Initialiser();
        editeur->RafraichirConfig();
        editeur->RafraichirSet();
        break;
    case ONGLET_SET :
        editeur->set.RecupererInstrument(editeur->instruSel)->Initialiser();
        editeur->RafraichirSet();
        break;
    case ONGLET_VOICE :
        editeur->voice.Initialiser();
        editeur->RafraichirVoice();
        break;
    case ONGLET_OPERATEURS :
        editeur->voice.RecupererOP(editeur->OPSel)->Initialiser();
        editeur->RafraichirVoice();
        break;
    }
}

void MainWindow::on_actionRandomize_triggered(bool checked)
{
    switch(ui->tabWidget->currentIndex()) {
    case ONGLET_CONFIG :
        editeur->set.Randomiser();
        editeur->RafraichirConfig();
        break;
    case ONGLET_SET :
        editeur->set.RecupererInstrument(editeur->instruSel)->Randomiser();
        editeur->RafraichirSet();
        break;
    case ONGLET_VOICE :
        editeur->voice.Randomiser();
        editeur->RafraichirVoice();
        break;
    case ONGLET_OPERATEURS :
        editeur->voice.RecupererOP(editeur->OPSel)->Randomiser();
        editeur->RafraichirVoice();
        break;
    }
}

/*****************************************************************************/
void MainWindow::on_actionCopy_triggered(bool checked)
{
    switch(ui->tabWidget->currentIndex()) {
    case ONGLET_BANK :
        editeur->banks[editeur->bankSel]->CopierVoice(&editeur->copie);
        break;
    case ONGLET_SET :
        editeur->set.RecupererInstrument(editeur->instruSel)->Copier(&editeur->copie);
        break;
    case ONGLET_OPERATEURS :
        editeur->voice.RecupererOP(editeur->OPSel)->Copier(&editeur->copie);
        break;
    default: break;
    }
}

void MainWindow::on_actionPaste_triggered(bool checked)
{
    switch(ui->tabWidget->currentIndex()) {
    case ONGLET_BANK :
        editeur->banks[editeur->bankSel]->CollerVoice(&editeur->copie);
        editeur->RafraichirBank(editeur->bankSel);
        break;
    case ONGLET_SET :
        editeur->set.RecupererInstrument(editeur->instruSel)->Coller(&editeur->copie);
        editeur->RafraichirSet();
        break;
    case ONGLET_OPERATEURS :
        editeur->voice.RecupererOP(editeur->OPSel)->Coller(&editeur->copie);
        editeur->RafraichirVoice();
        break;
    default: break;
    }
}

void MainWindow::on_actionExchange_triggered(bool checked)
{
    switch(ui->tabWidget->currentIndex()) {
    case ONGLET_BANK :
        editeur->banks[editeur->bankSel]->EchangerVoice(&editeur->copie);
        editeur->RafraichirBank(editeur->bankSel);
        break;
    case ONGLET_SET :
        editeur->set.RecupererInstrument(editeur->instruSel)->Echanger(&editeur->copie);
        editeur->RafraichirSet();
        break;
    case ONGLET_OPERATEURS :
        editeur->voice.RecupererOP(editeur->OPSel)->Echanger(&editeur->copie);
        editeur->RafraichirVoice();
        break;
    default: break;
    }
}

/*****************************************************************************/
void MainWindow::on_actionSend_current_config_triggered(bool checked)
{
    try {
        editeur->config.EnvoyerTout();
    }catch(MIDI_ex ex) {
        QMessageBox::warning(this, "FB01 SE:", ex.Info());
    }
}

/*****************************************************************************/
void MainWindow::on_actionSend_current_bank_triggered(bool checked)
{
//Vérifie la bank sélectionnée
    if (editeur->bankSel >= 2) {
        QMessageBox::information(this, "FB01 SE:", "Cannot send the rom banks !");
        return;
    }
//Envoie le contenu
    try {
        editeur->banks[editeur->bankSel]->EnvoyerTout();
    }catch(MIDI_ex ex) {
        QMessageBox::warning(this, "FB01 SE:", ex.Info());
    }
}

void MainWindow::on_actionGet_current_bank_triggered(bool checked)
{
    editeur->ActualiserBank(editeur->bankSel);
}

/*****************************************************************************/
void MainWindow::on_actionSend_current_set_triggered(bool checked)
{
    try {
        editeur->set.EnvoyerTout();
    }catch(MIDI_ex ex) {
        QMessageBox::warning(this, "FB01 SE:", ex.Info());
    }
}

void MainWindow::on_actionGet_current_set_triggered(bool checked)
{
    editeur->ActualiserSet();
}

/*****************************************************************************/
void MainWindow::on_actionSend_current_voice_triggered(bool checked)
{
    try {
        editeur->voice.EnvoyerTout();
    }catch(MIDI_ex ex) {
        QMessageBox::warning(this, "FB01 SE:", ex.Info());
    }
}

void MainWindow::on_actionGet_current_voice_triggered(bool checked)
{
    editeur->ActualiserVoice();
}

/*****************************************************************************/
void MainWindow::on_actionQuit_triggered(bool checked)
{
    application->quit();
}

/*****************************************************************************/
void MainWindow::on_actionAbout_FB01SE_triggered(bool checked)
{
    QString text;
//Informations sur le logiciel
    text.append("FB01 Sound Editor : V2.1 (20/07/10)\n");
    text.append("Copyright Meslin Frederic 2009 - 2010\n\n");
    text.append("This is an open source computer editor for the Yamaha FB01 sound module.\n");
    text.append("This program is under a GPL license, for legal informations, please read the COPYING file.\n\n");
    text.append("In order to support the project or download the latest release, try the following links :\n\n");
    text.append("Main website     : http://sourceforge.net/projects/fb01editor/\n");
    text.append("Developer's mail : fredericmeslin@hotmail.com\n\n");
    text.append("Thank you very much for using my programs.\n");
    QMessageBox::information(this, "FB01 SE :", text);
}

void MainWindow::on_actionAbout_Qt_triggered(bool checked)
{
    QString Text;
//Informations sur la bibliothèque Qt
    QMessageBox::aboutQt(this, "FB01 SE:");
}

/*****************************************************************************/
void MainWindow::on_actionRead_this_triggered(bool checked)
{
    QString text;
//Informations supplémentaires
    text.append("FB01 Sound Editor : V2.1 (20/07/10)\nCopyright Meslin Frederic 2009 - 2010\n\n");
    text.append("This program was developped carefully but it might still contain ");
    text.append("bugs and features that need to be improved, so try to visit ");
    text.append("sometimes the main website to get the latest editor release.\n\n");
    text.append("As you can figure out, developping such a project needs a lot of time. ");
    text.append("While I am doing that, I do not work and I do not record music at all. ");
    text.append("I will release soon a CD version with the editor, a complete FM tutorial ");
    text.append("and a complete library of sounds ready to use.\n\n");
    text.append("If you want to support my work and take advantage of the tutorial and ");
    text.append("the sound library, please contact me to order this CD version.\n");
    QMessageBox::information(this, "FB01 SE :", text);
}

/*****************************************************************************/
void MainWindow::on_cmbBox_MIDICtrl_activated(int index)
{
    if (index < 1) {
    //Déselectionne le driver
        MIDI::ActiverCtrl(-1);
        ui->tab_automation->setEnabled(false);
    }else{
    //Sélectionne le driver
        try {
            MIDI::ActiverCtrl(index - 1);
        }catch (MIDI_ex ex) {
            ui->cmbBox_MIDICtrl->setCurrentIndex(0);
            QMessageBox::warning(this, "FB01 SE:", ex.Info());
            return;
        }
    //Active les automations
        ui->tab_automation->setEnabled(true);
    }
}

void MainWindow::on_cmbBox_MIDIIn_activated(int index)
{
    if (index < 1) {
    //Déselectionne le driver
        MIDI::ActiverIn(-1);
        editeur->ActiverOnglets(false);
        editeur->ActiverMenus(false);
    }else{
    //Sélectionne le driver
        try {
            MIDI::ActiverIn(index - 1);
        }catch (MIDI_ex ex){
            ui->cmbBox_MIDIIn->setCurrentIndex(0);
            QMessageBox::warning(this, "FB01 SE:", ex.Info());
            return;
        }
    //Actualise l'éditeur
        editeur->Actualiser();
    }
}

void MainWindow::on_cmbBox_MIDIOut_activated(int index)
{
    if (index < 1) {
    //Déselectionne le driver
        MIDI::ActiverOut(-1);
        editeur->ActiverOnglets(false);
        editeur->ActiverMenus(false);
    }else{
    //Sélectionne le driver
        try {
            MIDI::ActiverOut(index - 1);
        }catch (MIDI_ex ex) {
        //Désélectionne le driver
            ui->cmbBox_MIDIOut->setCurrentIndex(0);
            QMessageBox::warning(this, "FB01 SE:", ex.Info());
            return;
        }
    //Actualise l'éditeur
        editeur->Actualiser();
    }
}

void MainWindow::on_pshBut_refresh_midi_pressed()
{
//Initialise la liste des drivers
    ui->cmbBox_MIDICtrl->clear();
    ui->cmbBox_MIDIIn->clear();
    ui->cmbBox_MIDIOut->clear();
    ui->cmbBox_MIDICtrl->addItem((QString) "No driver selected", 0);
    ui->cmbBox_MIDIIn->addItem((QString) "No driver selected", 0);
    ui->cmbBox_MIDIOut->addItem((QString) "No driver selected", 0);
//Initialise la sélection
    ui->cmbBox_MIDICtrl->setCurrentIndex(0);
    ui->cmbBox_MIDIIn->setCurrentIndex(0);
    ui->cmbBox_MIDIOut->setCurrentIndex(0);
//Enumère les drivers
    try {
        MIDI::EnumererDrivers();
    }catch (MIDI_ex ex) {
        QMessageBox::warning(this, "FB01 SE:", ex.Info());
        return;
    }
//Ajoute les périphériques
    int nbIns  = MIDI::NbDriversIn();
    for (int i = 0; i < nbIns; i++) {
        ui->cmbBox_MIDICtrl->addItem((QString)MIDI::DriverIn(i), i+1);
        ui->cmbBox_MIDIIn->addItem((QString)MIDI::DriverIn(i), i+1);
    }
    int nbOuts = MIDI::NbDriversOut();
    for (int i = 0; i < nbOuts; i++)
        ui->cmbBox_MIDIOut->addItem((QString)MIDI::DriverOut(i), i+1);
}

/*****************************************************************************/
void MainWindow::on_pshBut_INToOUT_clicked(bool checked)
{
    MIDI::ActiverINToOUT(checked);
}

void MainWindow::on_pshBut_CTRLToOUT_clicked(bool checked)
{
    MIDI::ActiverCTRLToOUT(checked);
}

/*****************************************************************************/
void MainWindow::on_but_kybchan_valueChanged(int i)
{
    MIDI::ChoisirMidiChannel(i-1);
}

void MainWindow::on_but_kybvelo_valueChanged(int i)
{
    MIDI::ChoisirVelocity(i);
}

void MainWindow::on_pshBut_kybQWERTY_clicked(bool checked)
{
//Active la prise en charge du clavier
    if (checked) {
        ui->lbl_clavier->ChoisirClavier(QClavier::CLAVIER_QWERTY);
        ui->pshBut_kybAZERTY->setChecked(false);
    }else ui->lbl_clavier->ChoisirClavier(QClavier::CLAVIER_AUCUNE);
}

void MainWindow::on_pshBut_kybAZERTY_clicked(bool checked)
{
//Active la prise en charge du clavier
    if (checked) {
        ui->lbl_clavier->ChoisirClavier(QClavier::CLAVIER_AZERTY);
        ui->pshBut_kybQWERTY->setChecked(false);
    }else ui->lbl_clavier->ChoisirClavier(QClavier::CLAVIER_AUCUNE);
}

/*****************************************************************************/
void MainWindow::on_pshBut_bank_cur_1_clicked(bool checked)
{
    editeur->ChoisirBank(0);
}

void MainWindow::on_pshBut_bank_cur_2_clicked(bool checked)
{
    editeur->ChoisirBank(1);
}

void MainWindow::on_pshBut_bank_cur_3_clicked(bool checked)
{
    editeur->ChoisirBank(2);
}

void MainWindow::on_pshBut_bank_cur_4_clicked(bool checked)
{
    editeur->ChoisirBank(3);
}

void MainWindow::on_pshBut_bank_cur_5_clicked(bool checked)
{
    editeur->ChoisirBank(4);
}

void MainWindow::on_pshBut_bank_cur_6_clicked(bool checked)
{
    editeur->ChoisirBank(5);
}

void MainWindow::on_pshBut_bank_cur_7_clicked(bool checked)
{
    editeur->ChoisirBank(6);
}

/*****************************************************************************/
void MainWindow::on_pshBut_loadAll_pressed()
{
    if (QMessageBox::question(this, "FB01 SE :", "Loading all the banks at once takes some time,\nwould you like to continue ?",
                              QMessageBox::Yes, QMessageBox::No) == QMessageBox::No) return;
    editeur->ActualiserBanks();
}

void MainWindow::on_pshBut_loadCurrent_pressed()
{
    editeur->ActualiserBank(editeur->bankSel);
}

void MainWindow::on_pshBut_copyBank_pressed()
{
    editeur->banks[editeur->bankSel]->Copier(&editeur->copie);
}

void MainWindow::on_pshBut_pasteBank_pressed()
{
    editeur->banks[editeur->bankSel]->Coller(&editeur->copie);
    editeur->RafraichirBank(editeur->bankSel);
}

void MainWindow::on_pshBut_exchangeBank_pressed()
{
    editeur->banks[editeur->bankSel]->Echanger(&editeur->copie);
    editeur->RafraichirBank(editeur->bankSel);
}

/*****************************************************************************/
void MainWindow::on_pshBut_inst_cur_1_clicked(bool checked)
{
    editeur->ChoisirInstru(0);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_2_clicked(bool checked)
{
    editeur->ChoisirInstru(1);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_3_clicked(bool checked)
{
    editeur->ChoisirInstru(2);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_4_clicked(bool checked)
{
    editeur->ChoisirInstru(3);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_5_clicked(bool checked)
{
    editeur->ChoisirInstru(4);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_6_clicked(bool checked)
{
    editeur->ChoisirInstru(5);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_7_clicked(bool checked)
{
    editeur->ChoisirInstru(6);
    editeur->ActualiserVoice();
}

void MainWindow::on_pshBut_inst_cur_8_clicked(bool checked)
{
    editeur->ChoisirInstru(7);
    editeur->ActualiserVoice();
}

/*****************************************************************************/
void MainWindow::on_pshBut_op_cur_1_clicked(bool checked)
{
    editeur->ChoisirOP(0);
}

void MainWindow::on_pshBut_op_cur_2_clicked(bool checked)
{
    editeur->ChoisirOP(1);
}

void MainWindow::on_pshBut_op_cur_3_clicked(bool checked)
{
    editeur->ChoisirOP(2);
}

void MainWindow::on_pshBut_op_cur_4_clicked(bool checked)
{
    editeur->ChoisirOP(3);
}

/*****************************************************************************/
void MainWindow::on_pshBut_OPon_1_clicked(bool checked)
{
    editeur->voice.EcrireParam(VOICE_ENABLE_OP1, checked);
}

void MainWindow::on_pshBut_OPon_2_clicked(bool checked)
{
    editeur->voice.EcrireParam(VOICE_ENABLE_OP2, checked);
}

void MainWindow::on_pshBut_OPon_3_clicked(bool checked)
{
    editeur->voice.EcrireParam(VOICE_ENABLE_OP3, checked);
}

void MainWindow::on_pshBut_OPon_4_clicked(bool checked)
{
    editeur->voice.EcrireParam(VOICE_ENABLE_OP4, checked);
}

/*****************************************************************************/
void MainWindow::on_pshBut_nextSet_pressed()
{
    editeur->ChoisirPageSet(1 - editeur->pageSetSel);
}

void MainWindow::on_pshBut_nextBank_pressed()
{
    editeur->ChoisirPageBank(1 - editeur->pageBankSel);
}

/*****************************************************************************/
void MainWindow::on_tabWidget_currentChanged(int index)
{
    editeur->ActualiserEdition(index);
}

/*****************************************************************************/
void MainWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
