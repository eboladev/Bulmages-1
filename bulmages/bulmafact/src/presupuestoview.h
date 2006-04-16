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

#ifndef BUDGET_H
#define BUDGET_H

// PRESUPUESTOS.

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_budgetbase.h"
#include "postgresiface2.h"
#include "presupuesto.h"
#include "listlinpresupuestoview.h"
#include "listdescpresupuestoview.h"
#include "busquedacliente.h"
#include "busquedaformapago.h"
#include "busquedafecha.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "fixed.h"
#include "dialogchanges.h"


class company;

class PresupuestoView : public QWidget, public Ui_BudgetBase , public presupuesto, public dialogChanges {
    Q_OBJECT

private:
    cursor2 *m_cursorcombo;

public:
    PresupuestoView(company *, QWidget *, const char *);
    ~PresupuestoView();
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);
    void inicialize();
    void manageArticle(int);
    void pintaNumPresupuesto(QString id) {
        m_numpresupuesto->setText(id);
    };
    void pintaFPresupuesto(QString id) {
        m_fpresupuesto->setText(id);
    };
    void pintaVencPresupuesto(QString id) {
        m_vencpresupuesto->setText(id);
    };
    void pintaContractPresupuesto(QString id) {
        m_contactpresupuesto->setText(id);
    };
    void pintaTelPresupuesto(QString id) {
        m_telpresupuesto->setText(id);
    };
    void pintaComentPresupuesto(QString id) {
        m_comentpresupuesto->setText(id);
    };
    void pintaidcliente(QString id) {
        m_cliente->setidcliente(id);
    };
    void pintarefpresupuesto(QString id) {
        m_refpresupuesto->setText(id);
    };
    void pintaidforma_pago(QString id) {
        m_forma_pago->setidforma_pago(id);
    };
    void pintaidalmacen(QString id) {
        m_almacen->setidalmacen(id);
    };
    void pintaidtrabajador(QString id) {
        m_trabajador->setidtrabajador(id);
    };
    void pintaprocesadopresupuesto(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadopresupuesto->setChecked(TRUE);
        } else {
            m_procesadopresupuesto->setChecked(FALSE);
        }
    };
    void pintadescpresupuesto(QString id) {
        m_descpresupuesto->setText(id);
    };

private:
    void generarPedidoCliente();
    virtual void closeEvent(QCloseEvent *);
    virtual int guardar();

public slots:
    virtual void on_mui_informereferencia_clicked();
    virtual int cargar(QString id);
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_guardar_clicked() {
        guardar();
    };
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        calculaypintatotales();
    };
    virtual void on_mui_pasarapedido_clicked() {
        generarPedidoCliente();
    };
    virtual void on_m_descuentos_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_subform2_editFinish(int, int) {
        calculaypintatotales();
    };
    virtual void on_mui_aceptar_clicked() {
        if (!guardar() )
            close();
    };
};

#endif
