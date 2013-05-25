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
#include <QtWidgets/QFileDialog>

#include "alumnoview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
AlumnoView::AlumnoView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Alumno" ) );
        setDbTableName ( "alumno" );
        setDbFieldId ( "idalumno" );
        addDbField ( "idalumno", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id alumno" ) );
        addDbField ( "dnialumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del alumno" ) );
        addDbField ( "nombrealumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del alumno" ) );
        addDbField ( "apellido1alumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "1er apellido" ) );
        addDbField ( "apellido2alumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "2o apellido" ) );
        addDbField ( "telalumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono" ) );
        addDbField ( "movilalumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Movil" ) );
        addDbField ( "observalumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Observaciones" ) );
        addDbField ( "emailalumno", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "E-Mail" ) );
        addDbField ( "fechanacimientoalumno", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha Nacimiento" ) );
        addDbField ( "activoalumno", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Activo" ) );
        addDbField ( "idclase", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Clase" ) );        
        m_archivoimagen = "";
        mui_imagen->setPixmap ( QPixmap ( g_confpr->value( CONF_PROGDATA ) + "images/logopeq.png" ) );

        /// Datos por defecto.
        mui_tutoresList->setMainCompany ( mainCompany() );
        mui_actividadesList->setMainCompany ( mainCompany() );

	
	/// Presentamos una carga ficticia para que se acabe de iniciar el subformulario.
        mui_tutoresList->load ( "0" );

	

        mui_idclase->setMainCompany ( mainCompany() );
        mui_idclase->setQuery ( "SELECT * FROM clase ORDER BY nomclase" );
        mui_idclase->setTableName ( "clase" );
        mui_idclase->setFieldId ( "idclase" );
        mui_idclase->m_valores["nomclase"] = "";
        mui_idclase->setAllowNull ( true );
        mui_idclase->setId ( "" );

        insertWindow ( windowTitle(), this, false );
        pintar();
        dialogChanges_readValues();
        blScript(this);
    } catch ( ... ) {
    
        blMsgInfo ( _ ( "Error al crear el alumno" ), this );
        
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
AlumnoView::~AlumnoView()
{
    BL_FUNC_DEBUG
    
}

QString AlumnoView::templateName ( void )
{
    BL_FUNC_DEBUG
    

    return QString ( "alumno" );
}

void AlumnoView::imprimir()
{
    BL_FUNC_DEBUG
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idalumno" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->run ( "AlumnoView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    
}

/** Se encarga de presentar la ventana con los datos cargados en la clase BlDbRecord.
    Tambien cambia el titulo de la ventana para que contenga la informacion correcta.
*/
/**
**/
void AlumnoView::pintarPost()
{
    BL_FUNC_DEBUG

    /// Comprueba que exista la imagen del alumno y sino carga la imagen por defecto para indicar
    /// que el alumno no tiene imagen asociada.

    QString archivoimagen;
    archivoimagen = g_confpr->value( CONF_DIR_IMG_ALUMNOS ) + dbValue ( "idalumno" ) + ".jpg";

    QFile archivo;
    archivo.setFileName ( archivoimagen );

    if ( archivo.exists() ) {
        /// Muestra la imagen si existe el archivo.
        mui_imagen->setPixmap ( QPixmap ( archivoimagen ) );
    } else  {
        /// Muestra la imagen por defecto.
        mui_imagen->setPixmap ( QPixmap ( g_confpr->value( CONF_PROGDATA ) + "images/logopeq.png" ) );
    } // end if

    
}

int AlumnoView::afterSave()
{
    BL_FUNC_DEBUG
    
    mui_tutoresList->setColumnValue ( "idalumno", dbValue ( "idalumno" ) );
    mui_tutoresList->save();

    mui_actividadesList->setColumnValue ( "idalumno", dbValue ( "idalumno" ) );
    mui_actividadesList->save();
    
    /// Guardamos la imagen, si es que existe.
    if ( !m_archivoimagen.isEmpty() ) {
        
        m_archivoimagen = m_archivoimagen.replace ( " ", "\\ " );

        /// Coge la imagen del recuadro y la guarda en un archivo con el nombre correcto.
        if ( mui_imagen->pixmap()->save ( g_confpr->value( CONF_DIR_IMG_ALUMNOS ) + dbValue ( "idalumno" ) + ".jpg" ) == false ) {
            blMsgError ( _ ( "No se ha podido guardar la imagen.\nRevise los permisos de escritura y que disponga\nde espacio libre suficiente en el disco duro." ) );
        } // end if

        
    } // end if

    
    return 0;
}

int AlumnoView::beforeDelete()
{
    BL_FUNC_DEBUG
    
    /// Borramos la imagen
    QString archivoimagen;
    archivoimagen = g_confpr->value( CONF_DIR_IMG_ALUMNOS ) + dbValue ( "idalumno" ) + ".jpg";

    QFile archivo;
    archivo.setFileName ( archivoimagen );

    if ( archivo.exists() ) {
    
        if ( archivo.remove() == false ) {
            blMsgError ( _ ( "No se ha podido borrar la imagen del alumno.\nCompruebe que el archivo tenga los permisos correctos." ) );
        } // end if
            
    } // end if

    QString query = "DELETE FROM alumnocliente WHERE idalumno =" + dbValue ( "idalumno" );
    mainCompany()->runQuery ( query );

    query = "DELETE FROM alumnoactividad WHERE idalumno =" + dbValue ( "idalumno" );
    mainCompany()->runQuery ( query );
    
    query = "DELETE FROM faltaasistenciaalumnoactividad WHERE idalumno =" + dbValue ( "idalumno" );
    mainCompany()->runQuery ( query );
    
    

    return 0;
}

int AlumnoView::cargarPost ( QString id )
{
    BL_FUNC_DEBUG

    mui_tutoresList->load ( id );
    mui_actividadesList->load ( id );

    
    
    return 0;
}

/** SLOT que responde a la pulsacion del boton de cambio de imagen.
    Abre la imagen y la almacenta al mismo tiempo que la presenta.
    Es el metodo de guardado quien determina como almacenarla.
*/
/**
**/
void AlumnoView::on_mui_cambiarimagen_clicked()
{
    BL_FUNC_DEBUG
    
    QPixmap imagen;

    m_archivoimagen = QFileDialog::getOpenFileName (
        this,
        _ ( "Seleccione un archivo de imagen" ),
        "",
        _ ( "Imagenes (*.jpg)" ) 
    );

    /// Comprueba si se ha seleccionado un archivo.
    if ( !m_archivoimagen.isNull() ) {
        /// Comprueba que la imagen del archivo es valida.
        if ( imagen.load ( m_archivoimagen ) == false ) {
            blMsgError ( _ ( "No se ha podido cargar la imagen.\nCompruebe que la imagen sea valida." ) );
            return;
        } // end if

        /// Muestra la imagen en el recuadro de la imagen.
        mui_imagen->setPixmap ( imagen );
    } // end if

    
}

/** Elimina la imagen del alumno asociado, si existe.
**/
void AlumnoView::on_mui_borrarimagen_clicked()
{
    BL_FUNC_DEBUG

    QString archivoimagen;
    archivoimagen = g_confpr->value( CONF_DIR_IMG_ALUMNOS ) + dbValue ( "idalumno" ) + ".jpg";

    QFile archivo;
    archivo.setFileName ( archivoimagen );

    if ( archivo.exists() ) {
    
        int val = QMessageBox::question ( this,
            _ ( "Borrar imagen del alumno" ),
            _ ( "Esta seguro que quiere borrar\nla imagen asociada a este alumno?" ),
            QMessageBox::Yes,
            QMessageBox::Cancel | QMessageBox::Escape | QMessageBox::Default 
        );

        if ( val == QMessageBox::Yes ) {
            /// Se borra el archivo de la imagen y se muestra la imagen por defecto en el QLabel.
            if ( archivo.remove() == false ) {
                blMsgError ( _ ( "No se ha podido borrar el archivo.\nCompruebe que el archivo tenga los permisos correctos." ) );
            } // end if
        } // end if
    } // end if

    pintarPost();
    
    
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
    BL_FUNC_DEBUG
    
    setDbTableName ( "alumnocliente" );
    setDbFieldId ( "idalumnocliente" );
    addSubFormHeader ( "idalumnocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre tutor" ) );
    addSubFormHeader ( "apellido1cliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "1er Apellido" ) );
    addSubFormHeader ( "apellido2cliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNoWrite, _ ( "2do Apellido" ) );
//    addSubFormHeader ( "porcentalumnocliente", BlDbField::DbNumeric, BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Porcentaje" ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView, _ ( "Id tutor" ) );

    setInsert ( true );
    setOrdenEnabled ( false );
    
    
}

///
/**
\param idalumno
**/
void ListAlumnosTutorView::load ( QString idalumno )
{
    BL_FUNC_DEBUG
    BlSubForm::load ( "SELECT * FROM alumnocliente LEFT JOIN cliente ON alumnocliente.idcliente = cliente.idcliente  WHERE alumnocliente.idalumno =" + idalumno  );
    
}

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
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombreactividad", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre actividad" ) );
    addSubFormHeader ( "idactividad", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView, _ ( "Id actividad" ) );

    setInsert ( true );
    setOrdenEnabled ( false );
    
    
}

///
/**
\param idalumno
**/
void ListAlumnosActividadView::load ( QString idalumno )
{
    BL_FUNC_DEBUG
    
    BlSubForm::load ( "SELECT * FROM alumnoactividad LEFT JOIN actividad ON alumnoactividad.idactividad = actividad.idactividad  WHERE alumnoactividad.idalumno =" + idalumno  );
    
    
}
