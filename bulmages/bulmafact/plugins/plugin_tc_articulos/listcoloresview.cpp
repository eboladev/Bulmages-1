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

#include "listcoloresview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/

ListColoresView::ListColoresView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ListColoresView::ListColoresView", 1 );
    setTitleName ( _( "Colores" ) );
    setDBTableName ( "tc_color" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END ListColoresView::ListColoresView", 1 );
}


///
/**
**/

void ListColoresView::on_mui_aceptar_clicked()
{
    _depura ( "ListColoresView::on_mui_aceptar_clicked", 0 );
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al guardar los colores" ) );
    } // end try
    _depura ( "END ListColoresView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/

ListColoresView::~ListColoresView()
{
    _depura ( "ListColoresView::~ListColoresView", 0 );
    _depura ( "END ListColoresView::~ListColoresView", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/

ListColoresSubForm::ListColoresSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListColoresSubForm::ListColoresSubForm", 0 );
    setDBTableName ( "tc_color" );
    setDBCampoId ( "idtc_color" );
    addSHeader ( "idtc_color", BlDbField::DBint, BlDbField::DBPrimaryKey, BlSubFormHeader::DBNoView, _( "Id Color" ) );
    addSHeader ( "rgbtc_color", BlDbField::DBvarchar, BlDbField::DBNothing, BlSubFormHeader::DBNone, _( "RGB Color" ) );
    addSHeader ( "nomtc_color", BlDbField::DBvarchar, BlDbField::DBNotNull, BlSubFormHeader::DBNone, _( "Nombre Color" ) );
    setinsercion ( TRUE );
    _depura ( "END ListColoresSubForm::ListColoresSubForm", 0 );
}


///
/**
**/

void ListColoresSubForm::cargar()
{
    _depura ( "ListColoresSubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM tc_color" );
    _depura ( "END ListColoresSubForm::cargar", 0 );
}

