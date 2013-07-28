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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>
#include <QtCore/QTextStream>
#include <QtWidgets/QLayout>
#include <QtWidgets/QMessageBox>

#include "listtallasview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/

ListTallasView::ListTallasView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Tallas" ) );
    setDbTableName ( "tc_talla" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->load();
    insertWindow ( windowTitle(), this, false );
    blScript(this);
    
}


///
/**
**/

void ListTallasView::on_mui_aceptar_clicked()
{
    BL_FUNC_DEBUG
    try {
        mui_listado->save();
        close();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar los colores" ), this );
    } // end try
    
}


///
/**
**/

ListTallasView::~ListTallasView()
{
    BL_FUNC_DEBUG
    
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/

ListTallasSubForm::ListTallasSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "tc_talla" );
    setDbFieldId ( "idtc_talla" );
    addSubFormHeader ( "idtc_talla", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView, _ ( "Id color" ) );
    addSubFormHeader ( "nomtc_talla", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre talla" ) );
    setInsert ( true );
    
}


///
/**
**/

void ListTallasSubForm::load()
{
    BL_FUNC_DEBUG
    BlSubForm::load ( "SELECT * FROM tc_talla" );
    
}

