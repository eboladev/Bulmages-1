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
#include "listlinpedidoclienteview.h"
#include "funcaux.h"
#include "fixed.h"


ListLinPedidoClienteView::ListLinPedidoClienteView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lpedidocliente");
    setDBCampoId("numlpedidocliente");
    addSHeader("puntlpedidocliente", DBCampo::DBboolean, DBCampo::DBNotNull, SHeader::DBNone, tr("Puntl pedido cliente"));
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("numlpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Numl pedido cliente"));
    addSHeader("desclpedidocliente", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descl pedido cliente"));
    addSHeader("cantlpedidocliente", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantl pedido cliente"));
    addSHeader("pvplpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl pedido cliente"));
    addSHeader("ivalpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("IVAl pedido cliente"));
    addSHeader("descuentolpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuentol pedido cliente"));
    addSHeader("idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id pedido cliente"));
    setinsercion(TRUE);
}


void ListLinPedidoClienteView::on_mui_list_editFinished(int row, int col, int key) {
    _depura("ListLinPedidoClienteView::editFinished", 0);
    SubForm3::on_mui_list_editFinished(row, col, key);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();


    /// Si el campo no ha sido cambiado se termina sin cambiar nada.
    if ( ! camp->cambiado() ) {
    	SubForm3::on_mui_list_editFinished(row, col, key);
	return;
    } // end if

    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo='" + camp->text() + "'");
        if (!cur->eof() ) {
            rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclpedidocliente", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlpedidocliente", "1.00");
            rec->setDBvalue("descuentolpedidocliente","0.00");
            rec->setDBvalue("pvplpedidocliente", cur->valor("pvparticulo"));
        } // end if

        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva=" + cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
            rec->setDBvalue("ivalpedidocliente", cur1->valor("porcentasa_iva"));
        } // end if
        delete cur1;
        delete cur;
    } // end if
}


void ListLinPedidoClienteView::cargar(QString idpedidocliente) {
    _depura("ListLinPedidoClienteView::cargar\n", 0);
    mdb_idpedidocliente = idpedidocliente;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM lpedidocliente LEFT JOIN articulo ON lpedidocliente.idarticulo = articulo.idarticulo WHERE idpedidocliente=" + mdb_idpedidocliente);
    SubForm3::cargar(cur);
    delete cur;
}


Fixed ListLinPedidoClienteView::calculabase() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplpedidocliente", i)) * Fixed(DBvalue("cantlpedidocliente", i));
        base = base + totpar;
    } // end for
    return base;
}


Fixed ListLinPedidoClienteView::calculaiva() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplpedidocliente", i)) * Fixed(DBvalue("ivalpedidocliente", i));
        base = base + totpar;
    } // end for
    return base;
}

