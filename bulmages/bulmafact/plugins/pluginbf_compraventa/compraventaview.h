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

#ifndef COMPRAVENTAVIEW_H
#define COMPRAVENTAVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "bldialogchanges.h"
#include "blfixed.h"
#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscaralmacen.h"
#include "bfbuscartrabajador.h"
#include "ui_compraventabase.h"
#include "bfform.h"


class BfCompany;


/// Muestra el albar&aacute;n a cliente.
/** Clase que globaliza los Albaranes a Cliente derivando de las clases Ficha y AlbaranCliente.
    Se encarga del control de la pantalla de albaranes a clientes, el paso de mensajes y la visualizacion.
    Contiene todos los metodos de pintado que son llamados desde AlbaranCliente.
*/
class CompraVentaView : public BfForm, public Ui_CompraVentaBase
{
    Q_OBJECT

public:
    BlDbRecord *m_albaranp;
    virtual int beforeDelete();
    virtual int cargarPost ( QString );
    CompraVentaView ( BfCompany *, QWidget * );
    ~CompraVentaView();
    void inicializar();

    void pintatotales ( BlFixed, BlFixed, BlFixed, BlFixed, BlFixed, BlFixed );
    void generarFactura();
    void generarFacturaProveedor();
    virtual int afterSave();
    void imprimir();

public slots:
    /// Este slot se activa cuando hay cambios en los subformularios.
    virtual void s_pintaTotales();
    virtual void on_m_descuentos_editFinish ( int, int );
    virtual void on_subform2_editFinish ( int, int );
    virtual void on_mui_cobrar_clicked();
    virtual void on_mui_idcliente_valueChanged ( QString id );
    virtual void on_mui_idalmacen_valueChanged ( QString id );
};

#endif

