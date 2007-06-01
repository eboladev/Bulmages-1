/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef COMPANY_H
#define COMPANY_H

#include <QObject>
#include <QAssistantClient>
#include <QLibraryInfo>

#include "postgresiface2.h"
#include "listventanas.h"
#include "qworkspace2.h"
#include "empresabase.h"


class ProveedorList;
class ClientsList;
class ClienteView;
class ArticuloList;
class orderslist;
class PresupuestoList;
class AlbaranClienteList;
class FacturasList;
class PedidosClienteList;
class PedidosProveedorList;
class AlbaranesProveedor;
class CobrosList;
class FacturasProveedorList;
class PagosList;
class PresupuestoView;
class FacturaView;
class FacturaProveedorView;
class PedidoProveedorView;
class ArticuloView;
class CobroView;
class PagoView;
class AlbaranClienteView;
class AlbaranProveedorView;
class PedidoClienteView;
class TipoArticuloList;
class ProveedorView;
class FamiliasView;
class ListTipoIVAView;


/// Clase company (empresa).
/** Clase principal del programa donde se almacenan y gestionan
    todos los datos de la empresa con la que se est&aacute; trabajando.
    Deriva de postgresiface ya que tiene el tratamiento de la base de datos. */
class company : public EmpresaBase {
private:
    /// Puntero a la ventana de lista de proveedores. Esta ventana se abre una vez al principio.
    ProveedorList *m_providerslist;
    /// Puntero a la ventan ade lista de clientes. Esta ventana se abre una vez al inicio del programa.
    ClientsList *m_clientsList;
    /// Puntero a la ventana de lista de articulos. Esta ventan ase abre una vez al inicio del programa.
    ArticuloList *m_articleslist;
    /// Puntero a la ventana de lista de presupuestos. Esta ventana se abre una vez al inicio del program.a \TODO: Cambiarle el nombre.
    PresupuestoList *m_presupuestosList;
    /// Puntero al a ventana de lista de albarnaes a cliente. Esta ventana se crea una vez al inicio del programa.
    AlbaranClienteList *m_clientDelivNotesList;
    /// Puntero a la ventana de lista de facturas a cliente. Esta ventana se crea una vez al inicoi del programa.
    FacturasList *m_facturasList;
    /// Puntero a la lista de pedidos de cliente. Esta ventana se crea una vez al inicio del programa.
    PedidosClienteList *m_pedidosclienteList;
    /// Puntero a la lista de pedidos a proveedor. Esta ventana se crea una vez al inicoi del programa.
    PedidosProveedorList *m_pedidosproveedorList;
    /// Puntero a la lista de albaranes de proveedor. Esta ventana se crea una vez al inicio del programa.
    AlbaranesProveedor *m_albaranesproveedor;
    /// Puntero al a lista de cobros. Esta ventana se crea una vez al inicio del programa.
    CobrosList *m_cobrosList;
    /// Puntero a la ventan ade facturas de proveedor. Esta ventana se crea una vez al inicio del programa.
    FacturasProveedorList *m_facturasproveedorlist;
    /// Puntero a la ventana de pagos. Esta ventana se crea una vez al inicio del programa.
    PagosList *m_pagosList;

public:
    company();
    ~company();
    FacturasProveedorList *facturasProveedorList();
    PresupuestoList *presupuestoList();
    void listproviders();
    void listClients();
    void listarticles();
    void listorders();
    void lAlbaranesProveedor();
    void listBudgets();
    void listClientDelivNotes();
    void newClientDelivNote();
    void newPedidoCliente();
    ClienteView *newClienteView();
    ProveedorView *newProveedorView();
    ArticuloView *newArticuloView();
    void createMainWindows();
    void refreshArticles();
    void refreshPresupuestos();
    void refreshCobrosCliente();
    void refreshPedidosCliente();
    void refreshPedidosProveedor();
    void refreshClientDelivNotes();
    void refreshAlbaranesCliente();
    void refreshAlbaranesProveedor();
    void refreshFacturas();
    void refreshFacturasProveedor();
    void refreshClientes();
    void s_FPago();
    void s_Familias();
    void s_trabajadores();
    void s_bancos();
    void s_seriesFactura();
    void s_listPedidosCli();
    void s_listFacturasCli();
    void s_listPedidosPro();
    void s_listFacturasPro();
    void s_newFacturaPro();
    void s_newAlbaranPro();
    void s_newPedidoPro();
    void s_newPresupuestoCli();
    void s_newPedidoClienteView();
    void s_newPedidoProveedorView();
    void s_newAlbaranClienteView();
    void s_newAlbaranProveedorView();
    void s_newFacturaCli();
    void s_newArticulo();
    void s_newCobroView();
    void s_newPagoView();
    void s_newClienteView();
    void s_newProveedorView();
    void s_newListConfiguracionView();
    void s_newTipoArticuloList();
    void s_newfamiliasview();
    void s_provincias();
    void s_almacenes();
    void viewPagosList();
    void viewCobrosList();
    void s_TipoIVAView();
    void s_TasaIVAView();
    FacturaProveedorView *newFacturaProveedorView();
    PresupuestoView *nuevoPresupuestoView();
    PedidoProveedorView *nuevoPedidoProveedorView();
    FacturaView *newFacturaView();
    CobroView *newCobroView();
    PagoView *newPagoView();
    AlbaranClienteView *newAlbaranClienteView();
    AlbaranProveedorView *newAlbaranProveedorView();
    PedidoClienteView *newPedidoClienteView();
    TipoArticuloList *newTipoArticuloList(QWidget *parent = 0, bool modoConsulta = FALSE);
    FamiliasView *newfamiliasview(QWidget *parent = 0, bool modoConsulta = FALSE);
};

#endif

