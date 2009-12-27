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
    _depura ( "BcAmortizacionListView::BcAmortizacionListView", 0 );

    this->setAttribute ( Qt::WA_DeleteOnClose );
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

    meteWindow ( windowTitle() );
    _depura ( "END BcAmortizacionListView::BcAmortizacionListView", 0 );
}


/** Destructor de la clase que libera mermoria y saca la ventana de la lista
 * de ventanas.
 */
/**
**/
BcAmortizacionListView::~BcAmortizacionListView()
{
    _depura ( "BcAmortizacionListView::~BcAmortizacionListView", 0 );
    sacaWindow();
    _depura ( "END BcAmortizacionListView::~BcAmortizacionListView", 0 );
}


/** SLOT que responde a la pulsacion del boton crear una nueva amortizacion.
 * Crea una instancia de la clase BcAmortizacionView y la llama.
 */
/**
**/
void BcAmortizacionListView::crear()
{
    _depura ( "BcAmortizacionListView::on_mui_crear_released", 0 );
    BcAmortizacionView *amor = new BcAmortizacionView ( ( BcCompany * ) mainCompany(), 0 );
    mainCompany() ->pWorkspace() ->addWindow ( amor );
    amor->show();
    _depura ( "END BcAmortizacionListView::on_mui_crear_released", 0 );
}


/// Esta funci&oacute;n se encarga de borrar una amortizaci&oacute;n.
/// La que esta seleccionada en el listado.
/**
**/
void BcAmortizacionListView::borrar()
{
    _depura ( "BcAmortizacionListView::on_mui_borrar_released", 0 );
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
        _depura ( _ ( "Error al borrar la amortizacion" ), 2 );
    } // end try
    _depura ( "END BcAmortizacionListView::on_mui_borrar_released", 0 );
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
    _depura ( "BcAmortizacionListView::editAmortizacion " + row, 0 );
    mdb_idamortizacion = mui_listado->dbValue ( "idamortizacion" );
    mdb_nomamortizacion = mui_listado->dbValue ( "nomamortizacion" );
    if ( modoEdicion() ) {
        /// Creamos el objeto BcMasaPatrimonialView, y lo lanzamos.
        BcAmortizacionView * amor = new BcAmortizacionView ( ( BcCompany * ) mainCompany(), 0 );
        amor->cargar ( mdb_idamortizacion );
        mainCompany() ->pWorkspace() ->addWindow ( amor );
        amor->show();
    } else {
        close();
        emit ( selected ( mdb_idamortizacion ) );
    } // end if
    _depura ( "END BcAmortizacionListView::editAmortizacion", 0 );
}

