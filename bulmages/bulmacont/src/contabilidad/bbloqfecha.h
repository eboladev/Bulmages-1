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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef BbloqFecha_H
#define BbloqFecha_H

#include <q3listview.h>

#include "ui_bbloqfechabase.h"
#include "postgresiface2.h"


class empresa;


class myQListViewItem : public Q3ListViewItem {
public:
    myQListViewItem(Q3ListView *parent, QString label1);
    myQListViewItem(myQListViewItem *parent, QString label1);
    ~myQListViewItem() {
        return;
    };
    QString ej;
    QString per;
};


class BbloqFecha : public QWidget, public Ui_BBloqFechaBase {
private:
    empresa *empresaactual;

public:
    BbloqFecha(empresa *emp, QWidget * parent = 0);
    ~BbloqFecha();

public slots:
    virtual void dobleClick (Q3ListViewItem *item);
    virtual void boto1_click();
};

#endif

