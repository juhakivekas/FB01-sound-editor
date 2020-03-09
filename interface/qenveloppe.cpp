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

#include "qenveloppe.h"

/*****************************************************************************/
QEnveloppe::QEnveloppe(QWidget * parent, Qt::WindowFlags f) : QLabel(parent, f)
{
//Positionne les points
    DefinirEnveloppe(0, 0, 0, 0, 0);
}

QEnveloppe::~QEnveloppe()
{
}

/*****************************************************************************/
void QEnveloppe::DefinirEnveloppe(int AR, int DR1, int SL, int DR2, int RR)
{
//Calcule la position
    Points[0].setX(0);
    Points[0].setY(31);
//Affichage de l'attaque
    if (AR == 0) {
    //Pas d'attaque
        Points[1].setX(31);  Points[1].setY(31);
        Points[2].setX(62);  Points[2].setY(31);
        Points[3].setX(93);  Points[3].setY(31);
        Points[4].setX(108); Points[4].setY(31);
    }else{
    //Attaque non nulle
        Points[1].setX(31 - AR); Points[1].setY(0);
    //Affichage du decay 1
        if (DR1 == 0) {
        //Pas de decay 1
            Points[2].setX(Points[1].x() + 31); Points[2].setY(0);
            Points[3].setX(Points[2].x() + 31); Points[3].setY(0);
        //Affichage du release
            if (RR == 0) {
                Points[4].setX(Points[3].x() + 15); Points[4].setY(0);
            }else{
                Points[4].setX(Points[3].x() + 15 - RR); Points[4].setY(31);
            }
        }else{
        //Affichage du sustain
            Points[2].setX(Points[1].x() + 31 - DR1); Points[2].setY(31 - (SL * 31) / 15);
        //Affichage du decay 2
            if (DR2 == 0) {
            //Pas de decay 2
                Points[3].setX(Points[2].x() + 31); Points[3].setY(Points[2].y());
            //Affichage du release
                if (RR == 0) {
                    Points[4].setX(Points[3].x() + 15); Points[4].setY(Points[3].y());
                }else{
                    Points[4].setX(Points[3].x() + 15 - RR); Points[4].setY(31);
                }
            }else{
            //Decay 2 non nul
                Points[3].setX(Points[2].x() + 31 - DR2); Points[3].setY(31);
                if (RR == 0)
                {//Pas de relachement
                    Points[4].setX(Points[3].x() + 15); Points[4].setY(Points[3].y());
                }else{
                    Points[4].setX(Points[3].x() + 15 - RR); Points[4].setY(31);
                }
            }
        }
    }
}

/*****************************************************************************/
void QEnveloppe::paintEvent(QPaintEvent * event)
{
//Dessine le widget
    QWidget::paintEvent(event);
    QPainter painter(this);
    painter.translate(4, 4);
//Trace le repère
    painter.setPen(Qt::blue);
    painter.drawLine(0, 31, 108, 31);
    painter.drawLine(0, 31, 0, 0);
//Trace l'enveloppe
    painter.setPen(Qt::red);
    painter.drawPolyline(Points, 5);
//Trace les poignées
    for (int i = 1; i < 5; i++)
        painter.fillRect(Points[i].x()-2, Points[i].y()-2, 4, 4,Qt::SolidPattern);
}
