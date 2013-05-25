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

#include "listcontrolstockview.h"


///
/**
\param parent
**/
ListControlStockView::ListControlStockView ( QWidget *parent, const char * )
        : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "controlstock" );
    setDbFieldId ( "idarticulo" );
    addSubFormHeader ( "punteocontrolstock", BlDbField::DbBoolean, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Punteado" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "Codigo almacen" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre almacen" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo completo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre articulo" ) );
    addSubFormHeader ( "stockantcontrolstock", BlDbField::DbNumeric, BlDbField::DbNothing, BlSubFormHeader::DbNoWrite, _ ( "Stock actual" ) );
    addSubFormHeader ( "stocknewcontrolstock", BlDbField::DbNumeric, BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Stock revisado" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, _ ( "Id articulo" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, "Id almacen" );
    addSubFormHeader ( "idinventario", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNoWrite | BlSubFormHeader::DbHideView, "Id inventario" );
    setInsert ( false );
    mdb_idinventario = "";
    
}


///
/**
**/
ListControlStockView::~ListControlStockView()
{
    BL_FUNC_DEBUG
}

