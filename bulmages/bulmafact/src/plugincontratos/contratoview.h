/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef CONTRATOVIEW_H
#define CONTRATOVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_contratobase.h"
#include "contrato.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaseriefactura.h"
#include "busquedaalmacen.h"
#include "dialogchanges.h"
#include "fichabf.h"


class company;


/// Muestra y administra la ventana de una contrato a cliente.
/** */
class ContratoView : public Contrato, public Ui_ContratoBase {
    Q_OBJECT

public:
    ContratoView(company *, QWidget *parent = 0);
    ~ContratoView();
    void inicializar();
    void pintaidcliente(QString id) {
        mui_idcliente->setidcliente(id);
    };
    void pintafincontrato(QString id) {
        mui_fincontrato->setText(id);
    };
    void pintadesccontrato(QString id) {
        mui_desccontrato->setText(id);
    };
    void pintarefcontrato(QString id) {
        mui_refcontrato->setText(id);
    };

    /// Estos metodos deben existir para poder trabajar con la clase Ficha
    virtual int guardar();
    virtual int cargar(QString id);
    virtual int borrar() {
        return Contrato::borrar();
    };

public slots:
/*
    virtual void on_mui_guardar_clicked() {
        guardar();
    };
    virtual void on_mui_imprimir_clicked() {
        imprimir();
    };
*/
    virtual void on_m_cliente_valueChanged(QString id);
};

#endif

