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

#ifndef QBANK_H
#define QBANK_H

#include <QWidget>
#include "../ui_qbank.h"

#include "../core/midi.h"
#include "../core/bank.h"

namespace Ui {
    class QBank;
}

/*****************************************************************************/
class QBank : public QWidget {
    Q_OBJECT
public:
//Constructeurs
    QBank(QWidget *parent = 0);
    ~QBank();
//Association classe - contrôle
    void DefinirBank(Bank * bank);
//Actualisation graphique
    void Rafraichir();
protected:
    void changeEvent(QEvent *e);
private:
    Ui::QBank * m_ui;
    Bank * bank;
    bool attente;
private slots:
    void on_txtEdit_bankName_textChanged();
    void on_cmbBox_style_activated(int i);
    void on_txtEdit_voiceName_textChanged();
    void on_listWidget_voices_cellClicked(int row, int column);
};

#endif // QBANK_H
