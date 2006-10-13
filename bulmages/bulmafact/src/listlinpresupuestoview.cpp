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
#include "listlinpresupuestoview.h"
#include "funcaux.h"
#include "fixed.h"


listlinpresupuestoview::listlinpresupuestoview(QWidget *parent) : SubForm2Bf(parent) {
    setDBTableName("lpresupuesto");
    setDBCampoId("idlpresupuesto");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre del articulo"));
    addSHeader("idlpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("No de linea"));
    addSHeader("desclpresupuesto", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcion presupuesto"));
    addSHeader("cantlpresupuesto", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidad"));
    addSHeader("pvplpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("Precio de venta s/IVA"));
    addSHeader("ivalpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("% IVA"));
    addSHeader("descuentolpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("% Descuento"));
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id presupuesto"));
    setinsercion(TRUE);
}


void listlinpresupuestoview::on_mui_list_editFinished(int row, int col, int key) {
    _depura("listlinpresupuestoview::editFinished", 0);
    SDBRecord *rec = lineaat(row);
    SDBCampo *camp = (SDBCampo *) item(row, col);
    camp->refresh();
    if (camp->nomcampo() == "codigocompletoarticulo") {
        cursor2 *cur = companyact()->cargacursor("SELECT * FROM articulo WHERE codigocompletoarticulo = '" + camp->text() + "'");
        if (!cur->eof()) {
            rec->setDBvalue("idarticulo", cur->valor("idarticulo"));
            rec->setDBvalue("codigocompletoarticulo", cur->valor("codigocompletoarticulo"));
            rec->setDBvalue("nomarticulo", cur->valor("nomarticulo"));
            rec->setDBvalue("desclpresupuesto", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlpresupuesto", "1.00");
            rec->setDBvalue("descuentolpresupuesto", "0.00");
            rec->setDBvalue("pvplpresupuesto", cur->valor("pvparticulo"));
        } // end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva = " + cur->valor("idtipo_iva") + " ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
            rec->setDBvalue("ivalpresupuesto", cur1->valor("porcentasa_iva"));
        } // end if
        delete cur1;
        delete cur;
    } // end if
    SubForm3::on_mui_list_editFinished(row, col, key);    
    _depura("END listlinpresupuestoview::editFinished", 0);
}


void listlinpresupuestoview::cargar(QString idpresupuesto) {
    _depura("listlinpresupuestoview::cargar\n", 0);
    mdb_idpresupuesto = idpresupuesto;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE idpresupuesto = " + mdb_idpresupuesto);
    SubForm3::cargar(cur);
    delete cur;
}


Fixed listlinpresupuestoview::calculabase() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplpresupuesto", i)) * Fixed(DBvalue("cantlpresupuesto", i));
        base = base + totpar;
    } // end for
    return base;
}


Fixed listlinpresupuestoview::calculaiva() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplpresupuesto", i)) * Fixed(DBvalue("ivalpresupuesto", i));
        base = base + totpar;
    } // end for
    return base;
}

