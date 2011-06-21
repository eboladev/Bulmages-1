/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef LISTLTARIFAVIEW_H
#define LISTLTARIFAVIEW_H

#include "bltablewidget.h"
#include "bfsubform.h"
#include "blwidget.h"


class ListLTarifaView : public BfSubForm
{
    Q_OBJECT

public:
    QString mdb_idarticulo;
    ListLTarifaView ( QWidget *parent = 0 );
    ~ListLTarifaView() {}
    ;

public slots:
    virtual void load ( QString idarticulo ) {
        BL_FUNC_DEBUG
        mdb_idarticulo = idarticulo;
        QString SQLQuery = "SELECT * FROM (SELECT * FROM almacen, tarifa) AS t2 LEFT JOIN (SELECT * FROM articulo WHERE idarticulo = " + mdb_idarticulo + ") AS t3 ON 1 = 1 ";
        SQLQuery += " LEFT JOIN (SELECT * FROM ltarifa WHERE idarticulo = " + mdb_idarticulo + ") as t1 ON t1.idtarifa = t2.idtarifa AND t1.idalmacen = t2.idalmacen ";
        BfSubForm::load ( SQLQuery );
    };
};


class ListLTarifaView1 : public BfSubForm
{
    Q_OBJECT

public:
    ListLTarifaView1 ( QWidget *parent = 0 );
    ~ListLTarifaView1() {}
    ;

public slots:
    virtual void load ( QString SQLQuery ) {
        BL_FUNC_DEBUG
        BfSubForm::load ( SQLQuery );
    };
};


#include "bfbulmafact.h"
#endif

