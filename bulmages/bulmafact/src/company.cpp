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
#include <Qt>
#include <QObject>

#include "company.h"
#include "stdio.h"
#include "presupuestoview.h"
#include "albaranclienteview.h"
#include "providerslist.h"
#include "provedit.h"
#include "clientslist.h"
#include "clienteview.h"
#include "articulolist.h"
#include "presupuestolist.h"
#include "albaranclientelist.h"
#include "abreempresaview.h"
#include "fpagoview.h"
#include "familiasview.h"
#include "facturaslist.h"
#include "pedidosclientelist.h"
#include "pedidosproveedorlist.h"
#include "pedidoclienteview.h"
#include "pedidoproveedorview.h"
#include "albaranesproveedor.h"
#include "facturasplist.h"
#include "cobroslist.h"
#include "trabajadorview.h"
#include "pagoslist.h"
#include "listseriefacturaview.h"
#include "listprovinciasview.h"
#include "facturapview.h"
#include "albaranproveedorview.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "facturaview.h"
#include "articuloview.h"
#include "funcaux.h"
#include "inventariosview.h"
#include "inventarioview.h"
#include "plugins.h"
#include "cobroview.h"
#include "pagoview.h"
#include "listalmacenview.h"
#include "listconfiguracionview.h"
#include "tiposarticuloview.h"


company::company() {}


void company::init(QString bd) {
    if (bd == "")
        bd = searchCompany();
    inicializa(bd);
}


/// Se utiliza para mostrar un selector de empresas abreempresaview
/// Al usuario debe seleccionar una empresa y el sistema empieza la inicializacion de
/// clases a partir de dicha inicializacion.
QString company::searchCompany() {
    /// El cambio de empresa se realiza desde el selector.
    _depura("empresa::searchCompany vamos a mostrar el abreempresaview\n", 0);
    abreempresaview *nuevae = new abreempresaview(0, "BulmaFact");
    nuevae->exec();
    _depura("Vamos a cambiar la empresa \n", 0);
    QString bd = nuevae->nomDB();
    _depura("Empresa cambiada a" + bd, 0);
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if (bd == "")
        exit(1);
    return(bd);
}


void company::createMainWindows() {
    _depura("company::createMainWindows", 0);
    m_articleslist = new ArticuloList(this, 0, theApp->translate("Articulos","company"), 0, ArticuloList::EditMode);
    m_pWorkspace->addWindow(m_articleslist);
    m_articleslist->hide();

    _depura("company::createMainWindows inicializamos m_providerslist\n", 1);

    m_providerslist = new ProveedorList(this, 0, theApp->translate("Proveedores", "company"));
    m_pWorkspace->addWindow(m_providerslist);
    m_providerslist->hide();

    _depura("company::createMainWindows inicializamos m_clientesList\n", 1);

    m_clientsList = new ClientsList(this, 0, theApp->translate("Clientes", "company"));
    m_pWorkspace->addWindow(m_clientsList);
    m_clientsList->hide();

    _depura("company::createMainWindows inicializamos m_cobrosList\n", 1);

    m_cobrosList = new CobrosList(this, 0, theApp->translate("Cobros", "company"));
    m_pWorkspace->addWindow(m_cobrosList);
    m_cobrosList->hide();

    _depura("company::createMainWindows inicializamos m_pagosList\n", 1);
    m_pagosList = new PagosList (this, 0, theApp->translate("Pagos", "company"));
    m_pWorkspace->addWindow(m_pagosList);
    m_pagosList->hide();

    m_budgetsList = new PresupuestoList(this, 0, theApp->translate("Presupuestos a clientes", "company"));
    m_pWorkspace->addWindow(m_budgetsList);
    m_budgetsList->hide();

    _depura("company::createMainWindows inicializamos m_pedidosclienteList\n", 1);

    m_pedidosclienteList = new PedidosClienteList(this, 0, theApp->translate("Pedidos de clientes", "company"));
    m_pWorkspace->addWindow(m_pedidosclienteList);
    m_pedidosclienteList->hide();

    m_clientDelivNotesList = new AlbaranClienteList(this, 0, theApp->translate("Albaranes de clientes", "company"));
    m_pWorkspace->addWindow(m_clientDelivNotesList);
    m_clientDelivNotesList->hide();

    _depura("company::createMainWindows inicializamos m_facturasList\n", 1);

    m_facturasList = new FacturasList(this, 0, theApp->translate("Facturas a cliente", "company"));
    m_pWorkspace->addWindow(m_facturasList);
    m_facturasList->hide();

    m_pedidosproveedorList = new PedidosProveedorList(this, 0, theApp->translate("Pedidos a proveedor", "company"));
    m_pWorkspace->addWindow(m_pedidosproveedorList);
    m_pedidosproveedorList->hide();

    m_albaranesproveedor = new AlbaranesProveedor(this, 0, theApp->translate("Albaranes de proveedor", "company"));
    m_pWorkspace->addWindow(m_albaranesproveedor);
    m_albaranesproveedor->hide();

    m_facturasproveedorlist = new FacturasProveedorList(this, 0, theApp->translate("Facturas de proveedor", "company"));
    m_pWorkspace->addWindow(m_facturasproveedorlist);
    m_facturasproveedorlist->hide();

    _depura("END company::createMainWindows\n", 0);
}


