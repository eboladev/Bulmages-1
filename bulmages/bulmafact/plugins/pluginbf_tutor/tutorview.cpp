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

#include "tutorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "busquedafecha.h"
#include "blfunctions.h"
#include "blpostgresqlclient.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
TutorView::TutorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    _depura ( "TutorView::TutorView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _( "Tutor" ) );
        setDbTableName ( "tutor" );
        setDbFieldId ( "idtutor" );
        addDbField ( "idtutor", BlDbField::DbInt, BlDbField::DbPrimaryKey, _( "ID tutor" ) );
        addDbField ( "nombretutor", BlDbField::DbVarChar, BlDbField::DbNothing, _( "Nombre del tutor" ) );

        mui_alumnosList->setMainCompany(mainCompany());
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
        mensajeInfo ( _( "Error al crear el tutor" ), this );
    } // end try
    _depura ( "END TutorView::TutorView", 0 );
}


/** No precisa acciones adicionales en el destructor.
*/
TutorView::~TutorView()
{
    _depura ( "TutorView::~TutorView", 0 );
    _depura ( "END TutorView::~TutorView", 0 );
}


QString TutorView::nombrePlantilla(void) 
{
   return QString("recibo");
}

void TutorView::imprimir()
{
    _depura ( "TutorView::imprimir", 0 );
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idtutor" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    } 
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "TutorView_imprimir", this );
    if ( res != 0 ) {
        return;
    } // end if
    BfForm::imprimir();

    _depura ( "END TutorView::imprimir", 0 );
}


int TutorView::guardarPost() {
	_depura(" TutorView::guardarPost", 0);
   
  mui_alumnosList->setColumnValue("idtutor", dbValue("idtutor"));
  mui_alumnosList->guardar();
   
  if (mui_numsocio->text() != "") {
    QString query1 = "SELECT * FROM socio WHERE idtutor = " + dbValue("idtutor");
    BlDbRecordSet *cur = mainCompany()->loadQuery(query1);
    if (!cur->eof()) {
      QString query = "UPDATE socio SET numsocio = '"+mui_numsocio->text()+"', nomsocio='"+dbValue("nombretutor")+"' WHERE idtutor= "+dbValue("idtutor");
      mainCompany()->runQuery(query);
      mainCompany()->runQuery(query);
    } else {
      QString query = "INSERT INTO socio (numsocio, idtutor, nomsocio) VALUES ('"+mui_numsocio->text()+"', "+dbValue("idtutor")+", '"+dbValue("nombretutor")+"')";
      mainCompany()->runQuery(query);
    } // end if
    delete cur;
  } else {
    QString query = "DELETE FROM socio WHERE idtutor=" + dbValue("idtutor");
    mainCompany()->runQuery(query);
  } // end if
	_depura("END TutorView::guardarPost", 0);
  return 0;
}

int TutorView::borrarPre() {
    QString query = "DELETE FROM socio WHERE idtutor=" + dbValue("idtutor");
    mainCompany()->runQuery(query);
    query = "DELETE FROM alumnotutor WHERE idtutor=" + dbValue("idtutor");
    mainCompany()->runQuery(query);
    return 0;
}



int TutorView::cargarPost(QString id) {
   _depura(" TutorView::cargarPost", 0);

    QString query1 = "SELECT * FROM socio WHERE idtutor = " + id;
    BlDbRecordSet *cur = mainCompany()->loadQuery(query1);
    if (!cur->eof()) {
      mui_numsocio->setText(cur->valor("numsocio"));
    } // end if
    delete cur;

    mui_alumnosList->cargar(id);

   _depura("END TutorView::cargarPost", 0);
  return 0;
}



/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla factura.
*/
/**
\param parent
**/


///
/**
\param parent
**/
ListAlumnosTutorView::ListAlumnosTutorView ( QWidget *parent ) : BfSubForm ( parent )
{
    _depura ( "ListAlumnosTutorView::ListAlumnosTutorView", 0 );
    setDbTableName ( "alumnotutor" );
    setDbFieldId ( "idalumnotutor" );
    addSubFormHeader ( "idalumnotutor", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _( "Identificador" ) );   
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _( "Nombre alumno" ) );
    addSubFormHeader ( "idtutor", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _( "Id tutor" ) );

    setinsercion ( TRUE );
    setOrdenEnabled ( TRUE );
    _depura ( "END ListAlumnosTutorView::ListAlumnosTutorView", 0 );
}


///
/**
\param idcontrato
**/
void ListAlumnosTutorView::cargar ( QString idtutor )
{
    _depura ( "ListAlumnosTutorView::cargar", 0 );
    BlSubForm::cargar ( "SELECT * FROM alumnotutor LEFT JOIN alumno ON alumnotutor.idalumno = alumno.idalumno  WHERE alumnotutor.idtutor=" + idtutor  );
    _depura ( "END ListAlumnosTutorView::cargar", 0 );
}


