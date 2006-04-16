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

#ifndef PEDIDOCLIENTEVIEW_H
#define PEDIDOCLIENTEVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_pedidoclientebase.h"
#include "factura.h"
#include "listlinpedidoclienteview.h"
#include "listdescpedidoclienteview.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "dialogchanges.h"
#include "fixed.h"
#include "pedidocliente.h"
#include "postgresiface2.h"


class PedidoClienteView : public QWidget, public Ui_PedidoClienteBase, public PedidoCliente, public dialogChanges {
    Q_OBJECT
public:
    PedidoClienteView(company *, QWidget *parent = 0, const char *name = 0);
    ~PedidoClienteView();
    void closeEvent(QCloseEvent *);
    void generarAlbaran();
    void pintaidcliente(QString id) {
        m_cliente->setidcliente(id);
    };
    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    };
    void pintaidpedidocliente(QString) {}
    ;
    void pintanumpedidocliente(QString id) {
        m_numpedidocliente->setText(id);
    };
    void pintafechapedidocliente(QString id) {
        m_fechapedidocliente->setText(id);
    };
    void pintadescpedidocliente(QString id) {
        m_descpedidocliente->setText(id);
    };
    void pintaidforma_pago(QString id) {
        m_forma_pago->setidforma_pago(id);
    };
    void pintaidtrabajador(QString id) {
        m_trabajador->setidtrabajador(id);
    };
    void pintacomentpedidocliente(QString id) {
        m_comentpedidocliente->setText(id);
    };
    void pintarefpedidocliente(QString id) {
        m_refpedidocliente->setText(id);
    };
    void pintacontactpedidocliente(QString id) {
        m_contactpedidocliente->setText(id);
    };
    void pintatelpedidocliente(QString id) {
        m_telpedidocliente->setText(id);
    };
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);
    void pintaprocesadopedidocliente(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadopedidocliente->setChecked(TRUE);
        } else {
            m_procesadopedidocliente->setChecked(FALSE);
        }
    };

public slots:
    virtual void on_mui_guardar_clicked() {
        guardar();
    };

    virtual int cargar(QString id);

    virtual void on_mui_borrar_clicked() {
        int val = QMessageBox::warning( this, tr("Borrar Pedido Cliente."),
                                        tr("ADVERTENCIA: Eliminar pedidos de cliente puede disminuir la eficacia economica de la empresa. Continuar?"),tr("SI"),tr("No"),tr("Ojala"),0,2);
        if (val == 0) {

            if (!borrar()) {
                dialogChanges_cargaInicial();
                _depura("Pedido borrado satisfactoriamente",2);
                close();
            }// end if
        }// end if
    };

    virtual void on_mui_imprimir_clicked() {
        imprimirPedidoCliente();
    };

    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual int guardar();
    virtual void s_pintaTotales() {
        calculaypintatotales();
    };
    virtual void on_mui_verpresupuesto_clicked();

    virtual void on_mui_pasaraalbaran_clicked() {
        generarAlbaran();
    };

    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_informereferencia_clicked();
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform3_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_mui_aceptar_clicked() {
        if (!guardar() )
            close();
    };
};

#endif
