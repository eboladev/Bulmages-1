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

#include "tutorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"
#include "blpostgresqlclient.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
TutorView::TutorView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    _depura ( "TutorView::TutorView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        centrarEnPantalla ( this );

        setTitleName ( _ ( "Tutor" ) );
        setDbTableName ( "cliente" );
        setDbFieldId ( "idcliente" );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID tutor" ) );
        addDbField ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del tutor" ) );
        addDbField ( "numsociocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del tutor" ) );
        addDbField ( "sociocliente", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Es Socio" ) );
        addDbField ( "cifcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "codcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "dircliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "poblcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "bancocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "cpcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "telcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "teltrabcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "movilcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "faxcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "mailcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Numero de Socio" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de Pago" ) );
        addDbField ( "idprovincia", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Provincia" ) );


        mui_alumnosList->setMainCompany ( mainCompany() );
        mui_idforma_pago->setMainCompany( mainCompany() );
        mui_idprovincia->setMainCompany ( mainCompany() );
        mui_idprovincia->setQuery ( "SELECT * FROM provincia LEFT JOIN pais ON provincia.idpais = pais.idpais ORDER BY descpais, provincia" );
        mui_idprovincia->setTableName ( "provincia" );
        mui_idprovincia->setFieldId ( "idprovincia" );
        mui_idprovincia->m_valores["provincia"] = "";
        mui_idprovincia->m_valores["descpais"] = "";
        mui_idprovincia->setAllowNull ( TRUE );
        mui_idprovincia->setId ( "" );

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
        
    } catch ( ... ) {
    
        mensajeInfo ( _ ( "Error al crear el tutor" ), this );
        
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

void TutorView::on_mui_sociocliente_toggled ( bool state )
{
    _depura ( "TutorView::on_mui_sociocliente_toggled", 0 );
    
    if ( mui_numsociocliente->text() == "" && state ) {
        QString query = "SELECT COALESCE(max(numsociocliente) + 1 , 0) AS numsoc FROM cliente";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        mui_numsociocliente->setText ( cur->valor ( "numsoc" ) );
    } // end if
    
    _depura ( "END TutorView::on_mui_sociocliente_toggled", 0 );
}

QString TutorView::nombrePlantilla ( void )
{
    _depura ( "TutorView::nombrePlantilla", 0 );
    _depura ( "END TutorView::nombrePlantilla", 0 );
    
    return QString ( "tutor" );
}

void TutorView::imprimir()
{
    _depura ( "TutorView::imprimir", 0 );
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
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

int TutorView::guardarPost()
{
    _depura ( "TutorView::guardarPost", 0 );

    mui_alumnosList->setColumnValue ( "idcliente", dbValue ( "idcliente" ) );
    mui_alumnosList->guardar();

    /*

      if (mui_numsocio->text() != "") {
        QString query1 = "SELECT * FROM socio WHERE idcliente = " + dbValue("idcliente");
        BlDbRecordSet *cur = mainCompany()->loadQuery(query1);
        if (!cur->eof()) {
          QString query = "UPDATE socio SET numsocio = '"+mui_numsocio->text()+"', nomsocio='"+dbValue("nombretutor")+"' WHERE idcliente= "+dbValue("idcliente");
          mainCompany()->runQuery(query);
          mainCompany()->runQuery(query);
        } else {
          QString query = "INSERT INTO socio (numsocio, idcliente, nomsocio) VALUES ('"+mui_numsocio->text()+"', "+dbValue("idcliente")+", '"+dbValue("nombretutor")+"')";
          mainCompany()->runQuery(query);
        } // end if
        delete cur;
      } else {
        QString query = "DELETE FROM socio WHERE idcliente=" + dbValue("idcliente");
        mainCompany()->runQuery(query);
      } // end if
    */
    
    _depura ( "END TutorView::guardarPost", 0 );
    
    return 0;
}

int TutorView::borrarPre()
{
    _depura ( "TutorView::borrarPre", 0 );

    /*
        QString query = "DELETE FROM socio WHERE idcliente=" + dbValue("idcliente");
        mainCompany()->runQuery(query);
    */
    
    QString query = "DELETE FROM alumnocliente WHERE idcliente=" + dbValue ( "idcliente" );
    mainCompany()->runQuery ( query );
    
    _depura ( "END TutorView::borrarPre", 0 );
    
    return 0;
}

int TutorView::cargarPost ( QString id )
{
    _depura ( " TutorView::cargarPost", 0 );

    /*
        QString query1 = "SELECT * FROM socio WHERE idcliente = " + id;
        BlDbRecordSet *cur = mainCompany()->loadQuery(query1);
        if (!cur->eof()) {
          mui_numsocio->setText(cur->valor("numsocio"));
        } // end if
        delete cur;
    */
    
    mui_alumnosList->cargar ( id );

    _depura ( "END TutorView::cargarPost", 0 );
    
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
    addSubFormHeader ( "idalumnocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
    addSubFormHeader ( "porcentalumnocliente", BlDbField::DbNumeric, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Porcentaje Fact." ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id tutor" ) );

    setInsert ( TRUE );
    setSortingEnabled ( TRUE );
    
    _depura ( "END ListAlumnosTutorView::ListAlumnosTutorView", 0 );
}

///
/**
\param idcliente
**/
void ListAlumnosTutorView::cargar ( QString idcliente )
{
    _depura ( "ListAlumnosTutorView::cargar", 0 );
    
    BlSubForm::cargar ( "SELECT *, (apellido1alumno || ' ' || apellido2alumno || ', ' || nombrealumno) AS nombrealumno1 FROM alumnocliente LEFT JOIN alumno ON alumnocliente.idalumno = alumno.idalumno  WHERE alumnocliente.idcliente=" + idcliente  );
    
    _depura ( "END ListAlumnosTutorView::cargar", 0 );
}
