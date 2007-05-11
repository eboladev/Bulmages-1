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
ClientsList::ClientsList(company *comp, QWidget *parent, Qt::WFlags flag, edmode editmode)
        : FichaBf(comp, parent, flag), pgimportfiles(comp) {
    _depura("ClientsList::ClientsList", 0);
    setupUi(this);

    /// Disparamos los plugins.
    int res = g_plugins->lanza("ClientsList_ClientsList", this);
    if (res != 0)
        return;

    mui_list->setEmpresaBase(comp);
    mdb_idcliente = "";
    mdb_cifcliente = "";
    mdb_nomcliente = "";
    m_modo = editmode;
    hideBusqueda();
    /// Si estamos en el modo edici&oacute;n metemos la ventana en el workSpace.
    if (m_modo == EditMode) {
        empresaBase()->meteWindow(windowTitle(), this);
    } else {
        setWindowTitle(tr("Selector de clientes"));
        mui_editar->setHidden(TRUE);
        mui_crear->setHidden(TRUE);
        mui_borrar->setHidden(TRUE);
        mui_exportar->setHidden(TRUE);
        mui_importar->setHidden(TRUE);
        mui_imprimir->setHidden(TRUE);
    } // end if
    presenta();
    _depura("END ClientsList::ClientsList", 0);
}


/** No requiere acciones especiales en el destructor de la clase.
*/
ClientsList::~ClientsList() {
    _depura("ClientsList::~ClientsList", 0);
    empresaBase()->sacaWindow(this);
    _depura("END ClientsList::~ClientsList", 0);
}


/** Todo el listado de clientes lo presenta el Subformulario mui_list del tipo ListLinCliente.
*/
/// \TODO: Mejorar el sistema de filtrado incluyendo una funcion de generar Filtro.
void ClientsList::presenta() {
    _depura("ClientsList::presenta", 0);
    mui_list->cargar("SELECT * FROM cliente  WHERE nomcliente LIKE '%" + m_findClient->text() + "%' ORDER BY nomcliente");
    _depura("END ClientsList::presenta", 0);
}


/** LA accion de editar tiene una doble vertiente dependiendo del modo en que se ha abierto la ventana.
    Modo Edicion:
	- Crea una instancia de ClienteView, lo inicializa y lo presenta.
    Modo Seleccion:
	- Establece cual es el elemento seleccionado y se cierra la ventana para devolver el control.
*/
void ClientsList::editar(int row) {
    _depura("ClientsList::editar", 0);
    mdb_idcliente = mui_list->DBvalue("idcliente", row);
    mdb_cifcliente = mui_list->DBvalue("cifcliente", row);
    mdb_nomcliente = mui_list->DBvalue("nomcliente", row);
    if (m_modo == 0) {
        ClienteView *prov = empresaBase()->newClienteView();
        if (prov->cargar(mdb_idcliente)) {
            delete prov;
            return;
        } // end if
        empresaBase()->m_pWorkspace->addWindow(prov);
        prov->show();
    } else {
        emit(selected(mdb_idcliente));
    } // end if
    _depura("END ClientsList::editar", 0);
}


