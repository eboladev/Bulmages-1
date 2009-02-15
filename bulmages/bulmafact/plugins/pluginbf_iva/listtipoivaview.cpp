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
#include "company.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
ListTipoIVAView::ListTipoIVAView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ListTipoIVAView::ListTipoIVAView", 1 );
    setTitleName ( _( "Tipo IVA" ) );
    setDBTableName ( "tipo_iva" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setEmpresaBase ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this );
    _depura ( "END ListTipoIVAView::ListTipoIVAView", 1 );
}


///
/**
**/
void ListTipoIVAView::on_mui_aceptar_clicked()
{
    _depura ( "ListTipoIVAView::on_mui_aceptar_clicked", 0 );
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al guardar los tipos de IVA" ) );
    } // end try
    _depura ( "END ListTipoIVAView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
ListTipoIVAView::~ListTipoIVAView()
{
    _depura ( "ListTipoIVAView::~ListTipoIVAView", 0 );
    _depura ( "END ListTipoIVAView::~ListTipoIVAView", 0 );
}

/*
int ListTipoIVAView::sacaWindow() {
    _depura("ListTipoIVAView::sacaWindow", 0);
    companyact->sacaWindow(this);
    _depura("END ListTipoIVAView::sacaWindow", 0);
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
    _depura ( "ListTipoIVASubForm::ListTipoIVASubForm", 0 );
    setDBTableName ( "tipo_iva" );
    setDBCampoId ( "idtipo_iva" );
    addSHeader ( "idtipo_iva", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Id Tipo IVA" ) );
    addSHeader ( "desctipo_iva", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Tipo IVA" ) );
    setinsercion ( TRUE );
    _depura ( "END ListTipoIVASubForm::ListTipoIVASubForm", 0 );
}


///
/**
**/
void ListTipoIVASubForm::cargar()
{
    _depura ( "ListTipoIVASubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM tipo_iva" );
    _depura ( "END ListTipoIVASubForm::cargar", 0 );
}

