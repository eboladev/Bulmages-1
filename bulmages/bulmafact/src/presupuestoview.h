/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef PRESUPUESTOCLIENTE_H
#define PRESUPUESTOCLIENTE_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_presupuestoclientebase.h"
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
#include "ficha.h"


class Company;


/// Muestra y administra la ventana con la informaci&oacute;n de un presupuesto.
/** */
class PresupuestoView : public Presupuesto, public Ui_PresupuestoClienteBase {
    Q_OBJECT

private:
    cursor2 *m_cursorcombo;

public:
    PresupuestoView(Company *, QWidget *);
    ~PresupuestoView();
    void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq);
    void inicializar();
    void manageArticle(int);
    void pintaNumPresupuesto(QString id);
    void pintaFPresupuesto(QString id);
    void pintaVencPresupuesto(QString id);
    void pintaContractPresupuesto(QString id);
    void pintaTelPresupuesto(QString id);
    void pintaComentPresupuesto(QString id);
    void pintaidcliente(QString id);
    void pintarefPresupuesto(QString id);
    void pintaidforma_pago(QString id);
    void pintaidalmacen(QString id);
    void pintaidtrabajador(QString id);
    void pintaprocesadoPresupuesto(QString id);
    void pintadescPresupuesto(QString id);

private:
    void generarPedidoCliente();

public:
    /// Estos m&eacute;todos deben existir para poder trabajar con la clase Ficha.
    virtual int guardar();
    virtual int cargar(QString id);

public slots:
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_imprimir_clicked();
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales();
    virtual void on_mui_pasarapedido_clicked();
    virtual void on_m_descuentos_editFinish(int, int);
    virtual void on_subform2_editFinish(int, int);
    virtual void on_m_cliente_valueChanged(QString id);
};

#endif

