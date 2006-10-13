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


ListLinFacturaProveedorView::ListLinFacturaProveedorView(QWidget *parent)
        : SubForm2Bf(parent) {
    setDBTableName("lfacturap");
    setDBCampoId("idlfacturap");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre"));
    addSHeader("idlfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Linea"));
    addSHeader("desclfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcion"));
    addSHeader("cantlfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad"));
    addSHeader("pvplfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("P.V.P."));
    addSHeader("ivalfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("% I.V.A."));
    addSHeader("descuentolfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descuento"));
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Factura"));
    setinsercion(TRUE);
}


void ListLinFacturaProveedorView::on_mui_list_editFinished(int row, int col, int key) {
    _depura("ListLinAlbaranProveedorView::editFinished", 0);
    SubForm3::on_mui_list_editFinished(row, col, key);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'");
        if (!cur->eof()) {
            rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclfacturap", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlfacturap", "1.00");
            rec->setDBvalue("descuentolfacturap", "0.00");
            rec->setDBvalue("pvplfacturap", cur->valor("pvparticulo"));
        } // end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof()) {
            rec->setDBvalue("ivalfacturap", cur1->valor("porcentasa_iva"));
        } // end if
        delete cur1;
        delete cur;
    } // end if
}


void ListLinFacturaProveedorView::cargar(QString idfacturap) {
    _depura("ListLinFacturaProveedorView::cargar\n", 0);
    mdb_idfacturap = idfacturap;
    cursor2 *cur = companyact()->cargacursor("SELECT * FROM lfacturap LEFT JOIN articulo ON lfacturap.idarticulo = articulo.idarticulo WHERE idfacturap = " + mdb_idfacturap);
    SubForm3::cargar(cur);
    delete cur;
}


Fixed ListLinFacturaProveedorView::calculabase() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplfacturap", i)) * Fixed(DBvalue("cantlfacturap", i));
        base = base + totpar;
    } // end for
    return base;
}


Fixed ListLinFacturaProveedorView::calculaiva() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplfacturap", i)) * Fixed(DBvalue("ivalfacturap", i)) / 100;
        base = base + totpar;
    } // end for
    return base;
}

