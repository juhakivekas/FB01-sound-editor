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
#include "qautomation.h"

extern Editeur * editeur;

/*****************************************************************************/
QAutomation::QAutomation(QWidget *parent) : QWidget(parent), m_ui(new Ui::QAutomation)
{
//Initialise l'interface
    m_ui->setupUi(this);
    m_ui->but_INFrom->setValue(0);
    m_ui->but_INTo->setValue(127);
    m_ui->but_OUTFrom->setValue(0);
    m_ui->but_OUTTo->setValue(127);
//Initialise le timer
    timer = 0;
}

QAutomation::~QAutomation()
{
    if (timer != 0) this->killTimer(timer);
    delete m_ui;
}

/*****************************************************************************/
void QAutomation::InitialiserListes()
{
    QString num;
//Ajoute tous les MIDI CCs
    m_ui->listWidget_CCs->clear();
    for (uint i = 0; i < AUTO_NB_CCS; i ++) {
        num.setNum(i, 10);
        QListWidgetItem * item = new QListWidgetItem("CC#" + num);
        m_ui->listWidget_CCs->addItem(item);
    }
//Ajoute toutes les callbacks
    m_ui->listWidget_FMs->clear();
    for (uint i = 0; i < Automation::NbCallbacks(); i ++) {
        QListWidgetItem * item = new QListWidgetItem(Automation::NomCallback(i));
        m_ui->listWidget_FMs->addItem(item);
    }
}

/*****************************************************************************/
void QAutomation::Actualiser()
{
//Ajoute toutes les automations
    m_ui->listWidget_autos->clear();
    for (uint i = 0; i < Automation::NbAutomations(); i ++) {
        QListWidgetItem * item = new QListWidgetItem(Automation::DescAutomation(i));
        m_ui->listWidget_autos->addItem(item);
    }
}

/*****************************************************************************/
void QAutomation::Rafraichir()
{
    QString num;
//Affiche le dernier MIDI CC reçu
    num.setNum(Automation::DernierCC());
    m_ui->lbl_lastCC->setText("Msg.: " + num);
    m_ui->lbl_lastCC->repaint();
//Affiche la dernière valeur reçue
    num.setNum(Automation::DerniereValeur());
    m_ui->lbl_lastValue->setText("Value: " + num);
    m_ui->lbl_lastValue->repaint();
}

/*****************************************************************************/
void QAutomation::on_pshBut_add_pressed()
{
//Vérifie la sélection
    if (m_ui->listWidget_CCs->currentRow() < 0) return;
    if (m_ui->listWidget_FMs->currentRow() < 0) return;
//Ajoute une automation
    try {
        Automation::Ajouter(m_ui->listWidget_CCs->currentRow(), m_ui->but_INFrom->value(), m_ui->but_INTo->value(),
                            m_ui->listWidget_FMs->currentRow(), m_ui->but_OUTFrom->value(), m_ui->but_OUTTo->value());
    }catch (Automation_ex ex) {
        QMessageBox::warning(this, "FB01 SE:", ex.Info());
        return;
    }
    Actualiser();
}

void QAutomation::on_pshBut_delete_pressed()
{
    if (m_ui->listWidget_autos->currentRow() < 0) return;
    Automation::Enlever(m_ui->listWidget_autos->currentRow());
    Actualiser();
}

void QAutomation::on_pshBut_deleteAll_pressed()
{
    Automation::EnleverTout();
    Actualiser();
}

/*****************************************************************************/
void QAutomation::on_pshBut_autorefresh_clicked(bool checked)
{
    if (checked) timer = this->startTimer(QAUTO_PAUSE_ACTU);
    else this->killTimer(timer);
}

/*****************************************************************************/
void QAutomation::changeEvent(QEvent * event)
{
    QWidget::changeEvent(event);
    switch (event->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QAutomation::timerEvent(QTimerEvent * event)
{
    editeur->Rafraichir(true);
}

