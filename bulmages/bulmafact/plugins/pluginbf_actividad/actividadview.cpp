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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

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
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Actividad" ) );
        setDbTableName ( "actividad" );
        setDbFieldId ( "idactividad" );
        addDbField ( "idactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID actividad" ) );
        addDbField ( "idtipoactividad", BlDbField::DbInt, BlDbField::DbNothing, _ ( "ID tipo actividad" ) );
        addDbField ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del actividad" ) );
        addDbField ( "codigoactividad", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Codigo" ) );
        addDbField ( "idprofesor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id profesor" ) );
        addDbField ( "precioactividad", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Precio" ) );
        addDbField ( "finicialactividad", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Inicial" ) );
        addDbField ( "ffinalactividad", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Final" ) );
        addDbField ( "vigenteactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Vigente" ) );
        addDbField ( "comentariosactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "lunesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Lunes" ) );
        addDbField ( "martesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Martes" ) );
        addDbField ( "miercolesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Miercoles" ) );
        addDbField ( "juevesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Jueves" ) );
        addDbField ( "viernesactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Viernes" ) );
        addDbField ( "sabadoactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Sabado" ) );
        addDbField ( "domingoactividad", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Domingo" ) );
        addDbField ( "periodoactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Periodo" ) );
        addDbField ( "hinicialactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Hora Inicio" ) );
        addDbField ( "duracionactividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Duracion" ) );
        addDbField ( "lugaractividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Lugar" ) );
        addDbField ( "diractividad", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Direccion" ) );
	
        /// Disparamos los plugins.
        int res = g_plugins->run ( "ActividadView_ActividadView", this );
        if ( res != 0 ) {
            return;
        } // end if

        insertWindow ( windowTitle(), this, false );

        /// Inicializamos el subformulario de alumnos
        mui_alumnosList->setMainCompany ( comp );
        mui_faltasAsistenciaList->setMainCompany ( comp );
	mui_tutoresList->setMainCompany( comp );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idprofesor->setMainCompany ( comp );
        mui_idprofesor->setLabel ( _ ( "Profesor:" ) );
        mui_idprofesor->setTableName ( "profesor" );
        mui_idprofesor->setFieldId("idprofesor");
        mui_idprofesor->m_valores["nombreprofesor"] = "";

        /// Establecemos los parametros de busqueda de tipo de actividad.
        mui_idtipoactividad->setMainCompany ( mainCompany() );
        mui_idtipoactividad->setQuery ( "SELECT * FROM tipoactividad ORDER BY ascii(nombretipoactividad), nombretipoactividad" );
        mui_idtipoactividad->setTableName ( "tipoactividad" );
        mui_idtipoactividad->setFieldId ( "idtipoactividad" );
        mui_idtipoactividad->m_valores["nombretipoactividad"] = "";
        mui_idtipoactividad->setAllowNull ( false );

        pintar();
        dialogChanges_readValues();

        mui_idtipoactividad->setId ( "" );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el actividad" ), this );
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
ActividadView::~ActividadView()
{
    BL_FUNC_DEBUG
    
}

QString ActividadView::templateName ( void )
{
    BL_FUNC_DEBUG
    
    
    return QString ( "actividad" );
}

void ActividadView::imprimir()
{
    BL_FUNC_DEBUG
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idprofesor" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->run ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    
}

int ActividadView::afterSave()
{
    BL_FUNC_DEBUG
    
    mui_alumnosList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_alumnosList->save();
    
    mui_faltasAsistenciaList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_faltasAsistenciaList->save();
    
    mui_tutoresList->setColumnValue ( "idactividad", dbValue ( "idactividad" ) );
    mui_tutoresList->save();
    
    
    return 0;
}

int ActividadView::beforeDelete()
{
    BL_FUNC_DEBUG
    
    QString query = "DELETE FROM alumnoactividad WHERE idactividad=" + dbValue ( "idactividad" );
    mainCompany()->runQuery ( query );
    
    query = "DELETE FROM clienteactividad WHERE idactividad=" + dbValue ( "idactividad" );
    mainCompany()->runQuery ( query );
    
    
    
    return 0;
}

int ActividadView::cargarPost ( QString id )
{
    BL_FUNC_DEBUG

    mui_alumnosList->load ( id );
    mui_faltasAsistenciaList->load ( id );
    mui_tutoresList->load ( id );
    return 0;
}

/// =============================================================================
///                    SUBFORMULARIOS
/// =============================================================================

///
/**
\param parent
**/
ListAlumnosActividadView::ListAlumnosActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    
    setDbTableName ( "alumnoactividad" );
    setDbFieldId ( "idalumnoactividad" );
    addSubFormHeader ( "idalumnoactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired | BlDbField::DbUnique , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "bancoalumnoactividad", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Banco alumno" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );

    setInsert ( true );
    setSortingEnabled ( true );
}

///
/**
\param idactividad
**/
void ListAlumnosActividadView::load ( QString idactividad )
{
    BL_FUNC_DEBUG
    
    BlSubForm::load ( "SELECT *, (apellido1alumno || ' ' || apellido2alumno || ', ' || nombrealumno) AS nombrealumno1 FROM alumnoactividad LEFT JOIN alumno ON alumnoactividad.idalumno = alumno.idalumno WHERE alumnoactividad.idactividad=" + idactividad  );
    
    
}


///
/**
\param parent
**/
ListTutoresActividadView::ListTutoresActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    
    setDbTableName ( "clienteactividad" );
    setDbFieldId ( "idclienteactividad" );
    addSubFormHeader ( "idclienteactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired  | BlDbField::DbUnique, BlSubFormHeader::DbHideView, _ ( "Id tutor" ) );
//    addSubFormHeader ( "nombretutor1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre tutor" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre" ) );
    addSubFormHeader ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "CIF" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );

    setInsert ( true );
    setSortingEnabled ( true );
    
    
}

///
/**
\param idactividad
**/
void ListTutoresActividadView::load ( QString idactividad )
{
    BL_FUNC_DEBUG
    
    BlSubForm::load ( "SELECT ( COALESCE(apellido1cliente,'') || ' ' || COALESCE(apellido2cliente,'') || ', ' || COALESCE(nomcliente,'')) AS nomcliente, * FROM clienteactividad LEFT JOIN cliente ON clienteactividad.idcliente = cliente.idcliente WHERE clienteactividad.idactividad=" + idactividad  );
    
    
}






///
/**
\param parent
**/
ListFaltasAsistenciaActividadView::ListFaltasAsistenciaActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    
    setDbTableName ( "faltaasistenciaalumnoactividad" );
    setDbFieldId ( "idfaltaasistenciaalumnoactividad" );
    addSubFormHeader ( "idfaltaasistenciaalumnoactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbHideView, _ ( "Id Actividad" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "fechafaltaasistenciaalumnoactividad", BlDbField::DbDate, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Fecha de la falta" ) );

    setInsert ( true );
    setSortingEnabled ( true );
    
    
}

///
/**
\param idactividad
**/
void ListFaltasAsistenciaActividadView::load ( QString idactividad )
{
    BL_FUNC_DEBUG
    
    BlSubForm::load ( "SELECT *, (apellido1alumno || ' ' || apellido2alumno || ', ' || nombrealumno) AS nombrealumno1 FROM faltaasistenciaalumnoactividad LEFT JOIN alumno ON faltaasistenciaalumnoactividad.idalumno = alumno.idalumno WHERE faltaasistenciaalumnoactividad.idactividad=" + idactividad  );
    
    
}
