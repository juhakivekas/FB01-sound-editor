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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QList>

#include "../ui_mainwindow.h"

#include "../core/midi.h"

#include "qbank.h"
#include "qinstrument.h"
#include "qvoice.h"
#include "qoperateur.h"

namespace Ui
{
    class MainWindow;
}

/*****************************************************************************/
class MainWindow : public QMainWindow{
    Q_OBJECT
    friend class Editeur;
public:
//Constantes générales
    typedef enum {
        ONGLET_CONFIG = 0,
        ONGLET_AUTOMATION,
        ONGLET_BANK,
        ONGLET_SET,
        ONGLET_VOICE,
        ONGLET_OPERATEURS
    }ONGLET_NOM;
//Constructeurs
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::MainWindow *ui;
private slots:
//Callbacks MIDI
    void on_cmbBox_MIDICtrl_activated(int index);
    void on_cmbBox_MIDIIn_activated(int index);
    void on_cmbBox_MIDIOut_activated(int index);
    void on_pshBut_refresh_midi_pressed();
    void on_pshBut_INToOUT_clicked(bool checked);
    void on_pshBut_CTRLToOUT_clicked(bool checked);
//Callbacks clavier virtuel
    void on_but_kybchan_valueChanged(int i);
    void on_but_kybvelo_valueChanged(int i);
    void on_pshBut_kybQWERTY_clicked(bool checked);
    void on_pshBut_kybAZERTY_clicked(bool checked);
//Menu Fichier
    void on_actionQuit_triggered(bool checked = false);
    void on_actionLoad_voice_triggered(bool checked = false);
    void on_actionSave_voice_triggered(bool checked = false);
    void on_actionLoad_set_triggered(bool checked = false);
    void on_actionSave_set_triggered(bool checked = false);
    void on_actionLoad_automations_triggered(bool checked = false);
    void on_actionSave_automations_triggered(bool checked = false);
    void on_actionImport_bank_triggered(bool checked = false);
    void on_actionExport_bank_triggered(bool checked = false);
    void on_actionImport_set_triggered(bool checked = false);
    void on_actionExport_set_triggered(bool checked = false);
    void on_actionImport_voice_triggered(bool checked = false);
    void on_actionExport_voice_triggered(bool checked = false);
//Menu édition
    void on_actionInitialize_triggered(bool checked = false);
    void on_actionRandomize_triggered(bool checked = false);
    void on_actionCopy_triggered(bool checked = false);
    void on_actionPaste_triggered(bool checked = false);
    void on_actionExchange_triggered(bool checked = false);
    void on_actionSend_current_config_triggered(bool checked = false);
    void on_actionSend_current_bank_triggered(bool checked = false);
    void on_actionGet_current_bank_triggered(bool checked = false);
    void on_actionSend_current_set_triggered(bool checked = false);
    void on_actionGet_current_set_triggered(bool checked = false);
    void on_actionSend_current_voice_triggered(bool checked = false);
    void on_actionGet_current_voice_triggered(bool checked = false);
//Menu aide
    void on_actionAbout_FB01SE_triggered(bool checked = false);
    void on_actionAbout_Qt_triggered(bool checked = false);
    void on_actionRead_this_triggered(bool checked = false);
//Sélection des banks
    void on_pshBut_bank_cur_1_clicked(bool checked);
    void on_pshBut_bank_cur_2_clicked(bool checked);
    void on_pshBut_bank_cur_3_clicked(bool checked);
    void on_pshBut_bank_cur_4_clicked(bool checked);
    void on_pshBut_bank_cur_5_clicked(bool checked);
    void on_pshBut_bank_cur_6_clicked(bool checked);
    void on_pshBut_bank_cur_7_clicked(bool checked);
//Edition des banks
    void on_pshBut_loadCurrent_pressed();
    void on_pshBut_loadAll_pressed();
    void on_pshBut_copyBank_pressed();
    void on_pshBut_pasteBank_pressed();
    void on_pshBut_exchangeBank_pressed();
//Sélection des instruments
    void on_pshBut_inst_cur_1_clicked(bool checked);
    void on_pshBut_inst_cur_2_clicked(bool checked);
    void on_pshBut_inst_cur_3_clicked(bool checked);
    void on_pshBut_inst_cur_4_clicked(bool checked);
    void on_pshBut_inst_cur_5_clicked(bool checked);
    void on_pshBut_inst_cur_6_clicked(bool checked);
    void on_pshBut_inst_cur_7_clicked(bool checked);
    void on_pshBut_inst_cur_8_clicked(bool checked);
//Sélection des opérateurs
    void on_pshBut_op_cur_1_clicked(bool checked);
    void on_pshBut_op_cur_2_clicked(bool checked);
    void on_pshBut_op_cur_3_clicked(bool checked);
    void on_pshBut_op_cur_4_clicked(bool checked);
//Callbacks choix de page
    void on_pshBut_nextSet_pressed();
    void on_pshBut_nextBank_pressed();
//Callbacks activation
    void on_pshBut_OPon_1_clicked(bool checked);
    void on_pshBut_OPon_2_clicked(bool checked);
    void on_pshBut_OPon_3_clicked(bool checked);
    void on_pshBut_OPon_4_clicked(bool checked);
//Changement d'onglet
    void on_tabWidget_currentChanged(int index);
};

#endif // MAINWINDOW_H
