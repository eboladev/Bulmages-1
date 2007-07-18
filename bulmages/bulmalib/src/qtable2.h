/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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
#include <QItemDelegate>
#include "funcaux.h"


/** Implementa los elementos de edicion por defecto de un QTableWidgetItem2.
Mediante esta clase se crean editores especificos que luego pueden sobrecargarse.
*/
class QTableItemTextDelegate : public QItemDelegate {
public:
    QTableItemTextDelegate(QObject *);
    ~QTableItemTextDelegate();
    void setEditorData(QWidget *, const QModelIndex &index) const;
    void setModelData(QWidget *editor,  QAbstractItemModel *model, const QModelIndex &index) const;
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};


/// Reimplementa los items de QTableWidget2 para que podamos programar cosas en ellos.
class QTableWidgetItem2 : public QTableWidgetItem {
public:
    int modo;
    QTableWidgetItem2(int type = Type, int mode = 0);
    QTableWidgetItem2(const QString &text, int type = Type, int mode = 0);
    ~QTableWidgetItem2();
//    virtual bool operator< (const QTableWidgetItem &other) const;
};


/// Clase que deriva de QTableWidget para poder reprogramar ciertos aspectos y
/// funcionalidades.
/** Se usa principalmente como tabla para mostrar subformularios.*/
class QTableWidget2 : public QTableWidget {
    Q_OBJECT

private:
    int modo; /// Indica el modo de dibujo.
    int m_tipoorden;
    int m_colorden;
public:
    int m_teclasalida; /// Indica con que tecla se ha salido de la edición.
public:
    int tipoorden();
    int colorden();
    void settipoorden(int t);
    void setcolorden(int t);
    void columnMoved(int column, int oldIndex, int newIndex);
    QTableWidget2(QWidget *parent = 0);
    ~QTableWidget2();
    void editItem(QTableWidgetItem *it);
    virtual bool eventFilter(QObject *obj, QEvent *event);
    void setText(int x, int y, const QString &val);
    virtual void ordenar();
    void sortByColumn(int column, Qt::SortOrder order = Qt::AscendingOrder);
    void sortItems(int column, Qt::SortOrder order = Qt::AscendingOrder);

public slots:
    virtual void sitemChanged(QTableWidgetItem *it);

signals:
    void pulsadomas(int, int, int);
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

