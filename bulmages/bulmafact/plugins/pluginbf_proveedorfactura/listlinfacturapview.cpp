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

#include <QtWidgets/QMessageBox>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include "listlinfacturapview.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "blplugins.h"


///
/**
\param parent
\return
**/
ListLinFacturaProveedorView::ListLinFacturaProveedorView ( QWidget *parent )
        : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "lfacturap" );
    setDbFieldId ( "idlfacturap" );

    /// Disparamos los plugins.
    int res = g_plugins->run ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView", this );
    if ( res != 0 )
        return;

    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, _ ( "Articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo completo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    addSubFormHeader ( "idlfacturap", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, _ ( "Linea" ) );
    addSubFormHeader ( "desclfacturap", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Descripcion" ) );
    addSubFormHeader ( "cantlfacturap", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad" ) );
    addSubFormHeader ( "pvplfacturap", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "P.V.P." ) );
    addSubFormHeader ( "ivalfacturap", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "% I.V.A." ) );
    addSubFormHeader ( "reqeqlfacturap", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "% Recargo" ) );
    addSubFormHeader ( "descuentolfacturap", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Descuento" ) );
    addSubFormHeader ( "idfacturap", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbDisableView, _ ( "Factura" ) );
    addSubFormHeader ( "ordenlfacturap", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, _ ( "Orden" ) );
    setInsert ( true );
    setOrdenEnabled ( true );

    /// Disparamos los plugins.
    g_plugins->run ( "ListLinFacturaProveedorView_ListLinFacturaProveedorView_Post", this );

    
}

///
/**
\param idfacturap
**/
void ListLinFacturaProveedorView::load ( QString idfacturap )
{
    BL_FUNC_DEBUG
    mdb_idfacturap = idfacturap;
    BlSubForm::load ( "SELECT * FROM lfacturap LEFT JOIN articulo ON lfacturap.idarticulo = articulo.idarticulo WHERE idfacturap = " + mdb_idfacturap + " ORDER BY ordenlfacturap" );
    
}
