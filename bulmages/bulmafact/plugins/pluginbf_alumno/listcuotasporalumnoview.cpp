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

#include "listcuotasporalumnoview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
ListCuotasPorAlumnoView::ListCuotasPorAlumnoView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "ListCuotasPorAlumnoView::ListCuotasPorAlumnoView", 1 );
    setTitleName ( _ ( "CuotasPorAlumno" ) );
    setDbTableName ( "cuotaporalumno" );
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    mui_listado->setMainCompany ( comp );
    mui_listado->cargar();
    meteWindow ( windowTitle(), this, FALSE );
    _depura ( "END ListCuotasPorAlumnoView::ListCuotasPorAlumnoView", 1 );
}


///
/**
**/
void ListCuotasPorAlumnoView::on_mui_aceptar_clicked()
{
    _depura ( "ListCuotasPorAlumnoView::on_mui_aceptar_clicked", 0 );
    try {
        mui_listado->guardar();
        close();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al guardar las cuotas" ) );
    } // end try
    _depura ( "END ListCuotasPorAlumnoView::on_mui_aceptar_clicked", 0 );
}


///
/**
**/
ListCuotasPorAlumnoView::~ListCuotasPorAlumnoView()
{
    _depura ( "ListCuotasPorAlumnoView::~ListCuotasPorAlumnoView", 0 );
    _depura ( "END ListCuotasPorAlumnoView::~ListCuotasPorAlumnoView", 0 );
}


/// ===================================== SUBFORMULARIO ===============================================
///
/**
\param parent
**/
ListCuotasPorAlumnoSubForm::ListCuotasPorAlumnoSubForm ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListCuotasPorAlumnoSubForm::ListCuotasPorAlumnoSubForm", 0 );
    setDbTableName ( "cuotaporalumno" );
    setDbFieldId ( "numalumnoscuotaporalumno" );
    addSubFormHeader ( "numalumnoscuotaporalumno", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNone, _ ( "Codigo almacen" ) );
    addSubFormHeader ( "cuotacuotaporalumno", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cuota" ) );
    setInsert ( TRUE );
    _depura ( "END ListCuotasPorAlumnoSubForm::ListCuotasPorAlumnoSubForm", 0 );
}


///
/**
**/
void ListCuotasPorAlumnoSubForm::cargar()
{
    _depura ( "ListCuotasPorAlumnoSubForm::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM cuotaporalumno" );
    _depura ( "END ListCuotasPorAlumnoSubForm::cargar", 0 );
}

