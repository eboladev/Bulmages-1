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

#include <qnamespace.h>

#include "company.h"
#include "stdio.h"
#include "budget.h"
#include "albaranclienteview.h"
#include "providerslist.h"
#include "provedit.h"
#include "qobject.h"
#include "clientslist.h"
#include "clientedit.h"
#include "articleslist.h"
#include "budgetslist.h"
#include "clientdelivnoteslist.h"
#include "abreempresaview.h"
#include "fpago.h"
#include "familiasview.h"
#include "facturaslist.h"
#include "pedidosclientelist.h"
#include "pedidosproveedorlist.h"
#include "pedidoclienteview.h"
#include "pedidoproveedorview.h"
#include "albaranesproveedor.h"
#include "facturasplist.h"
#include "cobroslist.h"
#include "trabajador.h"
#include "pagoslist.h"
#include "listseriefacturaview.h"
#include "listprovinciasview.h"
#include "facturapview.h"
#include "albaranproveedorview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "facturaview.h"
#include "articleedit.h"
#include "funcaux.h"
#include "inventariosview.h"
#include "inventarioview.h"
#include "plugins.h"

company::company() {}// end company


void company::init(QString bd) {
    if (bd == "")
        bd = searchCompany();
    inicializa(bd);
}// end init


/** \brief Se utiliza para mostrar un selector de empresas \ref abreempresaview
  * Al usuario debe seleccionar una empresa y el sistema empieza la inicializaci� de clases a partir de dicha inicializaci�.
  */
QString company::searchCompany() {
    //El cambio de empresa se realiza desde el selector.
    _depura("empresa::searchCompany vamos a mostrar el abreempresaview\n",0);
    abreempresaview *nuevae = new abreempresaview(0,"BulmaFact" );
    nuevae->exec();
    _depura("Vamos a cambiar la empresa \n",0);
    QString bd= nuevae->nomDB();
    _depura("Empresa cambiada a"+bd,0);
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if (bd == "")
        exit(1);
    return(bd);
}// end searchCompany



void company::createMainWindows() {
    m_articleslist = new articleslist(this, 0,theApp->translate("Artiulos","company"),0,articleslist::EditMode);
    m_pWorkspace->addWindow(m_articleslist);
    m_articleslist->hide();

    m_providerslist = new providerslist(this, 0,theApp->translate("Proveedores","company"));
    m_pWorkspace->addWindow(m_providerslist);
    m_providerslist->hide();

    m_clientsList = new ClientsList(this, 0,theApp->translate("Clientes","company"));
    m_pWorkspace->addWindow(m_clientsList);
    m_clientsList->hide();

    m_cobrosList = new CobrosList(this, 0,theApp->translate("Cobros","company"));
    m_pWorkspace->addWindow(m_cobrosList);
    m_cobrosList->hide();

    m_pagosList =  new PagosList (this, 0, theApp->translate("Pagos","company"));
    m_pWorkspace->addWindow(m_pagosList);
    m_pagosList->hide();

    m_budgetsList = new BudgetsList(this, 0,theApp->translate("Presupuestos a Clientes","company"));
    m_pWorkspace->addWindow(m_budgetsList);
    m_budgetsList->hide();

    m_pedidosclienteList = new PedidosClienteList(this, 0,theApp->translate("Pedidos de Clientes","company"));
    m_pWorkspace->addWindow(m_pedidosclienteList);
    m_pedidosclienteList->hide();

    m_clientDelivNotesList = new ClientDelivNotesList(this, 0,theApp->translate("Albaranes de Clientes","company"));
    m_pWorkspace->addWindow(m_clientDelivNotesList);
    m_clientDelivNotesList->hide();

    m_facturasList = new FacturasList(this, 0,theApp->translate("Facturas a Cliente","company"));
    m_pWorkspace->addWindow(m_facturasList);
    m_facturasList->hide();

    m_pedidosproveedorList = new PedidosProveedorList(this, 0,theApp->translate("Pedidos a Proveedor","company"));
    m_pWorkspace->addWindow(m_pedidosproveedorList);
    m_pedidosproveedorList->hide();

    m_albaranesproveedor = new AlbaranesProveedor(this, 0,theApp->translate("Albaranes Proveedor","company"));
    m_pWorkspace->addWindow(m_albaranesproveedor);
    m_albaranesproveedor->hide();

    m_facturasproveedorlist = new FacturasProveedorList(this, 0,theApp->translate("Facturas Proveedor","company"));
    m_pWorkspace->addWindow(m_facturasproveedorlist);
    m_facturasproveedorlist->hide();

    _depura("Fin de createMainWindows\n",0);
}// end createMainWindows


