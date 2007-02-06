/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#include <Qt>
#include <QObject>
#include <qnamespace.h>

#include "stdio.h"

#include "abreempresaview.h"
#include "albaranclientelist.h"
#include "albaranclienteview.h"
#include "albaranesproveedor.h"
#include "albaranproveedorview.h"
#include "articulolist.h"
#include "articuloview.h"
#include "clienteview.h"
#include "clientslist.h"
#include "cobroslist.h"
#include "cobroview.h"
#include "company.h"
#include "facturapview.h"
#include "facturaslist.h"
#include "facturasplist.h"
#include "facturaview.h"
#include "familiasview.h"
#include "fpagoview.h"
#include "funcaux.h"
#include "inventariosview.h"
#include "inventarioview.h"
#include "listalmacenview.h"
#include "listconfiguracionview.h"
#include "listprovinciasview.h"
#include "listseriefacturaview.h"
#include "pagoslist.h"
#include "pagoview.h"
#include "pedidoclienteview.h"
#include "pedidoproveedorview.h"
#include "pedidosclientelist.h"
#include "pedidosproveedorlist.h"
#include "plugins.h"
#include "presupuestolist.h"
#include "presupuestoview.h"
#include "provedit.h"
#include "providerslist.h"
#include "tiposarticuloview.h"
#include "trabajadorview.h"


/** No precisa de operaciones en su construccion.
*/
company::company() : EmpresaBase(){
    _depura("company::company", 0);
    _depura("END company::company", 0);
}


/// El destructor de la clase company borra toda la memoria almacenada.
company::~company() {
    _depura("company::~company", 0);

    /// Borramos el resto de ventanas.
    delete m_facturasproveedorlist;
    m_facturasproveedorlist = NULL;
    delete m_albaranesproveedor;
    m_albaranesproveedor = NULL;
    delete m_pedidosproveedorList;
    m_pedidosproveedorList = NULL;
    delete m_facturasList;
    m_facturasList = NULL;
    delete m_articleslist;
    m_articleslist = NULL;
    delete m_providerslist;
    m_providerslist = NULL;
    delete m_clientsList;
    m_clientsList = NULL;
    delete m_cobrosList;
    m_cobrosList = NULL;
    delete m_pagosList;
    m_pagosList = NULL;
    delete m_budgetsList;
    m_budgetsList = NULL;
    delete m_pedidosclienteList;
    m_pedidosclienteList = NULL;
    delete m_clientDelivNotesList;
    m_clientDelivNotesList = NULL;
    _depura("END company::~company", 0);
}


