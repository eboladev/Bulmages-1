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

#ifndef FACTURAVIEW_H
#define FACTURAVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_facturabase.h"
#include "factura.h"
#include "listlinpresupuestoview.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaseriefactura.h"
#include "busquedaalmacen.h"
#include "dialogchanges.h"
#include "fichabf.h"


class company;


/// Muestra y administra la ventana de una factura a cliente.
/** */
class FacturaView : public Factura, public Ui_FacturaBase {
    Q_OBJECT

public:
    FacturaView(company *, QWidget *parent = 0);
    ~FacturaView();
    void inicializar();
    void pintaidcliente(QString id) {
        m_cliente->setidcliente(id);
    };
    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    };
    void pintaNumFactura(QString id) {
        m_numfactura->setText(id);
    };
    void pintacodigoserie_factura(QString id) {
        m_codigoserie_factura->setcodigoserie_factura(id);
    };
    void pintafechafactura(QString id) {
        m_fechafactura->setText(id);
    };
    void pintadescfactura(QString id) {
        m_descfactura->setText(id);
    };
    void pintaComentFactura(QString id) {
        m_comentfactura->setPlainText(id);
    };
    void pintareffactura(QString id) {
        m_reffactura->setText(id);
    };
    void pintaidforma_pago(QString id) {
        m_forma_pago->setidforma_pago(id);
    };
    void pintaprocesadafactura(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadafactura->setChecked(TRUE);
        } else {
            m_procesadafactura->setChecked(FALSE);
        } // end if
    };
    void pintatotales(Fixed, Fixed, Fixed, Fixed, Fixed, Fixed);

    /// Estos metodos deben existir para poder trabajar con la clase Ficha
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {
        return Factura::borrar();
    };

public slots:
    virtual void on_mui_guardar_clicked() {
        guardar();
    };
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    };
    virtual void on_mui_agregaralbaran_clicked();
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        calculaypintatotales();
    };
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform2_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_veralbaranes_clicked();
    virtual void on_m_cliente_valueChanged(QString id);
};

#endif

