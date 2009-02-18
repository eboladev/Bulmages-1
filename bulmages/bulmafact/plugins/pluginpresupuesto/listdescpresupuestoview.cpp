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

#include "listdescpresupuestoview.h"
#include "blfunctions.h"


///
/**
\param parent
**/
ListDescuentoPresupuestoView::ListDescuentoPresupuestoView ( QWidget *parent )
        : BfSubForm ( parent )
{
    _depura ( "ListDescuentoPresupuestoView::ListDescuentoPresupuestoView", 0 );
    setDBTableName ( "dpresupuesto" );
    setDBCampoId ( "iddpresupuesto" );
    addSubFormHeader ( "iddpresupuesto", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id descuento" ) );
    addSubFormHeader ( "conceptdpresupuesto", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Concepto descuento" ) );
    addSubFormHeader ( "proporciondpresupuesto", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSubFormHeader ( "idpresupuesto", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id presupuesto" ) );
    setinsercion ( TRUE );
    _depura ( "END ListDescuentoPresupuestoView::ListDescuentoPresupuestoView", 0 );
}

