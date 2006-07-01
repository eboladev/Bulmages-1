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

#include "listdescfacturaprovview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


ListDescuentoFacturaProvView::ListDescuentoFacturaProvView(QWidget *parent)
        : SubForm2Bf(parent) {
    setDBTableName("dfacturap");
    setDBCampoId("iddfacturap");
    addSHeader("iddfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, tr("Idd facturap"));
    addSHeader("conceptdfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, tr("Conceptd facturap"));
    addSHeader("proporciondfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, tr("Proporciond facturap"));
    addSHeader("idfacturap", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, tr("Id facturap"));
    setinsercion(TRUE);
}

