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
        addDbField ( "fecha1convocatoriareunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha 1ª Convocatoria" ) );
        addDbField ( "fecha2convocatoriareunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha 2ª Convocatoria" ) );
        addDbField ( "tiporeunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Tipo" ) );
        addDbField ( "conceptoreunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Concepto" ) );
        addDbField ( "resolucionreunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Resolucion" ) );

        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "asistentereunion" );
        mui_list->setDbFieldId ( "idasistentereunion" );
        mui_list->addSubFormHeader ( "idasistentereunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Miembro" ) );
        mui_list->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
        mui_list->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Socio" ) );
        mui_list->addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbHideView, _ ( "Socio" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setDelete ( TRUE );
        mui_list->setSortingEnabled ( FALSE );
        
        mui_list->cargar("SELECT * from asistentereunion NATURAL LEFT JOIN cliente WHERE 1 = 2");


        mui_ordendia->setMainCompany( comp );
        mui_ordendia->setDbTableName ( "ordendiareunion" );
        mui_ordendia->setDbFieldId ( "idordendiareunion" );
        mui_ordendia->addSubFormHeader ( "idordendiareunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Miembro" ) );
        mui_ordendia->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
        mui_ordendia->addSubFormHeader ( "conceptoordendiareunion", BlDbField::DbVarChar, BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Concepto" ) );
        mui_ordendia->addSubFormHeader ( "textoordendiareunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Texto" ) );
        mui_ordendia->addSubFormHeader ( "resolucionordendiareunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Resolucion" ) );

        mui_ordendia->setInsert ( TRUE );
        mui_ordendia->setDelete ( TRUE );
        mui_ordendia->setSortingEnabled ( FALSE );
        
        mui_ordendia->cargar("SELECT * from ordendiareunion  WHERE 1 = 2");
        


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
    
    mui_ordendia->setColumnValue("idreunion", dbValue("idreunion") );
    mui_ordendia->guardar();
    
    _depura ( "END ConvReunionView::guardarPost", 0 );
}


int ConvReunionView::borrarPre()
{

    return 0;
}



int ConvReunionView::cargarPost ( QString id )
{
    _depura ( " ConvReunionView::cargarPost", 0 );
    mui_list->cargar("SELECT * FROM asistentereunion NATURAL LEFT JOIN cliente WHERE idreunion = " + id);
    mui_ordendia->cargar("SELECT * FROM ordendiareunion WHERE idreunion = " + id);
    _depura ( "END ConvReunionView::cargarPost", 0 );
    return 0;
}



