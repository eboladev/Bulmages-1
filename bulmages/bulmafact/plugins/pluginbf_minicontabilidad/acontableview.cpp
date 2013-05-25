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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

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
    BL_FUNC_DEBUG

//    setAttribute ( Qt::WA_DeleteOnClose );

    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Diario Contable" ) );
        setDbTableName ( "acontable" );
        setDbFieldId ( "idacontable" );
	
        insertWindow ( windowTitle(), this, false );

        /// Activamos el listado de lineas de recibo
        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "acontable" );
        mui_list->setDbFieldId ( "idacontable" );
        mui_list->addSubFormHeader ( "idacontable", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
        mui_list->addSubFormHeader ( "idpartida", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id. Partida" ) );
        mui_list->addSubFormHeader ( "fechaacontable", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Fecha" ) );
        mui_list->addSubFormHeader ( "codigocompletopartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Codigo Partida" ) );
        mui_list->addSubFormHeader ( "nombrepartida", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre Partida" ) );
        mui_list->addSubFormHeader ( "conceptoacontable", BlDbField::DbVarChar, BlDbField::DbNotNull , BlSubFormHeader::DbNone, _ ( "Concepto" ) );
        mui_list->addSubFormHeader ( "debeacontable", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Ingreso" ) );
        mui_list->addSubFormHeader ( "haberacontable", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Gasto" ) );

        mui_list->setInsert ( true );
        mui_list->setOrdenEnabled ( true );

        // ======================================================

        pintar();
	load("0");
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el recibo" ), this );
    } // end try

    
}

/** No precisa acciones adicionales en el destructor.
*/
ApunteContableView::~ApunteContableView()
{
    BL_FUNC_DEBUG
    
}

QString ApunteContableView::templateName ( void )
{
    BL_FUNC_DEBUG
    
    
    return QString ( "acontable" );
}

void ApunteContableView::imprimir()
{
    BL_FUNC_DEBUG

    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";


    /// Disparamos los plugins
    int res = g_plugins->run ( "ApunteContableView_on_mui_imprimir_clicked", this );

    if ( res != 0 ) {
        return;
    } // end if

    BfForm::imprimir();

    
}

int ApunteContableView::save()
{
    BL_FUNC_DEBUG

    mui_list->save();

    
    return 0;
}


int ApunteContableView::load ( QString id )
{
    BL_FUNC_DEBUG

    mui_list->load ("SELECT * FROM acontable NATURAL LEFT JOIN partida" );
    
    
    
    return 0;
}


