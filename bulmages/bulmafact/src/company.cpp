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


/// El constructor de la clase company no hace nada.
company::company() {}


/// El destructor de la clase company borra toda la memoria almacenada.
company::~company() {
    _depura("Destructor de company", 0);
    /// Primero cerramos todas las ventanas y las DestructiveClose se borran
    m_listventanas->vaciar();
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
    _depura("END Destructor de company", 0);
}


/// Inicializa la base de datos que se pasa, si se pasa una cadena vacia entonces aparece
/// el selector de empresa.
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
    m_articleslist = new ArticuloList(this, 0, QApplication::translate("company", "Listado de articulos"), 0, ArticuloList::EditMode);
    m_pWorkspace->addWindow(m_articleslist);
    m_articleslist->hide();

    _depura("company::createMainWindows inicializamos m_providerslist\n", 1);

    m_providerslist = new ProveedorList(this, 0, QApplication::translate("company", "Listado de proveedores"));
    m_pWorkspace->addWindow(m_providerslist);
    m_providerslist->hide();

    _depura("company::createMainWindows inicializamos m_clientesList\n", 1);

    m_clientsList = new ClientsList(this, 0, QApplication::translate("company", "Listado de clientes"));
    m_pWorkspace->addWindow(m_clientsList);
    m_clientsList->hide();

    _depura("company::createMainWindows inicializamos m_cobrosList\n", 1);

    m_cobrosList = new CobrosList(this, 0, QApplication::translate("company", "Listado de cobros"));
    m_pWorkspace->addWindow(m_cobrosList);
    m_cobrosList->hide();

    _depura("company::createMainWindows inicializamos m_pagosList\n", 1);
    m_pagosList = new PagosList (this, 0, QApplication::translate("company", "Listado de pagos"));
    m_pWorkspace->addWindow(m_pagosList);
    m_pagosList->hide();

    m_budgetsList = new PresupuestoList(this, 0, QApplication::translate("company", "Listado de presupuestos a clientes"));
    m_pWorkspace->addWindow(m_budgetsList);
    m_budgetsList->hide();

    _depura("company::createMainWindows inicializamos m_pedidosclienteList\n", 1);

    m_pedidosclienteList = new PedidosClienteList(this, 0, QApplication::translate("company", "Listado de pedidos de clientes"));
    m_pWorkspace->addWindow(m_pedidosclienteList);
    m_pedidosclienteList->hide();

    m_clientDelivNotesList = new AlbaranClienteList(this, 0, QApplication::translate("company", "Listado de albaranes de clientes"));
    m_pWorkspace->addWindow(m_clientDelivNotesList);
    m_clientDelivNotesList->hide();

    _depura("company::createMainWindows inicializamos m_facturasList\n", 1);

    m_facturasList = new FacturasList(this, 0, QApplication::translate("company", "Listador de facturas a cliente"));
    m_pWorkspace->addWindow(m_facturasList);
    m_facturasList->hide();

    m_pedidosproveedorList = new PedidosProveedorList(this, 0, QApplication::translate("company", "Listador de pedidos a proveedor"));
    m_pWorkspace->addWindow(m_pedidosproveedorList);
    m_pedidosproveedorList->hide();

    m_albaranesproveedor = new AlbaranesProveedor(this, 0, QApplication::translate("company", "Listado de albaranes de proveedor"));
    m_pWorkspace->addWindow(m_albaranesproveedor);
    m_albaranesproveedor->hide();

    m_facturasproveedorlist = new FacturasProveedorList(this, 0, QApplication::translate("company", "Listado de facturas de proveedor"));
    m_pWorkspace->addWindow(m_facturasproveedorlist);
    m_facturasproveedorlist->hide();

    _depura("END company::createMainWindows\n", 0);
}



void company::viewCobrosList() {
    m_cobrosList->show();
    m_cobrosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_cobrosList);
}


void company::viewPagosList() {
    m_pagosList->show();
    m_pagosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pagosList);
}


void company::listproviders() {
    m_providerslist->show();
    m_providerslist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_providerslist);
}


void company::listClients() {
    m_clientsList->show();
    m_clientsList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_clientsList);
}


void company::listarticles() {
    m_articleslist->show();
    m_articleslist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_articleslist);
}


void company::refreshArticles() {
    if(m_articleslist != NULL)
        m_articleslist->presenta();
}


ClienteView * company::newClienteView() {
    /// Lanzamos los plugins necesarios.
    ClienteView *bud;
    if (g_plugins->lanza("company_newClienteView", this, (void **)&bud))
        return bud;
    bud = new ClienteView(this, 0, QApplication::translate("company", "Edicion de clientes"));
    return bud;
}


