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

#include "listlinfacturaview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"
#include "plugins.h"


ListLinFacturaView::ListLinFacturaView(QWidget *parent) : SubForm2Bf(parent) {
    _depura("ListLinFacturaView::ListLinFacturaView", 0);
    setDBTableName("lfactura");
    setDBCampoId("idlfactura");

    /// Disparamos los plugins.
    int res = g_plugins->lanza("ListLinFacturaView_ListLinFacturaView", this);
    if (res != 0)
        return;

    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("idlfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idl factura"));
    addSHeader("desclfactura", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcionl factura"));
    addSHeader("cantlfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidadl factura"));
    addSHeader("pvplfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl factura"));
    addSHeader("ivalfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("IVAl factura"));
    addSHeader("reqeqlfactura", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, tr("% Recargo E.Q."));
    addSHeader("descuentolfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuentol factura"));
    addSHeader("idfactura", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id factura"));
    addSHeader("ordenlfactura", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);
    /// Disparamos los plugins.
    g_plugins->lanza("ListLinFacturaView_ListLinFacturaView_Post", this);

    _depura("END ListLinFacturaView::ListLinFacturaView", 0);
}


void ListLinFacturaView::cargar(QString idfactura) {
    _depura("ListLinFacturaView::cargar", 0);
    mdb_idfactura = idfactura;
    cursor2 * cur= empresaBase()->cargacursor("SELECT * FROM lfactura LEFT JOIN articulo ON lfactura.idarticulo = articulo.idarticulo WHERE idfactura=" + mdb_idfactura + " ORDER BY ordenlfactura");
    SubForm3::cargar(cur);
    delete cur;
    _depura("END ListLinFacturaView::cargar", 0);
}



