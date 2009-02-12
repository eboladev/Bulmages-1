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

    addSHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Id articulo" ) );
    addSHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo articulo" ) );
    addSHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre articulo" ) );
    addSHeader ( "numlalbaranp", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Numero de linea" ) );
    addSHeader ( "desclalbaranp", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlalbaranp", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplalbaranp", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Precio" ) );
    addSHeader ( "ivalalbaranp", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlalbaranp", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "% R.E." ) );
    addSHeader ( "descuentolalbaranp", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% Descuento" ) );
    addSHeader ( "idalbaranp", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Id albaran" ) );
    addSHeader ( "ordenlalbaranp", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
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
    BlSubForm::cargar ( "SELECT * FROM lalbaranp LEFT JOIN articulo ON lalbaranp.idarticulo = articulo.idarticulo WHERE idalbaranp = " + mdb_idalbaranp + " ORDER BY ordenlalbaranp" );
    _depura ( "END ListLinPedidoProveedorView::cargar", 0 );
}



