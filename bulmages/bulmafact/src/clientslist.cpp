/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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
#include <QTextStream>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>
#include <QMenu>

#include "clientslist.h"
#include "clienteview.h"
#include "company.h"
#include "funcaux.h"
#include "pgimportfiles.h"
#include "informereferencia.h"
#include "plugins.h"


/** Inicializa todos los componentes del listado.
    Hace una presentacion inicial.
    Mete la ventana en el workSpace si estamos en modo edicion.
*/
ClientsList::ClientsList ( Company *comp, QWidget *parent, Qt::WFlags flag, edmode editmode )
        : Listado ( comp, parent, flag, editmode ), pgimportfiles ( comp )
{
    _depura ( "ClientsList::ClientsList", 0 );
    setupUi ( this );

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "ClientsList_ClientsList", this );
    if ( res != 0 )
        return;

    mui_list->setEmpresaBase ( comp );
    setSubForm ( mui_list );
    mdb_idcliente = "";
    mdb_cifcliente = "";
    mdb_nomcliente = "";
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if ( modoEdicion() ) {
        empresaBase() ->meteWindow ( windowTitle(), this );
    } else {
        setWindowTitle ( tr ( "Selector de clientes" ) );
        mui_editar->setHidden ( TRUE );
        mui_crear->setHidden ( TRUE );
        mui_borrar->setHidden ( TRUE );
        mui_exportar->setHidden ( TRUE );
        mui_importar->setHidden ( TRUE );
        mui_imprimir->setHidden ( TRUE );
    } // end if
    presentar();
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "cliente" );
    _depura ( "END ClientsList::ClientsList", 0 );
}


/** No requiere acciones especiales en el destructor de la clase.
*/
ClientsList::~ClientsList()
{
    _depura ( "ClientsList::~ClientsList", 0 );
    _depura ( "END ClientsList::~ClientsList", 0 );
}


/** Todo el listado de clientes lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
void ClientsList::presentar()
{
    _depura ( "ClientsList::presenta", 0 );
    mui_list->cargar ( "SELECT * FROM cliente  WHERE lower(nomcliente) LIKE lower('%" + m_filtro->text() + "%') ORDER BY nomcliente" );
    _depura ( "END ClientsList::presenta", 0 );
}


/** LA accion de editar tiene una doble vertiente dependiendo del modo en que se ha abierto la ventana.
    Modo Edicion:
 - Crea una instancia de ClienteView, lo inicializa y lo presenta.
    Modo Seleccion:
 - Establece cual es el elemento seleccionado y se cierra la ventana para devolver el control.
*/
void ClientsList::editar ( int row )
{
    _depura ( "ClientsList::editar", 0 );
    mdb_idcliente = mui_list->DBvalue ( "idcliente", row );
    mdb_cifcliente = mui_list->DBvalue ( "cifcliente", row );
    mdb_nomcliente = mui_list->DBvalue ( "nomcliente", row );
    if ( modoEdicion() ) {
        ClienteView * prov = ( ( Company * ) empresaBase() ) ->newClienteView();
        if ( prov->cargar ( mdb_idcliente ) ) {
            delete prov;
            return;
        } // end if
        empresaBase() ->m_pWorkspace->addWindow ( prov );
        prov->show();
    } else {
        emit ( selected ( mdb_idcliente ) );
    } // end if
    _depura ( "END ClientsList::editar", 0 );
}


/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a SubForm3
*/
void ClientsList::imprimir()
{
    _depura ( "ClientsList::on_mui_imprimir_clicked", 0 );
    mui_list->imprimirPDF ( tr ( "Listado de Clientes" ) );
    _depura ( "ClientsList::on_mui_imprimir_clicked", 0 );
}



