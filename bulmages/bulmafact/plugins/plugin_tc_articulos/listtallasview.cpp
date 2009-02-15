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

#include "listtallasview.h"
#include "company.h"
#include "blconfiguration.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/

ListTallasView::ListTallasView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ListTallasView::ListTallasView", 1 );
    setTitleName ( _( "Tallas" ) );
    setDBTableName ( "tc_talla" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setEmpresaBase ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END ListTallasView::ListTallasView", 1 );
}


///
/**
**/

void ListTallasView::on_mui_aceptar_clicked()
{
    _depura ( "ListTallasView::on_mui_aceptar_clicked", 0 );
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al guardar los colores" ), this );
    } // end try
    _depura ( "END ListTallasView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/

ListTallasView::~ListTallasView()
{
    _depura ( "ListTallasView::~ListTallasView", 0 );
    _depura ( "END ListTallasView::~ListTallasView", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/

ListTallasSubForm::ListTallasSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListTallasSubForm::ListTallasSubForm", 0 );
    setDBTableName ( "tc_talla" );
    setDBCampoId ( "idtc_talla" );
    addSHeader ( "idtc_talla", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Id color" ) );
    addSHeader ( "nomtc_talla", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Nombre talla" ) );
    setinsercion ( TRUE );
    _depura ( "END ListTallasSubForm::ListTallasSubForm", 0 );
}


///
/**
**/

void ListTallasSubForm::cargar()
{
    _depura ( "ListTallasSubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM tc_talla" );
    _depura ( "END ListTallasSubForm::cargar", 0 );
}

