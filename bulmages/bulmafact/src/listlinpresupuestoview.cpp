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
#include <QWaitCondition>

#include "articulolist.h"
#include "listlinpresupuestoview.h"
#include "funcaux.h"
#include "fixed.h"


ListLinPresupuestoView::ListLinPresupuestoView(QWidget *parent) : SubForm2Bf(parent) {
    _depura("ListLinPresupuestoView::ListLinPresupuestoView", 0);
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
    addSHeader("reqeqlpresupuesto", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, tr("% Recargo E.Q."));
    addSHeader("descuentolpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, tr("% Descuento"));
    addSHeader("idpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id presupuesto"));
    addSHeader("ordenlpresupuesto", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);
    _depura("END ListLinPresupuestoView::ListLinPresupuestoView", 0);
}


void ListLinPresupuestoView::cargar(QString idpresupuesto) {
    _depura("ListLinPresupuestoView::cargar", 0);
    mdb_idpresupuesto = idpresupuesto;
    /// Al hacer la carge usamos el ordenlpresupuesto para indicar que el campo de ordenacion es el que toca.
    cursor2 *cur = empresaBase()->cargacursor("SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE idpresupuesto = " + mdb_idpresupuesto +" ORDER BY ordenlpresupuesto");
    SubForm3::cargar(cur);
    delete cur;
    _depura("END ListLinPresupuestoView::cargar", 0);
}