company::~company() {
    _depura("Destructor de company",0);
    delete m_facturasproveedorlist;
    delete m_albaranesproveedor;
    delete m_pedidosproveedorList;
    delete m_facturasList;
    delete m_articleslist;
    delete m_providerslist;
    delete m_clientsList;
    delete m_cobrosList;
    delete m_pagosList;
    delete m_budgetsList;
    delete m_pedidosclienteList;
    delete m_clientDelivNotesList;
    _depura("END Destructor de company",0);
}


void company::listproviders () {
    m_providerslist->hide();
    m_providerslist->showMaximized();
    m_providerslist->setActiveWindow();
}

void company::listClients () {
    m_clientsList->hide();
    m_clientsList->showMaximized();
    m_clientsList->setActiveWindow();
}

void company::newClient() {
    ClientEdit *mclientEdit = new ClientEdit(this,0,theApp->translate("Editar/A�dir cliente","company"));
    m_pWorkspace->addWindow(mclientEdit);
    mclientEdit->showMaximized();
}// end newClient


void company::s_newProveedor() {
    provedit *prov = new provedit(this,0,theApp->translate("Editar/Anadir proveedor","company"));
    m_pWorkspace->addWindow(prov);
    prov->showMaximized();
}// end newProveedor

void company::listarticles () {
    m_articleslist->hide();
    m_articleslist->showMaximized();
    m_articleslist->setActiveWindow();
}

void company::refreshArticles() {
    m_articleslist->presenta();
}// end refreshOrders


void company::s_newFacturaPro() {
    FacturaProveedorView *bud = new FacturaProveedorView(this,0,theApp->translate("Edicion de FacturasProveedor", "company"));
    m_pWorkspace->addWindow(bud);
    bud->pintaFacturaProveedor();
    bud->show();
}// end s_nuevaFacturaProveedor

void company::s_newAlbaranPro() {
    AlbaranProveedorView *bud = new AlbaranProveedorView(this,0,theApp->translate("Edicion de Albaranes Proveedor", "company"));
    m_pWorkspace->addWindow(bud);
    bud->pintaAlbaranProveedor();
    bud->show();
}// end s_newAlbaranProveedor

void company::s_newPedidoPro() {
    PedidoProveedorView *bud = new PedidoProveedorView(this,0,theApp->translate("Edicion de Pedidos Proveedor", "company"));
    m_pWorkspace->addWindow(bud);
    bud->pintaPedidoProveedor();
    bud->show();
}// end s_newAlbaranProveedor



void company::lAlbaranesProveedor () {
    m_albaranesproveedor->hide();
    m_albaranesproveedor->showMaximized();
    m_albaranesproveedor->setActiveWindow();
}// end lAlbaranesProveedor

void company::s_listPedidosCli () {
    m_pedidosclienteList->hide();
    m_pedidosclienteList->showMaximized();
    m_pedidosclienteList->setActiveWindow();
}// end s_listPedidosCli

void company::s_listPedidosPro () {
    m_pedidosproveedorList->hide();
    m_pedidosproveedorList->showMaximized();
    m_pedidosproveedorList->setActiveWindow();
}// end s_listPedidosPro



void company::s_listFacturasCli () {
    m_facturasList->hide();
    m_facturasList->showMaximized();
    m_facturasList->setActiveWindow();
}// end s_listFacturasCli

void company::s_listFacturasPro () {
    m_facturasproveedorlist->hide();
    m_facturasproveedorlist->showMaximized();
    m_facturasproveedorlist->setActiveWindow();
}// end s_listFacturasPro

void company::listBudgets() {
    m_budgetsList->hide();
    m_budgetsList->showMaximized();
    m_budgetsList->setActiveWindow();
}// end listbudgets




Budget * company::newBudget() {
    /// Lanzamos los plugins necesarios.
    Budget *bud;
    if (g_plugins->lanza("company_newBudget", this, (void **)&bud) )
        return bud;
    bud = new Budget(this , 0,theApp->translate("Edicion de Proveedores", "company"));
    return bud;
}
void company::s_newPresupuestoCli() {
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newPresupuestoCli",this) )
        return;
    Budget *bud = newBudget();
    m_pWorkspace->addWindow(bud);
    bud->show();
}// end bud


