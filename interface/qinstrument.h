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

#ifndef QINSTRUMENT_H
#define QINSTRUMENT_H

#include <QWidget>
#include "../ui_qinstrument.h"

#include "../core/midi.h"
#include "../core/instrument.h"

#include "../excep/midi_ex.h"
#include "../excep/memory_ex.h"

namespace Ui {
    class QInstrument;
}

/*****************************************************************************/
class QInstrument : public QWidget {
    Q_OBJECT
public:
//Constructeurs
    QInstrument(QWidget *parent = 0);
    ~QInstrument();
//Association classe - contrôle
    void DefinirInstrument(Instrument * instrument);
//Actualisation graphique
    void Rafraichir();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::QInstrument *m_ui;
    Instrument * instrument;
//Fonctions internes
    bool attente;
    void InitialiserNotes(QComboBox * box);
private slots:
    void on_but_notes_valueChanged(int i);
    void on_but_channel_valueChanged(int i);
    void on_cmbBox_upper_activated(int i);
    void on_cmbBox_lower_activated(int i);
    void on_but_bank_valueChanged(int i);
    void on_but_voice_valueChanged(int i);
    void on_but_detune_valueChanged(int i);
    void on_but_trans_valueChanged(int i);
    void on_hzSlider_volume_valueChanged(int i);
    void on_hzSlider_pan_valueChanged(int i);
    void on_pshBut_LFO_clicked(bool checked);
    void on_pshBut_poly_clicked(bool checked);
    void on_but_porta_valueChanged(int i);
    void on_but_pitch_valueChanged(int i);
    void on_cmbBox_pmdctl_activated(int i);
};

#endif // QINSTRUMENT_H
