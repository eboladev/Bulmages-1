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

#include "listlinfacturapview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"
#include "plugins.h"


ListLinFacturaProveedorView::ListLinFacturaProveedorView(QWidget *parent)
        : SubForm2Bf(parent) {
    _depura("ListLinFacturaProveedorView::ListLinFacturaProveedorView", 0);
    setDBTableName("lfacturap");
    setDBCampoId("idlfacturap");

    /// Disparamos los plugins.
    int res = g_plugins->lanza("ListLinFacturaProveedorView_ListLinFacturaProveedorView", this);
    if (res != 0)
        return;

    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre"));
    addSHeader("idlfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Linea"));
    addSHeader("desclfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcion"));
    addSHeader("cantlfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad"));
    addSHeader("pvplfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("P.V.P."));
    addSHeader("ivalfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("% I.V.A."));
    addSHeader("reqeqlfacturap", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, tr("% Recargo"));
    addSHeader("descuentolfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuento"));
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Factura"));
    addSHeader("ordenlfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);

    /// Disparamos los plugins.
    g_plugins->lanza("ListLinFacturaProveedorView_ListLinFacturaProveedorView_Post", this);

    _depura("END ListLinFacturaProveedorView::ListLinFacturaProveedorView", 0);
}


void ListLinFacturaProveedorView::cargar(QString idfacturap) {
    _depura("ListLinFacturaProveedorView::cargar", 0);
    mdb_idfacturap = idfacturap;
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM lfacturap LEFT JOIN articulo ON lfacturap.idarticulo = articulo.idarticulo WHERE idfacturap = " + mdb_idfacturap + " ORDER BY ordenlfacturap");
    SubForm3::cargar(cur);
    delete cur;
    _depura("END ListLinFacturaProveedorView::cargar", 0);
}
