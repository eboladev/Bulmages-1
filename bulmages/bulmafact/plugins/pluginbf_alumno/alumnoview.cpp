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

#include "alumnoview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
AlumnoView::AlumnoView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "AlumnoView::AlumnoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _( "Alumno" ) );
        setDbTableName ( "alumno" );
        setDbFieldId ( "idalumno" );
        addDbField ( "idalumno", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "Id alumno" ) );
        addDbField ( "nombrealumno", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Nombre del alumno" ) );
        addDbField ( "apellido1alumno", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Nombre del alumno" ) );
        addDbField ( "fechanacimientoalumno", BlDbField::DbDate, BlDbField::DbNothing, _( "Nombre del alumno" ) );

        /// Datos por defecto.
        mui_tutoresList->setMainCompany ( mainCompany() );
        mui_actividadesList->setMainCompany ( mainCompany() );
        
        mui_idprovincia->setMainCompany ( mainCompany() );
    mui_idprovincia->setQuery("SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia");
    mui_idprovincia->setTableName ("provincia");
    mui_idprovincia->setFieldId ("idprovincia");
    mui_idprovincia->m_valores["provincia"] = "";
    mui_idprovincia->m_valores["descpais"] = "";
    mui_idprovincia->setAllowNull(TRUE);
        mui_idprovincia->setId ( "" );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el alumno" ), this );
    } // end try
    _depura ( "END AlumnoView::AlumnoView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
AlumnoView::~AlumnoView()
{
    _depura ( "AlumnoView::~AlumnoView", 0 );
    _depura ( "END AlumnoView::~AlumnoView", 0 );
}


QString AlumnoView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void AlumnoView::imprimir()
{
    _depura ( "AlumnoView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    } 
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "CoboView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    BfForm::imprimir();

    _depura ( "END AlumnoView::imprimir", 0 );
}


int AlumnoView::guardarPost() {
	_depura(" AlumnoView::guardarPost", 0);
 mui_tutoresList->setColumnValue("idalumno", dbValue("idalumno"));
  mui_tutoresList->guardar();

 mui_actividadesList->setColumnValue("idalumno", dbValue("idalumno"));
  mui_actividadesList->guardar();
   
	_depura("END AlumnoView::guardarPost", 0);

}



int AlumnoView::borrarPre() {
    QString query = "DELETE FROM alumnocliente WHERE idalumno =" + dbValue("idalumno");
    mainCompany()->runQuery(query);

    QString query1 = "DELETE FROM alumnoactividad WHERE idalumno =" + dbValue("idalumno");
    mainCompany()->runQuery(query1);

    return 0;
}



int AlumnoView::cargarPost(QString id) {
   _depura(" AlumnoView::cargarPost", 0);

    mui_tutoresList->cargar(id);
    mui_actividadesList->cargar(id);

   _depura("END AlumnoView::cargarPost", 0);
  return 0;
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

///
/**
\param parent
**/
ListAlumnosTutorView::ListAlumnosTutorView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlumnosTutorView::ListAlumnosTutorView", 0 );
    setDbTableName ( "alumnocliente" );
    setDbFieldId ( "idalumnocliente" );
    addSubFormHeader ( "idalumnocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _( "Id alumno" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Nombre alumno" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView, _( "Id tutor" ) );

    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    _depura ( "END ListAlumnosTutorView::ListAlumnosTutorView", 0 );
}


///
/**
\param idcontrato
**/
void ListAlumnosTutorView::cargar ( QString idalumno )
{
    _depura ( "ListAlumnosTutorView::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM alumnocliente LEFT JOIN cliente ON alumnocliente.idcliente = cliente.idcliente  WHERE alumnocliente.idalumno =" + idalumno  );
    _depura ( "END ListAlumnosTutorView::cargar", 0 );
}



///
/**
\param parent
**/
ListAlumnosActividadView::ListAlumnosActividadView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlumnosActividadView::ListAlumnosActividadView", 0 );
    setDbTableName ( "alumnoactividad" );
    setDbFieldId ( "idalumnoactividad" );
    addSubFormHeader ( "idalumnoactividad", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _( "Id alumno" ) );
    addSubFormHeader ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Nombre actividad" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView, _( "Id actividad" ) );

    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    _depura ( "END ListAlumnosActividadView::ListAlumnosActividadView", 0 );
}


///
/**
\param idcontrato
**/
void ListAlumnosActividadView::cargar ( QString idalumno )
{
    _depura ( "ListAlumnosActividadView::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM alumnoactividad LEFT JOIN actividad ON alumnoactividad.idactividad = actividad.idactividad  WHERE alumnoactividad.idalumno =" + idalumno  );
    _depura ( "END ListAlumnosActividadView::cargar", 0 );
}

