/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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


/** Se encarga de la ventana de albar&aacute;n de proveedor.
    Deriva de Ficha para temas de visualizacion y de AlbaranProveedor para cuestiones
    de manejo de la base de datos.
 */
class AlbaranProveedorView : public AlbaranProveedor, public Ui_AlbaranProveedorBase {
    Q_OBJECT

public:
    AlbaranProveedorView(company *, QWidget *);
    ~AlbaranProveedorView();
    void inicializar();
    void pintaidalbaranp(QString);
    void pintanumalbaranp(QString val);
    void pintafechaalbaranp(QString val);
    void pintaIdUsuario(QString);
    void pintaComentAlbaran(QString val);
    void pintaidproveedor(QString val);
    void pintaidforma_pago(QString val);
    void pintaidalmacen(QString id);
    void pintadescalbaranp(QString val);
    void pintarefalbaranp(QString val);
    void pintatotales(Fixed, Fixed, Fixed, Fixed, Fixed, Fixed);
    void generarFactura();
    /// Estos metodos deben existir para poder trabajar con la clase Ficha
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar();
    void generarFacturaProveedor();

public slots:
    virtual void on_mui_facturar_clicked();
    virtual void s_comentalbaranptextChanged();
    virtual void s_almacenvalueChanged(QString val);
    virtual void s_numalbaranptextChanged(const QString &val);
    virtual void s_proveedorvalueChanged(QString val);
    virtual void s_fechaalbaranpvalueChanged(QString val);
    virtual void s_forma_pagovalueChanged(QString val);
    virtual void s_refalbaranptextChanged(const QString &val);
    virtual void s_descalbaranptextChanged(const QString &val);
    virtual void on_mui_guardar_clicked();
    virtual void s_printAlbaranProveedor();
    virtual void s_verpedidoproveedor();
    virtual void s_generarFactura();
    virtual void on_m_descuentos_editFinish(int, int);
    virtual void on_subform2_editFinish(int, int);
    virtual void on_mui_pagar_clicked();
    virtual void on_mui_verpedidosproveedor_clicked();
    virtual void on_m_proveedor_valueChanged(QString);
};

#endif