void company::s_newPedidoCli() {
    PedidoClienteView *bud = new PedidoClienteView(this , 0,theApp->translate("Pedido Cliente", "company"));
    m_pWorkspace->addWindow(bud);
    bud->pintaPedidoCliente();
    bud->show();
}// end bud

void company::s_newAlbaranCli() {
    AlbaranClienteView *bud = new AlbaranClienteView(this , 0,theApp->translate("Edicion de Albaran", "company"));
    m_pWorkspace->addWindow(bud);
    bud->pintaAlbaranCliente();
    bud->show();
}// end bud


FacturaView * company::newFacturaView() {
    /// Lanzamos los plugins necesarios.
    FacturaView *bud;
    if (g_plugins->lanza("company_newFacturaView", this, (void **)&bud) )
        return bud;
    bud = new FacturaView(this , 0,theApp->translate("Edicion de Factura", "company"));
    return bud;
}
void company::s_newFacturaCli() {
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newFacturaCli",this) )
        return;
    FacturaView *bud = newFacturaView();
    m_pWorkspace->addWindow(bud);
    bud->pintaFactura();
    bud->show();
}// end bud




void company::refreshBudgets() {
    m_budgetsList->presenta();
}// end refreshBudgets

void company::refreshFacturas() {
    m_facturasList->presenta();
}// end refreshFacturas


void company::listClientDelivNotes() {
    m_clientDelivNotesList->hide();
    m_clientDelivNotesList->showMaximized();
    m_clientDelivNotesList->setActiveWindow();
}// end listbudgets

void company::newClientDelivNote() {
    AlbaranClienteView *cDelivNote = new AlbaranClienteView(this,0,theApp->translate("Edicion de Albaranes", "company"));
    m_pWorkspace->addWindow(cDelivNote);
    cDelivNote->showMaximized();
}// end newClientDelivNote

void company::refreshClientDelivNotes() {
    m_clientDelivNotesList->presenta();
}
void company::refreshAlbaranesCliente() {
    m_clientDelivNotesList->presenta();
}

void company::refreshAlbaranesProveedor() {
    m_albaranesproveedor->presenta();
}

void company::refreshClientes() {
    m_clientsList->presenta();
}


void company::newPedidoCliente() {
    PedidoClienteView *bud = new PedidoClienteView(this ,0,theApp->translate("Edicion de Pedidos de Cliente", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}// end bud

void company::refreshPedidosCliente() {
    m_pedidosclienteList->presenta();
}// end refreshPedidosCliente


void company::refreshPedidosProveedor() {
    m_pedidosproveedorList->presenta();
}// end refreshPedidosCliente

/** Presenta la ventana de formas de pago y espera la ejecuci� de la misma */
void company::s_FPago() {
    fpago *f = new fpago(this,NULL,theApp->translate("Formas de Pago", "company"));
    f->exec();
    delete f;
}// end newOrder


void company::s_trabajadores() {
    Trabajador *t = new Trabajador(this, NULL, theApp->translate("Trabajadores", "company"));
    t->exec();
    delete t;
}

void company::s_Familias() {
    familiasview *fam = new familiasview(this, 0,0);
    fam->exec();
    delete fam;
}// end s_Familias


void company::s_seriesFactura() {
    ListSerieFacturaView *lser = new ListSerieFacturaView(this, 0,0);
    lser->exec();
    delete lser;
}// end s_seriesFactura


void company::s_provincias() {
    ListProvinciasView *lser = new ListProvinciasView(this, 0,0);
    lser->exec();
    delete lser;
}// end s_seriesFactura

void company::s_newArticulo() {
    QString idArt = "0";
    articleedit *art = new articleedit(this,0,theApp->translate("Edición de Artículos", "company"));
    art->chargeArticle(idArt);
    m_pWorkspace->addWindow(art);
    art->show();
}


void company::s_inventarios() {
    InventariosView *bud = new InventariosView(this,0,theApp->translate("Listado de Inventarios", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}// end s_nuevaFacturaProveedor

void company::s_newInventario() {
    InventarioView *bud = new InventarioView(this,0,theApp->translate("Nuevo Inventario", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}// end s_nuevaFacturaProveedor
