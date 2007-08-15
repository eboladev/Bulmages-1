/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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


/// Muestra el albar&aacute;n a cliente.
/** Clase que globaliza los Albaranes a Cliente derivando de las clases Ficha y AlbaranCliente.
    Se encarga del control de la pantalla de albaranes a clientes, el paso de mensajes y la visualizacion.
    Contiene todos los metodos de pintado que son llamados desde AlbaranCliente.
*/
class AlbaranClienteView : public AlbaranCliente, public Ui_AlbaranClienteBase {
    Q_OBJECT

public:
    AlbaranClienteView(Company *, QWidget *);
    ~AlbaranClienteView();
    void inicializar();
    void pintaIdAlbaran(QString);
    void pintaNumAlbaran(QString val);
    void pintafechaalbaran(QString val);
    void pintaIdUsuario(QString);
    void pintaComentAlbaran(QString val);
    void pintaComentPrivAlbaran(QString val);
    void pintaidcliente(QString val);
    void pintaidforma_pago(QString val);
    void pintaIdFactura(QString);
    void pintaidalmacen(QString id);
    void pintaidtrabajador(QString id);
    void pintadescalbaran(QString val);
    void pintarefalbaran(QString val);
    void pintacontactalbaran(QString val);
    void pintatelalbaran(QString val);
    void pintaprocesadoalbaran(QString id);
    void pintaNumFactura(QString);
    void pintatotales(Fixed, Fixed, Fixed, Fixed, Fixed, Fixed);
    void generarFactura();
    void agregarFactura();
    /// Estos m&eacute;todos deben existir para poder trabajar con la clase Ficha.
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar();

public slots:
    virtual void on_mui_guardar_clicked();
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales();
    virtual void s_verpresupuesto();
    virtual void on_mui_verpedidocliente_clicked();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_facturar_clicked();
    virtual void on_mui_agregarafactura_clicked();
    virtual void on_m_descuentos_editFinish(int, int);
    virtual void on_subform2_editFinish(int, int);
    virtual void on_mui_cobrar_clicked();
    virtual void on_m_cliente_valueChanged(QString id);
};

#endif

