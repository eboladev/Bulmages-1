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
ClienteView::ClienteView(company *comp, QWidget *parent)
        : Cliente(comp, parent) {
    _depura("ClienteView::ClienteView", 0);
    setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    try {
        /// Disparamos los plugins.
        int res = g_plugins->lanza("ClienteView_ClienteView", this);
        if (res != 0) {
            return;
        } // end if
        m_provcliente->setcompany(empresaBase());
        m_provcliente->setProvincia("");

        /// Inicializamos las pantallas auxiliares a esta.
        m_listpresupuestos->setEmpresaBase(empresaBase());
        m_listpedidos->setEmpresaBase(empresaBase());
        m_listalbaranes->setEmpresaBase(empresaBase());
        m_listfacturas->setEmpresaBase(empresaBase());
        m_listcobros->setEmpresaBase(empresaBase());
        mui_forma_pago->setEmpresaBase(empresaBase());
        mui_forma_pago->setidforma_pago("");

        /// Metemos la ventana en el workSpace.
        meteWindow(windowTitle(), this, FALSE);
        dialogChanges_cargaInicial();
        /// Disparamos los plugins.
        res = g_plugins->lanza("ClienteView_ClienteView_Post", this);
    } catch (...) {
        mensajeInfo(tr("Error al crear el cliente"));
    } // end try
    _depura("END ClienteView::ClienteView", 0);
}


