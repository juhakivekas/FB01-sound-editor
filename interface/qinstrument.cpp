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

#include "qinstrument.h"

/*****************************************************************************/
QInstrument::QInstrument(QWidget *parent) : QWidget(parent), m_ui(new Ui::QInstrument)
{
    m_ui->setupUi(this);
//Initialise la liste des notes
    attente = true;
    InitialiserNotes(m_ui->cmbBox_upper);
    InitialiserNotes(m_ui->cmbBox_lower);
//Initialise certains boutons
    m_ui->but_channel->setValue(1);
    m_ui->but_bank->setValue(1);
    m_ui->but_voice->setValue(1);
    attente = false;
}

QInstrument::~QInstrument()
{
    delete m_ui;
}

/*****************************************************************************/
void QInstrument::Rafraichir()
{
    attente = true;
//Actualise le contenu des contrôles
    m_ui->but_notes->setValue(instrument->LireParam(INSTRU_NB_NOTES));
    m_ui->but_channel->setValue(instrument->LireParam(INSTRU_CHANNEL) + 1);
    m_ui->cmbBox_upper->setCurrentIndex(instrument->LireParam(INSTRU_UPPER));
    m_ui->cmbBox_lower->setCurrentIndex(instrument->LireParam(INSTRU_LOWER));
    m_ui->but_bank->setValue(instrument->LireParam(INSTRU_BANK) + 1);
    m_ui->but_voice->setValue(instrument->LireParam(INSTRU_VOICE) + 1);
    m_ui->but_detune->setValue(((char) instrument->LireParam(INSTRU_DETUNE)));
    m_ui->but_trans->setValue(instrument->LireParam(INSTRU_TRANS) - 2);
    m_ui->hzSlider_volume->setValue(instrument->LireParam(INSTRU_VOLUME));
    m_ui->hzSlider_pan->setValue(instrument->LireParam(INSTRU_PAN));
    m_ui->pshBut_LFO->setChecked(!instrument->LireParam(INSTRU_LFO));
    m_ui->but_porta->setValue(instrument->LireParam(INSTRU_PORTAMENTO));
    m_ui->but_pitch->setValue(instrument->LireParam(INSTRU_PITCHBEND));
    m_ui->pshBut_poly->setChecked(!instrument->LireParam(INSTRU_POLY));
    m_ui->cmbBox_pmdctl->setCurrentIndex(min(instrument->LireParam(INSTRU_CONTROLLER), 4));
    attente = false;
}

/*****************************************************************************/
void QInstrument::DefinirInstrument(Instrument * instrument)
{
    QString num;
    this->instrument = instrument;
    num.setNum(instrument->LireId() + 1, 10);
    this->m_ui->grpBox_instru->setTitle((QString) "Instrument " + num + " :");
}

/*****************************************************************************/
const char notes[12][3] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B"};
void QInstrument::InitialiserNotes(QComboBox * box)
{
    box->clear();
    for (int n = 0; n < 128; n ++)
    {
    //Créé le nom de la note
        QString num; num.setNum(n / 12 - 2);
        QString item = notes[n % 12];
        item.append(" "); item.append(num);
    //Ajoute à la liste
        box->addItem(item, 0);
    }
}

/*****************************************************************************/
void QInstrument::on_but_notes_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_NB_NOTES, i);
}

void QInstrument::on_but_channel_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_CHANNEL, i - 1);
}

void QInstrument::on_cmbBox_upper_activated(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_UPPER, i);
}

void QInstrument::on_cmbBox_lower_activated(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_LOWER, i);
}

void QInstrument::on_but_bank_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_BANK, i - 1);
}

void QInstrument::on_but_voice_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_VOICE, i - 1);
}

void QInstrument::on_but_detune_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_DETUNE, i);
}

void QInstrument::on_but_trans_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_TRANS, i + 2);
}

void QInstrument::on_hzSlider_volume_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_VOLUME, i);
}

void QInstrument::on_hzSlider_pan_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_PAN, i);
}

void QInstrument::on_pshBut_LFO_clicked(bool checked)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_LFO, !checked);
}

void QInstrument::on_pshBut_poly_clicked(bool checked)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_POLY, !checked);
}

void QInstrument::on_but_porta_valueChanged(int i)
{
    if (!attente)
    instrument->EcrireParam(INSTRU_PORTAMENTO, i);
}

void QInstrument::on_but_pitch_valueChanged(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_PITCHBEND, i);
}

void QInstrument::on_cmbBox_pmdctl_activated(int i)
{
    if (!attente)
        instrument->EcrireParam(INSTRU_CONTROLLER, i);
}

/*****************************************************************************/
void QInstrument::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
