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

/** *@author Tomeu Borrás Riera */


class QTableWidgetItem2 : public QTableWidgetItem {
public:
   int modo;
public:
QTableWidgetItem2(int type = Type, int mode=0): QTableWidgetItem(type) {modo=mode;}
QTableWidgetItem2(const QString & text, int type = Type, int mode=0): QTableWidgetItem(text, type) {modo=mode;}
};

class QTableWidget2 : public QTableWidget{
  Q_OBJECT
public:
   int modo; // Indica el modo de dibujo.
public:
  QTableWidget2(QWidget * parent = 0 );
//  void sortColumn ( int col, bool ascending = TRUE, bool wholeRows = TRUE );
//  QWidget *beginEdit (int, int, bool);
   virtual bool eventFilter( QObject *obj, QEvent *event );
//   virtual void setItem(int, int, QTableWidgetItem2 *);
//   void  paintCell ( QPainter * , int , int , const QRect &, bool  );

	void setText( int x, int y, const QString & val) {
			QTableWidgetItem2 *newitem = new QTableWidgetItem2(val);
		setItem(x, y, newitem);
	}// end setText

signals:
   void pulsadomas(int, int, int);  
};

#endif
