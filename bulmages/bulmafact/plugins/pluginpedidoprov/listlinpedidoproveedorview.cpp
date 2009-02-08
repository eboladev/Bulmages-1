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

#include "articulolist.h"
#include "listlinpedidoproveedorview.h"
#include "funcaux.h"
#include "blfixed.h"


///
/**
\param parent
**/
ListLinPedidoProveedorView::ListLinPedidoProveedorView ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLinPedidoProveedorView::ListLinPedidoProveedorView", 0 );
    setDBTableName ( "lpedidoproveedor" );
    setDBCampoId ( "idlpedidoproveedor" );
    addSHeader ( "puntlpedidoproveedor", DBCampo::DBboolean, DBCampo::DBNotNull, SHeader::DBNone, _( "Punteo" ) );
    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "idlpedidoproveedor", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, _( "Numero de linea" ) );
    addSHeader ( "desclpedidoproveedor", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlpedidoproveedor", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "Precio" ) );
    addSHeader ( "ivalpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlpedidoproveedor", DBCampo::DBint, DBCampo::DBNothing, SHeader::DBNone, _( "% R.E." ) );
    addSHeader ( "descuentolpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id pedido" ) );
    addSHeader ( "ordenlpedidoproveedor", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Orden" ) );
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
    SubForm3::cargar ( "SELECT * FROM lpedidoproveedor LEFT JOIN articulo ON lpedidoproveedor.idarticulo = articulo.idarticulo WHERE idpedidoproveedor=" + mdb_idpedidoproveedor + " ORDER BY ordenlpedidoproveedor" );
    _depura ( "END ListLinPedidoProveedorView::cargar", 0 );
}

