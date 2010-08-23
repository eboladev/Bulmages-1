/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <fstream>

#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>

#include "acontableview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ApunteContableView::ApunteContableView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    blDebug ( "ApunteContableView::ApunteContableView", 0 );

    setAttribute ( Qt::WA_DeleteOnClose );

    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "Diario Contable" ) );
        setDbTableName ( "acontable" );
        setDbFieldId ( "idacontable" );
	
        meteWindow ( windowTitle(), this, FALSE );

        /// Activamos el listado de lineas de recibo
        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "acontable" );
        mui_list->setDbFieldId ( "idacontable" );
        mui_list->addSubFormHeader ( "idacontable", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
        mui_list->addSubFormHeader ( "idpartida", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id. Partida" ) );
        mui_list->addSubFormHeader ( "fechaacontable", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Fecha" ) );
        mui_list->addSubFormHeader ( "codigocompletopartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo Partida" ) );
        mui_list->addSubFormHeader ( "nombrepartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre Partida" ) );
        mui_list->addSubFormHeader ( "conceptoacontable", BlDbField::DbVarChar, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _ ( "Concepto" ) );
        mui_list->addSubFormHeader ( "debeacontable", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Ingreso" ) );
        mui_list->addSubFormHeader ( "haberacontable", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Gasto" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setOrdenEnabled ( TRUE );

        // ======================================================

        pintar();
	cargar("0");
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el recibo" ), this );
    } // end try

    blDebug ( "END ApunteContableView::ApunteContableView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
ApunteContableView::~ApunteContableView()
{
    blDebug ( "ApunteContableView::~ApunteContableView", 0 );
    blDebug ( "END ApunteContableView::~ApunteContableView", 0 );
}

QString ApunteContableView::nombrePlantilla ( void )
{
    blDebug ( "ApunteContableView::nombrePlantilla", 0 );
    blDebug ( "END ApunteContableView::nombrePlantilla", 0 );
    
    return QString ( "acontable" );
}

void ApunteContableView::imprimir()
{
    blDebug ( "ApunteContableView::imprimir", 0 );

    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";


    /// Disparamos los plugins
    int res = g_plugins->lanza ( "ApunteContableView_on_mui_imprimir_clicked", this );

    if ( res != 0 ) {
        return;
    } // end if

    BfForm::imprimir();

    blDebug ( "END ApunteContableView::imprimir", 0 );
}

int ApunteContableView::guardar()
{
    blDebug ( "ApunteContableView::guardarPost", 0 );

    mui_list->guardar();

    blDebug ( "END ApunteContableView::guardarPost", 0 );
    return 0;
}


int ApunteContableView::cargar ( QString id )
{
    blDebug ( "ApunteContableView::cargarPost", 0 );

    mui_list->cargar ("SELECT * FROM acontable NATURAL LEFT JOIN partida" );
    
    blDebug ( "END ApunteContableView::cargarPost", 0 );
    
    return 0;
}


