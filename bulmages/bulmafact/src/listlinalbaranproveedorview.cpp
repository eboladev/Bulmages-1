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
#include <Q3PopupMenu>
#include <QKeyEvent>
#include <QEvent>

#include "listlinalbaranproveedorview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"


ListLinAlbaranProveedorView::ListLinAlbaranProveedorView(QWidget *parent)
        : SubForm2Bf(parent) {
    setDBTableName("lalbaranp");
    setDBCampoId("numlalbaranp");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("numlalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Numerol albaranp"));
    addSHeader("desclalbaranp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcionl albaranp"));
    addSHeader("cantlalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidadl albaranp"));
    addSHeader("pvplalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl albaranp"));
    addSHeader("ivalalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("IVAl albaranp"));
    addSHeader("descontlalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descontl albaranp"));
    addSHeader("idalbaranp", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id albaranp"));
    setinsercion(TRUE);
}


void ListLinAlbaranProveedorView::on_mui_list_editFinished(int row, int col) {
    _depura("ListLinAlbaranProveedorView::editFinished", 0);
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
            rec->setDBvalue("desclalbaranp", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlalbaranp", "1.00");
            rec->setDBvalue("descontlalbaranp","0.00");
            rec->setDBvalue("pvplalbaranp", cur->valor("pvparticulo"));
        } // end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva=" + cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
            rec->setDBvalue("ivalalbaranp", cur1->valor("porcentasa_iva"));
        } // end if
        delete cur1;
        delete cur;
    } // end if
}


void ListLinAlbaranProveedorView::cargar(QString idalbaranp) {
    _depura("ListLinPedidoProveedorView::cargar\n", 0);
    mdb_idalbaranp = idalbaranp;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM lalbaranp LEFT JOIN articulo ON lalbaranp.idarticulo = articulo.idarticulo WHERE idalbaranp=" + mdb_idalbaranp);
    SubForm3::cargar(cur);
    delete cur;
}


Fixed ListLinAlbaranProveedorView::calculabase() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplalbaranp", i)) * Fixed(DBvalue("cantlalbaranp", i));
        base = base + totpar;
    } // end for
    return base;
}


Fixed ListLinAlbaranProveedorView::calculaiva() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplalbaranp", i)) * Fixed(DBvalue("ivalalbaranp", i));
        base = base + totpar;
    } // end for
    return base;
}

