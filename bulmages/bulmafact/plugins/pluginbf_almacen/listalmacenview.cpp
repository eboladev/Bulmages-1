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

#include "listalmacenview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
ListAlmacenView::ListAlmacenView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ListAlmacenView::ListAlmacenView", 1 );
    setTitleName ( _( "Almacen" ) );
    setDbTableName ( "almacen" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END ListAlmacenView::ListAlmacenView", 1 );
}


///
/**
**/
void ListAlmacenView::on_mui_aceptar_clicked()
{
    _depura ( "ListAlmacenView::on_mui_aceptar_clicked", 0 );
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al guardar los almacenes" ) );
    } // end try
    _depura ( "END ListAlmacenView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
ListAlmacenView::~ListAlmacenView()
{
    _depura ( "ListAlmacenView::~ListAlmacenView", 0 );
    _depura ( "END ListAlmacenView::~ListAlmacenView", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListAlmacenSubForm::ListAlmacenSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlmacenSubForm::ListAlmacenSubForm", 0 );
    setDbTableName ( "almacen" );
    setDbFieldId ( "idalmacen" );
    addSubFormHeader ( "idalmacen", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Id almacen" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Codigo almacen" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Nombre almacen" ) );
    setinsercion ( TRUE );
    _depura ( "END ListAlmacenSubForm::ListAlmacenSubForm", 0 );
}


///
/**
**/
void ListAlmacenSubForm::cargar()
{
    _depura ( "ListAlmacenSubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM almacen" );
    _depura ( "END ListAlmacenSubForm::cargar", 0 );
}

