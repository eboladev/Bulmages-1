/***************************************************************************
                          qtable2.h  -  description
                             -------------------
    begin                : Mon Jan 13 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QTABLE2_H
#define QTABLE2_H

#include <QEvent>
#include <QTableWidget>

#include "funcaux.h"

/** *@author Tomeu Borrás Riera */


class QTableWidgetItem2 : public QTableWidgetItem {
public:
    int modo;
public:
    QTableWidgetItem2(int type = Type, int mode=0): QTableWidgetItem(type) {
        modo=mode;
    }
    QTableWidgetItem2(const QString & text, int type = Type, int mode=0): QTableWidgetItem(text, type) {
        modo=mode;
    }
    ~QTableWidgetItem2() {
        _depura("~QTableWidgetItem2",1);
    };

    virtual bool operator < ( const QTableWidgetItem & other);

};

class QTableWidget2 : public QTableWidget {
    Q_OBJECT
private:
    int modo; // Indica el modo de dibujo.
    int m_tipoorden;
    int m_colorden;
public:
    int tipoorden() {return m_tipoorden;};
    int colorden() {return m_colorden;};

    void settipoorden(int t) { m_tipoorden = t;};
    void setcolorden(int t) { m_colorden = t;};


    QTableWidget2(QWidget * parent = 0 );
    ~QTableWidget2() {
        _depura("~QTableWidget2",0);
    };

    virtual bool eventFilter( QObject *obj, QEvent *event );
    void setText( int x, int y, const QString & val) {
	_depura("QTableWidget::setText",0);
        QTableWidgetItem2 *newitem = new QTableWidgetItem2(val);
        setItem(x, y, newitem);
    }// end setText
	void sortColumn ( int col, Qt::SortOrder tipoorden);
	virtual void ordenar();
public slots:
	virtual void sortByColumn(int col);

signals:
    void pulsadomas(int, int, int);
};

#endif
