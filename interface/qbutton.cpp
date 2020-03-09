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

#include "qbutton.h"

/*****************************************************************************/
QButton::QButton(QWidget * parent) : QPushButton(parent)
{
//Initialise les valeurs
    valeur = 0;
    ancValeur = 0;
    valeurMin = 0;
    valeurMax = 0;
//Initialise le tracking
    click = false;
}

QButton::~QButton()
{

}

/*****************************************************************************/
void QButton::setValue(int value)
{
    static QString num;
//Change la valeur interne
    this->valeur = value;
    this->ancValeur = valeur;
    emit valueChanged(valeur);
//Affiche la nouvelle valeur
    num.setNum(valeur, 10);
    setText(num);
    repaint();
}

int QButton::value()
{
    return valeur;
}

/*****************************************************************************/
void QButton::mouseMoveEvent(QMouseEvent * event)
{
    static QString num;
    if (!click) return;
//Augmente ou diminue la valeur
    valeur = clickValeur;
    valeur += (sourisY - event->y()) >> 2;
    valeur += (event->x() - sourisX) >> 4;
//Limite la plage
    if(valeur > valeurMax) valeur = valeurMax;
    if(valeur < valeurMin) valeur = valeurMin;
//Actualise le contrôle
    if (valeur != ancValeur) {
    //Emet le signal
        emit valueChanged(valeur);
        ancValeur = valeur;
    //Change le texte
        num.setNum(valeur, 10);
        setText(num);
        repaint();
    }
}

/*****************************************************************************/
void QButton::mousePressEvent(QMouseEvent * event)
{
//Sauvegarde la position et la valeur
    clickValeur = valeur;
    sourisX = event->x();
    sourisY = event->y();
//Initialise le tracking
    QVariant valmin = this->property("minValue");
    if (valmin.isValid()) valeurMin = valmin.toInt();
    QVariant valmax = this->property("maxValue");
    if (valmax.isValid()) valeurMax = valmax.toInt();
    click = true;
}

void QButton::mouseReleaseEvent(QMouseEvent * event)
{
    click = false;
}
