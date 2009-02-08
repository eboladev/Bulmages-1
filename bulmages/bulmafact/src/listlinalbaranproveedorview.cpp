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

#include "listlinalbaranproveedorview.h"
#include "articulolist.h"
#include "funcaux.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
\return
**/
ListLinAlbaranProveedorView::ListLinAlbaranProveedorView ( QWidget *parent ) : SubForm2Bf ( parent )
{
    _depura ( "ListLinAlbaranProveedorView::ListLinAlbaranProveedorView", 0 );
    setDBTableName ( "lalbaranp" );
    setDBCampoId ( "numlalbaranp" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinAlbaranProveedorView_ListLinAlbaranProveedorView", this );
    if ( res != 0 )
        return;

    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "numlalbaranp", DBCampo::DBint, DBCampo::DBPrimaryKey, SHeader::DBNoView, _( "Numero de linea" ) );
    addSHeader ( "desclalbaranp", DBCampo::DBvarchar, DBCampo::DBNotNull, SHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "Precio" ) );
    addSHeader ( "ivalalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlalbaranp", DBCampo::DBnumeric, DBCampo::DBNothing, SHeader::DBNone, _( "% R.E." ) );
    addSHeader ( "descuentolalbaranp", DBCampo::DBnumeric, DBCampo::DBNotNull, SHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idalbaranp", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView | SHeader::DBNoWrite, _( "Id albaran" ) );
    addSHeader ( "ordenlalbaranp", DBCampo::DBint, DBCampo::DBNotNull, SHeader::DBNoView, _( "Orden" ) );
    setinsercion ( TRUE );
    setDelete ( TRUE );
    setOrdenEnabled ( TRUE );
    /// Disparamos los plugins.
    g_plugins->lanza ( "ListLinAlbaranProveedorView_ListLinAlbaranProveedorView_Post", this );
    _depura ( "END ListLinAlbaranProveedorView::ListLinAlbaranProveedorView", 0 );
}


///
/**
\param isalbaranp
**/
void ListLinAlbaranProveedorView::cargar ( QString idalbaranp )
{
    _depura ( "ListLinPedidoProveedorView::cargar", 0 );
    mdb_idalbaranp = idalbaranp;
    SubForm3::cargar ( "SELECT * FROM lalbaranp LEFT JOIN articulo ON lalbaranp.idarticulo = articulo.idarticulo WHERE idalbaranp = " + mdb_idalbaranp + " ORDER BY ordenlalbaranp" );
    _depura ( "END ListLinPedidoProveedorView::cargar", 0 );
}