/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el cliente seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el listado ya que asi preservamos el tema plugins.
*/
void ClientsList::borrar()
{
    _depura ( "ClientsList::on_mui_borrar_clicked", 0 );
    try {
        QString idcliente = mui_list->DBvalue ( "idcliente" );
        ClienteView *cli = ( ( Company * ) empresaBase() ) ->newClienteView();
        if ( cli->cargar ( idcliente ) ) {
            delete cli;
            throw - 1;
        } // end if
        cli->on_mui_borrar_clicked();
        delete cli;
        presentar();
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al borrar un cliente" ), this );
    } // end try
    _depura ( "END:ClientsList::on_mui_borrar_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton exportar.
    Saca un dialog selector de archivo para que indiquemos a que archivo exportar.
    Llama a bulmafact2XML para hacer la exportacion.
*/
void ClientsList::on_mui_exportar_clicked()
{
    _depura ( "ClientsList::on_mui_exportar_clicked", 0 );
    QFile filexml ( QFileDialog::getSaveFileName (
                        this,
                        tr ( "Elija el archivo" ),
                        confpr->valor ( CONF_DIR_USER ),
                        tr ( "Clientes (*.xml)" ) ) );
    if ( filexml.open ( QIODevice::WriteOnly ) ) {
        bulmafact2XML ( filexml, IMPORT_CLIENTES );
        filexml.close();
    } else {
        _depura ( "ERROR AL ABRIR EL ARCHIVO", 2 );
    } // end if
    _depura ( "END ClientsList::on_mui_exportar_clicked", 0 );
}


/** SLOT que responde a la pulsacion del boton importar.
    Saca un selector de archivos para indicar desde que archivo importar.
    Llama a XML2BulmaFact para hacer la importacion.
    Refresca el listado.
*/
void ClientsList::on_mui_importar_clicked()
{
    _depura ( "ClientsList::on_mui_importar_clicked", 0 );
    QFile filexml ( QFileDialog::getOpenFileName (
                        this,
                        tr ( "Elija el archivo" ),
                        confpr->valor ( CONF_DIR_USER ),
                        tr ( "Clientes (*.xml)" ) ) );

    if ( filexml.open ( QIODevice::ReadOnly ) ) {
        XML2BulmaFact ( filexml, IMPORT_CLIENTES );
        filexml.close();
        presentar();
    } else {
        _depura ( "ERROR AL ABRIR EL ARCHIVO\n", 2 );
    } // end if
    _depura ( "ClientsList::on_mui_importar_clicked", 0 );
}

/** Devuelve el identificador del cliente seleccionado
**/
QString ClientsList::idclient()
{
    return mdb_idcliente;
}

/** Devuelve el nombre del cliente seleccionado
**/
QString ClientsList::nomclient()
{
    return mdb_nomcliente;
}

/** Devuelve el cif del cliente seleccionado
**/
QString ClientsList::cifclient()
{
    return mdb_cifcliente;
}


/** SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
**/
void ClientsList::crear()
{
    ( ( Company * ) empresaBase() ) ->s_newClienteView();
}


/** \TODO: REVISAR ESTE METODO YA QUE NO PARECE SER EL ADECUADO
    EN LA LLAMADA DE SUBMENUS
*/
void ClientsList::submenu ( const QPoint & )
{
    _depura ( "ArticuloList::on_mui_list_customContextMenuRequested", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 )
        return;
    QMenu *popup = new QMenu ( this );
    QAction *edit = popup->addAction ( tr ( "Editar cliente" ) );
    QAction *del = popup->addAction ( tr ( "Borrar cliente" ) );
    QAction *opcion = popup->exec ( QCursor::pos() );
    if ( opcion == del )
        on_mui_borrar_clicked();
    if ( opcion == edit )
        on_mui_editar_clicked();
    delete popup;
}

/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** PRepara el subformulario para trabajar con la tabla cliente.
    Establece que no se puedan insertar ni borrar lineas.
*/
ClienteListSubform::ClienteListSubform ( QWidget *parent, const char * ) : SubForm2Bf ( parent )
{
    _depura ( "ClienteListSubform::ClienteListSubform", 0 );
    setDBTableName ( "cliente" );
    setDBCampoId ( "idcliente" );
    addSHeader ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr ( "ID cliente" ) );
    addSHeader ( "codcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Codigo" ) );
    addSHeader ( "cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "C.I.F." ) );
    addSHeader ( "nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Nombre de cliente" ) );
    addSHeader ( "nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Nombre alternativo del cliente" ) );
    addSHeader ( "bancocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Numero cuenta corriente" ) );
    addSHeader ( "dircliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Direccion" ) );
    addSHeader ( "poblcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Poblacion" ) );
    addSHeader ( "cpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Codigo postal" ) );
    addSHeader ( "telcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Numero de telefono" ) );
    addSHeader ( "faxcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Numero de fax" ) );
    addSHeader ( "mailcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Direccion de correo electronica" ) );
    addSHeader ( "urlcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Direccion URL" ) );
    addSHeader ( "corpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Empresa" ) );
    addSHeader ( "faltacliente", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha de alta del cliente" ) );
    addSHeader ( "fbajacliente", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Fecha de baja del cliente" ) );
    addSHeader ( "comentcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr ( "Comentarios" ) );
    setinsercion ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    _depura ( "END ClienteListSubform::ClienteListSubform", 0 );
}

ClienteListSubform::~ClienteListSubform()
{
    _depura ( "ClienteListSubform::~ClienteListSubform", 0 );
    _depura ( "END ClienteListSubform::~ClienteListSubform", 0 );
}
