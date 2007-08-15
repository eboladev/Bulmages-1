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

#ifndef PEDIDOCLIENTEVIEW_H
#define PEDIDOCLIENTEVIEW_H

#include <QLineEdit>
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
#include "ficha.h"


/// Muestra y administra la ventana con la informaci&oacute;n de un pedido de cliente.
/** */
class PedidoClienteView : public PedidoCliente, public Ui_PedidoClienteBase {
    Q_OBJECT

public:
    PedidoClienteView(Company *, QWidget *parent = 0);
    ~PedidoClienteView();
    void generarAlbaran();
    void inicializar();
    void pintaidcliente(QString id) {
        m_cliente->setidcliente(id);
    }
    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    }
    void pintaidpedidocliente(QString) {}
    void pintanumpedidocliente(QString id) {
        m_numpedidocliente->setText(id);
    }
    void pintafechapedidocliente(QString id) {
        m_fechapedidocliente->setText(id);
    }
    void pintadescpedidocliente(QString id) {
        m_descpedidocliente->setText(id);
    }
    void pintaidforma_pago(QString id) {
        m_forma_pago->setidforma_pago(id);
    }
    void pintaidtrabajador(QString id) {
        m_trabajador->setidtrabajador(id);
    }
    void pintacomentpedidocliente(QString id) {
        m_comentpedidocliente->setPlainText(id);
    }
    void pintarefpedidocliente(QString id) {
        m_refpedidocliente->setText(id);
    }
    void pintacontactpedidocliente(QString id) {
        m_contactpedidocliente->setText(id);
    }
    void pintatelpedidocliente(QString id) {
        m_telpedidocliente->setText(id);
    }
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed, Fixed);
    void pintaprocesadopedidocliente(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadopedidocliente->setChecked(TRUE);
        } else {
            m_procesadopedidocliente->setChecked(FALSE);
        } // end if
    }
    /// Estos m&eacute;todos deben existir para poder trabajar con la clase Ficha
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {
        return PedidoCliente::borrar();
    }

public slots:
    virtual void on_mui_guardar_clicked() {
        guardar();
    }
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    }
    virtual void s_pintaTotales() {
        calculaypintatotales();
    }
    virtual void on_mui_verpresupuesto_clicked();
    virtual void on_mui_pasaraalbaran_clicked() {
	   _depura("on_mui_pasaraalbaran_clicked", 0);
       generarAlbaran();
	   _depura("END on_mui_pasaraalbaran_clicked", 0);
    };
    virtual void on_mui_cobrar_clicked();
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform3_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_m_cliente_valueChanged(QString id);
};

#endif

