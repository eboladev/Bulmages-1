/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "bcamortizacionlistview.h"
#include "bcamortizacionview.h"

#include "bccompany.h"


/** Constructor de la clase del listado.
 *  Inicializa las variables y elementos del listado.
 */
/**
\param emp
\param parent
**/
BcAmortizacionListView::BcAmortizacionListView ( BcCompany *emp, QWidget *parent )
        : BlFormList ( emp, parent )
{
    BL_FUNC_DEBUG

    //this->setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    setSubForm ( mui_listado );
    /// Hacemos la inicializacion de un listado embebido.
    mui_listado->setMainCompany ( mainCompany() );
    mui_listado->setDbTableName ( "amortizacion" );
    mui_listado->setFileConfig ( "AmortizacionListSubform" );
    mui_listado->setDbFieldId ( "idamortizacion" );
    mui_listado->addSubFormHeader ( "idamortizacion", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id amortizacion" ) );
    mui_listado->addSubFormHeader ( "nomamortizacion", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre" ) );
    mui_listado->addSubFormHeader ( "fechacompra", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha compra" ) );
    mui_listado->addSubFormHeader ( "fecha1cuota", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha 1 cuota" ) );
    mui_listado->addSubFormHeader ( "numcuotas", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Num cuotas" ) );
    mui_listado->setInsert ( FALSE );
    presentar();

    insertWindow ( windowTitle() );
   
    /// Lanzamos los posibles scripts
    blScript(this);

    
}


/** Destructor de la clase que libera mermoria y saca la ventana de la lista
 * de ventanas.
 */
/**
**/
BcAmortizacionListView::~BcAmortizacionListView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


/** SLOT que responde a la pulsacion del boton crear una nueva amortizacion.
 * Crea una instancia de la clase BcAmortizacionView y la llama.
 */
/**
**/
void BcAmortizacionListView::crear()
{
    BL_FUNC_DEBUG
    BcAmortizacionView *amor = new BcAmortizacionView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addSubWindow ( amor );
    amor->show();
    
}


/// Esta funci&oacute;n se encarga de borrar una amortizaci&oacute;n.
/// La que esta seleccionada en el listado.
/**
**/
void BcAmortizacionListView::remove()
{
    BL_FUNC_DEBUG
    try {
        QString codigo = mui_listado->dbValue ( "idamortizacion" );
        if ( codigo != "" ) {
            QString query = "DELETE FROM linamortizacion WHERE idamortizacion = " + codigo;
            mainCompany() ->begin();
            mainCompany() ->runQuery ( query );
            query = "DELETE FROM amortizacion WHERE idamortizacion = " + codigo;
            mainCompany() ->runQuery ( query );
            mainCompany() ->commit();
            presentar();
        } // end if
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al borrar la amortizacion.") );
	blMsgError(_("Error al borrar la amortizacion."));
    } // end try
    
}



/** Se encarga de la accion preseleccionada al hacer doble click o al darle
    al boton de editar.
    Si estamos en modo seleccion actualiza el valor de los campos de seleccion y
    cierra la ventana.
    Si estamos en modo edicion abre una instancia de ArticuloView y lo carga con el
    valor seleccionado.
*/
/**
\param row
**/
void BcAmortizacionListView::editar ( int row )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Fila '%1'")).arg(QString::number(row)) );
    mdb_idamortizacion = mui_listado->dbValue ( "idamortizacion" );
    mdb_nomamortizacion = mui_listado->dbValue ( "nomamortizacion" );
    if ( editMode() ) {
        /// Creamos el objeto BcMasaPatrimonialView, y lo lanzamos.
        BcAmortizacionView * amor = new BcAmortizacionView ( ( BcCompany * ) mainCompany(), 0 );
        amor->load ( mdb_idamortizacion );
        mainCompany() ->pWorkspace() ->addSubWindow ( amor );
        amor->show();
    } else {
        close();
        emit ( selected ( mdb_idamortizacion ) );
    } // end if
    
}

