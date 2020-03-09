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

#ifndef QAUTOMATION_H
#define QAUTOMATION_H

#include <QWidget>
#include <QMessageBox>

#include "ui_qautomation.h"

#include "../core/midi.h"
#include "../core/automation.h"

#include "../excep/midi_ex.h"
#include "../excep/automation_ex.h"

namespace Ui {
    class QAutomation;
}

/*****************************************************************************/
class QAutomation : public QWidget {
    Q_OBJECT
public:
//Intervalle d'actualisation
    #define QAUTO_PAUSE_ACTU 200
//Constructeurs
    QAutomation(QWidget *parent = 0);
    ~QAutomation();
//Actualisation du contrôle
    void InitialiserListes();
    void Actualiser();
    void Rafraichir();
protected:
    void changeEvent(QEvent *e);
    void timerEvent(QTimerEvent *e);
private:    
    Ui::QAutomation *m_ui;
    int timer;
private slots:
    void on_pshBut_add_pressed();
    void on_pshBut_delete_pressed();
    void on_pshBut_deleteAll_pressed();
    void on_pshBut_autorefresh_clicked(bool checked);
};

#endif // QAUTOMATION_H
