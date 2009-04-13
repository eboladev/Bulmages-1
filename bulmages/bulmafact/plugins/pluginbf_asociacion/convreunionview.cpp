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

#include "convreunionview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ConvReunionView::ConvReunionView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "ConvReunionView::ConvReunionView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "ConvReunion" ) );
        setDbTableName ( "reunion" );
        setDbFieldId ( "idreunion" );
        addDbField ( "idreunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID reunion" ) );
        addDbField ( "fechainreunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha Constitucion" ) );
        addDbField ( "fechafinreunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha Cese" ) );

        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "miembroreunion" );
        mui_list->setDbFieldId ( "idmiembroreunion" );
        mui_list->addSubFormHeader ( "idmiembroreunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Miembro" ) );
        mui_list->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
        mui_list->addSubFormHeader ( "fechainmiembroreunion", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "Fecha posesion" ) );
        mui_list->addSubFormHeader ( "fechafinmiembroreunion", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Fecha Cese" ) );
        mui_list->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Socio" ) );
        mui_list->addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView, _ ( "Socio" ) );
        mui_list->addSubFormHeader ( "cargomiembroreunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Cargo" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setDelete ( TRUE );
        mui_list->setSortingEnabled ( FALSE );
        
        mui_list->cargar("SELECT * from miembroreunion NATURAL LEFT JOIN cliente WHERE 1 = 2");
        
        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear el reunion" ), this );
    } // end try
    _depura ( "END ConvReunionView::ConvReunionView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
ConvReunionView::~ConvReunionView()
{
    _depura ( "ConvReunionView::~ConvReunionView", 0 );
    _depura ( "END ConvReunionView::~ConvReunionView", 0 );
}


QString ConvReunionView::nombrePlantilla ( void )
{
    return QString ( "reunion" );
}

void ConvReunionView::imprimir()
{
    _depura ( "ConvReunionView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el reunion.
    QString SQLQuery = "";

    if ( dbValue ( "idreunion" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "ConvReunionView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    BfForm::imprimir();

    _depura ( "END ConvReunionView::imprimir", 0 );
}


int ConvReunionView::guardarPost()
{
    _depura ( " ConvReunionView::guardarPost", 0 );
    mui_list->setColumnValue("idreunion", dbValue("idreunion") );
    mui_list->guardar();
    _depura ( "END ConvReunionView::guardarPost", 0 );
}


int ConvReunionView::borrarPre()
{

    return 0;
}



int ConvReunionView::cargarPost ( QString id )
{
    _depura ( " ConvReunionView::cargarPost", 0 );
    mui_list->cargar("SELECT * FROM miembroreunion NATURAL LEFT JOIN cliente WHERE idreunion = " + id);
    _depura ( "END ConvReunionView::cargarPost", 0 );
    return 0;
}



