/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <fstream>

#include "listmodificadoresview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
ListModificadoresView::ListModificadoresView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "INIT_ListModificadoresView::ListModificadoresView", 1 );
    setTitleName ( _ ( "Modificadores" ) );
    setDbTableName ( "modificador" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this );
    blScript(this);
    blDebug ( "END_ListModificadoresView::ListModificadoresView", 1 );
}


///
/**
**/
ListModificadoresView::~ListModificadoresView()
{
    blDebug ( "ListModificadoresView::~ListModificadoresView", 0 );
    blDebug ( "END ListModificadoresView::~ListModificadoresView", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListModificadoresSubForm::ListModificadoresSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ListModificadoresSubForm::ListModificadoresSubForm", 0 );
    setDbTableName ( "modificador" );
    setDbFieldId ( "idmodificador" );
    addSubFormHeader ( "idmodificador", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "Id modificador" );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView | BlSubFormHeader::DbDisableView, "Id articulo" );
    addSubFormHeader ( "nombremodificador", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre modificador" ) );
    addSubFormHeader ( "varpreciomodificador", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Alt. precio" ) );
    setInsert ( TRUE );
    blDebug ( "END ListModificadoresSubForm::ListModificadoresSubForm", 0 );
}

