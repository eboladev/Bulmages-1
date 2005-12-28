/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#ifndef BbloqFecha_H
#define BbloqFecha_H

#include "uibloqfecha.h"
#include "postgresiface2.h"

#include <q3listview.h>

/**
@author Josep Burcion
*/


class myQListViewItem : public Q3ListViewItem {
public:
	myQListViewItem(Q3ListView *parent, QString label1);
	myQListViewItem(myQListViewItem *parent, QString label1);
	~myQListViewItem() {return;};
	QString ej;
	QString per;
};


class BbloqFecha : public UIbloqFecha {
private:
	postgresiface2 *conexionbase;

public:
    BbloqFecha(QString * NombreBaseDatos=0, QWidget * parent = 0, const char * name = 0, Qt::WFlags f = Qt::WType_TopLevel);
    ~BbloqFecha();


public slots:
    virtual void dobleClick (Q3ListViewItem *item);
    virtual void boto1_click();
};

#endif
