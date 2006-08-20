/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef QTABLE1_H
#define QTABLE1_H

#include <Q3Table>
#include <QEvent>

#include <Q3TableItem>
#include <QColorGroup>
#include <QPainter>
#include <QRect>


class QTableItem1 : public Q3TableItem {

public:
    int modo;

public:
    QTableItem1(Q3Table *table, EditType et, const QString &text, int mode) : Q3TableItem(table, et, text) {
        modo = mode;
    }
    void paint(QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected);
};


class QTable1 : public Q3Table {
    Q_OBJECT

public:
    int modo; /// Indica el modo de dibujo.

public:
    QTable1(QWidget * parent = 0, const char * name = 0);
    void sortColumn (int col, bool ascending = TRUE, bool wholeRows = TRUE);
    QWidget *beginEdit (int, int, bool);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void setItem(int, int, Q3TableItem *);
    void paintCell (QPainter *, int, int, const QRect &, bool);

signals:
    void pulsadomas(int, int, int);
};

#endif

