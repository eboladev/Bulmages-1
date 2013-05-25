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

#include <QtWidgets/QMessageBox>
#include <QtGui/QCloseEvent>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "reciboview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "bldatesearch.h"
#include "blfunctions.h"


/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ReciboView::ReciboView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG

    setAttribute ( Qt::WA_DeleteOnClose );

    try {
        setupUi ( this );
        blCenterOnScreen ( this );

        setTitleName ( _ ( "Recibo" ) );
        setDbTableName ( "recibo" );
        setDbFieldId ( "idrecibo" );
        addDbField ( "idrecibo", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "ID recibo" ) );
        addDbField ( "cantrecibo", BlDbField::DbNumeric, BlDbField::DbNothing, _ ( "Cantidad" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "ID cliente" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "ID Forma Pago" ) );
        addDbField ( "descrecibo", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion" ) );
        addDbField ( "fecharecibo", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha de creacion" ) );
        addDbField ( "pagadorecibo", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Pagado" ) );
        addDbField ( "devueltorecibo", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Devuelto" ) );

        insertWindow ( windowTitle(), this, false );

        mui_idforma_pago->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idcliente->setMainCompany ( comp );
        mui_idcliente->setLabel ( _ ( "Tutor/Socio:" ) );
        mui_idcliente->setTableName ( "cliente" );
	mui_idcliente->setFieldId( "idcliente" );
	mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";
	mui_idcliente->m_valores["apellido1cliente"] = "";
	mui_idcliente->m_valores["apellido2cliente"] = "";
	mui_idcliente->setMask("[cifcliente] .- [apellido1cliente] [apellido2cliente], [nomcliente]");

        /// Activamos el listado de lineas de recibo
        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "lrecibo" );
        mui_list->setDbFieldId ( "idlrecibo" );
        mui_list->addSubFormHeader ( "idlrecibo", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
        mui_list->addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _ ( "ID recibo" ) );
        mui_list->addSubFormHeader ( "cantlrecibo", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad Linea Recibo" ) );
        mui_list->addSubFormHeader ( "conceptolrecibo", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Concepto" ) );

        mui_list->setInsert ( true );
        mui_list->setOrdenEnabled ( true );

        mui_list->load("SELECT * from lrecibo WHERE 1=2");

        pintar();
        dialogChanges_readValues();
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el recibo" ), this );
    } // end try

    
}


/** No precisa acciones adicionales en el destructor.
*/
ReciboView::~ReciboView()
{
    BL_FUNC_DEBUG
    
}


QString ReciboView::templateName ( void )
{
    BL_FUNC_DEBUG
    
    
    return QString ( "recibod" );
}


void ReciboView::imprimir()
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
    int res = g_plugins->run ( "ReciboView_on_mui_imprimir_clicked", this );

    if ( res != 0 ) {
        return;
    } // end if

    BfForm::imprimir();

    
}


int ReciboView::afterSave()
{
    BL_FUNC_DEBUG

    mui_list->setColumnValue("idrecibo", dbValue("idrecibo"));
    mui_list->save();

    
    return 0;
}


int ReciboView::beforeDelete()
{
    BL_FUNC_DEBUG

    mainCompany()->runQuery("DELETE FROM lrecibo WHERE idrecibo = " + dbValue("idrecibo") );

    
    
    return 0;
}


int ReciboView::cargarPost ( QString id )
{
    BL_FUNC_DEBUG

    mui_list->load ("SELECT * FROM lrecibo WHERE idrecibo = " + id );
    
    QString total = mui_list->sumarCampo("cantlrecibo").toQString();
    m_cantrecibo->setText(total);
    setDbValue("cantrecibo", total);

    
    
    return 0;
}


///
/**
**/
void ReciboView::on_mui_list_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    
    m_cantrecibo->setText(mui_list->sumarCampo("cantlrecibo").toQString());
    
    
}


/// Esta funcion trabaja a muy bajo nivel ya que aprovecha una ficha cargada para hacer otra ficha.
/**
**/
void ReciboView::on_mui_reemitir_clicked (  )
{
    BL_FUNC_DEBUG
    QString id;

   try {
   setDbValue("idrecibo", "");
   setNuevo(true);

   mui_fecharecibo->setText("");
   recogeValores();
   mainCompany() ->begin();
   dbSave ( id );
   setDbValue ( m_campoid, id );
   mainCompany() ->commit();
   mui_list->setColumnValue("idrecibo", dbValue("idrecibo"));
   mui_list->save();

   BlDbRecordSet *curcuota = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre='CuotaReemisionRecibo'" );
   if (!curcuota->eof()) {
      QString query = "INSERT INTO lrecibo (idrecibo, cantlrecibo, conceptolrecibo) VALUES ("+dbValue("idrecibo")+", "+curcuota->value("valor")+", \'Cuota por devolucion recibo\')";
      mainCompany()->runQuery(query);
   } // end if

   load(dbValue("idrecibo"));
   blMsgInfo(_("Recibo reemitido"));
   } catch(...) {
      blMsgError(_("Error en la creacion del recibo"));
   } // end try

    
}


/// Hace la exportacion del campo a XML
QString ReciboView::exportXML() {
    BL_FUNC_DEBUG
    QString val;
    int error;
    BlDbField *campo;

    val = "<RECIBOVIEW>\n";
    val += "\t" + BlDbRecord::exportXML().replace("\t<","\t\t<").replace("\n<","\n\t<");
    val += "\t" + mui_list->exportXML().replace("\t<","\t\t<").replace("\n<","\n\t<");
    val += "</RECIBOVIEW>\n";

    return val;
    
}

