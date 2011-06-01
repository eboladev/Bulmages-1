/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QComboBox>
#include <QToolButton>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>

#include "listinventariosimpleview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

///
/**
\param comp
\param parent
**/
ListInventarioSimpleView::ListInventarioSimpleView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "ListInventarioSimpleView::ListInventarioSimpleView", 1 );
    
    setTitleName ( _ ( "InventarioSimple" ) );
    setDbTableName ( "inventariosimple" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->inicializar();
    mui_listado->load();
    insertWindow ( windowTitle(), this, FALSE );
    blScript(this);
    blDebug ( "END ListInventarioSimpleView::ListInventarioSimpleView", 1 );
}

///
/**
**/
void ListInventarioSimpleView::on_mui_aceptar_clicked()
{
    blDebug ( "ListInventarioSimpleView::on_mui_aceptar_clicked", 0 );
    
    try {
        mui_listado->save();
        close();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar el inventario" ) );
    } // end try
    
    blDebug ( "END ListInventarioSimpleView::on_mui_aceptar_clicked", 0 );
}

///
/**
**/
ListInventarioSimpleView::~ListInventarioSimpleView()
{
    blDebug ( "ListInventarioSimpleView::~ListInventarioSimpleView", 0 );
    blDebug ( "END ListInventarioSimpleView::~ListInventarioSimpleView", 0 );
}

/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListInventarioSimpleSubForm::ListInventarioSimpleSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ListInventarioSimpleSubForm::ListInventarioSimpleSubForm", 0 );
    
    setDbTableName ( "inventariosimple" );
    setDbFieldId ( "idinventariosimple" );
    addSubFormHeader ( "idinventariosimple", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id Inventario" ) );
    addSubFormHeader ( "nominventariosimple", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre" ) );
    addSubFormHeader ( "stockinventariosimple", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Stock" ) );
    setInsert ( TRUE );
    setOrdenEnabled ( TRUE );
    blDebug ( "END ListInventarioSimpleSubForm::ListInventarioSimpleSubForm", 0 );
}

///
/**
**/
void ListInventarioSimpleSubForm::load()
{
    blDebug ( "ListInventarioSimpleSubForm::cargar", 0 );
    
    BlSubForm::load ( "SELECT * FROM inventariosimple" );
    
    blDebug ( "END ListInventarioSimpleSubForm::cargar", 0 );
}
