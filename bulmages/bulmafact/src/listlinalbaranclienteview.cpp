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
    addSHeader("reqeqlalbaran", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, tr("% Recargo E.Q."));
    addSHeader("descuentolalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Descontl albaran"));
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id albaran"));
    addSHeader("ordenlalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);
}

void ListLinAlbaranClienteView::cargar(QString idalbaran) {
    _depura("ListLinAlbaranClienteView::cargar", 0);
    mdb_idalbaran = idalbaran;
    cursor2 * cur= companyact()->cargacursor("SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran=" + mdb_idalbaran + "   ORDER BY ordenlalbaran");
    SubForm3::cargar(cur);
    delete cur;
    _depura("END ListLinAlbaranClienteView::cargar", 0);
}


