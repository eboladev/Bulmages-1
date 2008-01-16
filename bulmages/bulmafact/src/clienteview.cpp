/***************************************************************************
 *   Copyright (C) 2004 by Marcos Mezo                                     *
 *   mmezo@selexco.net                                                     *
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

#include <QLineEdit>
#include <QMessageBox>
#include <QToolButton>
#include <QWidget>
#include <QComboBox>
#include <QCloseEvent>

#include "clienteview.h"
#include "company.h"
#include "presupuestolist.h"
#include "pedidosclientelist.h"
#include "albaranclientelist.h"
#include "cobroslist.h"
#include "facturaslist.h"
#include "busquedaprovincia.h"
#include "funcaux.h"
#include "plugins.h"
#include "informereferencia.h"


/** Inicializa todos los componentes.
    MEte la ventana en el workSpace.
    Resetea el control de cambios.
*/
/**
\param comp
\param parent
\return
**/
ClienteView::ClienteView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "ClienteView::ClienteView", 0 );
    setupUi ( this );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {

        setTitleName ( tr ( "Cliente" ) );
        setDBTableName ( "cliente" );
        setDBCampoId ( "idcliente" );
        addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Cliente", "ID cliente" ) );
        addDBCampo ( "nomcliente", DBCampo::DBvarchar, DBCampo::DBNotNull, QApplication::translate ( "Cliente", "Nombre del cliente" ) );
        addDBCampo ( "nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Nombre alternativo del cliente" ) );
        addDBCampo ( "cifcliente", DBCampo::DBvarchar, DBCampo::DBNotNull, QApplication::translate ( "Cliente", "C.I.F. del cliente" ) );
        addDBCampo ( "bancocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Numero cuenta corriente" ) );
        addDBCampo ( "dircliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Direccion" ) );
        addDBCampo ( "poblcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Poblacion" ) );
        addDBCampo ( "idprovincia", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Provincia" ) );
        addDBCampo ( "cpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Codigo postal" ) );
        addDBCampo ( "telcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Numero de telefono" ) );
        addDBCampo ( "teltrabcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Numero de telefono en el trabajo" ) );
        addDBCampo ( "movilcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Numero de telefono movil" ) );
        addDBCampo ( "faxcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Numero de fax" ) );
        addDBCampo ( "mailcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Direccion electronica" ) );
        addDBCampo ( "urlcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Identificador de presupuesto" ) );
        addDBCampo ( "faltacliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Fecha de alta del cliente" ) );
        addDBCampo ( "fbajacliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Fecha de baja del cliente" ) );
        addDBCampo ( "comentcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Comentarios" ) );
        addDBCampo ( "inactivocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Cliente inactivo" ) );
        addDBCampo ( "regimenfiscalcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Regimen fiscal" ) );
        addDBCampo ( "codcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Codigo" ) );
        addDBCampo ( "corpcliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Empresa" ) );
        addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Forma de pago" ) );
        addDBCampo ( "recargoeqcliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Cliente", "Recargo de Equivalencia" ) );


        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "ClienteView_ClienteView", this );
        if ( res != 0 ) {
            return;
        } // end if
        mui_idprovincia->setEmpresaBase ( empresaBase() );
        mui_idprovincia->setIdProvincia ( "" );

        /// Inicializamos las pantallas auxiliares a esta.
        m_listpresupuestos->setEmpresaBase ( empresaBase() );
        m_listpedidos->setEmpresaBase ( empresaBase() );
        m_listalbaranes->setEmpresaBase ( empresaBase() );
        m_listfacturas->setEmpresaBase ( empresaBase() );
        m_listcobros->setEmpresaBase ( empresaBase() );
        mui_idforma_pago->setEmpresaBase ( empresaBase() );
        mui_idforma_pago->setidforma_pago ( "" );

        /// Metemos la ventana en el workSpace.
        meteWindow ( windowTitle(), this, FALSE );
        dialogChanges_cargaInicial();
        /// Disparamos los plugins.
        res = g_plugins->lanza ( "ClienteView_ClienteView_Post", this );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el cliente" ) );
    } // end try
    _depura ( "END ClienteView::ClienteView", 0 );
}


/** No precisa de acciones adicionales.
*/
/**
**/
ClienteView::~ClienteView()
{
    _depura ( "ClienteView::~ClienteView", 0 );
    /// Disparamos los plugins.
    g_plugins->lanza ( "ClienteView_Des_ClienteView", this );
    empresaBase() ->sacaWindow ( this );
    _depura ( "END ClienteView::~ClienteView", 0 );
}


/**
* cargar
*
* Given a valid client ID this function loads the client into the
* form so that we can edit it.
*
* Otherwise it empties the form and sets it so that we can add
* a new client
*
* Si la funcion falla imprime un mensaje de error y devuelve -1
* Si todo va bien devuelve 0
**/
/**
\param idcliente
\return
**/
int ClienteView::cargarPost ( QString idcliente )
{
    _depura ( "ClienteView::cargarPost", 0 );


    /// Hacemos que el listado de presupuestos de un cliente se inicialize.
    m_listpresupuestos->setidcliente ( idcliente );
    m_listpresupuestos->presentar();
    m_listpedidos->setidcliente ( idcliente );
    m_listpedidos->presentar();
    m_listalbaranes->setidcliente ( idcliente );
    m_listalbaranes->presentar();
    m_listfacturas->setidcliente ( idcliente );
    m_listfacturas->presentar();
    m_listcobros->setidcliente ( idcliente );
    m_listcobros->presentar();


    _depura ( "END ClienteView::cargarPost", 0 );
    return 0;
}


/** SLOT que responde a la pulsacion del boton informe de cliente.
    Crea una instancia de InformeCliente, la inicializa y la lanza.
*/
/**
**/
void ClienteView::on_mui_informe_clicked()
{
    _depura ( "ClienteView::on_mui_informe_clicked", 0 );
    InformeCliente inf ( empresaBase() );
    inf.setCliente ( DBvalue ( "idcliente" ) );
    inf.generarInforme();
    _depura ( "END ClienteView::on_mui_informe_clicked", 0 );
}

