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
#include "funcaux.h"
#include "blfixed.h"
#include "plugins.h"


///
/**
\param parent
\return
**/
ListLinFacturaProveedorView::ListLinFacturaProveedorView ( QWidget *parent )
        : SubForm2Bf ( parent )
{
    _depura ( "ListLinFacturaProveedorView::ListLinFacturaProveedorView", 0 );
    setDBTableName ( "lfacturap" );
    setDBCampoId ( "idlfacturap" );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView", this );
    if ( res != 0 )
        return;

    addSHeader ( "idarticulo", DBCampo::DBint, DBCampo::DBNotNull, BlSubFormHeader::DBNoView, _( "Articulo" ) );
    addSHeader ( "codigocompletoarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNone, _( "Codigo completo" ) );
    addSHeader ( "nomarticulo", DBCampo::DBvarchar, DBCampo::DBNoSave, BlSubFormHeader::DBNoWrite, _( "Nombre" ) );
    addSHeader ( "idlfacturap", DBCampo::DBint, DBCampo::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Linea" ) );
    addSHeader ( "desclfacturap", DBCampo::DBvarchar, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Descripcion" ) );
    addSHeader ( "cantlfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Cantidad" ) );
    addSHeader ( "pvplfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "P.V.P." ) );
    addSHeader ( "ivalfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "% I.V.A." ) );
    addSHeader ( "reqeqlfacturap", DBCampo::DBnumeric, DBCampo::DBNothing, BlSubFormHeader::DBNone, _( "% Recargo" ) );
    addSHeader ( "descuentolfacturap", DBCampo::DBnumeric, DBCampo::DBNotNull, BlSubFormHeader::DBNone, _( "Descuento" ) );
    addSHeader ( "idfacturap", DBCampo::DBint, DBCampo::DBNotNull, BlSubFormHeader::DBNoView | BlSubFormHeader::DBNoWrite, _( "Factura" ) );
    addSHeader ( "ordenlfacturap", DBCampo::DBint, DBCampo::DBNotNull, BlSubFormHeader::DBNoView, _( "Orden" ) );
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