/** Crea todas las ventanas que aparecen creadas al inicio del programa.
    Todas estas ventanas son principalmente los listados mas utilizados a partir de los
    cuales se pueden crear nuevas fichas y manejar todo.
*/
void company::createMainWindows() {
    _depura("company::createMainWindows", 0);
    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
    /// pb = 0%
    _depura("company::createMainWindows inicializamos m_articleslist\n", 1);
    m_progressbar->setValue(0);
    m_articleslist = new ArticuloList(this);
    m_pWorkspace->addWindow(m_articleslist);
    m_articleslist->hide();

    /// pb = 8%
    _depura("company::createMainWindows inicializamos m_providerslist\n", 1);
    m_progressbar->setValue(8);
    m_providerslist = new ProveedorList(this);
    m_pWorkspace->addWindow(m_providerslist);
    m_providerslist->hide();

    /// pb = 16%
    _depura("company::createMainWindows inicializamos m_clientesList\n", 1);
    m_progressbar->setValue(16);
    m_clientsList = new ClientsList(this);
    m_pWorkspace->addWindow(m_clientsList);
    m_clientsList->hide();

    /// pb = 32%
    _depura("company::createMainWindows inicializamos m_cobrosList\n", 1);
    m_progressbar->setValue(32);
    m_cobrosList = new CobrosList(this);
    m_pWorkspace->addWindow(m_cobrosList);
    m_cobrosList->hide();

    /// pb = 40%
    _depura("company::createMainWindows inicializamos m_pagosList\n", 1);
    m_progressbar->setValue(40);
    m_pagosList = new PagosList(this);
    m_pWorkspace->addWindow(m_pagosList);
    m_pagosList->hide();

    /// pb = 48%
    _depura("company::createMainWindows inicializamos m_budgetsList\n", 1);
    m_progressbar->setValue(48);
    m_budgetsList = new PresupuestoList(this);
    m_pWorkspace->addWindow(m_budgetsList);
    m_budgetsList->hide();

    /// pb = 52%
    _depura("company::createMainWindows inicializamos m_pedidosclienteList\n", 1);
    m_progressbar->setValue(52);
    m_pedidosclienteList = new PedidosClienteList(this);
    m_pWorkspace->addWindow(m_pedidosclienteList);
    m_pedidosclienteList->hide();

    /// pb = 60%
    _depura("company::createMainWindows inicializamos m_clientDelivNotesList\n", 1);
    m_progressbar->setValue(60);
    m_clientDelivNotesList = new AlbaranClienteList(this);
    m_pWorkspace->addWindow(m_clientDelivNotesList);
    m_clientDelivNotesList->hide();

    /// pb = 68%
    _depura("company::createMainWindows inicializamos m_facturasList\n", 1);
    m_progressbar->setValue(68);
    m_facturasList = new FacturasList(this);
    m_pWorkspace->addWindow(m_facturasList);
    m_facturasList->hide();

    /// pb = 76%
    _depura("company::createMainWindows inicializamos m_pedidosproveedorlist\n", 1);
    m_progressbar->setValue(76);
    m_pedidosproveedorList = new PedidosProveedorList(this);
    m_pWorkspace->addWindow(m_pedidosproveedorList);
    m_pedidosproveedorList->hide();

    /// pb = 84%
    _depura("company::createMainWindows inicializamos m_albaranesproveedorlist\n", 1);
    m_progressbar->setValue(84);
    m_albaranesproveedor = new AlbaranesProveedor(this);
    m_pWorkspace->addWindow(m_albaranesproveedor);
    m_albaranesproveedor->hide();

    /// pb = 92%
    _depura("company::createMainWindows inicializamos m_facturasproveedorlist\n", 1);
    m_progressbar->setValue(92);
    m_facturasproveedorlist = new FacturasProveedorList(this);
    m_pWorkspace->addWindow(m_facturasproveedorlist);
    m_facturasproveedorlist->hide();

    /// pb = 100%
    m_progressbar->setValue(100);
    
    /// Disparamos los plugins.
    int res = g_plugins->lanza("company_createMainWindows_Post", this);
    if (res != 0)
    	return;	
    _depura("END company::createMainWindows\n", 0);
}


/** Metodo para ver la ventana de Cobros.
    Es invocado desde el menu de la aplicacion.
*/
void company::viewCobrosList() {
    _depura("company::viewCobrosList", 0);
    m_cobrosList->show();
    m_cobrosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_cobrosList);
    _depura("END company::viewCobrosList", 0);
}


/** Metodo para ver la ventana de PAgos.
    Es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::viewPagosList() {
    _depura("company::viewPagosList", 0);
    m_pagosList->show();
    m_pagosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pagosList);
    _depura("END company::viewPagosList", 0);
}


/** Muestra la ventana de proveedores.
    Es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::listproviders() {
    _depura("company::listproviders", 0);
    m_providerslist->show();
    m_providerslist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_providerslist);
    _depura("END company::listproviders", 0);
}


/** Muestra la ventana de listado de Clientes.
    Es invocada desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::listClients() {
    _depura("company::listClients", 0);
    m_clientsList->show();
    m_clientsList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_clientsList);
    _depura("END company::listClients", 0);
}


/** Muestra la ventana del listado de Articulos.
    Es invocada desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::listarticles() {
    _depura("company::listarticles", 0);
    m_articleslist->show();
    m_articleslist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_articleslist);
    _depura("END company::listarticles", 0);
}


/** Refresca el listado de articulos.
    Este metodo de paso de mensajes permite, desde cualquier otra clase refrescar los articulos sin
    tener una instancia de dicha clase.
*/
void company::refreshArticles() {
    _depura("company::refreshArticles", 0);
    if(m_articleslist != NULL)
        m_articleslist->presenta();
    _depura("END company::refreshArticles", 0);
}


