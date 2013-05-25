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

#include "convreunionview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ConvReunionView::ConvReunionView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Convocatoria de reunion" ) );
        setDbTableName ( "reunion" );
        setDbFieldId ( "idreunion" );
        addDbField ( "idreunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID reunion" ) );
        addDbField ( "fecha1convocatoriareunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha 1a Convocatoria" ) );
        addDbField ( "fecha2convocatoriareunion", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha 2a Convocatoria" ) );
        addDbField ( "hora1convocatoriareunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Hora 1a Convocatoria" ) );
        addDbField ( "hora2convocatoriareunion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Hora 2a Convocatoria" ) );
        addDbField ( "tiporeunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Tipo" ) );
        addDbField ( "conceptoreunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Concepto" ) );
        addDbField ( "resolucionreunion", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Resolucion" ) );

        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "asistentereunion" );
        mui_list->setDbFieldId ( "idasistentereunion" );
        mui_list->addSubFormHeader ( "idasistentereunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID asistentereunion" ) );
        mui_list->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Reunion" ) );
        mui_list->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Socio" ) );
        mui_list->addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre Socio" ) );

        mui_list->setInsert ( true );
        mui_list->setDelete ( true );
        mui_list->setSortingEnabled ( false );

	 /// Hacemos una carga vacia para que se inicie bien el subformulario
         mui_list->load("SELECT * from asistentereunion NATURAL LEFT JOIN cliente WHERE 1 = 2");

        mui_ordendia->setMainCompany( comp );
        mui_ordendia->setDbTableName ( "ordendiareunion" );
        mui_ordendia->setDbFieldId ( "idordendiareunion" );
        mui_ordendia->addSubFormHeader ( "idordendiareunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Orden del dia" ) );
        mui_ordendia->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Reunion" ) );
        mui_ordendia->addSubFormHeader ( "conceptoordendiareunion", BlDbField::DbVarChar, BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Concepto" ) );
        mui_ordendia->addSubFormHeader ( "textoordendiareunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Texto" ) );
        mui_ordendia->addSubFormHeader ( "resolucionordendiareunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Resolucion" ) );

        mui_ordendia->setInsert ( true );
        mui_ordendia->setDelete ( true );
        mui_ordendia->setSortingEnabled ( false );
        
	 /// Hacemos una carga vacia para que se inicie bien el subformulario
         mui_ordendia->load("SELECT * from ordendiareunion WHERE 1 = 2");

        insertWindow ( windowTitle(), this, false );
        pintar();
        dialogChanges_readValues();
        blScript(this);
    } catch ( ... ) {
    
        blMsgInfo ( _ ( "Error al crear la reunion" ), this );
        
    } // end try
    
    
}

/** No precisa acciones adicionales en el destructor.
*/
ConvReunionView::~ConvReunionView()
{
    BL_FUNC_DEBUG
    
}

QString ConvReunionView::templateName ( void )
{
    BL_FUNC_DEBUG
    

    return QString ( "reunion" );
}

void ConvReunionView::imprimir()
{
    BL_FUNC_DEBUG
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el reunion.
    QString SQLQuery = "";
    
    if ( dbValue ( "idreunion" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        
        return;
    } // end if
    
    /// Disparamos los plugins
    int res = g_plugins->run ( "ConvReunionView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        
        return;
    } // end if
    
    BfForm::imprimir();
    
    
}

int ConvReunionView::afterSave()
{
    BL_FUNC_DEBUG
    
    mui_list->setColumnValue("idreunion", dbValue("idreunion") );
    mui_list->save();
    
    mui_ordendia->setColumnValue("idreunion", dbValue("idreunion") );
    mui_ordendia->save();
    
    
    return 0;
}

int ConvReunionView::beforeDelete()
{
    BL_FUNC_DEBUG
    

    return 0;
}

int ConvReunionView::cargarPost ( QString id )
{
    BL_FUNC_DEBUG
    
    mui_list->load("SELECT * FROM asistentereunion NATURAL LEFT JOIN cliente WHERE idreunion = " + id);
    mui_ordendia->load("SELECT * FROM ordendiareunion WHERE idreunion = " + id);
    
    
    
    return 0;
}

void ConvReunionView::on_mui_email_clicked (  )
{
    BL_FUNC_DEBUG
    
//    cad = "kmail -s \" El Subject\" --body \" Adjunto remito \n Atentamente\n\" --attach " + g_confpr->value( CONF_DIR_USER ) + doc + num + ".pdf " + email;
    QString bcc = "";
    QString separador = "";
    
    QString query = "SELECT * FROM asistentereunion NATURAL LEFT JOIN cliente WHERE idreunion = " + dbValue("idreunion");
    BlDbRecordSet *cur = mainCompany()->loadQuery(query);
    
    while (! cur->eof()) {
        bcc = bcc + separador + cur->value("mailcliente");
        separador = ";";
        cur->nextRecord();
    } // end while
    
    delete cur;

    QString cad = "kmail -s \" El Subject\" --body \" Adjunto remito \n Atentamente\n\"  -b \"" + bcc + "\"";
    system ( cad.toLatin1().data() );

    
}

void ConvReunionView::on_m_agregarconvocados_clicked() 
{
    BL_FUNC_DEBUG
    
    BlDbSubFormRecord *linea1;

    /// Luego segun el tipo de agregados seleccionados habra que modificar el query.
    QString text = m_tipoagregados->currentText();

    QString query = "SELECT * FROM cliente";
    BlDbRecordSet *cur = mainCompany()->loadQuery(query);
    
    while ( !cur->eof() ) {
        linea1 = mui_list->lineaat ( mui_list->rowCount() - 1 );
        mui_list->newRecord();
        mui_list->setProcesarCambios ( false );
        linea1->setDbValue ( "idcliente", cur->value( "idcliente" ) );
        linea1->setDbValue ( "nomcliente", cur->value( "nomcliente" ) );
        mui_list->setProcesarCambios ( true );
        cur->nextRecord();
    } // end while
    
    delete cur;

    
}

int ConvReunionView::junta()
{
    BL_FUNC_DEBUG
    
    BlDbSubFormRecord *linea1;

    /// Luego segun el tipo de agregados seleccionados habra que modificar el query.
    QString text = m_tipoagregados->currentText();
    mui_tiporeunion->setCurrentIndex(mui_tiporeunion->findText ( "Junta Directiva" ) );
    QString query = "SELECT * FROM miembrojdirectiva NATURAL LEFT JOIN cliente WHERE idjdirectiva IN (SELECT max(idjdirectiva) FROM jdirectiva)";
    BlDbRecordSet *cur = mainCompany()->loadQuery(query);
    
    while (! cur->eof()) {
        linea1 = mui_list->lineaat ( mui_list->rowCount() - 1 );
        mui_list->newRecord();
        mui_list->setProcesarCambios ( false );
        linea1->setDbValue ( "idcliente", cur->value( "idcliente" ) );
        linea1->setDbValue ( "nomcliente", cur->value( "nomcliente" ) );
        mui_list->setProcesarCambios ( true );
        cur->nextRecord();
    } // end while
    
    delete cur;

    
    return 0;
}
