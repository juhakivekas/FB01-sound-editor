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

#ifndef QCONFIG_H
#define QCONFIG_H

#include <QWidget>
#include "../ui_qconfig.h"

#include "../core/midi.h"
#include "../core/config.h"
#include "../core/set.h"

#include "../excep/midi_ex.h"
#include "../excep/memory_ex.h"

namespace Ui {
    class QConfig;
}

/*****************************************************************************/
class QConfig : public QWidget {
    Q_OBJECT
public:
//Constructeurs
    QConfig(QWidget *parent = 0);
    ~QConfig();
//Association classe - contrôle
    void DefinirConfig(Config * config);
    void DefinirSet(Set * set);
//Actualisation graphique
    void Rafraichir(const bool noTexts = false);
protected:
    void changeEvent(QEvent *e);
private:
    Ui::QConfig *m_ui;
    Config * config;
    Set    * set;
    bool attente;
private slots:
    void on_but_syschan_valueChanged(int i);
    void on_pshBut_memory_clicked(bool checked = false);
    void on_but_setnum_valueChanged(int i);
    void on_but_mastdetune_valueChanged(int i);
    void on_hzSlider_mastvolume_valueChanged(int i);
    void on_txtEdit_name_textChanged();
    void on_pshBut_combine_clicked(bool checked = false);
    void on_cmbBox_reception_activated(int i);
    void on_but_LFOspeed_valueChanged(int i);
    void on_cmbBox_LFOwave_activated(int i);
    void on_but_AMD_valueChanged(int i);
    void on_but_PMD_valueChanged(int i);
};

#endif // QCONFIG_H