/** Crea una instancia de la ficha de cliente.
    Tiene la ventaja de que no debe implementarse en cada sitio que se quiera instanciar la ficha de lciente.
    Y ademas hace el tratamiento adecuado de plugins.
*/
ClienteView *company::newClienteView() {
    _depura("company::newClienteView", 0);
    /// Lanzamos los plugins necesarios.
    ClienteView *bud;
    if (g_plugins->lanza("company_newClienteView", this, (void **)&bud))
        return bud;
    bud = new ClienteView(this, 0);
    _depura("END company::newClienteView", 0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de cliente.
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newClienteView()
*/
void company::s_newClienteView() {
    _depura("company::s_newClienteView", 0);
    ClienteView *bud = newClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintaCliente();
    bud->show();
    _depura("END company::s_newClienteView", 0);
}


/** Crea una instancia de la ficha de proveedor.
    Tiene la ventaja de que no debe implementarse en cada sitio que se quiera instanciar la ficha de proveedor.
    Y ademas hace el tratamiento adecuado de plugins.
*/
ProveedorView *company::newProveedorView() {
    _depura("company::newProveedorView",0);
    /// Lanzamos los plugins necesarios.
    ProveedorView *bud;
    if (g_plugins->lanza("company_newProveedorView", this, (void **)&bud))
        return bud;
    bud = new ProveedorView(this, 0);
    _depura("END company::newProveedorView",0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de proveedor.
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newClienteView()
*/
void company::s_newProveedorView() {
    _depura("company::s_newProveedorView", 0);
    ProveedorView *bud = newProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END company::s_newProveedorView", 0);
}


/** Creacion de una ventana de factura de proveedor
    Es importante que siempre que se crea una factura de proveedor
    sea mediante este metodo de comany, ya que de esta forma nos
    aseguramos de que si existe un plugin que sustituye esta ventana,
    nuestra llamada devolvera el objeto adecuado
*/
FacturaProveedorView *company::newFacturaProveedorView() {
	_depura("company::newFacturaProveedorView", 0);
    /// Lanzamos los plugins necesarios.
    FacturaProveedorView *bud;
    if (g_plugins->lanza("company_newFacturaProveedorView", this, (void **)&bud))
        return bud;
    bud = new FacturaProveedorView(this, 0);
    _depura("END company::newFacturaProveedorView", 0);    
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de Factura de Proveedor
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newFacturaProveedor()
*/
void company::s_newFacturaPro() {
    FacturaProveedorView *bud = newFacturaProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
    _depura("END company::s_newFacturaPro", 0);
}


/** Creacion de una ventana de Cobro
    Es importante que siempre que se crea un cobro
    sea mediante este metodo de comany, ya que de esta forma nos
    aseguramos de que si existe un plugin que sustituye esta ventana,
    nuestra llamada devolvera el objeto adecuado
*/
CobroView *company::newCobroView() {
    _depura("company::newCobroView", 0);
    CobroView *bud;
    if (g_plugins->lanza("company_newCobroView", this, (void **)&bud))
        return bud;
    bud = new CobroView(this, 0);
    _depura("END company::newCobroView", 0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de Cobro
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newCobroView()
*/
void company::s_newCobroView() {
    _depura("company::s_newCobroView", 0);
    CobroView *bud = newCobroView();
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END company::s_newCobroView", 0);
}


/** Crea y agrega al workSpace una instancia de la ficha de Albaran Proveedor
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
*/
/// \TODO: No pasa por el proceso de plugins. Debe cambiarse.
void company::s_newAlbaranPro() {
    _depura ("company::s_newAlbaranPro", 0);
    AlbaranProveedorView *bud = new AlbaranProveedorView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura ("END company::s_newAlbaranPro", 0);
}


/** Crea y agrega al workSpace una instancia de la ficha de Pedido Proveedor
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
*/
/// \TODO: No pasa por el proceso de plugins. Debe cambiarse.
void company::s_newPedidoPro() {
    _depura ("company::s_newPedidoPro", 0);
    PedidoProveedorView *ppv = new PedidoProveedorView(this, 0);
    m_pWorkspace->addWindow(ppv);
    ppv->pintar();
    ppv->show();
    _depura ("END company::s_newPedidoPro", 0);
}


/** Muestra como ventana activa la ventana de los albaranes de Proveedor.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::lAlbaranesProveedor() {
    _depura("company::lAlbaranesProveedor", 0);
    m_albaranesproveedor->show();
    m_albaranesproveedor->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_albaranesproveedor);
    _depura("END company::lAlbaranesProveedor", 0);
}


/** Muestra como ventana activa la ventana de listado de pedidos de cliente.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::s_listPedidosCli() {
    _depura("company::s_listPedidosCli", 0);
    m_pedidosclienteList->show();
    m_pedidosclienteList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pedidosclienteList);
    _depura("END company::s_listPedidosCli", 0);
}


/** Muestra como ventana activa la ventana de listado de pedidos a proveedor.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::s_listPedidosPro() {
    _depura("company::s_listPedidosPro", 0);
    m_pedidosproveedorList->show();
    m_pedidosproveedorList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pedidosproveedorList);
    _depura("END company::s_listPedidosPro", 0);
}


/** Muestra como ventana activa la ventana de listado de facturas a Cliente.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::s_listFacturasCli() {
    _depura("company::s_listFacturasCli", 0);
    m_facturasList->show();
    m_facturasList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_facturasList);
    _depura("END company::s_listFacturasCli", 0);
}


/** Muestra como ventana activa la ventana de listado de facturas a Proveedor.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::s_listFacturasPro() {
    _depura("company::s_listFacturasPro", 0);
    m_facturasproveedorlist->show();
    m_facturasproveedorlist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_facturasproveedorlist);
    _depura("END company::s_listFacturasPro", 0);
}


/** Muestra como ventana activa la ventana de listado de Presupuestos
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void company::listBudgets() {
    _depura("company::listBudgets", 0);
    m_budgetsList->show();
    m_budgetsList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_budgetsList);
    _depura("END company::listBudgets", 0);
}


/** Crea una instancia de la clase PresupuestoView.
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PresupuestoView *company::nuevoPresupuesto() {
    _depura("company::nuevoPresupuesto", 0);
    /// Lanzamos los plugins necesarios.
    PresupuestoView *bud;
    if (g_plugins->lanza("company_nuevoPresupuesto", this, (void **)&bud))
        return bud;
    bud = new PresupuestoView(this, 0);
    _depura("END company::nuevoPresupuesto", 0);
    return bud;
}

/** Crea y muestra la Ficha de presupuesto.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo nuevoPresupuesto().
*/
void company::s_newPresupuestoCli() {
    _depura("company::s_newPresupuestoCli", 0);
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newPresupuestoCli", this))
        return;
    PresupuestoView *bud = nuevoPresupuesto();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura("END company::s_newPresupuestoCli", 0);
}


/** Crea una instancia de la clase FacturaView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
FacturaView *company::newFacturaView() {
    _depura("company::newFacturaView", 0);
    /// Lanzamos los plugins necesarios.
    FacturaView *bud;
    if (g_plugins->lanza("company_newFacturaView", this, (void **)&bud))
        return bud;
    bud = new FacturaView(this, 0);
    _depura("END company::newFacturaView", 0);
    return bud;
}


/** Crea y muestra la Ficha de Factura a Cliente
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newFacturaView().
*/
void company::s_newFacturaCli() {
    _depura("company::s_newFacturaCli", 0);
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newFacturaCli", this))
        return;
    FacturaView *bud = newFacturaView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintaFactura();
    bud->show();
    _depura("END company::s_newFacturaCli", 0);
}


/** Metodo para refrescar los presupuestos.
    De esta forma es posible actualizar el listado de presupuestos sin tener que 
    buscar un puntero a dicha clase.
*/
void company::refreshPresupuestos() {
    _depura("company::refreshPresupuestos", 0);
    m_budgetsList->presenta();
    _depura("END company::refreshPresupuestos", 0);
}


/** Metodo para refrescar las Facturas.
    De esta forma es posible actualizar el listado de facturas sin tener que 
    buscar un puntero a dicha clase.
*/
void company::refreshFacturas() {
    _depura("company::refreshFacturas", 0);
    if(m_facturasList != NULL)
        m_facturasList->presenta();
    _depura("END company::refreshFacturas", 0);
}


/** Metodo para refrescar las Facturas de proveedor
    De esta forma es posible actualizar el listado de facturas de proveedor sin tener que 
    buscar un puntero a dicha clase.
*/
void company::refreshFacturasProveedor() {
    _depura("company::refreshFacturasProveedor", 0);
    if(m_facturasproveedorlist != NULL)
        m_facturasproveedorlist->presenta();
    _depura("END company::refreshFacturasProveedor", 0);

}


/** Pone en primer plano los albaranes de cliente.
*/
/// \TODO: Renombrar este metodo por algo un poco mejor.
void company::listClientDelivNotes() {
    _depura("company::listClientDelivNotes", 0);
    m_clientDelivNotesList->show();
    m_clientDelivNotesList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_clientDelivNotesList);
    _depura("END company::listClientDelivNotes", 0);
}


/** Creacion de un albaran de cliente.
*/
/// \TODO: No pasa por el sistema de plugins. Deberia dividirse en dos.
void company::newClientDelivNote() {
    _depura("company::newClientDelivNote", 0);
    AlbaranClienteView *cDelivNote = new AlbaranClienteView(this, 0);
    m_pWorkspace->addWindow(cDelivNote);
    cDelivNote->showMaximized();
    _depura("END company::newClientDelivNote", 0);
}


/** Refresca el listado de Albaranes a Cliente
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
/// \TODO: ESte metodo esta duplicado junto con refreshAlbaranesCliente.
void company::refreshClientDelivNotes() {
    _depura("company::refreshClientDelivNotes", 0);
    if (m_clientDelivNotesList != NULL)
        m_clientDelivNotesList->presenta();
    _depura("END company::refreshClientDelivNotes", 0);
}


/** Refresca el listado de Albaranes a Cliente
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void company::refreshAlbaranesCliente() {
    _depura("company::refreshAlbaranesCliente", 0);
    if(m_clientDelivNotesList != NULL)
        m_clientDelivNotesList->presenta();
    _depura("company::refreshAlbaranesCliente", 0);
}


/** Refresca el listado de Albaranes de Proveedor
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void company::refreshAlbaranesProveedor() {
    _depura("company::refreshAlbaranesProveedor", 0);
    if(m_albaranesproveedor != NULL)
        m_albaranesproveedor->presenta();
    _depura("END company::refreshAlbaranesProveedor", 0);
}


/** Refresca el listado de Clientes.
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void company::refreshClientes() {
    _depura("company::refreshClientes", 0);
    if(m_clientsList != NULL)
        m_clientsList->presenta();
    _depura("END company::refreshClientes", 0);
}


/** Creacion de un pedido de cliente.
    Mediante este metodo podemos crear un nuevo pedido de cliente.
*/
/// \TODO: No implementa el soporte de plugins y deberia estar dividido en dos.
void company::newPedidoCliente() {
    _depura("company::newPedidoCliente", 0);
    PedidoClienteView *bud = new PedidoClienteView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END company::newPedidoCliente", 0);
}


/** Metodo para refrescar la lista de Pedidos a Cliente.
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void company::refreshPedidosCliente() {
    _depura("company::refreshPedidosCliente", 0);
    if(m_pedidosclienteList != NULL)
        m_pedidosclienteList->presenta();
    _depura("END company::refreshPedidosCliente", 0);
}


/** Metodo para refrescar la lista de Pedidos a Proveedor
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void company::refreshPedidosProveedor() {
    _depura("company::refreshPedidosProveedor", 0);
    if(m_pedidosproveedorList != NULL)
        m_pedidosproveedorList->presenta();
    _depura("END company::refreshPedidosProveedor", 0);
}


/** Crea y Presenta la ventana de formas de pago.
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void company::s_FPago() {
    _depura("company::s_FPago", 0);
    FPagoView *f = new FPagoView(this, NULL);
    m_pWorkspace->addWindow(f);
    f->show();
    _depura("END company::s_FPago", 0);
}


/** Crea y Presenta la ventana de trabajadores
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void company::s_trabajadores() {
    _depura("company::s_trabajadores", 0);
    TrabajadorView *t = new TrabajadorView(this, NULL);
    m_pWorkspace->addWindow(t);
    t->show();
    _depura("END company::s_trabajadores", 0);
}

/** Crea y Presenta la ventana de Series de Factura
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void company::s_seriesFactura() {
    _depura("company::s_seriesFactura", 0);
    ListSerieFacturaView *lser = new ListSerieFacturaView(this, 0);
    m_pWorkspace->addWindow( lser);
    lser->show();
    _depura("END company::s_seriesFactura", 0);
}


/** Crea y Presenta la ventana de provincias.
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void company::s_provincias() {
    _depura("INIT_company::s_provincias", 1);
    ListProvinciasView *lser =  new ListProvinciasView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_company::s_provincias", 1);
}


/** Crea una instancia de la clase ArticuloView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
ArticuloView *company::newArticuloView() {
    _depura("company::newArticuloView", 0);
    /// Lanzamos los plugins necesarios.
    ArticuloView *bud;
    if (g_plugins->lanza("company_newArticuloView", this, (void **)&bud))
        return bud;
    bud = new ArticuloView(this, 0);
    _depura("END company::newArticuloView", 0);
    return bud;
}


/** Crea y muestra la Ficha de Articulos
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newFacturaView().
*/
void company::s_newArticulo() {
    _depura("company::s_newArticulo", 0);
    ArticuloView *art = newArticuloView();
    m_pWorkspace->addWindow(art);
    art->show();
    art->setFocus();
    _depura("END company::s_newArticulo", 0);
}


/** Crea y Presenta la ventana de listado de inventarios
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void company::s_inventarios() {
    _depura("company::s_inventarios", 0);
    InventariosView *bud = new InventariosView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END company::s_inventarios", 0);
}


/** Crea y Presenta un inventario.
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void company::s_newInventario() {
    _depura("company::s_newInventario", 0);
    InventarioView *bud = new InventarioView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END company::s_newInventario", 0);
}


/** Crea una instancia de la clase AlbaranClienteView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
AlbaranClienteView *company::newAlbaranClienteView() {
    _depura("company::newAlbaranClienteView", 0);
    /// Lanzamos los plugins necesarios.
    AlbaranClienteView *bud;
    if (g_plugins->lanza("company_newAlbaranClienteView", this, (void **)&bud))
        return bud;
    bud = new AlbaranClienteView(this, 0);
    _depura("END company::newAlbaranClienteView", 0);
    return bud;
}


/** Crea y muestra la ficha de Albaran a Cliente
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newFacturaView().
*/
void company::s_newAlbaranClienteView() {
    _depura("company::s_newAlbaranClienteView", 0);
    AlbaranClienteView *bud = newAlbaranClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
    _depura("END company::s_newAlbaranClienteView", 0);

}


/** Crea una instancia de la clase AlbaranProveedorView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
AlbaranProveedorView *company::newAlbaranProveedorView() {
    _depura("company::newAlbaranProveedorView", 0);
    /// Lanzamos los plugins necesarios.
    AlbaranProveedorView *bud;
    if (g_plugins->lanza("company_newAlbaranProveedorView", this, (void **)&bud))
        return bud;
    bud = new AlbaranProveedorView(this, 0);
    _depura("END company::newAlbaranProveedorView", 0);
    return bud;
}


/** Crea y muestra la ficha de Albaran de Proveedor
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newAlbaranProveedorView().
*/
void company::s_newAlbaranProveedorView() {
    _depura("company::s_newAlbaranProveedorView", 0);
    AlbaranProveedorView *bud = newAlbaranProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
    _depura("END company::s_newAlbaranProveedorView", 0);
}


/** Crea una instancia de la clase PedidoClienteView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PedidoClienteView *company::newPedidoClienteView() {
    _depura("company::newPedidoClienteView", 0);
    /// Lanzamos los plugins necesarios.
    PedidoClienteView *bud;
    if (g_plugins->lanza("company_newPedidoClienteView", this, (void **)&bud))
        return bud;
    bud = new PedidoClienteView(this, 0);
    _depura("END company::newPedidoClienteView", 0);
    return bud;
}


/** Crea y muestra la ficha de Pedido a Cliente.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newPedidoClienteView().
*/
void company::s_newPedidoClienteView() {
    _depura("company::s_newPedidoClienteView", 0);
    PedidoClienteView *bud = newPedidoClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
    _depura("END company::s_newPedidoClienteView", 0);
}


/** Crea una instancia de la clase PedidoProveedorView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PedidoProveedorView *company::newPedidoProveedorView() {
    _depura("company::newPedidoProveedorView", 0);
    /// Lanzamos los plugins necesarios.
    PedidoProveedorView *bud;
    if (g_plugins->lanza("company_newPedidoProveedorView", this, (void **)&bud))
        return bud;
    bud = new PedidoProveedorView(this, 0);
    _depura("END company::newPedidoProveedorView", 0);
    return bud;
}


/** Crea y muestra la ficha de Pedido de Proveedor.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newPedidoProveedorView().
*/
void company::s_newPedidoProveedorView() {
    _depura("company::s_newPedidoProveedorView", 0);
    PedidoProveedorView *bud = newPedidoProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
    _depura("END company::s_newPedidoProveedorView", 0);
}


/** Crea una instancia de ListAlmacenView y la presenta.
*/
void company::s_almacenes() {
    _depura("company::s_almacenes", 0);
    ListAlmacenView *lser = new ListAlmacenView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_company::s_almacenes", 0);
}


/** Crea una instancia de ListConfiguracionView y la presenta.
*/
void company::s_newListConfiguracionView() {
    _depura("company::s_newListConfiguracionView", 0);
    ListConfiguracionView *lser = new ListConfiguracionView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END company::s_newListConfiguracionView", 0);
}


/** Crea una instancia de la clase PagoView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PagoView *company::newPagoView() {
    _depura("company::newPagoView", 0);
    /// Lanzamos los plugins necesarios.
    PagoView *bud;
    if (g_plugins->lanza("company_newPagoView", this, (void **)&bud))
        return bud;
    bud = new PagoView(this, 0);
    _depura("END company::newPagoView", 0);
    return bud;
}


/** Crea y muestra la ficha de PagoView.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newPedidoClienteView().
*/
void company::s_newPagoView() {
    _depura("company::s_newPagoView", 0);
    PagoView *pag = newPagoView();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END company::s_newPagoView", 0);
}


/** Crea una instancia de la clase TipoArticuloList
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
TipoArticuloList *company::newTipoArticuloList(QWidget *parent, bool modoConsulta) {
    _depura("company::newTipoArticuloList", 0);
    /// Lanzamos los plugins necesarios.
    TipoArticuloList *bud;
    if (g_plugins->lanza("company_newTipoArticuloList", this, (void **)&bud))
        return bud;
    bud = new TipoArticuloList(this, parent, modoConsulta);
    _depura("END company::newTipoArticuloList", 0);
    return bud;
}


/** Crea y muestra la ficha de Tipos de Articulo
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newTipoArticuloList().
*/
void company::s_newTipoArticuloList() {
    _depura("company::s_newTipoArticuloList", 0);
    TipoArticuloList *pag = newTipoArticuloList();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END company::s_newTipoArticuloList", 0);
}


/** Crea una instancia de la clase familiasview
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
FamiliasView *company::newfamiliasview(QWidget *parent, bool modoConsulta) {
    _depura("company::newfamiliasview", 0);
    /// Lanzamos los plugins necesarios.
    FamiliasView *bud;
    if (g_plugins->lanza("company_newfamiliasview", this, (void **)&bud))
        return bud;
    bud = new FamiliasView(this, parent, modoConsulta);
    _depura("END company::newfamiliasview", 0);
    return bud;
}


/** Crea y muestra la ficha de Familias
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newfamiliasview().
*/
void company::s_newfamiliasview() {
    _depura("company::s_newfamiliasview", 0);
    FamiliasView *pag = newfamiliasview();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END company::s_newfamiliasview", 0);
}




