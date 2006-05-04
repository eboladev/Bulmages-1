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

/*
-- client table
---------------+-------------------------+----------------------------------------------------------------
 idcliente     | integer                 | not null default nextval('public.cliente_idcliente_seq'::text)
 nomcliente    | character varying(100)  |
 nomaltcliente | character varying(300)  |
 cifcliente    | character varying(200)  |
 bancocliente  | character varying(35)   |
 dircliente    | character varying(100)  |
 poblcliente   | character varying(40)   |
 cpcliente     | character varying(10)   |
 telcliente    | character varying(20)   |
 faxcliente    | character varying(20)   |
 mailcliente   | character varying(100)  |
 urlcliente    | character varying(150)  |
 faltacliente  | date                    | default now()
 fbajacliente  | date                    |
 comentcliente | character varying(2000) |
 idrecargo     | integer                 | not null
�dices:
    "cliente_pkey" llave primaria, btree (idcliente)
Restricciones de llave for�ea:
    "$1" FOREIGN KEY (idrecargo) REFERENCES recargo(idrecargo)
 
*/


#include <QLineEdit>
#include <QMessageBox>
#include <Q3Table>
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

ClienteView::ClienteView(company *comp, QWidget *parent, const char *name)
        :  QWidget(parent, name, Qt::WDestructiveClose) , Cliente(comp), dialogChanges(this) {
    _depura("ClienteView::ClienteView",0);
    setupUi(this);
    /// Disparamos los plugins
    int res = g_plugins->lanza("ClienteView_ClienteView", this);
    if (res != 0)
        return;

    m_provcliente->setcompany(m_companyact);
    m_provcliente->setProvincia("");

    /// Desabilitamos los tabs que aun no se usan
    mui_tab->setTabEnabled(6,FALSE);
    mui_tab->setTabEnabled(7,FALSE);
    mui_tab->setTabEnabled(8,FALSE);


    /// Inicializamos las pantallas auxiliares a esta
    m_listpresupuestos->setcompany(m_companyact);
    m_listpedidos->setcompany(m_companyact);
    m_listalbaranes->setcompany(m_companyact);
    m_listfacturas->setcompany(m_companyact);
    m_listcobros->setcompany(m_companyact);

    m_companyact->meteWindow(caption(),this);
    dialogChanges_cargaInicial();
    /// Disparamos los plugins
    res = g_plugins->lanza("ClienteView_ClienteView_Post", this);
    _depura("ClienteView::ClienteView",0);

}

ClienteView::~ClienteView() {
    _depura("ClienteView::~ClienteView\n",0);
    m_companyact->sacaWindow(this);
    /// Disparamos los plugins
    g_plugins->lanza("ClienteView_Des_ClienteView", this);
    _depura("END ClienteView::~ClienteView\n",0);
}

/**
* loadClient
*
* Given a valid client ID this function loads the client into the
* form so that we can edit it.
* 
* Otherwise it empties the form and sets it so that we can add
* a new client
*/
int ClienteView::cargar(QString idcliente) {
    _depura("ClienteView::cargar",0);
    int error=0;

    Cliente::cargar(idcliente);
    setCaption("Cliente  "+DBvalue("nomcliente"));
    if(m_companyact->meteWindow(caption(),this))
        return -1;

    /// Hacemos que el listado de presupuestos de un cliente se inicialize.
    m_listpresupuestos->setidcliente(idcliente);
    m_listpresupuestos->presenta();
    m_listpedidos->setidcliente(idcliente);
    m_listpedidos->presenta();
    m_listalbaranes->setidcliente(idcliente);
    m_listalbaranes->presenta();
    m_listfacturas->setidcliente(idcliente);
    m_listfacturas->presenta();
    m_listcobros->setidcliente(idcliente);
    m_listcobros->presentar();

    pintaCliente();
    dialogChanges_cargaInicial();
    _depura("ClienteView::cargar",0);
    return error;
}



/**
* Empties the form.
*
*/
void ClienteView::emptyForm() {

    m_provcliente->setCurrentItem(0);
    dialogChanges_cargaInicial();
}


/**
* saveClient
*
* This function saves the current client. It checks
* if it is a new client that needs to be added or if
* it is an existing one that has to be modified
*/
void ClienteView::saveClient() {
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto
    int res = g_plugins->lanza("ClienteView_saveClient", this);
    if (res != 0)
        return;
    setDBvalue("nomcliente",m_nomcliente->text());
    setDBvalue("nomaltcliente",m_nomaltcliente->text());
    setDBvalue("cifcliente", m_cifcliente->text());
    setDBvalue("bancocliente", m_bancocliente->text());
    setDBvalue("dircliente", m_dircliente->text());
    setDBvalue("poblcliente", m_poblcliente->text());
    setDBvalue("cpcliente", m_cpcliente->text());
    setDBvalue("telcliente", m_telcliente->text());
    setDBvalue("faxcliente", m_faxcliente->text());
    setDBvalue("mailcliente", m_mailcliente->text());
    setDBvalue("urlcliente", m_urlcliente->text());
    setDBvalue("comentcliente", m_comentcliente->text());
    setDBvalue("provcliente", m_provcliente->currentText());

    guardaCliente();
    dialogChanges_cargaInicial();
}


/**
* For now this function cleans the form and sets it so that we can add
* a new client
*
* In the future it should really delete the client, or better yet
* mark it as deleted on an appropiate field in the DB
*/
void ClienteView::deleteClient() {
    borraCliente();
    emptyForm();
}


void ClienteView::on_mui_guardar_clicked() {
    _depura("ClienteView::on_mui_guardar_clicked",0);
    saveClient();
}

void ClienteView::on_mui_borrar_clicked() {
    int ret = QMessageBox::warning(this,tr("Edicion de clientes"),
                                   tr("Esta a punto de borrar un cliente.\n"
                                      "Esta seguro que desea borrarlo?"),
                                   QMessageBox::Yes | QMessageBox::Default,
                                   QMessageBox::No,
                                   QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes)
        deleteClient();
}

void ClienteView::closeEvent( QCloseEvent *e) {
    _depura("closeEvent",0);
    if (dialogChanges_hayCambios())  {
        int val = QMessageBox::warning( this, "Guardar Cliente",
                                        "Desea guardar los cambios.","Si","No","Cancelar",0,2);
        if (val == 0)
            saveClient();
        if (val == 2)
            e->ignore();
    }// end if
}
