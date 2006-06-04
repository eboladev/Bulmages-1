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

#ifndef FACTURAPVIEW_H
#define FACTURAPVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_facturapbase.h"
#include "facturap.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "dialogchanges.h"
#include "postgresiface2.h"
#include "funcaux.h"


class FacturaProveedorView : public QWidget,  public Ui_FacturaProveedorBase, public FacturaProveedor,
    public dialogChanges {
    Q_OBJECT
public:
    FacturaProveedorView(company *, QWidget *parent = 0, const char *name = 0);
    virtual ~FacturaProveedorView();
    void inicialize();
    void pintaidproveedor(QString id) {
        m_proveedor->setidproveedor(id);
    };
    void pintanumfacturap(QString id) {
        m_numfacturap->setText(id);
    };
    void pintafechafacturap(QString id) {
        m_fechafacturap->setText(id);
    };
    void pintadescfacturap(QString id) {
        m_descfacturap->setText(id);
    };
    void pintaComentFacturaProveedor(QString id) {
        m_comentfacturap->setText(id);
    };
    void pintareffacturap(QString id) {
        m_reffacturap->setText(id);
    };
    void pintaidforma_pago(QString id) {
        m_forma_pago->setidforma_pago(id);
    };
    void pintaprocesadafacturap(QString id) {
        if (id == "t" || id == "TRUE") {
            m_procesadafacturap->setChecked(TRUE);
        } else {
            m_procesadafacturap->setChecked(FALSE);
        }
    };
    virtual void pintatotales(Fixed base, Fixed iva);
    void closeEvent(QCloseEvent *);
    int guardar();

public slots:
    virtual void on_mui_guardar_clicked() {
        guardar();
    };
    virtual int cargar(QString id);
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales() {
        pintatotales(listalineas->calculabase(), listalineas->calculaiva());
    }
    virtual void s_nuevoCobro();
    virtual void on_m_descuentos_editFinish(int, int) {
        s_pintaTotales();
    };
    virtual void on_subform2_editFinish(int, int) {
        s_pintaTotales();
    };
    virtual void on_mui_borrar_clicked();
};

#endif

