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

#include <QMessageBox>
#include <QKeyEvent>
#include <QEvent>
#include <QFile>
#include <QTextStream>
#include <QMenu>

#include "company.h"
#include "listltarifaview.h"
#include "funcaux.h"


ListLTarifaView::ListLTarifaView(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("ListLTarifaView::ListLTarifaView", 0);
    setDBTableName("ltarifa");
    setDBCampoId("idltarifa");
    addSHeader("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre tarifa"));
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre almacen"));
    addSHeader("pvpltarifa", DBCampo::DBnumeric, DBCampo::DBRequired, SHeader::DBNone, tr("Cantidad comp articulo"));
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("idtarifa", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("idltarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, tr("Nombre articulo"));
    setinsercion(FALSE);
    _depura("END ListLTarifaView::ListLTarifaView", 0);
}


ListLTarifaView1::ListLTarifaView1(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("ListLTarifaView1::ListLTarifaView1", 0);
    setDBTableName("ltarifa");
    setDBCampoId("idltarifa");
    addSHeader("nomalmacen", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite , tr("Nombre almacen"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite , tr("Nombre articulo"));
    addSHeader("pvpltarifa", DBCampo::DBnumeric, DBCampo::DBRequired, SHeader::DBNone, tr("PVPl tarifa"));
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("idalmacen", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id almacen"));
    addSHeader("idtarifa", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id tarifa"));
    addSHeader("idltarifa", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idl tarifa"));
    addSHeader("nomtarifa", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoView, tr("Nombre tarifa"));
    setinsercion(FALSE);
    _depura("ListLTarifaView1::ListLTarifaView1", 0);
}

