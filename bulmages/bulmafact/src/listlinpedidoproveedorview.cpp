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

#include "articulolist.h"
#include "listlinpedidoproveedorview.h"
#include "funcaux.h"
#include "fixed.h"


///
/**
\param parent
**/
ListLinPedidoProveedorView::ListLinPedidoProveedorView(QWidget *parent) : SubForm2Bf(parent) {
    _depura("ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0);
    setDBTableName("lpedidoproveedor");
    setDBCampoId("numlpedidoproveedor");
    addSHeader("puntlpedidoproveedor", DBCampo::DBboolean, DBCampo::DBNotNull, SHeader::DBNone, tr("puntlpedidoproveedor"));
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("numlpedidoproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Numerol pedido proveedor"));
    addSHeader("desclpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcionl pedido proveedor"));
    addSHeader("cantlpedidoproveedor", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidadl pedido proveedor"));
    addSHeader("pvplpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl pedido proveedor"));
    addSHeader("ivalpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("IVAl pedido proveedor"));
    addSHeader("reqeqlpedidoproveedor", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, tr("Recargo pedido proveedor"));
    addSHeader("descuentolpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuentol pedido proveedor"));
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id pedido proveedor"));
    addSHeader("ordenlpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);
    setOrdenPorQuery(FALSE);
    _depura("END ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0);
}


///
/**
\param idpedidoproveedor
**/
void ListLinPedidoProveedorView::cargar(QString idpedidoproveedor) {
        _depura("ListLinPedidoProveedorView::cargar", 0);
        mdb_idpedidoproveedor = idpedidoproveedor;
        cursor2 * cur= empresaBase()->cargacursor("SELECT * FROM lpedidoproveedor LEFT JOIN articulo ON lpedidoproveedor.idarticulo = articulo.idarticulo WHERE idpedidoproveedor=" + mdb_idpedidoproveedor + " ORDER BY ordenlpedidoproveedor");
        SubForm3::cargar(cur);
        delete cur;
        _depura("END ListLinPedidoProveedorView::cargar", 0);
}