void company::s_newClienteView() {
    ClienteView *bud = newClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintaCliente();
    bud->show();
}


ProveedorView * company::newProveedorView() {
    /// Lanzamos los plugins necesarios.
    ProveedorView *bud;
    if (g_plugins->lanza("company_newProveedorView", this, (void **)&bud))
        return bud;
    bud = new ProveedorView(this, 0, QApplication::translate("company", "Edicion de proveedores"));
    return bud;
}


void company::s_newProveedorView() {
    ProveedorView *bud = newProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->show();
}



FacturaProveedorView * company::newFacturaProveedorView() {
    /// Lanzamos los plugins necesarios.
    FacturaProveedorView *bud;
    if (g_plugins->lanza("company_newFacturaProveedorView", this, (void **)&bud))
        return bud;
    bud = new FacturaProveedorView(this, 0, QApplication::translate("company", "Edicion de facturas de proveedor"));
    return bud;
}


void company::s_newFacturaPro() {
    FacturaProveedorView *bud = newFacturaProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
}


CobroView * company::newCobroView() {
    _depura("company::newCobroView", 0);
    CobroView *bud;
    if (g_plugins->lanza("company_newCobroView", this, (void **)&bud))
        return bud;
    bud = new CobroView(this, 0, QApplication::translate("company", "Edicion de cobros"));
    return bud;
}


void company::s_newCobroView() {
    _depura("company::s_newCobroView", 0);
    CobroView *bud = newCobroView();
    m_pWorkspace->addWindow(bud);
    bud->show();
}


void company::s_newAlbaranPro() {
    _depura ("company::s_newAlbaranPro", 0);
    AlbaranProveedorView *bud = new AlbaranProveedorView(this, 0, QApplication::translate("company", "Edicion de albaranes de proveedor"));
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura ("END company::s_newAlbaranPro", 0);
}


void company::s_newPedidoPro() {
    _depura ("company::s_newPedidoPro",0);
    PedidoProveedorView *bud = new PedidoProveedorView(this, 0, QApplication::translate("company", "Edicion de pedidos de proveedor"));
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


void company::lAlbaranesProveedor() {
    m_albaranesproveedor->show();
    m_albaranesproveedor->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_albaranesproveedor);
}


void company::s_listPedidosCli() {
    m_pedidosclienteList->show();
    m_pedidosclienteList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pedidosclienteList);
}


void company::s_listPedidosPro() {
    m_pedidosproveedorList->show();
    m_pedidosproveedorList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pedidosproveedorList);
}


void company::s_listFacturasCli() {
    m_facturasList->show();
    m_facturasList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_facturasList);
}


void company::s_listFacturasPro() {
    m_facturasproveedorlist->show();
    m_facturasproveedorlist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_facturasproveedorlist);
}


void company::listBudgets() {
    m_budgetsList->show();
    m_budgetsList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_budgetsList);
}


PresupuestoView * company::newBudget() {
    /// Lanzamos los plugins necesarios.
    PresupuestoView *bud;
    if (g_plugins->lanza("company_newBudget", this, (void **)&bud))
        return bud;
    bud = new PresupuestoView(this, 0, QApplication::translate("company", "Edicion de proveedores"));
    return bud;
}


void company::s_newPresupuestoCli() {
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newPresupuestoCli", this))
        return;
    PresupuestoView *bud = newBudget();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
}


FacturaView * company::newFacturaView() {
    /// Lanzamos los plugins necesarios.
    FacturaView *bud;
    if (g_plugins->lanza("company_newFacturaView", this, (void **)&bud))
        return bud;
    bud = new FacturaView(this, 0, QApplication::translate("company", "Edicion de factura"));
    return bud;
}


void company::s_newFacturaCli() {
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("company_s_newFacturaCli", this))
        return;
    FacturaView *bud = newFacturaView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintaFactura();
    bud->show();
}


void company::refreshBudgets() {
    m_budgetsList->presenta();
}


void company::refreshFacturas() {
    if(m_facturasList != NULL)
        m_facturasList->presenta();
}


void company::refreshFacturasProveedor() {
    _depura("company::refreshFacturasProveedor", 0);
    if(m_facturasproveedorlist != NULL)
        m_facturasproveedorlist->presenta();
    _depura("END company::refreshFacturasProveedor", 0);

}


void company::listClientDelivNotes() {
    m_clientDelivNotesList->show();
    m_clientDelivNotesList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_clientDelivNotesList);
}


void company::newClientDelivNote() {
    AlbaranClienteView *cDelivNote = new AlbaranClienteView(this, 0, QApplication::translate("company", "Edicion de albaranes"));
    m_pWorkspace->addWindow(cDelivNote);
    cDelivNote->showMaximized();
}


