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

#include "reciboview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ReciboView::ReciboView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "ReciboView::ReciboView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _( "Recibo" ) );
        setDbTableName ( "recibo" );
        setDbFieldId ( "idrecibo" );
        addDbField ( "idrecibo", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "ID recibo" ) );
        addDbField ( "nombrerecibo", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Nombre del recibo" ) );
        addDbField ( "codigorecibo", BlDbField::DbVarChar, BlDbField::DbNotNull, _( "Codigo" ) );
        
	addDbField ( "idprofesor", BlDbField::DbInt, BlDbField::DbNotNull, _("Id profesor"));

        meteWindow ( windowTitle(), this, FALSE );

      /// Inicializamos el subformulario de alumnos
//      mui_alumnosList->setMainCompany(comp);
      
		/// Establecemos los parametros de busqueda de Profesor
	mui_idprofesor->setMainCompany(comp);
    mui_idprofesor->setLabel ( _( "Profesor:" ) );
	mui_idprofesor->setTableName( "profesor" );
	mui_idprofesor->m_valores["nombreprofesor"] = "";


        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el recibo" ), this );
    } // end try
    _depura ( "END ReciboView::ReciboView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
ReciboView::~ReciboView()
{
    _depura ( "ReciboView::~ReciboView", 0 );
    _depura ( "END ReciboView::~ReciboView", 0 );
}


QString ReciboView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void ReciboView::imprimir()
{
    _depura ( "ReciboView::imprimir", 0 );
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

    _depura ( "END ReciboView::imprimir", 0 );
}


int ReciboView::guardarPost() {
	_depura(" ReciboView::guardarPost", 0);
//  mui_alumnosList->setColumnValue("idrecibo", dbValue("idrecibo"));
//  mui_alumnosList->guardar();
	_depura("END ReciboView::guardarPost", 0);
}


int ReciboView::borrarPre() {

  QString query = "DELETE FROM alumnorecibo WHERE idrecibo=" + dbValue("idrecibo");
    mainCompany()->runQuery(query);
    return 0;
}



int ReciboView::cargarPost(QString id) {
   _depura(" ReciboView::cargarPost", 0);

//    mui_alumnosList->cargar(id);

   _depura("END ReciboView::cargarPost", 0);
  return 0;
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================

///
/**
\param parent
**/
ListAlumnosReciboView::ListAlumnosReciboView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlumnosReciboView::ListAlumnosReciboView", 0 );
    setDbTableName ( "alumnorecibo" );
    setDbFieldId ( "idalumnorecibo" );
    addSubFormHeader ( "idalumnorecibo", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _( "Identificador" ) );   
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Nombre alumno" ) );
    addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _( "Id Actividad" ) );

    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    _depura ( "END ListAlumnosReciboView::ListAlumnosReciboView", 0 );
}


///
/**
\param idcontrato
**/
void ListAlumnosReciboView::cargar ( QString idrecibo )
{
    _depura ( "ListAlumnosReciboView::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM alumnorecibo LEFT JOIN alumno ON alumnorecibo.idalumno = alumno.idalumno  WHERE alumnorecibo.idrecibo=" + idrecibo  );
    _depura ( "END ListAlumnosReciboView::cargar", 0 );
}






