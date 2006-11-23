/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef ALBARANCLIENTEVIEW_H
#define ALBARANCLIENTEVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "dialogchanges.h"
#include "fixed.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "ui_albaranclientebase.h"
#include "albarancliente.h"
#include "ficha.h"


class company;
//class Empresa;


/// Muestra el albar&aacute;n a cliente.
/** */
class AlbaranClienteView : public Ficha, public Ui_AlbaranClienteBase, public AlbaranCliente {
    Q_OBJECT

public:
    AlbaranClienteView(company *, QWidget *);
    //AlbaranClienteView(Empresa *, QWidget *);
    ~AlbaranClienteView();
    void inicializar();
    void pintaIdAlbaran(QString) {}
    ;
    void pintaNumAlbaran(QString val) {
        m_numalbaran->setText(val);
    };
    void pintafechaalbaran(QString val) {
        m_fechaalbaran->setText(val);
    };
    void pintaIdUsuario(QString) {}
    ;
    void pintaComentAlbaran(QString val) {
        m_comentalbaran->setPlainText(val);
    };
    void pintaComentPrivAlbaran(QString val) {
        m_comentprivalbaran->setPlainText(val);
    };
    void pintaidcliente(QString val) {
        m_cliente->setidcliente(val);
    };
    void pintaidforma_pago(QString val) {
        m_forma_pago->setidforma_pago(val);
    };
    void pintaIdFactura(QString) {}
    ;
    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    };
    void pintaidtrabajador(QString id) {
        m_trabajador->setidtrabajador(id);
    };
    void pintadescalbaran(QString val) {
        m_descalbaran->setText(val);
    };
    void pintarefalbaran(QString val) {
        m_refalbaran->setText(val);
    };
    void pintacontactalbaran(QString val) {
        m_contactalbaran->setText(val);
    };
    void pintatelalbaran(QString val) {
        m_telalbaran->setText(val);
    };
    void pintaprocesadoalbaran(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadoalbaran->setChecked(TRUE);
        } else {
            m_procesadoalbaran->setChecked(FALSE);
        }
    };
    void pintaNumFactura(QString) {}
    ;
    void pintatotales(Fixed, Fixed, Fixed, Fixed);
    void generarFactura();
    void agregarFactura();
    /// Estos m&eacute;todos deben existir para poder trabajar con la clase Ficha.
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {
        return AlbaranCliente::borrar();
    };
    virtual int sacaWindow();

public slots:
    virtual void on_mui_guardar_clicked() {
        guardar();
    };
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        calculaypintatotales();
    }
    virtual void s_verpresupuesto();
    virtual void on_mui_verpedidocliente_clicked();
    virtual void on_mui_imprimir_clicked() {
        imprimirAlbaranCliente();
    };
    virtual void on_mui_facturar_clicked() {
        generarFactura();
    };
    virtual void on_mui_agregarafactura_clicked() {
        agregarFactura();
    };
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform2_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_mui_cobrar_clicked();
};

#endif

