/***************************************************************************
                          qlistviewitem1.h  -  description
                             -------------------
    begin                : lun jun 23 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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

#ifndef QLISTVIEWITEM1_H
#define QLISTVIEWITEM1_H

#include <q3listview.h>

/**
  *@author Tomeu Borrás Riera
  */

class QListViewItem1 : public Q3ListViewItem  {
public:
   int tipolista;
public: 
	QListViewItem1(Q3ListView *parent): Q3ListViewItem(parent) {};
   QListViewItem1(Q3ListViewItem *parent) : Q3ListViewItem(parent) {};
	~QListViewItem1() {};
   void  paintCell ( QPainter * , const QColorGroup & , int, int ,int  );
   void setTipo(int a) { tipolista = a;};
   void width(const QFontMetrics &, const Q3ListView *, int);
   
};

#endif
