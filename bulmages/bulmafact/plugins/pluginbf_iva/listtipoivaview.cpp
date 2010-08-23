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

#include "listtipoivaview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
ListTipoIVAView::ListTipoIVAView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    blDebug ( "ListTipoIVAView::ListTipoIVAView", 1 );
    setTitleName ( _ ( "Tipo IVA" ) );
    setDbTableName ( "tipo_iva" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this );
    blDebug ( "END ListTipoIVAView::ListTipoIVAView", 1 );
}


///
/**
**/
void ListTipoIVAView::on_mui_aceptar_clicked()
{
    blDebug ( "ListTipoIVAView::on_mui_aceptar_clicked", 0 );
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al guardar los tipos de IVA" ) );
    } // end try
    blDebug ( "END ListTipoIVAView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
ListTipoIVAView::~ListTipoIVAView()
{
    blDebug ( "ListTipoIVAView::~ListTipoIVAView", 0 );
    blDebug ( "END ListTipoIVAView::~ListTipoIVAView", 0 );
}

/*
int ListTipoIVAView::sacaWindow() {
    blDebug("ListTipoIVAView::sacaWindow", 0);
    companyact->sacaWindow(this);
    blDebug("END ListTipoIVAView::sacaWindow", 0);
    return 0;
}
*/


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListTipoIVASubForm::ListTipoIVASubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    blDebug ( "ListTipoIVASubForm::ListTipoIVASubForm", 0 );
    setDbTableName ( "tipo_iva" );
    setDbFieldId ( "idtipo_iva" );
    addSubFormHeader ( "idtipo_iva", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView, _ ( "Id Tipo IVA" ) );
    addSubFormHeader ( "desctipo_iva", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Tipo IVA" ) );
    setInsert ( TRUE );
    blDebug ( "END ListTipoIVASubForm::ListTipoIVASubForm", 0 );
}


///
/**
**/
void ListTipoIVASubForm::cargar()
{
    blDebug ( "ListTipoIVASubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM tipo_iva" );
    blDebug ( "END ListTipoIVASubForm::cargar", 0 );
}

