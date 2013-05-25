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
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Padre" ) );
        setDbTableName ( "cliente" );
        setDbFieldId ( "idcliente" );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID tutor" ) );
        addDbField ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Nombre del tutor" ) );
        addDbField ( "apellido1cliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "1er apellido del tutor" ) );
        addDbField ( "apellido2cliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "2o apellido del tutor" ) );
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
        mui_idprovincia->setAllowNull ( true );
        mui_idprovincia->setId ( "" );



	mui_listrecibos->setMainCompany( mainCompany() );
	mui_listrecibos->setDbTableName ( "recibo" );
	mui_listrecibos->setDbFieldId ( "idrecibo" );
	mui_listrecibos->addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Recibo" ) );
	mui_listrecibos->addSubFormHeader ( "cantrecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Cantidad" ) );
	mui_listrecibos->addSubFormHeader ( "pagadorecibo", BlDbField::DbBoolean, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Pagado" ) );
	mui_listrecibos->addSubFormHeader ( "fecharecibo", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha" ) );
	mui_listrecibos->addSubFormHeader ( "idcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "ID Tutor" ) );
	mui_listrecibos->addSubFormHeader ( "descforma_pago", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Forma de Pago" ) );
	mui_listrecibos->setInsert ( false );
	mui_listrecibos->setSortingEnabled ( true );

        /// Establecemos los parametros de busqueda de Profesor
        m_idalumno1->setMainCompany ( comp );
        m_idalumno1->setLabel ( _ ( "Alumno 1:" ) );
        m_idalumno1->setTableName ( "alumno" );
        m_idalumno1->setFieldId("idalumno");
        m_idalumno1->m_valores["nombrealumno"] = "";

        /// Establecemos los parametros de busqueda de Profesor
        m_idalumno2->setMainCompany ( comp );
        m_idalumno2->setLabel ( _ ( "Alumno 2:" ) );
        m_idalumno2->setTableName ( "alumno" );
        m_idalumno2->setFieldId("idalumno");
        m_idalumno2->m_valores["nombrealumno"] = "";
	
        /// Establecemos los parametros de busqueda de Profesor
        m_idalumno3->setMainCompany ( comp );
        m_idalumno3->setLabel ( _ ( "Alumno 3:" ) );
        m_idalumno3->setTableName ( "alumno" );
        m_idalumno3->setFieldId("idalumno");
        m_idalumno3->m_valores["nombrealumno"] = "";
	
        /// Establecemos los parametros de busqueda de Profesor
        m_idalumno4->setMainCompany ( comp );
        m_idalumno4->setLabel ( _ ( "Alumno 4:" ) );
        m_idalumno4->setTableName ( "alumno" );
        m_idalumno4->setFieldId("idalumno");
        m_idalumno4->m_valores["nombrealumno"] = "";
	
	/// Simulamos una carga para que el subformulario este operativo desde el principio.
        mui_alumnosList->load ( "0" );

	
        insertWindow ( windowTitle(), this, false );
        pintar();
        dialogChanges_readValues();
        blScript(this);
    } catch ( ... ) {
    
        blMsgInfo ( _ ( "Error al crear el tutor" ), this );
        
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
TutorView::~TutorView()
{
    BL_FUNC_DEBUG
    
}

void TutorView::on_mui_sociocliente_toggled ( bool state )
{
    BL_FUNC_DEBUG
    
    if ( mui_numsociocliente->text() == "" && state ) {
        QString query = "SELECT COALESCE(max(numsociocliente) + 1 , 0) AS numsoc FROM cliente";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
        mui_numsociocliente->setText ( cur->value( "numsoc" ) );
    } // end if
    
    
}

QString TutorView::templateName ( void )
{
    BL_FUNC_DEBUG
    
    
    return QString ( "tutor" );
}

void TutorView::imprimir()
{
    BL_FUNC_DEBUG
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }
    
    /// Disparamos los plugins
    int res = g_plugins->run ( "TutorView_imprimir", this );
    if ( res != 0 ) {
        return;
    } // end if
    
    BfForm::imprimir();

    
}

int TutorView::afterSave()
{
    BL_FUNC_DEBUG

    mui_alumnosList->setColumnValue ( "idcliente", dbValue ( "idcliente" ) );
    mui_alumnosList->save();

    if (m_idalumno1->id() != "") {
	    QString sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+m_idalumno1->id()+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
    } // end if
    
    
    if (m_idalumno2->id() != "") {
	    QString sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+m_idalumno2->id()+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
    } // end if
    
    if (m_idalumno3->id() != "") {
	    QString sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+m_idalumno3->id()+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
    } // end if
    
    if (m_idalumno4->id() != "") {
	    QString sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+m_idalumno4->id()+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
    } // end if
    
    
    m_idalumno1->setId("");
    m_idalumno2->setId("");
    m_idalumno3->setId("");
    m_idalumno4->setId("");
    
    /*
    if (mu_nombrealumno_1->text() != "") {
	QString sqlquery = "INSERT INTO alumno (nombrealumno, apellido1alumno, apellido2alumno) VALUES ('"+mu_nombrealumno_1->text()+"','"+mu_apellido1alumno_1->text()+"','"+mu_apellido2alumno_1->text()+"')";
	mainCompany()->runQuery(sqlquery);
	sqlquery = "SELECT MAX(idalumno) AS idalumno FROM alumno";
        BlDbRecordSet *cur = mainCompany()->loadQuery(sqlquery);
        if (!cur->eof()) {
	    sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+cur->value("idalumno")+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
	} // end if
	delete cur;
    } // end if
    
    if (mu_nombrealumno_2->text() != "") {
	QString sqlquery = "INSERT INTO alumno (nombrealumno, apellido1alumno, apellido2alumno) VALUES ('"+mu_nombrealumno_2->text()+"','"+mu_apellido1alumno_2->text()+"','"+mu_apellido2alumno_2->text()+"')";
	mainCompany()->runQuery(sqlquery);
	sqlquery = "SELECT MAX(idalumno) AS idalumno FROM alumno";
        BlDbRecordSet *cur = mainCompany()->loadQuery(sqlquery);
        if (!cur->eof()) {
	    sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+cur->value("idalumno")+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
	} // end if
	delete cur;
    } // end if

    if (mu_nombrealumno_3->text() != "") {
	QString sqlquery = "INSERT INTO alumno (nombrealumno, apellido1alumno, apellido2alumno) VALUES ('"+mu_nombrealumno_3->text()+"','"+mu_apellido1alumno_3->text()+"','"+mu_apellido2alumno_3->text()+"')";
	mainCompany()->runQuery(sqlquery);
	sqlquery = "SELECT MAX(idalumno) AS idalumno FROM alumno";
        BlDbRecordSet *cur = mainCompany()->loadQuery(sqlquery);
        if (!cur->eof()) {
	    sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+cur->value("idalumno")+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
	} // end if
	delete cur;
    } // end if

    if (mu_nombrealumno_4->text() != "") {
	QString sqlquery = "INSERT INTO alumno (nombrealumno, apellido1alumno, apellido2alumno) VALUES ('"+mu_nombrealumno_4->text()+"','"+mu_apellido1alumno_4->text()+"','"+mu_apellido2alumno_4->text()+"')";
	mainCompany()->runQuery(sqlquery);
	sqlquery = "SELECT MAX(idalumno) AS idalumno FROM alumno";
        BlDbRecordSet *cur = mainCompany()->loadQuery(sqlquery);
        if (!cur->eof()) {
	    sqlquery = "INSERT INTO alumnocliente (idalumno, idcliente) VALUES ("+cur->value("idalumno")+","+dbValue("idcliente")+")";
	    mainCompany()->runQuery(sqlquery);
	} // end if
	delete cur;
    } // end if

    mu_nombrealumno_1->setText("");
    mu_nombrealumno_2->setText("");
    mu_nombrealumno_3->setText("");
    mu_nombrealumno_4->setText("");

    mu_apellido1alumno_1->setText("");
    mu_apellido1alumno_2->setText("");
    mu_apellido1alumno_3->setText("");
    mu_apellido1alumno_4->setText("");
    
    mu_apellido2alumno_1->setText("");
    mu_apellido2alumno_2->setText("");
    mu_apellido2alumno_3->setText("");
    mu_apellido2alumno_4->setText("");
    */
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
    
    
    
    return 0;
}

int TutorView::beforeDelete()
{
    BL_FUNC_DEBUG

    /*
        QString query = "DELETE FROM socio WHERE idcliente=" + dbValue("idcliente");
        mainCompany()->runQuery(query);
    */
    
    QString query = "DELETE FROM alumnocliente WHERE idcliente=" + dbValue ( "idcliente" );
    mainCompany()->runQuery ( query );
    
    
    
    return 0;
}

int TutorView::cargarPost ( QString id )
{
    BL_FUNC_DEBUG

    /*
        QString query1 = "SELECT * FROM socio WHERE idcliente = " + id;
        BlDbRecordSet *cur = mainCompany()->loadQuery(query1);
        if (!cur->eof()) {
          mui_numsocio->setText(cur->value("numsocio"));
        } // end if
        delete cur;
    */
    
    mui_alumnosList->load ( id );

    mui_listrecibos->load("SELECT * FROM recibo LEFT JOIN forma_pago ON recibo.idforma_pago = forma_pago.idforma_pago LEFT JOIN banco ON recibo.idbanco = banco.idbanco WHERE recibo.idcliente = " + id);

    
    
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
    BL_FUNC_DEBUG
    
    setDbTableName ( "alumnocliente" );
    setDbFieldId ( "idalumnocliente" );
    addSubFormHeader ( "idalumnocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
    addSubFormHeader ( "idalumno", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired | BlDbField::DbUnique, BlSubFormHeader::DbHideView, _ ( "Id alumno" ) );
    addSubFormHeader ( "nombrealumno1", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre alumno" ) );
//    addSubFormHeader ( "porcentalumnocliente", BlDbField::DbNumeric, BlDbField::DbNotNull | BlDbField::DbRequired , BlSubFormHeader::DbHideView, _ ( "Porcentaje Fact." ) );
    addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView, _ ( "Id tutor" ) );

    setInsert ( true );
    setSortingEnabled ( true );
    
    
}

///
/**
\param idcliente
**/
void ListAlumnosTutorView::load ( QString idcliente )
{
    BL_FUNC_DEBUG
    
    BlSubForm::load ( "SELECT *, (COALESCE(apellido1alumno,'-') || ' ' || COALESCE(apellido2alumno,'-') || ', ' || COALESCE(nombrealumno,'-') ) AS nombrealumno1 FROM alumnocliente LEFT JOIN alumno ON alumnocliente.idalumno = alumno.idalumno  WHERE alumnocliente.idcliente=" + idcliente  );
    
    
}
