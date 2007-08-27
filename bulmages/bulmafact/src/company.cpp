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
#include <QFile>
#include <QTextStream>
#include <QDomDocument>

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
#include "bancoview.h"
#include "listtipoivaview.h"
#include "listtasaivaview.h"
#include "bulmafact.h"

/** No precisa de operaciones en su construccion.
*/
Company::Company(Bulmafact *bges) : EmpresaBase() {
    _depura("Company::Company", 0);
    m_bulmafact = bges;
    _depura("END Company::Company", 0);
}


/// El destructor de la clase Company borra toda la memoria almacenada.
Company::~Company() {
    _depura("Company::~Company", 0);

    /// Guardamos la configuracion.
    guardaConf();

    /// Borramos las ventanas flotantes antes de eliminar esta clase ya que se produciria un segfault
    m_listventanas->vaciarCompleto();
    _depura("END Company::~Company", 0);
}


/** Crea todas las ventanas que aparecen creadas al inicio del programa.
    Todas estas ventanas son principalmente los listados mas utilizados a partir de los
    cuales se pueden crear nuevas fichas y manejar todo.
*/
void Company::createMainWindows(Splash *splash) {
    _depura("Company::createMainWindows", 0);
    /// Establecemos el porcentaje del carga de informaci&oacute;n en las diferentes ventanas.



    /// pb = 0%
    _depura("Company::createMainWindows inicializamos m_articleslist", 1);
    splash->mensaje(QApplication::translate("Company", "Inicializando listado de Articulos"));
    splash->setBarraProgreso(7);
    m_progressbar->setValue(0);
    /// Comprobamos que tengamos permisos para trabajar con articulos.
    m_bulmafact->actionListado_de_Articulos->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Articulo->setEnabled(FALSE);
    m_bulmafact->actionGestion_Familias->setEnabled(FALSE);
    m_bulmafact->actionGestion_Tipos_Articulo->setEnabled(FALSE);
        if (has_table_privilege("articulo", "SELECT")) {
            m_articleslist = new ArticuloList(this);
            m_pWorkspace->addWindow(m_articleslist);
            m_articleslist->hide();
            m_bulmafact->actionListado_de_Articulos->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Articulo->setEnabled(TRUE);
            m_bulmafact->actionGestion_Familias->setEnabled(TRUE);
            m_bulmafact->actionGestion_Tipos_Articulo->setEnabled(TRUE);
        } // end if



    /// pb = 8%
    _depura("Company::createMainWindows inicializamos m_providerslist", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Proveedores"));
    splash->setBarraProgreso(10);
    m_progressbar->setValue(8);
    /// Comprobamos que tengamos permisos para trabajar con clientes.
    m_bulmafact->actionListado_Clientes->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Cliente->setEnabled(FALSE);
        if (has_table_privilege("cliente", "SELECT")) {
            m_clientsList = new ClientsList(this);
            m_pWorkspace->addWindow(m_clientsList);
            m_clientsList->hide();
            m_bulmafact->actionListado_Clientes->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Cliente->setEnabled(TRUE);
        } // end if



    /// pb = 16%
    _depura("Company::createMainWindows inicializamos m_clientesList", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Clientes"));
    splash->setBarraProgreso(16);
    m_progressbar->setValue(16);
    /// Comprobamos que tengamos permisos para trabajar con clientes.
    m_bulmafact->actionListado_Proveedores->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Proveedor->setEnabled(FALSE);
        if (has_table_privilege("proveedor", "SELECT")) {
            m_providerslist = new ProveedorList(this);
            m_pWorkspace->addWindow(m_providerslist);
            m_providerslist->hide();
            m_bulmafact->actionListado_Proveedores->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Proveedor->setEnabled(TRUE);
        } // end if




    /// pb = 32%
    _depura("Company::createMainWindows inicializamos m_presupuestosList", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Presupuestos"));
    splash->setBarraProgreso(32);
    m_progressbar->setValue(32);
    /// Comprobamos que tengamos permisos para trabajar con presupuestos.
    m_bulmafact->actionListado_Presupuestos->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Presupuesto->setEnabled(FALSE);
        if (has_table_privilege("presupuesto", "SELECT")) {
            m_presupuestosList = new PresupuestoList(this);
            m_pWorkspace->addWindow(m_presupuestosList);
            m_presupuestosList->hide();
            m_bulmafact->actionListado_Presupuestos->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Presupuesto->setEnabled(TRUE);
        } // end if


    /// pb = 40%
    _depura("Company::createMainWindows inicializamos m_pedidosclienteList", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Pedidos Cliente"));
    splash->setBarraProgreso(40);
    m_progressbar->setValue(40);
    /// Comprobamos que tengamos permisos para trabajar con pedidos de cliente.
    m_bulmafact->actionListado_Pedidos_Cliente->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Pedido_Cliente->setEnabled(FALSE);
        if (has_table_privilege("pedidocliente", "SELECT")) {
            m_pedidosclienteList = new PedidosClienteList(this);
            m_pWorkspace->addWindow(m_pedidosclienteList);
            m_pedidosclienteList->hide();
            m_bulmafact->actionListado_Pedidos_Cliente->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Pedido_Cliente->setEnabled(TRUE);
        } // end if




    /// pb = 48%
    _depura("Company::createMainWindows inicializamos m_clientDelivNotesList", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Albaranes Cliente"));
    splash->setBarraProgreso(48);
    m_progressbar->setValue(48);
    /// Comprobamos que tengamos permisos para trabajar con albaranes.
    m_bulmafact->actionListado_Albaranes_Cliente->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Albaran_Cliente->setEnabled(FALSE);
        if (has_table_privilege("albaran", "SELECT")) {
            m_clientDelivNotesList = new AlbaranClienteList(this);
            m_pWorkspace->addWindow(m_clientDelivNotesList);
            m_clientDelivNotesList->hide();
            m_bulmafact->actionListado_Albaranes_Cliente->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Albaran_Cliente->setEnabled(TRUE);
        } // end if


    /// pb = 56%
    _depura("Company::createMainWindows inicializamos m_facturasList", 0);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Facturas"));
    splash->setBarraProgreso(56);
    m_progressbar->setValue(56);
    /// Comprobamos que tengamos permisos para trabajar con Facturas.
    m_bulmafact->actionListado_Facturas_Cliente->setEnabled(FALSE);
    m_bulmafact->actionNueva_Factura_Cliente->setEnabled(FALSE);
        if (has_table_privilege("factura", "SELECT")) {
            m_facturasList = new FacturasList(this);
            m_pWorkspace->addWindow(m_facturasList);
            m_facturasList->hide();
            m_bulmafact->actionListado_Facturas_Cliente->setEnabled(TRUE);
            m_bulmafact->actionNueva_Factura_Cliente->setEnabled(TRUE);
        } // end if




    /// pb = 64%
    _depura("Company::createMainWindows inicializamos m_cobrosList", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Cobros"));
    splash->setBarraProgreso(64);
    m_progressbar->setValue(64);
    /// Comprobamos que tengamos permisos para trabajar con Facturas.
    m_bulmafact->actionListado_de_Cobros->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Cobro->setEnabled(FALSE);
        if (has_table_privilege("cobro", "SELECT")) {
            m_cobrosList = new CobrosList(this);
            m_pWorkspace->addWindow(m_cobrosList);
            m_cobrosList->hide();
            m_bulmafact->actionListado_de_Cobros->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Cobro->setEnabled(TRUE);
        } // end if


    /// pb = 72%
    _depura("Company::createMainWindows inicializamos m_pedidosproveedorlist", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Pedidos Proveedor"));
    splash->setBarraProgreso(72);
    m_progressbar->setValue(72);
    /// Comprobamos que tengamos permisos para trabajar con Pedidos de Proveedor.
    m_bulmafact->actionListado_Pedidos_Proveedor->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Pedido_Proveedor->setEnabled(FALSE);
        if (has_table_privilege("pedidoproveedor", "SELECT")) {
            m_pedidosproveedorList = new PedidosProveedorList(this);
            m_pWorkspace->addWindow(m_pedidosproveedorList);
            m_pedidosproveedorList->hide();
            m_bulmafact->actionListado_Pedidos_Proveedor->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Pedido_Proveedor->setEnabled(TRUE);
        } // end if



    /// pb = 80%
    _depura("Company::createMainWindows inicializamos m_albaranesproveedorlist", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Albaranes Proveedor"));
    splash->setBarraProgreso(80);
    m_progressbar->setValue(80);
    /// Comprobamos que tengamos permisos para trabajar con Albaranes de Proveedor.
    m_bulmafact->actionListado_Albaranes_Proveedor->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Albaran_Proveedor->setEnabled(FALSE);
        if (has_table_privilege("albaranp", "SELECT")) {
            m_albaranesproveedor = new AlbaranesProveedor(this);
            m_pWorkspace->addWindow(m_albaranesproveedor);
            m_albaranesproveedor->hide();
            m_bulmafact->actionListado_Albaranes_Proveedor->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Albaran_Proveedor->setEnabled(TRUE);
        } // end if



    /// pb = 88%
    _depura("Company::createMainWindows inicializamos m_facturasproveedorlist", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Facturas Proveedor"));
    splash->setBarraProgreso(88);
    m_progressbar->setValue(88);
    /// Comprobamos que tengamos permisos para trabajar con Facturas de Proveedor.
    m_bulmafact->actionListado_Facturas_Proveedor->setEnabled(FALSE);
    m_bulmafact->actionNueva_Factura_Proveedor->setEnabled(FALSE);
        if (has_table_privilege("facturap", "SELECT")) {
            m_facturasproveedorlist = new FacturasProveedorList(this);
            m_pWorkspace->addWindow(m_facturasproveedorlist);
            m_facturasproveedorlist->hide();
            m_bulmafact->actionListado_Facturas_Proveedor->setEnabled(TRUE);
            m_bulmafact->actionNueva_Factura_Proveedor->setEnabled(TRUE);
        } // end if


    /// pb = 96%
    _depura("Company::createMainWindows inicializamos m_pagosList", 1);
    splash->mensaje(QApplication::translate("Company","Inicializando listado de Pagos"));
    splash->setBarraProgreso(96);
    m_progressbar->setValue(96);
    /// Comprobamos que tengamos permisos para trabajar con Facturas de Proveedor.
    m_bulmafact->actionListado_de_Pagos->setEnabled(FALSE);
    m_bulmafact->actionNuevo_Pago->setEnabled(FALSE);
        if (has_table_privilege("pago", "SELECT")) {
            m_pagosList = new PagosList(this);
            m_pWorkspace->addWindow(m_pagosList);
            m_pagosList->hide();
            m_bulmafact->actionListado_de_Pagos->setEnabled(TRUE);
            m_bulmafact->actionNuevo_Pago->setEnabled(TRUE);
        } // end if


    /// Comprobamos que tengamos permisos para trabajar con Tipos de IVA.
    m_bulmafact->actionTasaIVA->setEnabled(FALSE);
    m_bulmafact->actionTipoIVA->setEnabled(FALSE);
        if (has_table_privilege("tipo_iva", "SELECT")) {
            m_bulmafact->actionTasaIVA->setEnabled(TRUE);
            m_bulmafact->actionTipoIVA->setEnabled(TRUE);
        } // end if

    /// Comprobamos que tengamos permisos para trabajar con Provincias.
    m_bulmafact->actionProvincias->setEnabled(FALSE);
        if (has_table_privilege("provincia", "SELECT")) {
            m_bulmafact->actionProvincias->setEnabled(TRUE);
        } // end if

    /// Comprobamos que tengamos permisos para trabajar con Provincias.
    m_bulmafact->actionTrabajadores->setEnabled(FALSE);
        if (has_table_privilege("trabajador", "SELECT")) {
            m_bulmafact->actionTrabajadores->setEnabled(TRUE);
        } // end if

    /// Comprobamos que tengamos permisos para trabajar con Provincias.
    m_bulmafact->actionFormas_de_Pago->setEnabled(FALSE);
        if (has_table_privilege("forma_pago", "SELECT")) {
            m_bulmafact->actionFormas_de_Pago->setEnabled(TRUE);
        } // end if

    /// Comprobamos que tengamos permisos para trabajar con Provincias.
    m_bulmafact->actionSeries_de_Factura->setEnabled(FALSE);
        if (has_table_privilege("serie_factura", "SELECT")) {
            m_bulmafact->actionSeries_de_Factura->setEnabled(TRUE);
        } // end if

    /// Comprobamos que tengamos permisos para trabajar con Provincias.
    m_bulmafact->actionBancos->setEnabled(FALSE);
        if (has_table_privilege("banco", "SELECT")) {
            m_bulmafact->actionBancos->setEnabled(TRUE);
        } // end if


    /// Comprobamos que tengamos permisos para trabajar con Almacenes.
    m_bulmafact->actionAlmacenes->setEnabled(FALSE);
        if (has_table_privilege("almacen", "SELECT")) {
            m_bulmafact->actionAlmacenes->setEnabled(TRUE);
        } // end if


    /// pb = 100%
    m_progressbar->setValue(100);

    /// Disparamos los plugins.
    int res = g_plugins->lanza("Company_createMainWindows_Post", this);
    if (res != 0)
        return;

    cargaConf();

    _depura("END Company::createMainWindows", 0);
}


/** Metodo para ver la ventana de Cobros.
    Es invocado desde el menu de la aplicacion.
*/
void Company::viewCobrosList() {
    _depura("Company::viewCobrosList", 0);
    m_cobrosList->show();
    m_cobrosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_cobrosList);
    _depura("END Company::viewCobrosList", 0);
}


/** Metodo para ver la ventana de PAgos.
    Es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::viewPagosList() {
    _depura("Company::viewPagosList", 0);
    m_pagosList->show();
    m_pagosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pagosList);
    _depura("END Company::viewPagosList", 0);
}


/** Muestra la ventana de proveedores.
    Es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::listproviders() {
    _depura("Company::listproviders", 0);
    m_providerslist->show();
    m_providerslist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_providerslist);
    _depura("END Company::listproviders", 0);
}


/** Muestra la ventana de listado de Clientes.
    Es invocada desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::listClients() {
    _depura("Company::listClients", 0);
    m_clientsList->show();
    m_clientsList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_clientsList);
    _depura("END Company::listClients", 0);
}


/** Muestra la ventana del listado de Articulos.
    Es invocada desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::listarticles() {
    _depura("Company::listarticles", 0);
    m_articleslist->show();
    m_articleslist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_articleslist);
    _depura("END Company::listarticles", 0);
}


/** Refresca el listado de articulos.
    Este metodo de paso de mensajes permite, desde cualquier otra clase refrescar los articulos sin
    tener una instancia de dicha clase.
*/
void Company::refreshArticles() {
    _depura("Company::refreshArticles", 0);
    if (m_articleslist != NULL)
        m_articleslist->presentar();
    _depura("END Company::refreshArticles", 0);
}


/** Crea una instancia de la ficha de cliente.
    Tiene la ventaja de que no debe implementarse en cada sitio que se quiera instanciar la ficha de lciente.
    Y ademas hace el tratamiento adecuado de plugins.
*/
ClienteView *Company::newClienteView() {
    _depura("Company::newClienteView", 0);
    /// Lanzamos los plugins necesarios.
    ClienteView *bud;
    if (g_plugins->lanza("Company_newClienteView", this, (void **)&bud))
        return bud;
    bud = new ClienteView(this, 0);
    _depura("END Company::newClienteView", 0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de cliente.
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newClienteView()
*/
void Company::s_newClienteView() {
    _depura("Company::s_newClienteView", 0);
    ClienteView *bud = newClienteView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
    _depura("END Company::s_newClienteView", 0);
}


/** Crea una instancia de la ficha de proveedor.
    Tiene la ventaja de que no debe implementarse en cada sitio que se quiera instanciar la ficha de proveedor.
    Y ademas hace el tratamiento adecuado de plugins.
*/
ProveedorView *Company::newProveedorView() {
    _depura("Company::newProveedorView",0);
    /// Lanzamos los plugins necesarios.
    ProveedorView *bud;
    if (g_plugins->lanza("Company_newProveedorView", this, (void **)&bud))
        return bud;
    bud = new ProveedorView(this, 0);
    _depura("END Company::newProveedorView",0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de proveedor.
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newClienteView()
*/
void Company::s_newProveedorView() {
    _depura("Company::s_newProveedorView", 0);
    ProveedorView *bud = newProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END Company::s_newProveedorView", 0);
}


/** Creacion de una ventana de factura de proveedor
    Es importante que siempre que se crea una factura de proveedor
    sea mediante este metodo de comany, ya que de esta forma nos
    aseguramos de que si existe un plugin que sustituye esta ventana,
    nuestra llamada devolvera el objeto adecuado
*/
FacturaProveedorView *Company::newFacturaProveedorView() {
    _depura("Company::newFacturaProveedorView", 0);
    /// Lanzamos los plugins necesarios.
    FacturaProveedorView *bud;
    if (g_plugins->lanza("Company_newFacturaProveedorView", this, (void **)&bud))
        return bud;
    bud = new FacturaProveedorView(this, 0);
    _depura("END Company::newFacturaProveedorView", 0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de Factura de Proveedor
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newFacturaProveedor()
*/
void Company::s_newFacturaPro() {
    FacturaProveedorView *bud = newFacturaProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura("END Company::s_newFacturaPro", 0);
}


/** Creacion de una ventana de Cobro
    Es importante que siempre que se crea un cobro
    sea mediante este metodo de comany, ya que de esta forma nos
    aseguramos de que si existe un plugin que sustituye esta ventana,
    nuestra llamada devolvera el objeto adecuado
*/
CobroView *Company::newCobroView() {
    _depura("Company::newCobroView", 0);
    CobroView *bud;
    if (g_plugins->lanza("Company_newCobroView", this, (void **)&bud))
        return bud;
    bud = new CobroView(this, 0);
    _depura("END Company::newCobroView", 0);
    return bud;
}


/** Crea y agrega al workSpace una instancia de la ficha de Cobro
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
    Si no se desea que la ventana aparezca en pantalla puede usarse newCobroView()
*/
void Company::s_newCobroView() {
    _depura("Company::s_newCobroView", 0);
    CobroView *bud = newCobroView();
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END Company::s_newCobroView", 0);
}


/** Crea y agrega al workSpace una instancia de la ficha de Albaran Proveedor
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
*/
/// \TODO: No pasa por el proceso de plugins. Debe cambiarse.
void Company::s_newAlbaranPro() {
    _depura ("Company::s_newAlbaranPro", 0);
    AlbaranProveedorView *bud = new AlbaranProveedorView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura ("END Company::s_newAlbaranPro", 0);
}


/** Crea y agrega al workSpace una instancia de la ficha de Pedido Proveedor
    Tiene la ventaja de que el tratamiento esta centralizado y es facil de invocar desde cualquier parte.
*/
/// \TODO: No pasa por el proceso de plugins. Debe cambiarse.
void Company::s_newPedidoPro() {
    _depura ("Company::s_newPedidoPro", 0);
    PedidoProveedorView *ppv = new PedidoProveedorView(this, 0);
    m_pWorkspace->addWindow(ppv);
    ppv->pintar();
    ppv->show();
    _depura ("END Company::s_newPedidoPro", 0);
}


/** Muestra como ventana activa la ventana de los albaranes de Proveedor.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::lAlbaranesProveedor() {
    _depura("Company::lAlbaranesProveedor", 0);
    m_albaranesproveedor->show();
    m_albaranesproveedor->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_albaranesproveedor);
    _depura("END Company::lAlbaranesProveedor", 0);
}


/** Muestra como ventana activa la ventana de listado de pedidos de cliente.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::s_listPedidosCli() {
    _depura("Company::s_listPedidosCli", 0);
    m_pedidosclienteList->show();
    m_pedidosclienteList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pedidosclienteList);
    _depura("END Company::s_listPedidosCli", 0);
}


/** Muestra como ventana activa la ventana de listado de pedidos a proveedor.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::s_listPedidosPro() {
    _depura("Company::s_listPedidosPro", 0);
    m_pedidosproveedorList->show();
    m_pedidosproveedorList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_pedidosproveedorList);
    _depura("END Company::s_listPedidosPro", 0);
}


/** Muestra como ventana activa la ventana de listado de facturas a Cliente.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::s_listFacturasCli() {
    _depura("Company::s_listFacturasCli", 0);
    m_facturasList->show();
    m_facturasList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_facturasList);
    _depura("END Company::s_listFacturasCli", 0);
}


/** Muestra como ventana activa la ventana de listado de facturas a Proveedor.
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::s_listFacturasPro() {
    _depura("Company::s_listFacturasPro", 0);
    m_facturasproveedorlist->show();
    m_facturasproveedorlist->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_facturasproveedorlist);
    _depura("END Company::s_listFacturasPro", 0);
}


/** Muestra como ventana activa la ventana de listado de Presupuestos
    Este metodo es invocado desde el menu de la aplicacion a traves de la clase BulmaFact.
*/
void Company::listBudgets() {
    _depura("Company::listBudgets", 0);
    m_presupuestosList->show();
    m_presupuestosList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_presupuestosList);
    _depura("END Company::listBudgets", 0);
}


/** Crea una instancia de la clase PresupuestoView.
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PresupuestoView *Company::nuevoPresupuestoView() {
    _depura("Company::nuevoPresupuesto", 0);
    /// Lanzamos los plugins necesarios.
    PresupuestoView *bud;
    if (g_plugins->lanza("Company_nuevoPresupuesto", this, (void **)&bud))
        return bud;
    bud = new PresupuestoView(this, 0);
    _depura("END Company::nuevoPresupuesto", 0);
    return bud;
}


/** Crea una instancia de la clase PedidoProveedorView.
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PedidoProveedorView *Company::nuevoPedidoProveedorView() {
    _depura("Company::nuevoPedidoProveedor", 0);
    /// Lanzamos los plugins necesarios.
    PedidoProveedorView *bud;
    if (g_plugins->lanza("Company_nuevoPedidoProveedor", this, (void **)&bud))
        return bud;
    bud = new PedidoProveedorView(this, 0);
    _depura("END Company::nuevoPedidoProveedor", 0);
    return bud;
}


/** Crea y muestra la Ficha de presupuesto.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo nuevoPresupuesto().
*/
void Company::s_newPresupuestoCli() {
    _depura("Company::s_newPresupuestoCli", 0);
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("Company_s_newPresupuestoCli", this))
        return;
    PresupuestoView *bud = nuevoPresupuestoView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->show();
    _depura("END Company::s_newPresupuestoCli", 0);
}


/** Crea una instancia de la clase FacturaView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
FacturaView *Company::newFacturaView() {
    _depura("Company::newFacturaView", 0);
    /// Lanzamos los plugins necesarios.
    FacturaView *bud;
    if (g_plugins->lanza("Company_newFacturaView", this, (void **)&bud))
        return bud;
    bud = new FacturaView(this, 0);
    _depura("END Company::newFacturaView", 0);
    return bud;
}


/** Crea y muestra la Ficha de Factura a Cliente
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newFacturaView().
*/
void Company::s_newFacturaCli() {
    _depura("Company::s_newFacturaCli", 0);
    /// Lanzamos los plugins necesarios.
    if (g_plugins->lanza("Company_s_newFacturaCli", this))
        return;
    FacturaView *bud = newFacturaView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintaFactura();
    bud->show();
    _depura("END Company::s_newFacturaCli", 0);
}


/** Metodo para refrescar los presupuestos.
    De esta forma es posible actualizar el listado de presupuestos sin tener que
    buscar un puntero a dicha clase.
*/
void Company::refreshPresupuestos() {
    _depura("Company::refreshPresupuestos", 0);
    m_presupuestosList->presentar();
    _depura("END Company::refreshPresupuestos", 0);
}


/** Metodo para refrescar las Facturas.
    De esta forma es posible actualizar el listado de facturas sin tener que
    buscar un puntero a dicha clase.
*/
void Company::refreshFacturas() {
    _depura("Company::refreshFacturas", 0);
    if (m_facturasList != NULL)
        m_facturasList->presentar();
    _depura("END Company::refreshFacturas", 0);
}


/** Metodo para refrescar las Facturas de proveedor
    De esta forma es posible actualizar el listado de facturas de proveedor sin tener que
    buscar un puntero a dicha clase.
*/
void Company::refreshFacturasProveedor() {
    _depura("Company::refreshFacturasProveedor", 0);
    if (m_facturasproveedorlist != NULL)
        m_facturasproveedorlist->presentar();
    _depura("END Company::refreshFacturasProveedor", 0);

}


/** Pone en primer plano los albaranes de cliente.
*/
/// \TODO: Renombrar este metodo por algo un poco mejor.
void Company::listClientDelivNotes() {
    _depura("Company::listClientDelivNotes", 0);
    m_clientDelivNotesList->show();
    m_clientDelivNotesList->parentWidget()->raise();
    m_pWorkspace->setActiveWindow(m_clientDelivNotesList);
    _depura("END Company::listClientDelivNotes", 0);
}


/** Creacion de un albaran de cliente.
*/
/// \TODO: No pasa por el sistema de plugins. Deberia dividirse en dos.
void Company::newClientDelivNote() {
    _depura("Company::newClientDelivNote", 0);
    AlbaranClienteView *cDelivNote = new AlbaranClienteView(this, 0);
    m_pWorkspace->addWindow(cDelivNote);
    cDelivNote->showMaximized();
    _depura("END Company::newClientDelivNote", 0);
}


/** Refresca el listado de Albaranes a Cliente
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
/// \TODO: ESte metodo esta duplicado junto con refreshAlbaranesCliente.
void Company::refreshClientDelivNotes() {
    _depura("Company::refreshClientDelivNotes", 0);
    if (m_clientDelivNotesList != NULL)
        m_clientDelivNotesList->presentar();
    _depura("END Company::refreshClientDelivNotes", 0);
}


/** Refresca el listado de Albaranes a Cliente
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void Company::refreshAlbaranesCliente() {
    _depura("Company::refreshAlbaranesCliente", 0);
    if (m_clientDelivNotesList != NULL)
        m_clientDelivNotesList->presentar();
    _depura("Company::refreshAlbaranesCliente", 0);
}


/** Refresca el listado de Albaranes de Proveedor
    Mediante este metodo es sencill actualizar el listado de albaranes de proveedor sin disponer
    de un puntero a dicha ventana.
*/
void Company::refreshAlbaranesProveedor() {
    _depura("Company::refreshAlbaranesProveedor", 0);
    if (m_albaranesproveedor != NULL)
        m_albaranesproveedor->presentar();
    _depura("END Company::refreshAlbaranesProveedor", 0);
}


/** Refresca el listado de Clientes.
*/
void Company::refreshClientes() {
    _depura("Company::refreshClientes", 0);
    if (m_clientsList != NULL)
        m_clientsList->presentar();
    _depura("END Company::refreshClientes", 0);
}


/** Refresca el listado de Proveedores.
*/
void Company::refreshProveedores() {
    _depura("Company::refreshProveedores", 0);
    if (m_providerslist != NULL)
        m_providerslist->presentar();
    _depura("END Company::refreshProveedores", 0);
}


/** Creacion de un pedido de cliente.
    Mediante este metodo podemos crear un nuevo pedido de cliente.
*/
/// \TODO: No implementa el soporte de plugins y deberia estar dividido en dos.
void Company::newPedidoCliente() {
    _depura("Company::newPedidoCliente", 0);
    PedidoClienteView *bud = new PedidoClienteView(this, 0);
    m_pWorkspace->addWindow(bud);
    bud->show();
    _depura("END Company::newPedidoCliente", 0);
}


/** Metodo para refrescar la lista de Cobros a Cliente.
    Mediante este metodo es sencillo actualizar el listado de cobros a cliente sin disponer
    de un puntero a dicha ventana.
*/
void Company::refreshCobrosCliente() {
    _depura("Company::refreshCobrosCliente", 0);
    if (m_cobrosList != NULL)
        m_cobrosList->presentar();
    _depura("END Company::refreshCobrosCliente", 0);
}


/** Metodo para refrescar la lista de Pedidos de Clientes.
    Mediante este metodo es sencillo actualizar el listado de pedidos de clientes sin disponer
    de un puntero a dicha ventana.
*/
void Company::refreshPedidosCliente() {
    _depura("Company::refreshPedidosCliente", 0);
    if (m_pedidosclienteList != NULL)
        m_pedidosclienteList->presentar();
    _depura("END Company::refreshPedidosCliente", 0);
}


/** Metodo para refrescar la lista de Pedidos a Proveedor
    Mediante este metodo es sencillo actualizar el listado de pedidos a proveedor sin disponer
    de un puntero a dicha ventana.
*/
void Company::refreshPedidosProveedor() {
    _depura("Company::refreshPedidosProveedor", 0);
    if (m_pedidosproveedorList != NULL)
        m_pedidosproveedorList->presentar();
    _depura("END Company::refreshPedidosProveedor", 0);
}


/** Crea y Presenta la ventana de formas de pago.
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void Company::s_FPago() {
    _depura("Company::s_FPago", 0);
    FPagoView *f = new FPagoView(this, NULL);
    m_pWorkspace->addWindow(f);
    f->show();
    _depura("END Company::s_FPago", 0);
}


/** Crea y Presenta la ventana de trabajadores
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void Company::s_trabajadores() {
    _depura("Company::s_trabajadores", 0);
    TrabajadorView *t = new TrabajadorView(this, NULL);
    m_pWorkspace->addWindow(t);
    t->show();
    _depura("END Company::s_trabajadores", 0);
}

/** Crea y Presenta la ventana de bancos
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void Company::s_bancos() {
    _depura("Company::s_bancos", 0);
    BancoView *t = new BancoView(this, NULL);
    m_pWorkspace->addWindow(t);
    t->show();
    _depura("END Company::s_bancos", 0);
}

/** Crea y Presenta la ventana de Series de Factura
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void Company::s_seriesFactura() {
    _depura("Company::s_seriesFactura", 0);
    ListSerieFacturaView *lser = new ListSerieFacturaView(this, 0);
    m_pWorkspace->addWindow( lser);
    lser->show();
    _depura("END Company::s_seriesFactura", 0);
}


/** Crea y Presenta la ventana de provincias.
*/
/// \TODO: Deberia dividirse en dos y deberia pasar por sistema de plugins.
void Company::s_provincias() {
    _depura("INIT_Company::s_provincias", 1);
    ListProvinciasView *lser =  new ListProvinciasView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_Company::s_provincias", 1);
}


/** Crea una instancia de la clase ArticuloView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
ArticuloView *Company::newArticuloView() {
    _depura("Company::newArticuloView", 0);
    /// Lanzamos los plugins necesarios.
    ArticuloView *bud;
    if (g_plugins->lanza("Company_newArticuloView", this, (void **)&bud))
        return bud;
    bud = new ArticuloView(this, 0);
    _depura("END Company::newArticuloView", 0);
    return bud;
}


/** Crea y muestra la Ficha de Articulos
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newFacturaView().
*/
void Company::s_newArticulo() {
    _depura("Company::s_newArticulo", 0);
    ArticuloView *art = newArticuloView();
    m_pWorkspace->addWindow(art);
    art->show();
    art->setFocus();
    _depura("END Company::s_newArticulo", 0);
}




/** Crea una instancia de la clase AlbaranClienteView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
AlbaranClienteView *Company::newAlbaranClienteView() {
    _depura("Company::newAlbaranClienteView", 0);
    /// Lanzamos los plugins necesarios.
    AlbaranClienteView *bud;
    if (g_plugins->lanza("Company_newAlbaranClienteView", this, (void **)&bud))
        return bud;
    bud = new AlbaranClienteView(this, 0);
    _depura("END Company::newAlbaranClienteView", 0);
    return bud;
}


/** Crea y muestra la ficha de Albaran a Cliente
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newFacturaView().
*/
void Company::s_newAlbaranClienteView() {
    _depura("Company::s_newAlbaranClienteView", 0);
    AlbaranClienteView *bud = newAlbaranClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
    _depura("END Company::s_newAlbaranClienteView", 0);

}


/** Crea una instancia de la clase AlbaranProveedorView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
AlbaranProveedorView *Company::newAlbaranProveedorView() {
    _depura("Company::newAlbaranProveedorView", 0);
    /// Lanzamos los plugins necesarios.
    AlbaranProveedorView *bud;
    if (g_plugins->lanza("Company_newAlbaranProveedorView", this, (void **)&bud))
        return bud;
    bud = new AlbaranProveedorView(this, 0);
    _depura("END Company::newAlbaranProveedorView", 0);
    return bud;
}


/** Crea y muestra la ficha de Albaran de Proveedor
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newAlbaranProveedorView().
*/
void Company::s_newAlbaranProveedorView() {
    _depura("Company::s_newAlbaranProveedorView", 0);
    AlbaranProveedorView *bud = newAlbaranProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
    _depura("END Company::s_newAlbaranProveedorView", 0);
}


/** Crea una instancia de la clase PedidoClienteView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PedidoClienteView *Company::newPedidoClienteView() {
    _depura("Company::newPedidoClienteView", 0);
    /// Lanzamos los plugins necesarios.
    PedidoClienteView *bud;
    if (g_plugins->lanza("Company_newPedidoClienteView", this, (void **)&bud))
        return bud;
    bud = new PedidoClienteView(this, 0);
    _depura("END Company::newPedidoClienteView", 0);
    return bud;
}


/** Crea y muestra la ficha de Pedido a Cliente.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newPedidoClienteView().
*/
void Company::s_newPedidoClienteView() {
    _depura("Company::s_newPedidoClienteView", 0);
    PedidoClienteView *bud = newPedidoClienteView();
    m_pWorkspace->addWindow(bud);
    bud->inicializar();
    bud->pintar();
    bud->show();
    _depura("END Company::s_newPedidoClienteView", 0);
}


/** Crea y muestra la ficha de Pedido de Proveedor.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newPedidoProveedorView().
*/
void Company::s_newPedidoProveedorView() {
    _depura("Company::s_newPedidoProveedorView", 0);
    PedidoProveedorView *bud = nuevoPedidoProveedorView();
    m_pWorkspace->addWindow(bud);
    bud->pintar();
    bud->show();
    _depura("END Company::s_newPedidoProveedorView", 0);
}


/** Crea una instancia de ListAlmacenView y la presenta.
*/
void Company::s_almacenes() {
    _depura("Company::s_almacenes", 0);
    ListAlmacenView *lser = new ListAlmacenView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END_Company::s_almacenes", 0);
}


/** Crea una instancia de ListConfiguracionView y la presenta.
*/
void Company::s_newListConfiguracionView() {
    _depura("Company::s_newListConfiguracionView", 0);
    ListConfiguracionView *lser = new ListConfiguracionView(this, 0);
    m_pWorkspace->addWindow(lser);
    lser->show();
    _depura("END Company::s_newListConfiguracionView", 0);
}


/** Crea una instancia de la clase PagoView
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
PagoView *Company::newPagoView() {
    _depura("Company::newPagoView", 0);
    /// Lanzamos los plugins necesarios.
    PagoView *bud;
    if (g_plugins->lanza("Company_newPagoView", this, (void **)&bud))
        return bud;
    bud = new PagoView(this, 0);
    _depura("END Company::newPagoView", 0);
    return bud;
}


/** Crea y muestra la ficha de PagoView.
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newPedidoClienteView().
*/
void Company::s_newPagoView() {
    _depura("Company::s_newPagoView", 0);
    PagoView *pag = newPagoView();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END Company::s_newPagoView", 0);
}


/** Crea una instancia de la clase TipoArticuloList
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
TipoArticuloList *Company::newTipoArticuloList(QWidget *parent, bool modoConsulta) {
    _depura("Company::newTipoArticuloList", 0);
    /// Lanzamos los plugins necesarios.
    TipoArticuloList *bud;
    if (g_plugins->lanza("Company_newTipoArticuloList", this, (void **)&bud))
        return bud;
    bud = new TipoArticuloList(this, parent, modoConsulta);
    _depura("END Company::newTipoArticuloList", 0);
    return bud;
}


/** Crea y muestra la ficha de Tipos de Articulo
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newTipoArticuloList().
*/
void Company::s_newTipoArticuloList() {
    _depura("Company::s_newTipoArticuloList", 0);
    TipoArticuloList *pag = newTipoArticuloList();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END Company::s_newTipoArticuloList", 0);
}


/** Crea una instancia de la clase familiasview
    Es importante tener la creacion de instancias centralizada en esta clase para asegurar
    Que se lanzan los plugins necesarios.
*/
FamiliasView *Company::newfamiliasview(QWidget *parent, bool modoConsulta) {
    _depura("Company::newfamiliasview", 0);
    /// Lanzamos los plugins necesarios.
    FamiliasView *bud;
    if (g_plugins->lanza("Company_newfamiliasview", this, (void **)&bud))
        return bud;
    bud = new FamiliasView(this, parent, modoConsulta);
    _depura("END Company::newfamiliasview", 0);
    return bud;
}


/** Crea y muestra la ficha de Familias
    Si solo se desea crear la ficha sin mostrarla debe usar el metodo newfamiliasview().
*/
void Company::s_newfamiliasview() {
    _depura("Company::s_newfamiliasview", 0);
    FamiliasView *pag = newfamiliasview();
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END Company::s_newfamiliasview", 0);
}


/** Crea y muestra el listado de Tipos de IVA
*/
void Company::s_TipoIVAView() {
    _depura("Company::s_TipoIVAView", 0);
    ListTipoIVAView *pag = new ListTipoIVAView(this, 0);
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END Company::s_TipoIVAView", 0);
}

/** Crea y muestra el listado de Tasas de IVA
*/
void Company::s_TasaIVAView() {
    _depura("Company::s_TasaIVAView", 0);
    ListTasaIVAView *pag = new ListTasaIVAView(this, 0);
    m_pWorkspace->addWindow(pag);
    pag->show();
    _depura("END Company::s_TasaIVAView", 0);
}


FacturasProveedorList * Company::facturasProveedorList() {
    _depura("Company::facturasProveedorList", 0);
    _depura("END Company::facturasProveedorList", 0);
    return m_facturasproveedorlist;
}

PresupuestoList * Company::presupuestoList() {
    _depura("Company::presupuestoList", 0);
    _depura("END Company::presupuestoList", 0);
    return m_presupuestosList;
}





/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
void Company::guardaConf() {
    QFile file(confpr->valor(CONF_DIR_USER) + "bulmafact_" + nameDB() + ".cfn");
    /// Guardado del orden y de configuraciones varias.
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "<CONFIG>\n";
        stream << "\t<PRINCIPAL>\n";
        stream << "\t\t\t<X>" + QString::number(m_bulmafact->x()) + "</X>\n";
        stream << "\t\t\t<Y>" + QString::number(m_bulmafact->y()) + "</Y>\n";
        stream << "\t\t\t<WIDTH>" + QString::number(m_bulmafact->width()) + "</WIDTH>\n";
        stream << "\t\t\t<HEIGHT>" + QString::number(m_bulmafact->height()) + "</HEIGHT>\n";
        stream << "\t\t\t<INDEXADOR>" + (m_bulmafact->actionIndexador->isChecked() ? QString("TRUE") : QString("FALSE")) + "</INDEXADOR>\n";
        stream << "\t</PRINCIPAL>\n";


        for (int i = 0; i < m_listventanas->numVentanas(); i++) {
		QObject *obj = m_listventanas->ventana(i);
                QWidget *wid = (QWidget *) obj;
		stream << "\t<VENTANA>\n";
		stream << "\t\t<VNAME>" + obj->objectName() + "</VNAME>\n";
		stream << "\t\t<VX>" + QString::number(wid->parentWidget()->x()) + "</VX>\n";
		stream << "\t\t<VY>" + QString::number(wid->parentWidget()->y()) + "</VY>\n";
		stream << "\t\t<VWIDTH>" + QString::number(wid->width()) + "</VWIDTH>\n";
		stream << "\t\t<VHEIGHT>" + QString::number(wid->height()) + "</VHEIGHT>\n";
		stream << "\t\t<VVISIBLE>" + (wid->isVisible() ? QString("TRUE") : QString("FALSE")) + "</VVISIBLE>\n";
		stream << "\t\t<VMAXIMIZED>" + (wid->isMaximized() ? QString("TRUE") : QString("FALSE")) + "</VMAXIMIZED>\n";
		stream << "\t\t<VACTIVEWINDOW>" + (m_bulmafact->workspace()->activeWindow() == wid ? QString("TRUE") : QString("FALSE")) + "</VACTIVEWINDOW>";
		stream << "\t</VENTANA>\n";
	} // end for

        stream << "</CONFIG>\n";
        file.close();
    } // end if
}

/// Guarda la configuracion de programa para poder recuperar algunas cosas de presentacion.
void Company::cargaConf() {
    QFile file(confpr->valor(CONF_DIR_USER) + "bulmafact_" + nameDB() + ".cfn");
    QDomDocument doc("mydocument");
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    // print out the element names of all elements that are direct children
    // of the outermost element.
    QDomElement docElem = doc.documentElement();
    QDomElement principal = docElem.firstChildElement("PRINCIPAL");
    /// Cogemos la coordenada X
    QString nx = principal.firstChildElement("X").toElement().text();

    /// Cogemos la coordenada Y
    QString ny = principal.firstChildElement("Y").toElement().text();

    /// Cogemos el ancho
    QString nwidth = principal.firstChildElement("WIDTH").toElement().text();

    /// Cogemos el alto
    QString nheight = principal.firstChildElement("HEIGHT").toElement().text();

    /// Establecemos la geometria de la ventana principal.
    m_bulmafact->setGeometry(nx.toInt(), ny.toInt(), nwidth.toInt(), nheight.toInt());

    /// Cogemos el indexador
    QString indexador = principal.firstChildElement("INDEXADOR").toElement().text();
    if (indexador == "TRUE") {
	s_indexadorCambiaEstado(TRUE);
	m_bulmafact->actionIndexador->setChecked(TRUE);
    } else {
	s_indexadorCambiaEstado(FALSE);
	m_bulmafact->actionIndexador->setChecked(FALSE);
    } // end if

    /// Tratamos cada ventana
   QWidget *activewindow = NULL;
   QDomNodeList nodos = docElem.elementsByTagName("VENTANA");
        for (int i = 0; i < nodos.count(); i++) {
            QDomNode ventana = nodos.item(i);
            QDomElement e1 = ventana.toElement(); /// try to convert the node to an element.
            if( !e1.isNull() ) { /// the node was really an element.
		     QString vname = e1.firstChildElement("VNAME").toElement().text();
			for (int j = 0; j < m_listventanas->numVentanas(); j++) {
				QObject *obj = m_listventanas->ventana(j);
				QWidget *wid = (QWidget *) obj;
				if (obj->objectName() == vname) {
					QString vx = e1.firstChildElement("VX").toElement().text();
					QString vy = e1.firstChildElement("VY").toElement().text();
					QString vwidth = e1.firstChildElement("VWIDTH").toElement().text();
					QString vheight = e1.firstChildElement("VHEIGHT").toElement().text();
					QString vvisible = e1.firstChildElement("VVISIBLE").toElement().text();
					QString vmaximized = e1.firstChildElement("VMAXIMIZED").toElement().text();
					QString vactivewindow = e1.firstChildElement("VACTIVEWINDOW").toElement().text();
					/// Establecemos la geometria de la ventana principal.
					wid->resize(vwidth.toInt(), vheight.toInt());
					wid->parentWidget()->move(vx.toInt(), vy.toInt());
					if ( vvisible == "TRUE") {
						wid->showNormal();
					} // end if
					if ( vmaximized == "TRUE") {
						wid->showMaximized();
					}
					if ( vactivewindow == "TRUE") {
						activewindow = wid;
					}
				} // end if
			} // end for
            } // end if
        } // end for
	/// Si hay una ventana activa se pone como activa.
	if (activewindow) 
		m_bulmafact->workspace()->setActiveWindow(activewindow);
}
