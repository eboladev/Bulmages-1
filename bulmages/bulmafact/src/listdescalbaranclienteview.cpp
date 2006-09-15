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
#include <QMenu>
#include <QKeyEvent>
#include <QEvent>

#include "listdescalbaranclienteview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoAlbaranClienteView::ListDescuentoAlbaranClienteView(QWidget *parent)
        : SubForm2Bf(parent) {
    setDBTableName("dalbaran");
    setDBCampoId("iddalbaran");
    addSHeader("iddalbaran", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idd albaran"));
    addSHeader("conceptdalbaran", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Conceptd albaran"));
    addSHeader("proporciondalbaran", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Proporciond albaran"));
    addSHeader("idalbaran", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id albaran"));
    setinsercion(TRUE);
}

void ListDescuentoAlbaranClienteView::cargar(QString idalbaran) {
        _depura("ListDescuentoAlbaranClienteView::cargar\n", 0);
        mdb_idalbaran = idalbaran;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM dalbaran WHERE idalbaran=" + mdb_idalbaran);
        SubForm3::cargar(cur);
        delete cur;
}
