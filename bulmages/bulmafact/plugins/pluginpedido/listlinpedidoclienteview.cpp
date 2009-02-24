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
#include "blfunctions.h"
#include "blfixed.h"


///
/**
\param parent
\return
**/
ListLinPedidoClienteView::ListLinPedidoClienteView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListLinPedidoClienteView::ListLinPedidoClienteView", 0 );
    setDbTableName ( "lpedidocliente" );
    setDbFieldId ( "numlpedidocliente" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinPedidoClienteView_ListLinPedidoClienteView", this );
    if ( res != 0 ) {
        return;
    } // end if
    addSubFormHeader ( "puntlpedidocliente", BlDbField::DbBoolean, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Punteo" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoView, _( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _( "Nombre articulo" ) );
    addSubFormHeader ( "numlpedidocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoView, _( "No de linea" ) );
    addSubFormHeader ( "desclpedidocliente", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Descripcion" ) );
    addSubFormHeader ( "cantlpedidocliente", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Cantidad" ) );
    addSubFormHeader ( "pvplpedidocliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "Precio de venta s/IVA" ) );
    addSubFormHeader ( "ivalpedidocliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "% I.V.A." ) );
    addSubFormHeader ( "reqeqlpedidocliente", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _( "% Recargo E.Q." ) );
    addSubFormHeader ( "descuentolpedidocliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _( "% Descuento" ) );
    addSubFormHeader ( "idpedidocliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoView | BlSubFormHeader::DbNoWrite, _( "Id pedido cliente" ) );
    addSubFormHeader ( "ordenlpedidocliente", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNoView, _( "Orden" ) );

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



