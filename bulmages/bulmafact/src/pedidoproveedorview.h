/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef PEDIDOPROVEEDORVIEW_H
#define PEDIDOPROVEEDORVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_pedidoproveedorbase.h"
#include "factura.h"
#include "listlinpedidoproveedorview.h"
#include "listdescpedidoproveedorview.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "dialogchanges.h"
#include "fixed.h"
#include "ficha.h"
#include "pedidoproveedor.h"

class company;


class PedidoProveedorView : public Ficha, public Ui_PedidoProveedorBase, public PedidoProveedor {
    Q_OBJECT

public:
    PedidoProveedorView(company *, QWidget *parent = 0);
    ~PedidoProveedorView();
    void generarAlbaran();
    void inicialize();
    void pintaidproveedor(QString id) {
        m_proveedor->setidproveedor(id);
    };
    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    };
    void pintaidpedidoproveedor(QString) {}
    ;
    void pintanumpedidoproveedor(QString id) {
        m_numpedidoproveedor->setText(id);
    };
    void pintafechapedidoproveedor(QString id) {
        m_fechapedidoproveedor->setText(id);
    };
    void pintadescpedidoproveedor(QString id) {
        m_descpedidoproveedor->setText(id);
    };
    void pintaidforma_pago(QString id) {
        m_forma_pago->setidforma_pago(id);
    };
    void pintaidtrabajador(QString id) {
        m_trabajador->setidtrabajador(id);
    };
    void pintacomentpedidoproveedor(QString id) {
        m_comentpedidoproveedor->setPlainText(id);
    };
    void pintarefpedidoproveedor(QString id) {
        m_refpedidoproveedor->setText(id);
    };
    void pintacontactpedidoproveedor(QString id) {
        m_contactpedidoproveedor->setText(id);
    };
    void pintatelpedidoproveedor(QString id) {
        m_telpedidoproveedor->setText(id);
    };
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);
    void pintaprocesadopedidoproveedor(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadopedidoproveedor->setChecked(TRUE);
        } else {
            m_procesadopedidoproveedor->setChecked(FALSE);
        }
    };


    /// Estos metodos deben existir para poder trabajar con la clase Ficha
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {return PedidoProveedorView::borrar();};

public slots:
    virtual void on_mui_guardar_clicked() {
        guardar();
        dialogChanges_cargaInicial();
    };
    virtual void on_mui_imprimir_clicked() {
        imprimirPedidoProveedor();
    };

    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        calculaypintatotales();
    };
    virtual void on_mui_pagar_clicked();
    virtual void on_mui_facturar_clicked() {
        generarAlbaran();
    };
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform3_editFinish(int, int) {
        calculaypintatotales();
    };

};

#endif

