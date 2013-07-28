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

#include "bfcompany.h"
#include "blconfiguration.h"
#include "listtipostrabajoview.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
ListTiposTrabajoView::ListTiposTrabajoView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( _ ( "almacen" ) );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );

    mui_listado->setDbTableName ( "tipotrabajo" );
    mui_listado->setDbFieldId ( "idtipotrabajo" );
    mui_listado->addSubFormHeader ( "idtipotrabajo", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView, _ ( "Id tipo trabajo" ) );
    mui_listado->addSubFormHeader ( "nomtipotrabajo", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Nombre tipo trabajo" ) );
    mui_listado->addSubFormHeader ( "colortipotrabajo", BlDbField::DbVarChar, BlDbField::DbNothing , BlSubFormHeader::DbNone, _ ( "Color tipo trabajo" ) );
    mui_listado->setInsert ( true );

    mui_listado->load ( "SELECT * FROM tipotrabajo" );
    insertWindow ( windowTitle(), this );
    blScript(this);
    
}


///
/**
**/
void ListTiposTrabajoView::on_mui_aceptar_clicked()
{
    try {
        mui_listado->save();
        g_theApp->emitDbTableChanged ( "tipotrabajo" );
        close();
    } catch ( ... ) {
        blMsgInfo ( "Error al guardar los tipos de trabajo" );
    } // end try
}


///
/**
**/
ListTiposTrabajoView::~ListTiposTrabajoView()
{
    BL_FUNC_DEBUG
    
}


