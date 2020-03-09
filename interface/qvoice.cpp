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

#include "qvoice.h"

/*****************************************************************************/
QString algoNoms[] = {":/ALGO1", ":/ALGO2", ":/ALGO3", ":/ALGO4", ":/ALGO5", ":/ALGO6", ":/ALGO7", ":/ALGO8"};
QVoice::QVoice(QWidget *parent) : QWidget(parent), m_ui(new Ui::QVoice)
{
//Initialise la classe
    m_ui->setupUi(this);
    for (uint i = 0; i < QVOICE_NB_ALGO; i++)
        algoIcones[i].load(algoNoms[i]);
//Initialise certains contrôles
    attente = true;
    m_ui->but_algo->setValue(1);
    m_ui->lbl_algo->setPixmap(algoIcones[0]);
    attente = false;
}

QVoice::~QVoice()
{
    delete m_ui;
}

/*****************************************************************************/
void QVoice::DefinirVoice(Voice * voice)
{
    this->voice = voice;
}

/*****************************************************************************/
void QVoice::Rafraichir(const bool noTexts)
{
    attente = true;
//Actualise les paramêtres principaux
    m_ui->but_algo->setValue(voice->LireParam(VOICE_ALGORITHM) + 1);
    m_ui->cmbBox_LFOwave->setCurrentIndex(voice->LireParam(VOICE_LFO_WAVE));
    m_ui->but_LFOspeed->setValue(voice->LireParam(VOICE_LFO_SPEED));
    m_ui->pshBut_LFOload->setChecked(!voice->LireParam(VOICE_LFO_LOAD));
    m_ui->pshBut_LFOsync->setChecked(!voice->LireParam(VOICE_LFO_SYNC));
    m_ui->but_feedback->setValue(voice->LireParam(VOICE_FEEDBACK));
    m_ui->but_AMD->setValue(voice->LireParam(VOICE_LFO_AMD));
    m_ui->but_PMD->setValue(voice->LireParam(VOICE_LFO_PMD));
    m_ui->but_PMS->setValue(voice->LireParam(VOICE_LFO_PMS));
    m_ui->but_AMS->setValue(voice->LireParam(VOICE_LFO_AMS));
    m_ui->but_trans->setValue(voice->LireParam(VOICE_TRANSPOSE) - 2);
    m_ui->pshBut_poly->setChecked(!voice->LireParam(VOICE_POLY));
    m_ui->but_porta->setValue(voice->LireParam(VOICE_PORTAMENTO));
    m_ui->but_pitch->setValue(voice->LireParam(VOICE_PITCHBEND));
    m_ui->cmbBox_pmdctl->setCurrentIndex(min(voice->LireParam(VOICE_CONTROLLER), 4));
    m_ui->cmbBox_style->setCurrentIndex(min(voice->LireParam(VOICE_USERCODE), 14));
//Actualise les paramêtres textuels
    if (!noTexts) {
        m_ui->txtEdit_voicename->setPlainText((QString) voice->LireNom());
        m_ui->txtEdit_voicename->repaint();
        m_ui->txtEdit_author->setPlainText((QString) voice->LireAuteur());
        m_ui->txtEdit_author->repaint();
        m_ui->txtEdit_comments->setPlainText((QString) voice->LireCommentaires());
        m_ui->txtEdit_comments->repaint();
    }
    attente = false;
}

/*****************************************************************************/
void QVoice::on_but_algo_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_ALGORITHM, i - 1);
    m_ui->lbl_algo->setPixmap(algoIcones[i - 1]);
}

/*****************************************************************************/
void QVoice::on_txtEdit_voicename_textChanged()
{
    if (!attente)
        voice->EcrireNom(m_ui->txtEdit_voicename->toPlainText().toAscii().data());
}

void QVoice::on_cmbBox_style_activated(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_USERCODE, i);
}

void QVoice::on_but_feedback_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_FEEDBACK, i);
}

void QVoice::on_but_trans_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_TRANSPOSE, i + 2);
}

void QVoice::on_pshBut_poly_clicked(bool checked)
{
    if (!attente)
        voice->EcrireParam(VOICE_POLY, !checked);
}

void QVoice::on_but_porta_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_PORTAMENTO, i);
}

void QVoice::on_but_pitch_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_PITCHBEND, i);
}

void QVoice::on_cmbBox_pmdctl_activated(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_CONTROLLER, i);
}

void QVoice::on_but_LFOspeed_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_SPEED, i);
}

void QVoice::on_cmbBox_LFOwave_activated(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_WAVE, i);
}

void QVoice::on_pshBut_LFOload_clicked(bool checked)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_LOAD, !checked);
}

void QVoice::on_pshBut_LFOsync_clicked(bool checked)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_SYNC, !checked);
}

void QVoice::on_but_AMD_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_AMD, i);
}

void QVoice::on_but_AMS_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_AMS, i);
}

void QVoice::on_but_PMD_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_PMD, i);
}

void QVoice::on_but_PMS_valueChanged(int i)
{
    if (!attente)
        voice->EcrireParam(VOICE_LFO_PMS, i);
}

/*****************************************************************************/
void QVoice::on_txtEdit_author_textChanged()
{
    if (!attente)
        voice->EcrireAuteur(m_ui->txtEdit_author->toPlainText().toAscii().data());
}

void QVoice::on_txtEdit_comments_textChanged()
{
    if (!attente)
        voice->EcrireCommentaires(m_ui->txtEdit_comments->toPlainText().toAscii().data());
}

/*****************************************************************************/
void QVoice::changeEvent(QEvent *e)
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
