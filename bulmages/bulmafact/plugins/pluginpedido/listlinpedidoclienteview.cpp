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
    addSHeader ( "puntlpedidocliente", DBCampo::DBboolean, DBCampo::DBNotNull, SHeader::DBNone, _( "Punteo" ) );
    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "numlpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, _( "No de linea" ) );
    addSHeader ( "desclpedidocliente", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlpedidocliente", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "Precio de venta s/IVA" ) );
    addSHeader ( "ivalpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlpedidocliente", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "% Recargo E.Q." ) );
    addSHeader ( "descuentolpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id pedido cliente" ) );
    addSHeader ( "ordenlpedidocliente", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Orden" ) );

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



