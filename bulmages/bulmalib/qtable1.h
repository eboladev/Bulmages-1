/***************************************************************************
                          qtable1.h  -  description
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

#ifndef QTABLE1_H
#define QTABLE1_H

#include <qtable.h>

/** *@author Tomeu Borrás Riera */


class QTableItem1 : public QTableItem {
public:
   int modo;
public:
QTableItem1(QTable *table ,EditType et ,const QString &text, int mode): QTableItem(table, et, text) {modo=mode;}
void paint( QPainter *p, const QColorGroup &cg, const QRect &cr, bool selected );
};



class QTable1 : public QTable{
  Q_OBJECT
public:
   int modo; // Indica el modo de dibujo.
public:
  QTable1(QWidget * parent = 0, const char * name = 0 );
  void sortColumn ( int col, bool ascending = TRUE, bool wholeRows = TRUE );
  QWidget *beginEdit (int, int, bool);
   virtual bool eventFilter( QObject *obj, QEvent *event );
   virtual void setItem(int, int, QTableItem *);
   void  paintCell ( QPainter * , int , int , const QRect &, bool  );
signals:
   void pulsadomas(int, int, int);  
};

#endif