/** No precisa de acciones adicionales.
*/
ClienteView::~ClienteView() {
    _depura("ClienteView::~ClienteView", 0);
    /// Disparamos los plugins.
    g_plugins->lanza("ClienteView_Des_ClienteView", this);
    empresaBase()->sacaWindow(this);
    _depura("END ClienteView::~ClienteView", 0);
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
int ClienteView::cargar(QString idcliente) {
    _depura("ClienteView::cargar", 0);
    try {
        Cliente::cargar(idcliente);
        setWindowTitle(tr("Cliente") + " " + DBvalue("nomcliente"));
        meteWindow(windowTitle(), this);

        /// Disparamos los plugins.
        int res = g_plugins->lanza("ClienteView_cargar", this);
        if (res != 0) {
            return res;
        } // end if

        /// Hacemos que el listado de presupuestos de un cliente se inicialize.
        m_listpresupuestos->setidcliente(idcliente);
        m_listpresupuestos->presentar();
        m_listpedidos->setidcliente(idcliente);
        m_listpedidos->presentar();
        m_listalbaranes->setidcliente(idcliente);
        m_listalbaranes->presentar();
        m_listfacturas->setidcliente(idcliente);
        m_listfacturas->presentar();
        m_listcobros->setidcliente(idcliente);
        m_listcobros->presentar();

        /// Pintamos
        pintaCliente();
        /// Reseteamos el control de cambios.
        dialogChanges_cargaInicial();
    } catch (...) {
        mensajeInfo(tr("Error al cargar el cliente"));
        return -1;
    } // end try
    g_plugins->lanza("ClienteView_cargar_post", this);
    _depura("ClienteView::cargar", 0);
    return 0;
}


/// Empties the form.
/** Vacia el formulario */
/// \TODO: Investigar la necesidad de este metodo.
void ClienteView::emptyForm() {
    _depura("ClienteView::emptyForm", 0);
    m_provcliente->setCurrentIndex(0);
    dialogChanges_cargaInicial();
    _depura("END ClienteView::emptyForm", 0);
}


/**
* guardar
*
* This function saves the current client. It checks
* if it is a new client that needs to be added or if
* it is an existing one that has to be modified
**/
/// \TODO: Deberia incorporar un bloque de excepciones try{} catch{}
int ClienteView::guardar() {
    _depura("ClienteView::guardar", 0);
    /// Disparamos los plugins
    int res = g_plugins->lanza("ClienteView_saveClient", this);
    if (res != 0) {
        return 0;
    } // end if
    setDBvalue("nomcliente", m_nomcliente->text());
    setDBvalue("nomaltcliente", m_nomaltcliente->text());
    setDBvalue("cifcliente", m_cifcliente->text());
    setDBvalue("bancocliente", m_bancocliente->text());
    setDBvalue("dircliente", m_dircliente->text());
    setDBvalue("poblcliente", m_poblcliente->text());
    setDBvalue("cpcliente", m_cpcliente->text());
    setDBvalue("telcliente", m_telcliente->text());
    setDBvalue("teltrabcliente", mui_teltrabcliente->text());
    setDBvalue("movilcliente", mui_movilcliente->text());
    setDBvalue("faxcliente", m_faxcliente->text());
    setDBvalue("mailcliente", m_mailcliente->text());
    setDBvalue("urlcliente", m_urlcliente->text());
    setDBvalue("comentcliente", m_comentcliente->toPlainText());
    setDBvalue("provcliente", m_provcliente->currentText());
    setDBvalue("codcliente", mui_codcliente->text());
    setDBvalue("corpcliente", mui_corpcliente->text());
    setDBvalue("idforma_pago", mui_forma_pago->idforma_pago());
    setDBvalue("recargoeqcliente",  mui_recargoeqcliente->isChecked() ? "TRUE" : "FALSE");
    setDBvalue("regimenfiscalcliente", mui_regimenfiscalcliente->currentText());
    int err = Cliente::guardar();
    if (!err) {
        dialogChanges_cargaInicial();
    } // end if
    _depura("ClienteView::guardar", 0);
    return err;
}


/**
* For now this function cleans the form and sets it so that we can add
* a new client
*
* In the future it should really delete the client, or better yet
* mark it as deleted on an appropiate field in the DB
**/
/// \TODO: Investigar la necesidad de este metodo.
void ClienteView::deleteClient() {
    _depura("ClienteView::deleteClient", 0);
    borraCliente();
    emptyForm();
    _depura("END ClienteView::deleteClient", 0);
}

/** SLOT que responde a la pulsacion del boton guardar.
    Invoca al metodo guardar().
*/
/// \TODO: Este metodo debe estar implementado en la clase Ficha.
void ClienteView::on_mui_guardar_clicked() {
    _depura("ClienteView::on_mui_guardar_clicked", 0);
    guardar();
    _depura("END ClienteView::on_mui_guardar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton borrar.
    Invoca al metodo deleteClient().
*/
/// \TODO: deleteClient deberia ser borrar(). y este metodo deberia estar en Ficha.
void ClienteView::on_mui_borrar_clicked() {
    _depura("ClienteView::on_mui_borrar_clicked", 0);
    int ret = QMessageBox::warning(this,
                                   tr("Edicion de clientes"),
                                   tr("Esta a punto de borrar un cliente.\n"
                                      "Esta seguro que desea borrarlo?"),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No,
                                   QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
        deleteClient();
    _depura("END ClienteView::on_mui_borrar_clicked", 0);
}


/** SLOT que responde a la pulsacion del boton informe de cliente.
    Crea una instancia de InformeCliente, la inicializa y la lanza.
*/
void ClienteView::on_mui_informe_clicked() {
    _depura("ClienteView::on_mui_informe_clicked", 0);
    InformeCliente inf(empresaBase());
    inf.setCliente(DBvalue("idcliente"));
    inf.generarInforme();
    _depura("END ClienteView::on_mui_informe_clicked", 0);
}


void ClienteView::pintanomcliente(QString val) {
    m_nomcliente->setText(val);
}


void ClienteView::pintanomaltcliente(QString val) {
    m_nomaltcliente->setText(val);
}


void ClienteView::pintacifcliente(QString val) {
    m_cifcliente->setText(val);
}


void ClienteView::pintacodcliente(QString val) {
    mui_codcliente->setText(val);
}


void ClienteView::pintacorpcliente(QString val) {
    mui_corpcliente->setText(val);
}


void ClienteView::pintabancocliente(QString val) {
    m_bancocliente->setText(val);
}


void ClienteView::pintadircliente(QString val) {
    m_dircliente->setText(val);
}


void ClienteView::pintapoblcliente(QString val) {
    m_poblcliente->setText(val);
}


void ClienteView::pintacpcliente(QString val) {
    m_cpcliente->setText(val);
}


void ClienteView::pintatelcliente(QString val) {
    m_telcliente->setText(val);
}


void ClienteView::pintateltrabcliente(QString val) {
    mui_teltrabcliente->setText(val);
}


void ClienteView::pintamovilcliente(QString val) {
    mui_movilcliente->setText(val);
}


void ClienteView::pintafaxcliente(QString val) {
    m_faxcliente->setText(val);
}


void ClienteView::pintamailcliente(QString val) {
    m_mailcliente->setText(val);
}


void ClienteView::pintaurlcliente(QString val) {
    m_urlcliente->setText(val);
}


void ClienteView::pintacomentcliente(QString val) {
    m_comentcliente->setPlainText(val);
}


void ClienteView::pintaprovcliente(QString val) {
    m_provcliente->setProvincia(val);
}


void ClienteView::pintaregimenfiscalcliente(QString val) {
    mui_regimenfiscalcliente->setRegimenFiscal(val);
}


void ClienteView::pintarecargoeqcliente(QString val) {
    /// Pintamos el recargo de equivalencia
    if (val == "t") {
        mui_recargoeqcliente->setChecked(TRUE);
    } else {
        mui_recargoeqcliente->setChecked(FALSE);
    } // end if
}


void ClienteView::pintaidforma_pago(QString val) {
    mui_forma_pago->setidforma_pago(val);
}

void ClienteView::on_mui_aceptar_clicked() {
    if (!guardar()) {
        close();
    } // end if
}


void ClienteView::pintaidcliente(QString) {
    _depura("ClienteView::pintaidcliente", 0);
}


void ClienteView::pintafaltacliente(QString) {
    _depura("ClienteView::pintafaltacliente", 0);
}


void ClienteView::pintafbajacliente(QString) {
    _depura("ClienteView::pintafbajacliente", 0);
}


void ClienteView::pintainactivocliente(QString) {
}

