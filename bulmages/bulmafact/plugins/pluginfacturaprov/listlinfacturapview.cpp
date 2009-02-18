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

#include "listlinfacturapview.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
\return
**/
ListLinFacturaProveedorView::ListLinFacturaProveedorView ( QWidget *parent )
        : BfSubForm ( parent )
{
    _depura ( "ListLinFacturaProveedorView::ListLinFacturaProveedorView", 0 );
    setDBTableName ( "lfacturap" );
    setDBCampoId ( "idlfacturap" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView", this );
    if ( res != 0 )
        return;

    addSubFormHeader ( "idarticulo", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DBvarchar, BlDbField::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre" ) );
    addSubFormHeader ( "idlfacturap", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Linea" ) );
    addSubFormHeader ( "desclfacturap", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSubFormHeader ( "cantlfacturap", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSubFormHeader ( "pvplfacturap", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "P.V.P." ) );
    addSubFormHeader ( "ivalfacturap", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "% I.V.A." ) );
    addSubFormHeader ( "reqeqlfacturap", BlDbField::DBnumeric, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "% Recargo" ) );
    addSubFormHeader ( "descuentolfacturap", BlDbField::DBnumeric, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Descuento" ) );
    addSubFormHeader ( "idfacturap", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Factura" ) );
    addSubFormHeader ( "ordenlfacturap", BlDbField::DBint, BlDbField::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );

    /// Disparamos los plugins.
    g_plugins->lanza ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView_Post", this );

    _depura ( "END ListLinFacturaProveedorView::ListLinFacturaProveedorView", 0 );
}

///
/**
\param idfacturap
**/
void ListLinFacturaProveedorView::cargar ( QString idfacturap )
{
    _depura ( "ListLinFacturaProveedorView::cargar", 0 );
    mdb_idfacturap = idfacturap;
    BlSubForm::cargar ( "SELECT * FROM lfacturap LEFT JOIN articulo ON lfacturap.idarticulo = articulo.idarticulo WHERE idfacturap = " + mdb_idfacturap + " ORDER BY ordenlfacturap" );
    _depura ( "END ListLinFacturaProveedorView::cargar", 0 );
}
