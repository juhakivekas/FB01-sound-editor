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

#include "qbank.h"

/*****************************************************************************/
QBank::QBank(QWidget *parent) : QWidget(parent), m_ui(new Ui::QBank)
{
    m_ui->setupUi(this);
//Créé les colonnes de la table
    m_ui->listWidget_voices->setColumnCount(2);
    QTableWidgetItem * voice  = new QTableWidgetItem("Voice");
    QTableWidgetItem * style  = new QTableWidgetItem("Style");
    m_ui->listWidget_voices->setHorizontalHeaderItem(0, voice);
    m_ui->listWidget_voices->setHorizontalHeaderItem(1, style);
    m_ui->listWidget_voices->setRowCount(BANK_NB_VOICES);
}

QBank::~QBank()
{
    delete m_ui;
}

/*****************************************************************************/
void QBank::DefinirBank(Bank * bank)
{
    QString num;
//Associe la bank
    this->bank = bank;
    num.setNum(bank->LireId() + 1, 10);
    m_ui->grpBox_bank->setTitle((QString) "Bank " + num + " :");
//Verrouille certains contrôles
    if (bank->LireId() >= 2) {
        m_ui->txtEdit_bankName->setEnabled(false);
        m_ui->txtEdit_voiceName->setEnabled(false);
        m_ui->cmbBox_style->setEnabled(false);
    }
}

/*****************************************************************************/
const char BankStyles[15][8] = {"Piano", "Keys", "Organ", "Guitar", "Bass", "Orch", "Brass", "Wood",
                                "Synth", "Pad", "Ethnic", "Bells", "Rythm", "Sfx", "Other"};
void QBank::Rafraichir()
{
    attente = true;
//Actualise le texte
    m_ui->txtEdit_bankName->setPlainText(bank->LireNom());
    m_ui->txtEdit_bankName->repaint();
    m_ui->txtEdit_voiceName->setPlainText("");
    m_ui->txtEdit_voiceName->repaint();
    m_ui->cmbBox_style->setCurrentIndex(0);
//Actualise la liste des voices
    m_ui->listWidget_voices->clearContents();
    for (uint i = 0; i < BANK_NB_VOICES; i++) {
    //Récupère les informations
        QString nom(bank->RecupererVoice(i)->LireNom());
        uchar s = bank->RecupererVoice(i)->LireParam(BANKVOICE_STYLE);
        if (s >= 14) s = 14; QString sty(BankStyles[s]);
    //Créé les items
        QTableWidgetItem * voice  = new QTableWidgetItem(nom);
        QTableWidgetItem * style  = new QTableWidgetItem(sty);
        m_ui->listWidget_voices->setItem(i, 0, voice);
        m_ui->listWidget_voices->setItem(i, 1, style);
    }
//Sélectionne la voice
    m_ui->listWidget_voices->setCurrentCell(bank->VoiceSelectionnee(), 0);
    attente = false;
}

/*****************************************************************************/
void QBank::on_txtEdit_bankName_textChanged()
{
    if (attente) return;
    bank->EcrireNom(m_ui->txtEdit_bankName->toPlainText().toAscii().data());
}

void QBank::on_txtEdit_voiceName_textChanged()
{
    Bank_voice * voice;
    if (attente) return;
    int row = m_ui->listWidget_voices->currentRow();
    if (row < 0) return;
//Modifie la voice
    voice = bank->RecupererVoice(row);
    voice->EcrireNom(m_ui->txtEdit_voiceName->toPlainText().toAscii().data());
//Actualise le nom
    QString nom = m_ui->txtEdit_voiceName->toPlainText().left(BANKVOICE_LEN_NOM);
    m_ui->listWidget_voices->item(row, 0)->setText(nom);
}

void QBank::on_cmbBox_style_activated(int Index)
{
    Bank_voice * voice;
    if (attente) return;
    int row = m_ui->listWidget_voices->currentRow();
    if (row < 0) return;
//Modifie la voice
    voice = bank->RecupererVoice(row);
    voice->EcrireParam(BANKVOICE_STYLE, m_ui->cmbBox_style->currentIndex());
//Actualise le style
    m_ui->listWidget_voices->item(row, 1)->setText((QString) BankStyles[m_ui->cmbBox_style->currentIndex()]);
}

/*****************************************************************************/
void QBank::on_listWidget_voices_cellClicked(int row, int column)
{
    Bank_voice * voice;
    attente = true;
//Actualise les paramêtres
    voice = bank->RecupererVoice(row);
    m_ui->txtEdit_voiceName->setPlainText((QString) voice->LireNom());
    m_ui->cmbBox_style->setCurrentIndex(voice->LireParam(BANKVOICE_STYLE));
//Sélectionne la voice
    bank->SelectionnerVoice(row);
    attente = false;
}

/*****************************************************************************/
void QBank::changeEvent(QEvent *e)
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
