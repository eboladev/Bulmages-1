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

#include "listlinalbaranclienteview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"


ListLinAlbaranClienteView::ListLinAlbaranClienteView(QWidget *parent)
        : SubForm2Bf(parent) {
    setDBTableName("lalbaran");
    setDBCampoId("numlalbaran");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("numlalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Numerol albaran"));
    addSHeader("desclalbaran", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcionl albaran"));
    addSHeader("cantlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidadl albaran"));
    addSHeader("pvplalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl albaran"));
    addSHeader("ivalalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("IVAl albaran"));
    addSHeader("descontlalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descontl albaran"));
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id albaran"));
    addSHeader("ordenlalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);
}


void ListLinAlbaranClienteView::on_mui_list_editFinished(int row, int col, int key) {
    _depura("ListLinAlbaranClienteView::editFinished", 0);
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
            rec->setDBvalue("desclalbaran", cur->valor("nomarticulo"));
            rec->setDBvalue("cantlalbaran", "1.00");
            rec->setDBvalue("descontlalbaran", "0.00");
            rec->setDBvalue("pvplalbaran", cur->valor("pvparticulo"));
        } // end if
        cursor2 *cur1 = companyact()->cargacursor("SELECT * FROM tasa_iva WHERE idtipo_iva=" + cur->valor("idtipo_iva") + "ORDER BY fechatasa_iva LIMIT 1");
        if (!cur->eof() ) {
            rec->setDBvalue("ivalalbaran", cur1->valor("porcentasa_iva"));
        } // end if
        delete cur1;
        delete cur;
    } // end if
}


void ListLinAlbaranClienteView::cargar(QString idalbaran) {
    _depura("ListLinAlbaranClienteView::cargar\n", 0);
    mdb_idalbaran = idalbaran;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran=" + mdb_idalbaran + " ORDER BY ordenlalbaran");
    SubForm3::cargar(cur);
    delete cur;
}


Fixed ListLinAlbaranClienteView::calculabase() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplalbaran", i)) * Fixed(DBvalue("cantlalbaran", i));
        base = base + totpar;
    } // end for
    return base;
}


Fixed ListLinAlbaranClienteView::calculaiva() {
    Fixed base("0.0");
    for (int i = 0; i < rowCount() - 1; i++) {
        Fixed totpar = Fixed(DBvalue("pvplalbaran", i)) * Fixed(DBvalue("ivalalbaran", i));
        base = base + totpar;
    } // end for
    return base;
}

