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

#ifndef QBUTTON_H
#define QBUTTON_H

/*****************************************************************************/
#include <QPushButton>
#include <QMouseEvent>

/*****************************************************************************/
class QButton : public QPushButton
{
    Q_OBJECT
public:
    QButton(QWidget * parent = 0);
    ~QButton();
//Accesseurs
    void setValue(int value);
    int  value();
protected:
//Réimplémentation de la souris
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
private:
//Valeurs du contrôle
    int valeur, ancValeur;
    int valeurMin, valeurMax;
//Tracking souris
    bool click;
    int clickValeur;
    int sourisX, sourisY;
signals :
    void valueChanged(int value);
};

#endif // QBUTTON_H
