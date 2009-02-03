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

#include "listdescalbaranprovview.h"
#include "articulolist.h"
#include "comparticulolistview.h"
#include "funcaux.h"


///
/**
\param parent
**/
ListDescuentoAlbaranProvView::ListDescuentoAlbaranProvView ( QWidget *parent )
        : SubForm2Bf ( parent )
{
    _depura ( "ListDescuentoAlbaranProvView::ListDescuentoAlbaranProvView", 0 );
    setDBTableName ( "dalbaranp" );
    setDBCampoId ( "iddalbaranp" );
    addSHeader ( "iddalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id descuento" ) );
    addSHeader ( "conceptdalbaranp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Concepto descuento" ) );
    addSHeader ( "proporciondalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idalbaranp", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id albaran" ) );
    setinsercion ( TRUE );
    _depura ( "END ListDescuentoAlbaranProvView::ListDescuentoAlbaranProvView", 0 );
}

