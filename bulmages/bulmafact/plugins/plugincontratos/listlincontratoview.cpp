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

#include "listlincontratoview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "fixed.h"
#include "plugins.h"


ListLinContratoView::ListLinContratoView(QWidget *parent) : SubForm2Bf(parent) {
    _depura("ListLinContratoView::ListLinContratoView", 0);
    setDBTableName("lcontrato");
    setDBCampoId("idlcontrato");
    addSHeader("idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id articulo"));
    addSHeader("codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, tr("Codigo completo articulo"));
    addSHeader("nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, tr("Nombre articulo"));
    addSHeader("idlcontrato", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idl contrato"));
    addSHeader("desclcontrato", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Descripcionl contrato"));
    addSHeader("cantlcontrato", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Cantidadl contrato"));
    addSHeader("pvplcontrato", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("PVPl contrato"));
    addSHeader("idcontrato", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id contrato"));
    addSHeader("ordenlcontrato", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Orden"));
    setinsercion(TRUE);
    setOrdenEnabled(TRUE);
    _depura("END ListLinContratoView::ListLinContratoView", 0);
}


void ListLinContratoView::cargar(QString idcontrato) {
    _depura("ListLinContratoView::cargar", 0);
    mdb_idcontrato = idcontrato;
    SubForm3::cargar("SELECT * FROM lcontrato LEFT JOIN articulo ON lcontrato.idarticulo = articulo.idarticulo WHERE idcontrato=" + mdb_idcontrato + " ORDER BY ordenlcontrato");
    _depura("END ListLinContratoView::cargar", 0);
}



