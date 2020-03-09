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

#ifndef QENVELOPPE_H
#define QENVELOPPE_H

#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

/*****************************************************************************/
class QEnveloppe : public QLabel
{
    Q_OBJECT
public:
    QEnveloppe(QWidget * parent = 0, Qt::WindowFlags f = 0);
    ~QEnveloppe();
    void DefinirEnveloppe(int AR, int DR1, int SL, int DR2, int RR);
protected:
    void paintEvent(QPaintEvent * event);
private:
    QPoint Points[5];    
};

#endif // QENVELOPPE_H
