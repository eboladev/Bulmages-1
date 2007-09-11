/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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


/// Muestra y administra la ventana con la informaci&oacute;n de un pedido a proveedor.
/** */
class PedidoProveedorView : public PedidoProveedor, public Ui_PedidoProveedorBase {
    Q_OBJECT

public:
    PedidoProveedorView(Company *, QWidget *parent = 0);
    ~PedidoProveedorView();
    void generarAlbaran();
    void inicialize();
    void pintaidproveedor(QString id) {
        mui_proveedor->setidproveedor(id);
    };
    void pintaidalmacen(QString id) {
        mui_almacenDestino->setidalmacen(id);
    };
    void pintaidpedidoproveedor(QString) {}
    ;
    void pintanumpedidoproveedor(QString id) {
        mui_numeroPedido->setText(id);
    };
    void pintafechapedidoproveedor(QString id) {
        mui_fechaPedido->setText(id);
    };
    void pintadescpedidoproveedor(QString id) {
        mui_descripcionPedido->setText(id);
    };
    void pintaidforma_pago(QString id) {
        mui_formaPago->setidforma_pago(id);
    };
    void pintaidtrabajador(QString id) {
        mui_trabajador->setidtrabajador(id);
    };
    void pintacomentpedidoproveedor(QString id) {
        mui_comentarios->setPlainText(id);
    };
    void pintarefpedidoproveedor(QString id) {
        mui_referenciaPedido->setText(id);
    };
    void pintacontactpedidoproveedor(QString id) {
        mui_personaContacto->setText(id);
    };
    void pintatelpedidoproveedor(QString id) {
        mui_telefonoContacto->setText(id);
    };
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq);
    void pintaprocesadopedidoproveedor(QString id) {
        if (id == "t" || id == "TRUE") {
            mui_tramitadoPedido->setChecked(TRUE);
        } else {
            mui_tramitadoPedido->setChecked(FALSE);
        }
    };
    /// Estos m&eacute;todos deben existir para poder trabajar con la clase Ficha.
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {
        return PedidoProveedor::borrar();
    };

public slots:
    virtual void on_mui_proveedor_valueChanged(QString);
    virtual void on_mui_guardar_clicked() {
        guardar();
        dialogChanges_cargaInicial();
    };
    virtual void on_mui_imprimir_clicked() {
        imprimirPedidoProveedor();
    };
    /// Este SLOT se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        calculaypintatotales();
    };
    virtual void on_mui_pagar_clicked();
    virtual void on_mui_facturar_clicked() {
        generarAlbaran();
    };
    virtual void on_mui_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_mui_lineasDetalle_editFinish(int, int) {
        calculaypintatotales();
    };
};

#endif

