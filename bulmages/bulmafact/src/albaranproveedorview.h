/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef ALBARANPROVEEDORVIEW_H
#define ALBARANPROVEEDORVIEW_H

#include <QLineEdit>
#include <QLabel>

#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "ui_albaranproveedorbase.h"
#include "albaranproveedor.h"
#include "ficha.h"


class company;
//class Empresa;


/** Se encarga de la ventana de albar&aacute;n de proveedor.
    Deriva de Ficha para temas de visualizacion y de AlbaranProveedor para cuestiones
    de manejo de la base de datos.
 */
class AlbaranProveedorView : public Ficha, public Ui_AlbaranProveedorBase, public AlbaranProveedor {
    Q_OBJECT

public:
    AlbaranProveedorView(company *, QWidget *);
    ~AlbaranProveedorView();
    void inicializar();
    void pintaidalbaranp(QString) {}
    ;
    void pintanumalbaranp(QString val) {
        m_numalbaranp->setText(val);
    };
    void pintafechaalbaranp(QString val) {
        m_fechaalbaranp->setText(val);
    };
    void pintaIdUsuario(QString);
    void pintaComentAlbaran(QString val) {
        m_comentalbaranp->setPlainText(val);
    };
    void pintaidproveedor(QString val) {
        m_proveedor->setidproveedor(val);
    };
    void pintaidforma_pago(QString val) {
        m_forma_pago->setidforma_pago(val);
    };

    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    };
    void pintadescalbaranp(QString val) {
        m_descalbaranp->setText(val);
    };
    void pintarefalbaranp(QString val) {
        m_refalbaranp->setText(val);
    };
    void pintatotales(Fixed, Fixed);
    void generarFactura();

    /// Estos metodos deben existir para poder trabajar con la clase Ficha
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {
        return AlbaranProveedor::borrar();
    };
    virtual int sacaWindow();

public slots:
    virtual void s_comentalbaranptextChanged() {
        setcomentalbaranp(m_comentalbaranp->toPlainText());
    };
    virtual void s_almacenvalueChanged(QString val) {
        setidalmacen(val);
    };
    virtual void s_numalbaranptextChanged(const QString &val) {
        setnumalbaranp(val);
    };
    virtual void s_proveedorvalueChanged(QString val) {
        setidproveedor(val);
    };
    virtual void s_fechaalbaranpvalueChanged(QString val) {
        setfechaalbaranp(val);
    };
    virtual void s_forma_pagovalueChanged(QString val) {
        setidforma_pago(val);
    };
    virtual void s_refalbaranptextChanged(const QString &val) {
        setrefalbaranp(val);
    };
    virtual void s_descalbaranptextChanged(const QString &val) {
        setdescalbaranp(val);
    };
    virtual void on_mui_guardar_clicked();
    virtual void s_printAlbaranProveedor() {}
    ;
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }
    virtual void s_verpedidoproveedor();
    virtual void s_imprimirAlbaranProveedor() {
        imprimirAlbaranProveedor();
    };
    virtual void s_generarFactura() {
        generarFactura();
    };
    virtual void calculaypintatotales() {
        s_pintaTotales();
    };
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform2_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_mui_pagar_clicked();
    virtual void on_mui_verpedidosproveedor_clicked();
};

#endif

