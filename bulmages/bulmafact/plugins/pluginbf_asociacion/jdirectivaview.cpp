/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include "jdirectivaview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
JDirectivaView::JDirectivaView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "JDirectivaView::JDirectivaView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "JDirectiva" ) );
        setDbTableName ( "jdirectiva" );
        setDbFieldId ( "idjdirectiva" );
        addDbField ( "idjdirectiva", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID jdirectiva" ) );
        addDbField ( "fechainjdirectiva", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha Constitucion" ) );
        addDbField ( "fechafinjdirectiva", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Cese" ) );

        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "miembrojdirectiva" );
        mui_list->setDbFieldId ( "idmiembrojdirectiva" );
        mui_list->addSubFormHeader ( "idmiembrojdirectiva", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Miembro" ) );
        mui_list->addSubFormHeader ( "idjdirectiva", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Junta Directiva" ) );
        mui_list->addSubFormHeader ( "fechainmiembrojdirectiva", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone , _ ( "Fecha posesion" ) );
        mui_list->addSubFormHeader ( "fechafinmiembrojdirectiva", BlDbField::DbDate, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Fecha Cese" ) );
        mui_list->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Socio" ) );
        mui_list->addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Socio" ) );
        mui_list->addSubFormHeader ( "cargomiembrojdirectiva", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Cargo" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setDelete ( TRUE );
        mui_list->setSortingEnabled ( FALSE );
        
        mui_list->cargar("SELECT * from miembrojdirectiva NATURAL LEFT JOIN cliente WHERE 1 = 2");
        
        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear el jdirectiva" ), this );
    } // end try
    
    _depura ( "END JDirectivaView::JDirectivaView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
JDirectivaView::~JDirectivaView()
{
    _depura ( "JDirectivaView::~JDirectivaView", 0 );
    _depura ( "END JDirectivaView::~JDirectivaView", 0 );
}


QString JDirectivaView::nombrePlantilla ( void )
{
    _depura ( "JDirectivaView::~nombrePlantilla", 0 );
    _depura ( "END JDirectivaView::~nombrePlantilla", 0 );
    
    return QString ( "jdirectiva" );
}


void JDirectivaView::imprimir()
{
    _depura ( "JDirectivaView::imprimir", 0 );
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el jdirectiva.
    QString SQLQuery = "";

    if ( dbValue ( "idjdirectiva" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "JDirectivaView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    _depura ( "END JDirectivaView::imprimir", 0 );
}

int JDirectivaView::guardarPost()
{
    _depura ( "JDirectivaView::guardarPost", 0 );
    
    mui_list->setColumnValue("idjdirectiva", dbValue("idjdirectiva") );
    mui_list->guardar();
    
    _depura ( "END JDirectivaView::guardarPost", 0 );
}

int JDirectivaView::borrarPre()
{
    return 0;
}

int JDirectivaView::cargarPost ( QString id )
{
    _depura ( "JDirectivaView::cargarPost", 0 );
    
    mui_list->cargar("SELECT * FROM miembrojdirectiva NATURAL LEFT JOIN cliente WHERE idjdirectiva = " + id);
    
    _depura ( "END JDirectivaView::cargarPost", 0 );
    
    return 0;
}
