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

#include "listdescpedidoclienteview.h"
#include "blfunctions.h"


///
/**
\param parent
**/
ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView ( QWidget *parent )
        : BfSubForm ( parent )
{
    _depura ( "ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView", 0 );
    setDBTableName ( "dpedidocliente" );
    setDBCampoId ( "iddpedidocliente" );
    addSubFormHeader ( "iddpedidocliente", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id descuento" ) );
    addSubFormHeader ( "conceptdpedidocliente", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Concepto descuento" ) );
    addSubFormHeader ( "proporciondpedidocliente", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSubFormHeader ( "idpedidocliente", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id pedido" ) );
    setinsercion ( TRUE );
    _depura ( "END ListDescuentoPedidoClienteView::ListDescuentoPedidoClienteView", 0 );
}


void ListDescuentoPedidoClienteView::cargar ( QString idpedidocliente )
{
    _depura ( "ListDescuentoPedidoClienteView::cargar", 0 );
    mdb_idpedidocliente = idpedidocliente;
    BlSubForm::cargar ( "SELECT * FROM dpedidocliente WHERE idpedidocliente = " + mdb_idpedidocliente );
    _depura ( "END ListDescuentoPedidoClienteView::cargar", 0 );
}

