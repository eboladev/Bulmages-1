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
#include "bldatesearch.h"
#include "blfunctions.h"

/** inicializa todos los componentes de la clase.
    Resetea el sistema de control de cambios para que considere que no hay cambios por parte del usuario.
    Mete la ventana en el workSpace.
*/
ReciboView::ReciboView ( BfCompany *comp, QWidget *parent ) : BfForm ( comp, parent )
{
    _depura ( "ReciboView::ReciboView", 0 );

    setAttribute ( Qt::WA_DeleteOnClose );

    try {
        setupUi ( this );
        centrarEnPantalla ( this );

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

        meteWindow ( windowTitle(), this, FALSE );

        mui_idforma_pago->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda de Profesor
        mui_idcliente->setMainCompany ( comp );
        mui_idcliente->setLabel ( _ ( "Tutor/Socio:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["nomcliente"] = "";

        /// Activamos el listado de lineas de recibo
        mui_list->setMainCompany( comp );
        mui_list->setDbTableName ( "lrecibo" );
        mui_list->setDbFieldId ( "idlrecibo" );
        mui_list->addSubFormHeader ( "idlrecibo", BlDbField::DbInt, BlDbField::DbPrimaryKey , BlSubFormHeader::DbHideView, _ ( "Identificador" ) );
        mui_list->addSubFormHeader ( "idrecibo", BlDbField::DbInt, BlDbField::DbNothing , BlSubFormHeader::DbHideView, _ ( "ID recibo" ) );
        mui_list->addSubFormHeader ( "cantlrecibo", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Cantidad Linea Recibo" ) );
        mui_list->addSubFormHeader ( "conceptolrecibo", BlDbField::DbVarChar, BlDbField::DbNotNull, BlSubFormHeader::DbNone, _ ( "Concepto" ) );

        mui_list->setInsert ( TRUE );
        mui_list->setOrdenEnabled ( TRUE );

	mui_list->cargar("SELECT * from lrecibo WHERE 1=2");

        // ======================================================

        pintar();
        dialogChanges_cargaInicial();
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error al crear el recibo" ), this );
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

QString ReciboView::nombrePlantilla ( void )
{
    _depura ( "ReciboView::nombrePlantilla", 0 );
    _depura ( "END ReciboView::nombrePlantilla", 0 );
    
    return QString ( "recibod" );
}

void ReciboView::imprimir()
{
    _depura ( "ReciboView::imprimir", 0 );

    /// Comprobamos que se disponen de los datos minimos para imprimir el recibo.
    QString SQLQuery = "";

    if ( dbValue ( "idcliente" ).isEmpty() ) {
        /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
        mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
        return;
    }

    /// Disparamos los plugins
    int res = g_plugins->lanza ( "ReciboView_on_mui_imprimir_released", this );

    if ( res != 0 ) {
        return;
    } // end if

    BfForm::imprimir();

    _depura ( "END ReciboView::imprimir", 0 );
}

int ReciboView::guardarPost()
{
    _depura ( "ReciboView::guardarPost", 0 );

    mui_list->setColumnValue("idrecibo", dbValue("idrecibo"));
    mui_list->guardar();

    _depura ( "END ReciboView::guardarPost", 0 );
    return 0;
}

int ReciboView::borrarPre()
{
    _depura ( "ReciboView::borrarPre", 0 );

    mainCompany()->runQuery("DELETE FROM lrecibo WHERE idrecibo = " + dbValue("idrecibo") );

    _depura ( "END ReciboView::borrarPre", 0 );
    
    return 0;
}

int ReciboView::cargarPost ( QString id )
{
    _depura ( "ReciboView::cargarPost", 0 );

    mui_list->cargar ("SELECT * FROM lrecibo WHERE idrecibo = " + id );
    
    QString total = mui_list->sumarCampo("cantlrecibo").toQString();
    m_cantrecibo->setText(total);
    setDbValue("cantrecibo", total);
    
    _depura ( "END ReciboView::cargarPost", 0 );
    
    return 0;
}

///
/**
**/
void ReciboView::on_mui_list_editFinish ( int, int )
{
    _depura ( "ReciboView::on_mui_list_editFinish", 0 );
    
    m_cantrecibo->setText(mui_list->sumarCampo("cantlrecibo").toQString());
    
    _depura ( "END ReciboView::on_mui_list_editFinish", 0 );
}

/// Esta funcion trabaja a muy bajo nivel ya que aprovecha una ficha cargada para hacer otra ficha.
/**
**/
void ReciboView::on_mui_reemitir_released (  )
{
    _depura ( "ReciboView::on_mui_reemitir_released", 0 );
    QString id;

   try {
   setDbValue("idrecibo", "");
   setNuevo(TRUE);

   mui_fecharecibo->setText("");
   recogeValores();
   mainCompany() ->begin();
   DBsave ( id );
   setDbValue ( m_campoid, id );
   mainCompany() ->commit();
   mui_list->setColumnValue("idrecibo", dbValue("idrecibo"));
   mui_list->guardar();

   QString query = "INSERT INTO lrecibo (idrecibo, cantlrecibo, conceptolrecibo) VALUES ("+dbValue("idrecibo")+", 12.5, \'Cuota por devolucion recibo\')";
   mainCompany()->runQuery(query);
   cargar(dbValue("idrecibo"));
   mensajeInfo(_("Recibo reemitido"));
   } catch(...) {
      mensajeError("Error en la creacion del recibo");
   } // end try

    _depura ( "END ReciboView::on_mui_reemitir_released", 0 );
}