company::~company() {
    _depura("Destructor de company", 0);
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
    _depura("END Destructor de company", 0);
}


void company::viewCobrosList () {
    m_cobrosList->hide();
    m_cobrosList->showMaximized();
    m_cobrosList->setActiveWindow();
}


void company::viewPagosList () {
    m_pagosList->hide();
    m_pagosList->showMaximized();
    m_pagosList->setActiveWindow();
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


void company::s_newProveedor() {
    ProveedorView *prov = new ProveedorView(this, 0, theApp->translate("Editar/anyadir proveedor", "company"));
    m_pWorkspace->addWindow(prov);
    prov->showMaximized();
}


void company::listarticles () {
    m_articleslist->hide();
    m_articleslist->showMaximized();
    m_articleslist->setActiveWindow();
}


void company::refreshArticles() {
    m_articleslist->presenta();
}


ClienteView * company::newClienteView() {
    /// Lanzamos los plugins necesarios.
    ClienteView *bud;
    if (g_plugins->lanza("company_newClienteView", this, (void **)&bud))
        return bud;
    bud = new ClienteView(this, 0, theApp->translate("Edicion de clientes", "company"));
    return bud;
}


void company::s_newClienteView() {
    ClienteView *bud = newClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintaCliente();
    bud->show();
}


FacturaProveedorView * company::newFacturaProveedorView() {
    /// Lanzamos los plugins necesarios.
    FacturaProveedorView *bud;
    if (g_plugins->lanza("company_newFacturaProveedorView", this, (void **)&bud))
        return bud;
    bud = new FacturaProveedorView(this, 0, theApp->translate("Edicion de facturas de proveedor", "company"));
    return bud;
}


void company::s_newFacturaPro() {
    FacturaProveedorView *bud = newFacturaProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


CobroView * company::newCobroView() {
    _depura("company::newCobroView", 0);
    CobroView *bud;
    if (g_plugins->lanza("company_newCobroView", this, (void **)&bud) )
        return bud;
    bud = new CobroView(this, 0, theApp->translate("Edicion de cobros", "company"));
    return bud;
}


void company::s_newCobroView() {
    _depura("company::s_newCobroView", 0);
    CobroView *bud = newCobroView();
    bud->exec();
}


void company::s_newAlbaranPro() {
    _depura ("company::s_newAlbaranPro", 0);
    AlbaranProveedorView *bud = new AlbaranProveedorView(this, 0, theApp->translate("Edicion de albaranes de proveedor", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura ("END company::s_newAlbaranPro", 0);
}


void company::s_newPedidoPro() {
    _depura ("company::s_newPedidoPro",0);
    PedidoProveedorView *bud = new PedidoProveedorView(this, 0, theApp->translate("Edicion de pedidos de proveedor", "company"));
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


void company::lAlbaranesProveedor () {
    m_albaranesproveedor->hide();
    m_albaranesproveedor->showMaximized();
    m_albaranesproveedor->setActiveWindow();
}


void company::s_listPedidosCli () {
    m_pedidosclienteList->hide();
    m_pedidosclienteList->showMaximized();
    m_pedidosclienteList->setActiveWindow();
}


void company::s_listPedidosPro () {
    m_pedidosproveedorList->hide();
    m_pedidosproveedorList->showMaximized();
    m_pedidosproveedorList->setActiveWindow();
}


void company::s_listFacturasCli () {
    m_facturasList->hide();
    m_facturasList->showMaximized();
    m_facturasList->setActiveWindow();
}


void company::s_listFacturasPro () {
    m_facturasproveedorlist->hide();
    m_facturasproveedorlist->showMaximized();
    m_facturasproveedorlist->setActiveWindow();
}


void company::listBudgets() {
    m_budgetsList->hide();
    m_budgetsList->showMaximized();
    m_budgetsList->setActiveWindow();
}


PresupuestoView * company::newBudget() {
    /// Lanzamos los plugins necesarios.
    PresupuestoView *bud;
    if (g_plugins->lanza("company_newBudget", this, (void **)&bud))
        return bud;
    bud = new PresupuestoView(this , 0, theApp->translate("Edicion de proveedores", "company"));
    return bud;
}


void company::s_newPresupuestoCli() {
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newPresupuestoCli", this))
        return;
    PresupuestoView *bud = newBudget();
    m_pWorkspace->addWindow(bud);
    bud->show();
}


FacturaView * company::newFacturaView() {
    /// Lanzamos los plugins necesarios.
    FacturaView *bud;
    if (g_plugins->lanza("company_newFacturaView", this, (void **)&bud))
        return bud;
    bud = new FacturaView(this, 0, theApp->translate("Edicion de factura", "company"));
    return bud;
}


void company::s_newFacturaCli() {
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newFacturaCli", this))
        return;
    FacturaView *bud = newFacturaView();
    m_pWorkspace->addWindow(bud);
    bud->cargar("0");
    bud->pintaFactura();
    bud->show();
}


void company::refreshBudgets() {
    m_budgetsList->presenta();
}


void company::refreshFacturas() {
    m_facturasList->presenta();
}


void company::listClientDelivNotes() {
    m_clientDelivNotesList->hide();
    m_clientDelivNotesList->showMaximized();
    m_clientDelivNotesList->setActiveWindow();
}


void company::newClientDelivNote() {
    AlbaranClienteView *cDelivNote = new AlbaranClienteView(this, 0, theApp->translate("Edicion de albaranes", "company"));
    m_pWorkspace->addWindow(cDelivNote);
    cDelivNote->showMaximized();
}


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
    PedidoClienteView *bud = new PedidoClienteView(this, 0, theApp->translate("Edicion de pedidos de cliente", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}


void company::refreshPedidosCliente() {
    m_pedidosclienteList->presenta();
}


void company::refreshPedidosProveedor() {
    m_pedidosproveedorList->presenta();
}


/// Presenta la ventana de formas de pago y espera la ejecucion de la misma.
void company::s_FPago() {
    FPagoView *f = new FPagoView(this, NULL, theApp->translate("Formas de pago", "company"));
    f->exec();
    delete f;
}


void company::s_trabajadores() {
    TrabajadorView *t = new TrabajadorView(this, NULL, theApp->translate("Trabajadores", "company"));
    t->exec();
    delete t;
}


void company::s_Familias() {
    familiasview *fam = new familiasview(this, 0, 0);
    fam->exec();
    delete fam;
}


void company::s_seriesFactura() {
    ListSerieFacturaView *lser = new ListSerieFacturaView(this, 0, 0);
    lser->exec();
    delete lser;
}


void company::s_provincias() {
    _depura("INIT_company::s_provincias", 1);
    ListProvinciasView lser(this, 0);
    lser.exec();
    _depura("END_company::s_provincias", 1);
}


ArticuloView * company::newArticuloView() {
    /// Lanzamos los plugins necesarios.
    ArticuloView *bud;
    if (g_plugins->lanza("company_newArticuloView", this, (void **)&bud))
        return bud;
    bud = new ArticuloView(this, 0, theApp->translate("Edicion de articulos", "company"));
    return bud;
}


void company::s_newArticulo() {
    //QString idArt = "0";
    ArticuloView *art = newArticuloView();
    //art->cargar(idArt);
    m_pWorkspace->addWindow(art);
    art->show();
}


void company::s_inventarios() {
    InventariosView *bud = new InventariosView(this, 0, theApp->translate("Listado de inventarios", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}


void company::s_newInventario() {
    InventarioView *bud = new InventarioView(this, 0, theApp->translate("Nuevo inventario", "company"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}


AlbaranClienteView * company::newAlbaranClienteView() {
    /// Lanzamos los plugins necesarios.
    AlbaranClienteView *bud;
    if (g_plugins->lanza("company_newAlbaranClienteView", this, (void **)&bud))
        return bud;
    bud = new AlbaranClienteView(this, 0, theApp->translate("Edicion de albaranes a clientes", "company"));
    return bud;
}


void company::s_newAlbaranClienteView() {
    AlbaranClienteView *bud = newAlbaranClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


PedidoClienteView * company::newPedidoClienteView() {
    /// Lanzamos los plugins necesarios.
    PedidoClienteView *bud;
    if (g_plugins->lanza("company_newPedidoClienteView", this, (void **)&bud))
        return bud;
    bud = new PedidoClienteView(this, 0, theApp->translate("Edicion de pedidos a clientes", "company"));
    return bud;
}


void company::s_newPedidoClienteView() {
    PedidoClienteView *bud = newPedidoClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


void company::s_almacenes() {
    _depura("INIT_company::s_almacenes", 1);
    ListAlmacenView lser(this, 0);
    lser.exec();
    _depura("END_company::s_almacenes", 1);
}


void company::s_newListConfiguracionView() {
    _depura("INIT_company::s_newListConfiguracionView", 1);
    ListConfiguracionView lser(this, 0);
    lser.exec();
    _depura("END_company::s_newListConfiguracionView", 1);
}


PagoView * company::newPagoView() {
    /// Lanzamos los plugins necesarios.
    PagoView *bud;
    if (g_plugins->lanza("company_newPagoView", this, (void **)&bud))
        return bud;
    bud = new PagoView(this, 0, theApp->translate("Pago", "company"));
    return bud;
}


void company::s_newPagoView() {
    _depura("INIT_company::s_newPagoView", 1);
    PagoView *pag =  newPagoView();
    pag->exec();
    delete pag;
    _depura("END_company::s_newPagoView", 1);
}


TipoArticuloList * company::newTipoArticuloList() {
    /// Lanzamos los plugins necesarios.
    TipoArticuloList *bud;
    if (g_plugins->lanza("company_newTipoArticuloList", this, (void **)&bud))
        return bud;
    bud = new TipoArticuloList(this, 0, theApp->translate("Tipos de articulo", "company"));
    return bud;
}


void company::s_newTipoArticuloList() {
    _depura("INIT_company::s_newPagoView", 1);
    TipoArticuloList *pag =  newTipoArticuloList();
    pag->exec();
    delete pag;
    _depura("END_company::s_newPagoView", 1);
}