void company::refreshClientDelivNotes() {
    if (m_clientDelivNotesList != NULL)
        m_clientDelivNotesList->presenta();
}


void company::refreshAlbaranesCliente() {
    if(m_clientDelivNotesList != NULL)
        m_clientDelivNotesList->presenta();
}


void company::refreshAlbaranesProveedor() {
    if(m_albaranesproveedor != NULL)
        m_albaranesproveedor->presenta();
}


void company::refreshClientes() {
    if(m_clientsList != NULL)
        m_clientsList->presenta();
}


void company::newPedidoCliente() {
    PedidoClienteView *bud = new PedidoClienteView(this, 0, QApplication::translate("company", "Edicion de pedidos de cliente"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}


void company::refreshPedidosCliente() {
    if(m_pedidosclienteList != NULL)
        m_pedidosclienteList->presenta();
}


void company::refreshPedidosProveedor() {
    if(m_pedidosproveedorList != NULL)
        m_pedidosproveedorList->presenta();
}


/// Presenta la ventana de formas de pago y espera la ejecucion de la misma.
void company::s_FPago() {
    FPagoView *f = new FPagoView(this, NULL, QApplication::translate("company", "Formas de pago"));
    f->exec();
    delete f;
}


void company::s_trabajadores() {
    TrabajadorView *t = new TrabajadorView(this, NULL, QApplication::translate("company", "Trabajadores"));
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
    bud = new ArticuloView(this, 0, QApplication::translate("company", "Edicion de articulos"));
    return bud;
}


void company::s_newArticulo() {
    ArticuloView *art = newArticuloView();
    m_pWorkspace->addWindow(art);
    art->show();
    art->setFocus();
}


void company::s_inventarios() {
    InventariosView *bud = new InventariosView(this, 0, QApplication::translate("company", "Listado de inventarios"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}


void company::s_newInventario() {
    InventarioView *bud = new InventarioView(this, 0, QApplication::translate("company", "Nuevo inventario"));
    m_pWorkspace->addWindow(bud);
    bud->show();
}


AlbaranClienteView * company::newAlbaranClienteView() {
    /// Lanzamos los plugins necesarios.
    AlbaranClienteView *bud;
    if (g_plugins->lanza("company_newAlbaranClienteView", this, (void **)&bud))
        return bud;
    bud = new AlbaranClienteView(this, 0, QApplication::translate("company", "Edicion de albaranes a clientes"));
    return bud;
}


void company::s_newAlbaranClienteView() {
    _depura("company::s_newAlbaranClienteView", 0);
    AlbaranClienteView *bud = newAlbaranClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
    _depura("END company::s_newAlbaranClienteView", 0);

}


AlbaranProveedorView * company::newAlbaranProveedorView() {
    /// Lanzamos los plugins necesarios.
    AlbaranProveedorView *bud;
    if (g_plugins->lanza("company_newAlbaranProveedorView", this, (void **)&bud))
        return bud;
    bud = new AlbaranProveedorView(this, 0, QApplication::translate("company", "Edicion de albaranes de proveedor"));
    return bud;
}


void company::s_newAlbaranProveedorView() {
    AlbaranProveedorView *bud = newAlbaranProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


PedidoClienteView * company::newPedidoClienteView() {
    /// Lanzamos los plugins necesarios.
    PedidoClienteView *bud;
    if (g_plugins->lanza("company_newPedidoClienteView", this, (void **)&bud))
        return bud;
    bud = new PedidoClienteView(this, 0, QApplication::translate("company", "Edicion de pedidos a clientes"));
    return bud;
}


void company::s_newPedidoClienteView() {
    PedidoClienteView *bud = newPedidoClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
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
    bud = new PagoView(this, 0, QApplication::translate("company", "Pago"));
    return bud;
}


void company::s_newPagoView() {
    _depura("INIT_company::s_newPagoView", 1);
    PagoView *pag = newPagoView();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END_company::s_newPagoView", 1);
}


TipoArticuloList * company::newTipoArticuloList() {
    /// Lanzamos los plugins necesarios.
    TipoArticuloList *bud;
    if (g_plugins->lanza("company_newTipoArticuloList", this, (void **)&bud))
        return bud;
    bud = new TipoArticuloList(this, 0, QApplication::translate("company", "Tipos de articulo"));
    return bud;
}


void company::s_newTipoArticuloList() {
    _depura("INIT_company::s_newPagoView", 1);
    TipoArticuloList *pag = newTipoArticuloList();
    pag->exec();
    delete pag;
    _depura("END_company::s_newPagoView", 1);
}


void company::s_indexadorCambiaEstado() {
    m_listventanas->cambiaVisible();
}

