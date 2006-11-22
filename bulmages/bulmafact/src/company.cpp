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

    /// Hacemos visible el ProgressBar mientras se habre la base de datos y se cargan
    /// los datos de la en las ventanas.
    m_progressbar->setVisible(TRUE);

    inicializa(bd);
}


/// Se utiliza para mostrar un selector de empresas abreempresaview
/// Al usuario debe seleccionar una empresa y el sistema empieza la inicializacion de
/// clases a partir de dicha inicializacion.
QString company::searchCompany() {
    /// El cambio de empresa se realiza desde el selector.
    _depura("empresa::searchCompany vamos a mostrar el abreempresaview\n", 0);
    abreempresaview *nuevae = new abreempresaview(0, QApplication::translate("company", "BulmaFact"));
    nuevae->exec();
    _depura("Vamos a cambiar la empresa \n", 0);
    QString bd = nuevae->nomDB();
    _depura("Empresa cambiada a " + bd, 0);
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if (bd == "")
        exit(1);
    return(bd);
}


void company::createMainWindows() {
    _depura("company::createMainWindows", 0);
    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.
    /// pb = 0%
    _depura("company::createMainWindows inicializamos m_articleslist\n", 1);
    m_progressbar->setValue(0);
    m_articleslist = new ArticuloList(this, 0, 0, ArticuloList::EditMode);
    m_pWorkspace->addWindow(m_articleslist);
    m_articleslist->hide();

    /// pb = 8%
    _depura("company::createMainWindows inicializamos m_providerslist\n", 1);
    m_progressbar->setValue(8);
    m_providerslist = new ProveedorList(this, 0);
    m_pWorkspace->addWindow(m_providerslist);
    m_providerslist->hide();

    /// pb = 16%
    _depura("company::createMainWindows inicializamos m_clientesList\n", 1);
    m_progressbar->setValue(16);
    m_clientsList = new ClientsList(this, 0);
    m_pWorkspace->addWindow(m_clientsList);
    m_clientsList->hide();

    /// pb = 32%
    _depura("company::createMainWindows inicializamos m_cobrosList\n", 1);
    m_progressbar->setValue(32);
    m_cobrosList = new CobrosList(this, 0);
    m_pWorkspace->addWindow(m_cobrosList);
    m_cobrosList->hide();

    /// pb = 40%
    _depura("company::createMainWindows inicializamos m_pagosList\n", 1);
    m_progressbar->setValue(40);
    m_pagosList = new PagosList(this, 0);
    m_pWorkspace->addWindow(m_pagosList);
    m_pagosList->hide();

    /// pb = 48%
    _depura("company::createMainWindows inicializamos m_budgetsList\n", 1);
    m_progressbar->setValue(48);
    m_budgetsList = new PresupuestoList(this, 0);
    m_pWorkspace->addWindow(m_budgetsList);
    m_budgetsList->hide();

    /// pb = 52%
    _depura("company::createMainWindows inicializamos m_pedidosclienteList\n", 1);
    m_progressbar->setValue(52);
    m_pedidosclienteList = new PedidosClienteList(this, 0);
    m_pWorkspace->addWindow(m_pedidosclienteList);
    m_pedidosclienteList->hide();

    /// pb = 60%
    _depura("company::createMainWindows inicializamos m_clientDelivNotesList\n", 1);
    m_progressbar->setValue(60);
    m_clientDelivNotesList = new AlbaranClienteList(this, 0);
    m_pWorkspace->addWindow(m_clientDelivNotesList);
    m_clientDelivNotesList->hide();

    /// pb = 68%
    _depura("company::createMainWindows inicializamos m_facturasList\n", 1);
    m_progressbar->setValue(68);
    m_facturasList = new FacturasList(this, 0);
    m_pWorkspace->addWindow(m_facturasList);
    m_facturasList->hide();

    /// pb = 76%
    _depura("company::createMainWindows inicializamos m_pedidosproveedorlist\n", 1);
    m_progressbar->setValue(76);
    m_pedidosproveedorList = new PedidosProveedorList(this, 0);
    m_pWorkspace->addWindow(m_pedidosproveedorList);
    m_pedidosproveedorList->hide();

    /// pb = 84%
    _depura("company::createMainWindows inicializamos m_albaranesproveedorlist\n", 1);
    m_progressbar->setValue(84);
    m_albaranesproveedor = new AlbaranesProveedor(this, 0);
    m_pWorkspace->addWindow(m_albaranesproveedor);
    m_albaranesproveedor->hide();

    /// pb = 92%
    _depura("company::createMainWindows inicializamos m_facturasproveedorlist\n", 1);
    m_progressbar->setValue(92);
    m_facturasproveedorlist = new FacturasProveedorList(this, 0);
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


ClienteView *company::newClienteView() {
    /// Lanzamos los plugins necesarios.
    ClienteView *bud;
    if (g_plugins->lanza("company_newClienteView", this, (void **)&bud))
        return bud;
    bud = new ClienteView(this, 0);
    return bud;
}


void company::s_newClienteView() {
    ClienteView *bud = newClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintaCliente();
    bud->show();
}


ProveedorView *company::newProveedorView() {
    /// Lanzamos los plugins necesarios.
    ProveedorView *bud;
    if (g_plugins->lanza("company_newProveedorView", this, (void **)&bud))
        return bud;
    bud = new ProveedorView(this, 0);
    return bud;
}


void company::s_newProveedorView() {
    ProveedorView *bud = newProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->show();
}


FacturaProveedorView *company::newFacturaProveedorView() {
    /// Lanzamos los plugins necesarios.
    FacturaProveedorView *bud;
    if (g_plugins->lanza("company_newFacturaProveedorView", this, (void **)&bud))
        return bud;
    bud = new FacturaProveedorView(this, 0);
    return bud;
}


void company::s_newFacturaPro() {
    FacturaProveedorView *bud = newFacturaProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
}


CobroView *company::newCobroView() {
    _depura("company::newCobroView", 0);
    CobroView *bud;
    if (g_plugins->lanza("company_newCobroView", this, (void **)&bud))
        return bud;
    bud = new CobroView(this, 0);
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
    AlbaranProveedorView *bud = new AlbaranProveedorView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura ("END company::s_newAlbaranPro", 0);
}


void company::s_newPedidoPro() {
    _depura ("company::s_newPedidoPro", 0);
    PedidoProveedorView *ppv = new PedidoProveedorView(this, 0);
    m_pWorkspace->addWindow(ppv);
    ppv->pintar();
    ppv->show();
    _depura ("END company::s_newPedidoPro", 0);
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


PresupuestoView *company::newBudget() {
    /// Lanzamos los plugins necesarios.
    PresupuestoView *bud;
    if (g_plugins->lanza("company_newBudget", this, (void **)&bud))
        return bud;
    bud = new PresupuestoView(this, 0);
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


FacturaView *company::newFacturaView() {
    /// Lanzamos los plugins necesarios.
    FacturaView *bud;
    if (g_plugins->lanza("company_newFacturaView", this, (void **)&bud))
        return bud;
    bud = new FacturaView(this, 0);
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
    AlbaranClienteView *cDelivNote = new AlbaranClienteView(this, 0);
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
    PedidoClienteView *bud = new PedidoClienteView(this, 0);
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
    _depura("company::s_FPago", 0);
    FPagoView *f = new FPagoView(this, NULL);
    m_pWorkspace->addWindow(f);
    f->show();
    _depura("END company::s_FPago", 0);

}


void company::s_trabajadores() {
    _depura("company::s_trabajadores", 0);
    TrabajadorView *t = new TrabajadorView(this, NULL);
    m_pWorkspace->addWindow(t);
    t->show();
    _depura("END company::s_trabajadores", 0);
}


void company::s_seriesFactura() {
    _depura("company::s_seriesFactura", 0);
    ListSerieFacturaView *lser = new ListSerieFacturaView(this, 0);
    m_pWorkspace->addWindow( lser);
    lser->show();
    _depura("END company::s_seriesFactura", 0);
}


void company::s_provincias() {
    _depura("INIT_company::s_provincias", 1);
    ListProvinciasView *lser =  new ListProvinciasView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_company::s_provincias", 1);
}


ArticuloView *company::newArticuloView() {
    /// Lanzamos los plugins necesarios.
    ArticuloView *bud;
    if (g_plugins->lanza("company_newArticuloView", this, (void **)&bud))
        return bud;
    bud = new ArticuloView(this, 0);
    return bud;
}


void company::s_newArticulo() {
    ArticuloView *art = newArticuloView();
    art->setFocus();
    m_pWorkspace->addWindow(art);
    art->show();
}


void company::s_inventarios() {
    InventariosView *bud = new InventariosView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->show();
}


void company::s_newInventario() {
    InventarioView *bud = new InventarioView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->show();
}


AlbaranClienteView *company::newAlbaranClienteView() {
    /// Lanzamos los plugins necesarios.
    AlbaranClienteView *bud;
    if (g_plugins->lanza("company_newAlbaranClienteView", this, (void **)&bud))
        return bud;
    bud = new AlbaranClienteView(this, 0);
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


AlbaranProveedorView *company::newAlbaranProveedorView() {
    /// Lanzamos los plugins necesarios.
    AlbaranProveedorView *bud;
    if (g_plugins->lanza("company_newAlbaranProveedorView", this, (void **)&bud))
        return bud;
    bud = new AlbaranProveedorView(this, 0);
    return bud;
}


void company::s_newAlbaranProveedorView() {
    AlbaranProveedorView *bud = newAlbaranProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


PedidoClienteView *company::newPedidoClienteView() {
    /// Lanzamos los plugins necesarios.
    PedidoClienteView *bud;
    if (g_plugins->lanza("company_newPedidoClienteView", this, (void **)&bud))
        return bud;
    bud = new PedidoClienteView(this, 0);
    return bud;
}


void company::s_newPedidoClienteView() {
    PedidoClienteView *bud = newPedidoClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
}


PedidoProveedorView *company::newPedidoProveedorView() {
    /// Lanzamos los plugins necesarios.
    PedidoProveedorView *bud;
    if (g_plugins->lanza("company_newPedidoProveedorView", this, (void **)&bud))
        return bud;
    bud = new PedidoProveedorView(this, 0);
    return bud;
}


void company::s_newPedidoProveedorView() {
    PedidoProveedorView *bud = newPedidoProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
}


void company::s_almacenes() {
    _depura("INIT_company::s_almacenes", 1);
    ListAlmacenView *lser = new ListAlmacenView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_company::s_almacenes", 1);
}


void company::s_newListConfiguracionView() {
    _depura("INIT_company::s_newListConfiguracionView", 1);
    ListConfiguracionView *lser = new ListConfiguracionView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_company::s_newListConfiguracionView", 1);
}


PagoView *company::newPagoView() {
    /// Lanzamos los plugins necesarios.
    PagoView *bud;
    if (g_plugins->lanza("company_newPagoView", this, (void **)&bud))
        return bud;
    bud = new PagoView(this, 0);
    return bud;
}


void company::s_newPagoView() {
    _depura("INIT_company::s_newPagoView", 1);
    PagoView *pag = newPagoView();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END_company::s_newPagoView", 1);
}


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


void company::s_newTipoArticuloList() {
    _depura("INIT_company::s_newPagoView", 1);
    TipoArticuloList *pag = newTipoArticuloList();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END_company::s_newPagoView", 1);
}


familiasview *company::newfamiliasview(QWidget *parent, bool modoConsulta) {
    _depura("company::newfamiliasview", 0);
    /// Lanzamos los plugins necesarios.
    familiasview *bud;
    if (g_plugins->lanza("company_newfamiliasview", this, (void **)&bud))
        return bud;
    bud = new familiasview(this, parent, modoConsulta);
    _depura("END company::newfamiliasview", 0);
    return bud;
}


void company::s_newfamiliasview() {
    _depura("INIT_company::s_newfamiliasview", 1);
    familiasview *pag = newfamiliasview();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END_company::s_newfamiliasview", 1);
}


void company::s_indexadorCambiaEstado() {
    m_listventanas->cambiaVisible();
}

