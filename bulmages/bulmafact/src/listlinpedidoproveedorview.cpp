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

#include <Q3Table>
#include <QMessageBox>
#include <Q3PopupMenu>
#include <QKeyEvent>
#include <QEvent>

#include "articulolist.h"
#include "listlinpedidoproveedorview.h"
#include "funcaux.h"
#include "fixed.h"


ListLinPedidoProveedorView::ListLinPedidoProveedorView(QWidget *parent) : SubForm2Bf(parent) {
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
    addSHeader("descuentolpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuentol pedido proveedor"));
    addSHeader("idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id pedido proveedor"));
    setinsercion(TRUE);
}


void ListLinPedidoProveedorView::on_mui_list_editFinished(int row, int col) {
    _depura("ListLinPedidoProveedorView::editFinished", 0);
    SubForm3::on_mui_list_editFinished(row, col);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='" + camp->text() + "'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclpedidoproveedor", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlpedidoproveedor", "1.00");
            rec->setDBvalue("descuentolpedidoproveedor", "0.00");
            rec->setDBvalue("pvplpedidoproveedor", cur->valor("pvparticulo"));
        } // end if

        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva=" + cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof()) {
	    rec->setDBvalue("ivalpedidoproveedor", cur1->valor("porcentasa_iva"));
        } // end if
	delete cur1;
	delete cur;

    } // end if
}


void ListLinPedidoProveedorView::cargar(QString idpedidoproveedor) {
        _depura("ListLinPedidoProveedorView::cargar\n", 0);
        mdb_idpedidoproveedor = idpedidoproveedor;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM lpedidoproveedor LEFT JOIN articulo ON lpedidoproveedor.idarticulo = articulo.idarticulo WHERE idpedidoproveedor=" + mdb_idpedidoproveedor);
        SubForm3::cargar(cur);
        delete cur;
}


Fixed ListLinPedidoProveedorView::calculabase() {
	Fixed base("0.0");
        for (int i = 0; i < rowCount() - 1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpedidoproveedor", i)) * Fixed(DBvalue("cantlpedidoproveedor", i));
		base = base + totpar;
        } // end for
	return base;
}


Fixed ListLinPedidoProveedorView::calculaiva() {
	Fixed base("0.0");
        for (int i = 0; i < rowCount() - 1; i++) {
		Fixed totpar = Fixed(DBvalue("pvplpedidoproveedor", i)) * Fixed(DBvalue("ivalpedidoproveedor", i));
		base = base + totpar;
        } // end for
	return base;
}

