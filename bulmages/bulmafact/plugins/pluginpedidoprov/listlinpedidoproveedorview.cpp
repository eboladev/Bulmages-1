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

#include "listlinpedidoproveedorview.h"
#include "blfunctions.h"
#include "blfixed.h"


///
/**
\param parent
**/
ListLinPedidoProveedorView::ListLinPedidoProveedorView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0 );
    setDBTableName ( "lpedidoproveedor" );
    setDBCampoId ( "idlpedidoproveedor" );
    addSHeader ( "puntlpedidoproveedor", BlDbField::DBboolean, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Punteo" ) );
    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "idlpedidoproveedor", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Numero de linea" ) );
    addSHeader ( "desclpedidoproveedor", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlpedidoproveedor", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplpedidoproveedor", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Precio" ) );
    addSHeader ( "ivalpedidoproveedor", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlpedidoproveedor", BlDbField::DBint, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "% R.E." ) );
    addSHeader ( "descuentolpedidoproveedor", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idpedidoproveedor", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id pedido" ) );
    addSHeader ( "ordenlpedidoproveedor", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Orden" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    setOrdenPorQuery ( FALSE );
    _depura ( "END ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0 );
}


///
/**
\param idpedidoproveedor
**/
void ListLinPedidoProveedorView::cargar ( QString idpedidoproveedor )
{
    _depura ( "ListLinPedidoProveedorView::cargar", 0 );
    mdb_idpedidoproveedor = idpedidoproveedor;
    BlSubForm::cargar ( "SELECT * FROM lpedidoproveedor LEFT JOIN articulo ON lpedidoproveedor.idarticulo = articulo.idarticulo WHERE idpedidoproveedor=" + mdb_idpedidoproveedor + " ORDER BY ordenlpedidoproveedor" );
    _depura ( "END ListLinPedidoProveedorView::cargar", 0 );
}

