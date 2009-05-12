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

#include "actividadview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ActividadView::ActividadView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "ActividadView::ActividadView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "Actividad" ) );
        setDbTableName ( "actividad" );
        setDbFieldId ( "idactividad" );
        addDbField ( "idactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID actividad" ) );
        addDbField ( "idtipoactividad", BlDbField::DbInt, BlDbField::DbNothing, _ ( "ID tipo actividad" ) );
        addDbField ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del actividad" ) );
        addDbField ( "codigoactividad", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Codigo" ) );
        addDbField ( "idprofesor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id profesor" ) );
        addDbField ( "precioactividad", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Precio Actividad" ) );
        addDbField ( "finicialactividad", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Inicial" ) );
        addDbField ( "ffinalactividad", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Final" ) );

        meteWindow ( windowTitle(), this, FALSE );

        /// Inicializamos el subformulario de alumnos
        mui_alumnosList->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idprofesor->setMainCompany ( comp );
        mui_idprofesor->setLabel ( _ ( "Profesor:" ) );
        mui_idprofesor->setTableName ( "profesor" );
        mui_idprofesor->m_valores["nombreprofesor"] = "";

        /// Establecemos los parametros de busqueda de tipo de actividad.
        mui_idtipoactividad->setMainCompany ( mainCompany() );
        mui_idtipoactividad->setQuery ( "SELECT * FROM tipoactividad ORDER BY ascii(nombretipoactividad), nombretipoactividad" );
        mui_idtipoactividad->setTableName ( "tipoactividad" );
        mui_idtipoactividad->setFieldId ( "idtipoactividad" );
        mui_idtipoactividad->m_valores["nombretipoactividad"] = "";
        mui_idtipoactividad->setAllowNull ( FALSE );
        mui_idtipoactividad->setId ( "" );

        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear el actividad" ), this );
    } // end try
    
    _depura ( "END ActividadView::ActividadView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
ActividadView::~ActividadView()
{
    _depura ( "ActividadView::~ActividadView", 0 );
    _depura ( "END ActividadView::~ActividadView", 0 );
}

QString ActividadView::nombrePlantilla ( void )
{
    _depura ( "ActividadView::nombrePlantilla", 0 );
    _depura ( "END ActividadView::nombrePlantilla", 0 );
    
    return QString ( "actividad" );
}

void ActividadView::imprimir()
{
    _depura ( "ActividadView::imprimir", 0 );
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idprofesor" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    _depura ( "END ActividadView::imprimir", 0 );
}

int ActividadView::guardarPost()
{
    _depura ( "ActividadView::guardarPost", 0 );
    
    mui_alumnosList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_alumnosList->guardar();
    
    _depura ( "END ActividadView::guardarPost", 0 );
}

int ActividadView::borrarPre()
{
    _depura ( "ActividadView::borrarPre", 0 );
    
    QString query = "DELETE FROM alumnoactividad WHERE idactividad=" + dbValue ( "idactividad" );
    mainCompany()->runQuery ( query );
    
    _depura ( "END ActividadView::borrarPre", 0 );
    
    return 0;
}

int ActividadView::cargarPost ( QString id )
{
    _depura ( "ActividadView::cargarPost", 0 );

    mui_alumnosList->cargar ( id );

    _depura ( "END ActividadView::cargarPost", 0 );
    
    return 0;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

///
/**
\param parent
**/
ListAlumnosActividadView::ListAlumnosActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlumnosActividadView::ListAlumnosActividadView", 0 );
    
    setDbTableName ( "alumnoactividad" );
    setDbFieldId ( "idalumnoactividad" );
    addSubFormHeader ( "idalumnoactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );

    setInsert ( FALSE );
    setOrdenEnabled ( FALSE );
    
    _depura ( "END ListAlumnosActividadView::ListAlumnosActividadView", 0 );
}

///
/**
\param idcontrato
**/
void ListAlumnosActividadView::cargar ( QString idactividad )
{
    _depura ( "ListAlumnosActividadView::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT * FROM alumnoactividad LEFT JOIN alumno ON alumnoactividad.idalumno = alumno.idalumno  WHERE alumnoactividad.idactividad=" + idactividad  );
    
    _depura ( "END ListAlumnosActividadView::cargar", 0 );
}
