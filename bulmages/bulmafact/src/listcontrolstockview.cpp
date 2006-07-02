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

#include "listcontrolstockview.h"


ListControlStockView::ListControlStockView(QWidget *parent, const char *)
        : SubForm2Bf(parent) {
    setDBTableName("controlstock");
    setDBCampoId("idarticulo");
    addSHeader("punteocontrolstock", DBCampo::DBboolean, DBCampo::DBNothing, SHeader::DBNone, tr("Punteo control stock"));
    addSHeader("codigoalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo almacen"));
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre almacen"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre almacen"));
    addSHeader("stockantcontrolstock", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNoWrite, tr("Stock ant control stock"));
    addSHeader("stocknewcontrolstock", DBCampo::DBnumeric, DBCampo::DBRequired, SHeader::DBNone, tr("Stock new control stock"));
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNone | SHeader::DBNoView, tr("Id articulo"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNone | SHeader::DBNoView, tr("Id almacen"));
    addSHeader("idinventario", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNone | SHeader::DBNoView, tr("Id inventario"));
    addSHeader("idarticulopk", DBCampo::DBint, DBCampo::DBNoSave | DBCampo::DBDupPrimaryKey, SHeader::DBNone | SHeader::DBNoView, tr("Id articulo"));
    addSHeader("idalmacenpk", DBCampo::DBint,  DBCampo::DBNoSave | DBCampo::DBDupPrimaryKey, SHeader::DBNone | SHeader::DBNoView, tr("Id almacen"));
    addSHeader("idinventariopk", DBCampo::DBint,  DBCampo::DBNoSave | DBCampo::DBDupPrimaryKey, SHeader::DBNone | SHeader::DBNoView, tr("Id inventario"));
    setinsercion(FALSE);
}

