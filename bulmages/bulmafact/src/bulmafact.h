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

#ifndef BULMAFACT_H
#define BULMAFACT_H

#include <QMainWindow>
#include <QWorkspace>

#include "qworkspace2.h"
#include "ui_bulmafactbase.h"
#include "company.h"
#include "listventanas.h"
#include "funcaux.h"


class bulmafact: public QMainWindow, public Ui_bulmafactbase {
    Q_OBJECT

private:
    QWorkspace2 *pWorkspace;
    company *m_company;
    listventanas *m_list;

public:
    bulmafact(QString bd);
    ~bulmafact();
    QWorkspace2 *workspace() {
        return pWorkspace;
    };
    company *getcompany() {
        return m_company;
    };

public slots:
    virtual void closeEvent(QCloseEvent *);
    virtual void emitirfactura();
    virtual void recibirfactura();
    virtual void s_FPago();
    virtual void s_ventanaCompleta();
    virtual void s_About();
    void about();
    void aboutQt();
    virtual void on_actionNuevo_Pedido_Proveedor_triggered() {
        m_company->s_newPedidoPro();
    };
    virtual void on_actionListado_Pedidos_Proveedor_triggered() {
        m_company->s_listPedidosPro();
    };
    virtual void on_actionNuevo_Albaran_Proveedor_triggered() {
        m_company->s_newAlbaranPro();
    };
    virtual void on_actionListado_Albaranes_Proveedor_triggered() {
        m_company->lAlbaranesProveedor();
    };
    virtual void on_actionNueva_Factura_Proveedor_triggered() {
        m_company->s_newFacturaPro();
    };
    virtual void on_actionListado_Facturas_Proveedor_triggered() {
        m_company->s_listFacturasPro();
    };
    virtual void on_actionNuevo_Presupuesto_triggered() {
        m_company->s_newPresupuestoCli();
    };
    virtual void on_actionListado_Presupuestos_triggered() {
        m_company->listBudgets();
    };
    virtual void on_actionNuevo_Pedido_Cliente_triggered() {
        m_company->s_newPedidoClienteView();
    };
    virtual void on_actionListado_Pedidos_Cliente_triggered() {
        m_company->s_listPedidosCli();
    };
    virtual void on_actionNuevo_Albaran_Cliente_triggered() {
        m_company->s_newAlbaranClienteView();
    };
    virtual void on_actionListado_Albaranes_Cliente_triggered() {
        m_company->listClientDelivNotes();
    };
    virtual void on_actionNueva_Factura_Cliente_triggered() {
        m_company->s_newFacturaCli();
    };
    virtual void on_actionListado_Facturas_Cliente_triggered() {
        m_company->s_listFacturasCli();
    };
    virtual void on_actionGestion_Familias_triggered() {
        m_company->s_newfamiliasview();
    };
    virtual void on_actionGestion_Tipos_Articulo_triggered() {
        m_company->s_newTipoArticuloList();
    };
    virtual void on_actionNuevo_Articulo_triggered() {
        m_company->s_newArticulo();
    };
    virtual void on_actionListado_de_Articulos_triggered() {
        m_company->listarticles();
    };
    virtual void on_actionInventarios_triggered() {
        m_company->s_inventarios();
    };
    virtual void on_actionNuevo_Proveedor_triggered() {
        m_company->s_newProveedorView();
    };
    virtual void on_actionListado_Proveedores_triggered() {
        m_company->listproviders();
    };
    virtual void on_actionNuevo_Cliente_triggered() {
        m_company->s_newClienteView();
    };
    virtual void on_actionListado_Clientes_triggered() {
        m_company->listClients();
    };
    virtual void on_actionProvincias_triggered() {
        m_company->s_provincias();
    };
    virtual void on_actionSeries_de_Factura_triggered() {
        m_company->s_seriesFactura();
    };
    virtual void on_actionTrabajadores_triggered() {
        m_company->s_trabajadores();
    };
    virtual void on_actionVentana_Completa_triggered() {
        s_ventanaCompleta();
    };
    virtual void on_actionAcerta_de_triggered() {
        s_About();
    };
    virtual void on_actionAlmacenes_triggered() {
        m_company->s_almacenes();
    };
    virtual void on_actionListado_de_Pagos_triggered() {
        m_company->viewPagosList();
    };
    virtual void on_actionNuevo_Pago_triggered() {
        m_company->s_newPagoView();
    };
    virtual void on_actionNuevo_Cobro_triggered() {
        m_company->s_newCobroView();
    };
    virtual void on_actionListado_de_Cobros_triggered() {
        m_company->viewCobrosList();
    };
    virtual void on_actionParametros_triggered() {
        m_company->s_newListConfiguracionView();
    };
    virtual void on_actionFormas_de_Pago_triggered() {
        s_FPago();
    };
    virtual void on_actionIndexador_triggered() {
        m_company->s_indexadorCambiaEstado();
    }
};

#endif

