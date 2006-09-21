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
#include <QWorkspace>

#include "postgresiface2.h"
#include "listventanas.h"
#include "qworkspace2.h"


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
class familiasview;


/// Clase company (empresa).
/** Clase principal del programa donde se almacenan y gestionan
    todos los datos de la empresa con la que se est&aacute; trabajando.*/
class company : public postgresiface2 {
private:
    listventanas *m_listventanas;
    ProveedorList *m_providerslist;
    ClientsList *m_clientsList;
    ArticuloList *m_articleslist;
    PresupuestoList *m_budgetsList;
    AlbaranClienteList *m_clientDelivNotesList;
    FacturasList *m_facturasList;
    PedidosClienteList *m_pedidosclienteList;
    PedidosProveedorList *m_pedidosproveedorList;
    AlbaranesProveedor *m_albaranesproveedor;
    CobrosList *m_cobrosList;
    FacturasProveedorList *m_facturasproveedorlist;
    PagosList *m_pagosList;

public:
    QWorkspace2 *m_pWorkspace;

public:
    company();
    ~company();
    void init(QString);
    void setListVentanas(listventanas *doc) {
        m_listventanas = doc;
    };
    void setWorkspace(QWorkspace2 *qw) {
        m_pWorkspace = qw;
    }
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
    void refreshBudgets();
    void refreshPedidosCliente();
    void refreshPedidosProveedor();
    void refreshClientDelivNotes();
    void refreshAlbaranesCliente();
    void refreshAlbaranesProveedor();
    void refreshFacturas();
    void refreshFacturasProveedor();
    void refreshClientes();
    int meteWindow(QString nom, QObject *obj, bool compdup = TRUE) {
        return m_listventanas->meteWindow(nom, obj, compdup);
    };
    void sacaWindow(QObject *nom) {
        m_listventanas->sacaWindow(nom);
    };
    void s_FPago();
    void s_Familias();
    void s_trabajadores();
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
    void s_inventarios();
    void s_newInventario();
    void viewPagosList();
    void viewCobrosList();
    void s_indexadorCambiaEstado();
    QString searchCompany();
    FacturaProveedorView *newFacturaProveedorView();
    PresupuestoView *newBudget();
    FacturaView *newFacturaView();
    CobroView *newCobroView();
    PagoView *newPagoView();
    AlbaranClienteView *newAlbaranClienteView();
    AlbaranProveedorView *newAlbaranProveedorView();
    PedidoClienteView *newPedidoClienteView();
    PedidoProveedorView *newPedidoProveedorView();
    TipoArticuloList *newTipoArticuloList(QWidget *parent = 0, bool modoConsulta = FALSE);
    familiasview *newfamiliasview(QWidget *parent = 0, bool modoConsulta = FALSE);
};

#endif

