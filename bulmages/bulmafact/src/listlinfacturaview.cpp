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


ListLinFacturaView::ListLinFacturaView(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lfactura");
    setDBCampoId("idlfactura");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("idlfactura", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idl factura"));
    addSHeader("desclfactura", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcionl factura"));
    addSHeader("cantlfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidadl factura"));
    addSHeader("pvplfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl factura"));
    addSHeader("ivalfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("IVAl factura"));
    addSHeader("descuentolfactura", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuentol factura"));
    addSHeader("idfactura", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id factura"));
    setinsercion(TRUE);
}


void ListLinFacturaView::on_mui_list_editFinished(int row, int col) {
    _depura("ListLinFacturaView::editFinished", 0);
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
            rec->setDBvalue("desclfactura", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlfactura", "1.00");
            rec->setDBvalue("descuentolfactura", "0.00");
            rec->setDBvalue("pvplfactura", cur->valor("pvparticulo"));
        } // end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva=" + cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
            rec->setDBvalue("ivalfactura", cur1->valor("porcentasa_iva"));
        } // end if
        delete cur1;
        delete cur;
    } // end if
}


void ListLinFacturaView::cargar(QString idfactura) {
    _depura("ListLinFacturaView::cargar\n", 0);
    mdb_idfactura = idfactura;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM lfactura LEFT JOIN articulo ON lfactura.idarticulo = articulo.idarticulo WHERE idfactura=" + mdb_idfactura);
    SubForm3::cargar(cur);
    delete cur;
}


Fixed ListLinFacturaView::calculabase() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplfactura", i)) * Fixed(DBvalue("cantlfactura", i));
        base = base + totpar;
    } // end for
    return base;
}


Fixed ListLinFacturaView::calculaiva() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplfactura", i)) * Fixed(DBvalue("ivalfactura", i));
        base = base + totpar;
    } // end for
    return base;
}

