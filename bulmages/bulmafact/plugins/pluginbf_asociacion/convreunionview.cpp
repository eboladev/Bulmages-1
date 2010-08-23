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
    blDebug ( "ConvReunionView::ConvReunionView", 0 );
    
    setAttribute ( Qt::WA_DeleteOnClose );
    
    try {
    
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Convocatoria de reunion" ) );
        setDbTableName ( "reunion" );
        setDbFieldId ( "idreunion" );
        addDbField ( "idreunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID reunion" ) );
        addDbField ( "fecha1convocatoriareunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha 1ª Convocatoria" ) );
        addDbField ( "fecha2convocatoriareunion", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha 2ª Convocatoria" ) );
        addDbField ( "hora1convocatoriareunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Hora 1ª Convocatoria" ) );
        addDbField ( "hora2convocatoriareunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Hora 2ª Convocatoria" ) );
        addDbField ( "tiporeunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Tipo" ) );
        addDbField ( "conceptoreunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Concepto" ) );
        addDbField ( "resolucionreunion", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "Resolucion" ) );

        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "asistentereunion" );
        mui_list->setDbFieldId ( "idasistentereunion" );
        mui_list->addSubFormHeader ( "idasistentereunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID asistentereunion" ) );
        mui_list->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Reunion" ) );
        mui_list->addSubFormHeader ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbRequired, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Socio" ) );
        mui_list->addSubFormHeader ( "nomcliente", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone, _ ( "Nombre Socio" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setDelete ( TRUE );
        mui_list->setSortingEnabled ( FALSE );

	 /// Hacemos una carga vacia para que se inicie bien el subformulario
         mui_list->cargar("SELECT * from asistentereunion NATURAL LEFT JOIN cliente WHERE 1 = 2");

        mui_ordendia->setMainCompany( comp );
        mui_ordendia->setDbTableName ( "ordendiareunion" );
        mui_ordendia->setDbFieldId ( "idordendiareunion" );
        mui_ordendia->addSubFormHeader ( "idordendiareunion", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Orden del dia" ) );
        mui_ordendia->addSubFormHeader ( "idreunion", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "ID Reunion" ) );
        mui_ordendia->addSubFormHeader ( "conceptoordendiareunion", BlDbField::DbVarChar, BlDbField::DbRequired, BlSubFormHeader::DbNone, _ ( "Concepto" ) );
        mui_ordendia->addSubFormHeader ( "textoordendiareunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Texto" ) );
        mui_ordendia->addSubFormHeader ( "resolucionordendiareunion", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Resolucion" ) );

        mui_ordendia->setInsert ( TRUE );
        mui_ordendia->setDelete ( TRUE );
        mui_ordendia->setSortingEnabled ( FALSE );
        
	 /// Hacemos una carga vacia para que se inicie bien el subformulario
         mui_ordendia->cargar("SELECT * from ordendiareunion WHERE 1 = 2");

        meteWindow ( windowTitle(), this, FALSE );
        pintar();
        dialogChanges_cargaInicial();
        
    } catch ( ... ) {
    
        blMsgInfo ( _ ( "Error al crear la reunion" ), this );
        
    } // end try
    
    blDebug ( "END ConvReunionView::ConvReunionView", 0 );
}

/** No precisa acciones adicionales en el destructor.
*/
ConvReunionView::~ConvReunionView()
{
    blDebug ( "ConvReunionView::~ConvReunionView", 0 );
    blDebug ( "END ConvReunionView::~ConvReunionView", 0 );
}

QString ConvReunionView::nombrePlantilla ( void )
{
    blDebug ( "ConvReunionView::nombrePlantilla", 0 );
    blDebug ( "END ConvReunionView::nombrePlantilla", 0 );

    return QString ( "reunion" );
}

