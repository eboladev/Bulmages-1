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
#include <QtWidgets/QMenu>
#include <QtGui/QKeyEvent>
#include <QtCore/QEvent>

#include "listdescpresupuestoview.h"
#include "blfunctions.h"


///
/**
\param parent
**/
ListDescuentoPresupuestoView::ListDescuentoPresupuestoView ( QWidget *parent )
        : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "dpresupuesto" );
    setDbFieldId ( "iddpresupuesto" );
    addSubFormHeader ( "iddpresupuesto", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id descuento" ) );
    addSubFormHeader ( "conceptdpresupuesto", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Concepto descuento" ) );
    addSubFormHeader ( "proporciondpresupuesto", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "% Descuento" ) );
    addSubFormHeader ( "idpresupuesto", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id presupuesto" ) );
    setInsert ( true );
    
}

