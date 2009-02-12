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

#include "listlinpedidoclienteview.h"
#include "funcaux.h"
#include "blfixed.h"


///
/**
\param parent
\return
**/
ListLinPedidoClienteView::ListLinPedidoClienteView ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLinPedidoClienteView::ListLinPedidoClienteView", 0 );
    setDBTableName ( "lpedidocliente" );
    setDBCampoId ( "numlpedidocliente" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinPedidoClienteView_ListLinPedidoClienteView", this );
    if ( res != 0 ) {
        return;
    } // end if
    addSHeader ( "puntlpedidocliente", BlDbField::DBboolean, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Punteo" ) );
    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "numlpedidocliente", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "No de linea" ) );
    addSHeader ( "desclpedidocliente", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlpedidocliente", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplpedidocliente", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Precio de venta s/IVA" ) );
    addSHeader ( "ivalpedidocliente", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlpedidocliente", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "% Recargo E.Q." ) );
    addSHeader ( "descuentolpedidocliente", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idpedidocliente", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id pedido cliente" ) );
    addSHeader ( "ordenlpedidocliente", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );

    /// Establecemos los modos de comportamiento del subformulario
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    setOrdenPorQuery ( FALSE );

    /// Lanzamos los plugins
    g_plugins->lanza ( "ListLinPedidoClienteView_ListLinPedidoClienteView_Post", this );
    _depura ( "END ListLinPedidoClienteView::ListLinPedidoClienteView", 0 );
}


///
/**
\param idpedidocliente
**/
void ListLinPedidoClienteView::cargar ( QString idpedidocliente )
{
    _depura ( "ListLinPedidoClienteView::cargar", 0 );
    mdb_idpedidocliente = idpedidocliente;
    BlSubForm::cargar ( "SELECT * FROM lpedidocliente LEFT JOIN articulo ON lpedidocliente.idarticulo = articulo.idarticulo WHERE idpedidocliente = " + mdb_idpedidocliente + " ORDER BY ordenlpedidocliente" );
    _depura ( "END ListLinPedidoClienteView::cargar", 0 );
}



