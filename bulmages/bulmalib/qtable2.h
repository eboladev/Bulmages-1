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

#ifndef QTABLE2_H
#define QTABLE2_H

#include <QEvent>
#include <QTableWidget>

#include "funcaux.h"


class QTableWidgetItem2 : public QTableWidgetItem {

public:
    int modo;
    QTableWidgetItem2(int type = Type, int mode = 0) : QTableWidgetItem(type) {
        modo = mode;
    }
    QTableWidgetItem2(const QString & text, int type = Type, int mode = 0) : QTableWidgetItem(text, type) {
        modo = mode;
    }
    ~QTableWidgetItem2() {
        _depura("~QTableWidgetItem2", 1);
    };
    virtual bool operator < (const QTableWidgetItem & other);
};


class QTableWidget2 : public QTableWidget {
    Q_OBJECT

private:
    int modo; /// Indica el modo de dibujo.
    int m_tipoorden;
    int m_colorden;

public:
    int tipoorden() {
        return m_tipoorden;
    };
    int colorden() {
        return m_colorden;
    };
    void settipoorden(int t) {
        m_tipoorden = t;
    };
    void setcolorden(int t) {
        m_colorden = t;
    };
    void columnMoved(int column, int oldIndex, int newIndex) {
        QTableWidget::columnMoved(column, oldIndex, newIndex);
    };
    QTableWidget2(QWidget * parent = 0);
    ~QTableWidget2() {
        _depura("~QTableWidget2", 0);
    };
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void setText(int x, int y, const QString & val);
    void sortColumn (int col, Qt::SortOrder tipoorden);
    virtual void ordenar();

public slots:
    virtual void sortByColumn(int col);

signals:
    void pulsadomas(int, int, int);
    void editFinished(int, int);
    void pressedAsterisk(int, int);
    void pressedPlus(int, int);
    void pressedMinus(int, int);
    void pressedSlash(int, int);
    void ctrlSubir(int, int);
    void ctrlBajar(int, int);
    void ctrlDerecha(int, int);
    void ctrlIzquierda(int, int);
};

#endif