void ConvReunionView::imprimir()
{
    blDebug ( "ConvReunionView::imprimir", 0 );
    
    /// Comprobamos que se disponen de los datos minimos para imprimir el reunion.
    QString SQLQuery = "";
    
    if ( dbValue ( "idreunion" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        blDebug ( "END ConvReunionView::imprimir", 0 );
        return;
    } // end if
    
    /// Disparamos los plugins
    int res = g_plugins->lanza ( "ConvReunionView_on_mui_imprimir_clicked", this );
    if ( res != 0 ) {
        blDebug ( "END ConvReunionView::imprimir", 0 );
        return;
    } // end if
    
    BfForm::imprimir();
    
    blDebug ( "END ConvReunionView::imprimir", 0 );
}

int ConvReunionView::guardarPost()
{
    blDebug ( " ConvReunionView::guardarPost", 0 );
    
    mui_list->setColumnValue("idreunion", dbValue("idreunion") );
    mui_list->guardar();
    
    mui_ordendia->setColumnValue("idreunion", dbValue("idreunion") );
    mui_ordendia->guardar();
    
    blDebug ( "END ConvReunionView::guardarPost", 0 );
    return 0;
}

int ConvReunionView::borrarPre()
{
    blDebug ( "ConvReunionView::borrarPre", 0 );
    blDebug ( "END ConvReunionView::borrarPre", 0 );

    return 0;
}

int ConvReunionView::cargarPost ( QString id )
{
    blDebug ( " ConvReunionView::cargarPost", 0 );
    
    mui_list->cargar("SELECT * FROM asistentereunion NATURAL LEFT JOIN cliente WHERE idreunion = " + id);
    mui_ordendia->cargar("SELECT * FROM ordendiareunion WHERE idreunion = " + id);
    
    blDebug ( "END ConvReunionView::cargarPost", 0 );
    
    return 0;
}

void ConvReunionView::on_mui_email_clicked (  )
{
    blDebug ( " ConvReunionView::cargarPost", 0 );
    
//    cad = "kmail -s \" El Subject\" --body \" Adjunto remito \n Atentamente\n\" --attach " + g_confpr->valor ( CONF_DIR_USER ) + doc + num + ".pdf " + email;
    QString bcc = "";
    QString separador = "";
    
    QString query = "SELECT * FROM asistentereunion NATURAL LEFT JOIN cliente WHERE idreunion = " + dbValue("idreunion");
    BlDbRecordSet *cur = mainCompany()->loadQuery(query);
    
    while (! cur->eof()) {
        bcc = bcc + separador + cur->valor("mailcliente");
        separador = ";";
        cur->nextRecord();
    } // end while
    
    delete cur;

    QString cad = "kmail -s \" El Subject\" --body \" Adjunto remito \n Atentamente\n\"  -b \"" + bcc + "\"";
    system ( cad.toAscii().data() );

    blDebug ( "END ConvReunionView::cargarPost", 0 );
}

void ConvReunionView::on_m_agregarconvocados_clicked() 
{
    blDebug ( "ConvReunionView::on_m_agregarconvocados_clicked", 0 );
    
    BlDbSubFormRecord *linea1;

    /// Luego segun el tipo de agregados seleccionados habra que modificar el query.
    QString text = m_tipoagregados->currentText();

    QString query = "SELECT * FROM cliente";
    BlDbRecordSet *cur = mainCompany()->loadQuery(query);
    
    while ( !cur->eof() ) {
        linea1 = mui_list->lineaat ( mui_list->rowCount() - 1 );
        mui_list->nuevoRegistro();
        mui_list->setProcesarCambios ( FALSE );
        linea1->setDbValue ( "idcliente", cur->valor ( "idcliente" ) );
        linea1->setDbValue ( "nomcliente", cur->valor ( "nomcliente" ) );
        mui_list->setProcesarCambios ( TRUE );
        cur->nextRecord();
    } // end while
    
    delete cur;

    blDebug ( "END ConvReunionView::on_m_agregarconvocados_clicked", 0 );
}

int ConvReunionView::junta()
{
    blDebug ( "ConvReunionView::junta", 0 );
    
    BlDbSubFormRecord *linea1;

    /// Luego segun el tipo de agregados seleccionados habra que modificar el query.
    QString text = m_tipoagregados->currentText();
    mui_tiporeunion->setCurrentIndex(mui_tiporeunion->findText ( "Junta Directiva" ) );
    QString query = "SELECT * FROM miembrojdirectiva NATURAL LEFT JOIN cliente WHERE idjdirectiva IN (SELECT max(idjdirectiva) FROM jdirectiva)";
    BlDbRecordSet *cur = mainCompany()->loadQuery(query);
    
    while (! cur->eof()) {
        linea1 = mui_list->lineaat ( mui_list->rowCount() - 1 );
        mui_list->nuevoRegistro();
        mui_list->setProcesarCambios ( FALSE );
        linea1->setDbValue ( "idcliente", cur->valor ( "idcliente" ) );
        linea1->setDbValue ( "nomcliente", cur->valor ( "nomcliente" ) );
        mui_list->setProcesarCambios ( TRUE );
        cur->nextRecord();
    } // end while
    
    delete cur;

    blDebug ( "END ConvReunionView::junta", 0 );
    return 0;
}
