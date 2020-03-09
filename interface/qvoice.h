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

#ifndef QVOICE_H
#define QVOICE_H

#include <QWidget>
#include <QPixmap>
#include "../ui_qvoice.h"

#include "../core/midi.h"
#include "../core/voice.h"

#include "../excep/midi_ex.h"
#include "../excep/memory_ex.h"

/*****************************************************************************/
namespace Ui {
    class QVoice;
}

class QVoice : public QWidget {
    Q_OBJECT
public:
//Constantes
    #define QVOICE_NB_ALGO 8
//Constructeurs
    QVoice(QWidget *parent = 0);
    ~QVoice();
//Association classe - contrôle
    void DefinirVoice(Voice * voice);
//Changement des paramètres supplémentaires
    void DefinirAuteur(QString auteur);
    void DefinirComment(QString comment);
//Actualisation graphique
    void Rafraichir(const bool noTexts = false);
protected:
    void changeEvent(QEvent *e);
private:
    Ui::QVoice *m_ui;
    QPixmap algoIcones[QVOICE_NB_ALGO];
    Voice * voice;
    bool attente;
private slots:
    void on_but_algo_valueChanged(int i);
    void on_txtEdit_voicename_textChanged();
    void on_cmbBox_style_activated(int i);
    void on_but_feedback_valueChanged(int i);
    void on_but_trans_valueChanged(int i);
    void on_pshBut_poly_clicked(bool checked);
    void on_but_porta_valueChanged(int i);
    void on_but_pitch_valueChanged(int i);
    void on_cmbBox_pmdctl_activated(int i);
    void on_but_LFOspeed_valueChanged(int i);
    void on_cmbBox_LFOwave_activated(int i);
    void on_pshBut_LFOload_clicked(bool checked);
    void on_pshBut_LFOsync_clicked(bool checked);
    void on_but_AMD_valueChanged(int i);
    void on_but_AMS_valueChanged(int i);
    void on_but_PMD_valueChanged(int i);
    void on_but_PMS_valueChanged(int i);
    void on_txtEdit_author_textChanged();
    void on_txtEdit_comments_textChanged();
};

#endif // QVOICE_H