/** SLOT que responde a la pulsacion del boton editar.
    Comprueba que existe un elemento seleccionado y llama al metodo editar().
*/
void ClientsList::on_mui_editar_clicked() {
    _depura("ClientsList::on_mui_editar_clicked", 0);
    if (mui_list->currentRow() < 0) {
        _depura("Debe seleccionar un elemento", 2);
        return;
    } // end if
    editar(mui_list->currentRow());
    _depura("END ClientsList::on_mui_editar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton imprimir.
    La impresion de listados esta completamente delegada a SubForm3
*/
void ClientsList::on_mui_imprimir_clicked() {
    _depura("ClientsList::on_mui_imprimir_clicked", 0);
    mui_list->imprimirPDF(tr("Listado de Clientes"));
    _depura("ClientsList::on_mui_imprimir_clicked", 0);
}




/** SLOT que responde a la pulsacion del boton borrar.
    Instancia la clase ClienteView, lo inicializa con el cliente seleccionado y le lanza el evento de borrar.
    Esta es la forma adecuada de borrar desde el listado ya que asi preservamos el tema plugins.
*/
void ClientsList::on_mui_borrar_clicked() {
    _depura("ClientsList::on_mui_borrar_clicked", 0);
    try {
        QString idcliente = mui_list->DBvalue("idcliente");
        ClienteView *cli = empresaBase()->newClienteView();
        if (cli->cargar(idcliente)) {
            delete cli;
            throw -1;
        } // end if
        cli->on_mui_borrar_clicked();
        delete cli;
        presenta();
    } catch (...) {
        mensajeInfo(tr("Error al borrar un cliente"));
    } // end try
    _depura("END:ClientsList::on_mui_borrar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton exportar.
    Saca un dialog selector de archivo para que indiquemos a que archivo exportar.
    Llama a bulmafact2XML para hacer la exportacion.
*/
void ClientsList::on_mui_exportar_clicked() {
    _depura("ClientsList::on_mui_exportar_clicked", 0);
    QFile filexml(QFileDialog::getSaveFileName(
                      this,
                      tr("Elija el archivo"),
                      confpr->valor(CONF_DIR_USER),
                      tr("Clientes (*.xml)")));

    if (filexml.open(QIODevice::WriteOnly)) {
        bulmafact2XML(filexml, IMPORT_CLIENTES);
        filexml.close();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
    _depura("END ClientsList::on_mui_exportar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton importar.
    Saca un selector de archivos para indicar desde que archivo importar.
    Llama a XML2BulmaFact para hacer la importacion.
    Refresca el listado.
*/
void ClientsList::on_mui_importar_clicked() {
    _depura("ClientsList::on_mui_importar_clicked", 0);
    QFile filexml(QFileDialog::getOpenFileName(
                      this,
                      tr("Elija el archivo"),
                      confpr->valor(CONF_DIR_USER),
                      tr("Clientes (*.xml)")));

    if (filexml.open(QIODevice::ReadOnly)) {
        XML2BulmaFact(filexml, IMPORT_CLIENTES);
        filexml.close();
        presenta();
    } else {
        _depura("ERROR AL ABRIR EL ARCHIVO\n", 2);
    } // end if
    _depura("ClientsList::on_mui_importar_clicked", 0);
}

/** Establece el modo de funcionamiento como selector para esta ventana
**/
void ClientsList::selectMode() {
    m_modo = SelectMode;
}

/** Establece el modo de funcionamiento como selector para edicion para esta ventana
**/
void ClientsList::editMode() {
    m_modo = EditMode;
}

/** Devuelve el identificador del cliente seleccionado
**/
QString ClientsList::idclient() {
    return mdb_idcliente;
}

/** Devuelve el nombre del cliente seleccionado
**/
QString ClientsList::nomclient() {
    return mdb_nomcliente;
}

/** Devuelve el cif del cliente seleccionado
**/
QString ClientsList::cifclient() {
    return mdb_cifcliente;
}
/** Oculta la botonera
*/
void ClientsList::hideBotonera() {
    m_botonera->hide();
}

/** Muestra la botonera
**/
void ClientsList::showBotonera() {
    m_botonera->show();
}

/** Oculta el layer de busqueda
**/
void ClientsList::hideBusqueda() {
    m_busqueda->hide();
}

/** Muestra el layer de busqueda
**/
void ClientsList::showBusqueda() {
    m_busqueda->show();
}

/** SLOT automatico que se ejecuta al cambiar el texto del QLineEdit de filtrado general
**/
void ClientsList::on_m_filtro_textChanged(const QString &text) {
    if (text.size() >= 3) {
        on_mui_actualizar_clicked();
    } // end if
}

/** SLOT automatico que se ejecuta al hacer doble click sobre un elemento determinado de la lista
**/
void ClientsList::on_mui_list_itemDoubleClicked(QTableWidgetItem *) {
    on_mui_editar_clicked();
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de crear en la botonera
**/
void ClientsList::on_mui_crear_clicked() {
    empresaBase()->s_newClienteView();
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton de actualizar en la botonera
**/
void ClientsList::on_mui_actualizar_clicked() {
    presenta();
}

/** SLOT automatico que se ejecuta al pulsar sobre el boton configurar en la botonera
**/
void ClientsList::on_mui_configurar_toggled(bool checked) {
    if (checked) {
        mui_list->showConfig();
    } else {
        mui_list->hideConfig();
    } // end if
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** PRepara el subformulario para trabajar con la tabla cliente.
    Establece que no se puedan insertar ni borrar lineas.
*/
ClienteListSubform::ClienteListSubform(QWidget *parent, const char *) : SubForm2Bf(parent) {
    _depura("ClienteListSubform::ClienteListSubform", 0);
    setDBTableName("cliente");
    setDBCampoId("idcliente");
    addSHeader("idcliente", DBCampo::DBint, DBCampo::DBNotNull | DBCampo::DBPrimaryKey, SHeader::DBNoView | SHeader::DBNoWrite, tr("ID cliente"));
    addSHeader("codcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo"));
    addSHeader("cifcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("C.I.F."));
    addSHeader("nomcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre de cliente"));
    addSHeader("nomaltcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Nombre alternativo del cliente"));
    addSHeader("bancocliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero cuenta corriente"));
    addSHeader("dircliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion"));
    addSHeader("poblcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Poblacion"));
    addSHeader("cpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Codigo postal"));
    addSHeader("telcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de telefono"));
    addSHeader("faxcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Numero de fax"));
    addSHeader("mailcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion de correo electronica"));
    addSHeader("urlcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Direccion URL"));
    addSHeader("corpcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Empresa"));
    addSHeader("faltacliente", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de alta del cliente"));
    addSHeader("fbajacliente", DBCampo::DBdate, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Fecha de baja del cliente"));
    addSHeader("comentcliente", DBCampo::DBvarchar, DBCampo::DBNoSave, SHeader::DBNone | SHeader::DBNoWrite, tr("Comentarios"));
    setinsercion(FALSE);
    setDelete(FALSE);
    setSortingEnabled(TRUE);
    _depura("END ClienteListSubform::ClienteListSubform", 0);
}

ClienteListSubform::~ClienteListSubform() {
    _depura("ClienteListSubform::~ClienteListSubform", 0);
    _depura("END ClienteListSubform::~ClienteListSubform", 0);
}
