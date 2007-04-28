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

#ifndef PAGOVIEW_H
#define PAGOVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_pagobase.h"
#include "postgresiface2.h"
#include "pago.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "dialogchanges.h"
#include "ficha.h"


class company;


/// Muestra y administra la ventana con la informaci&oacute;n de un pago.
/** */
class PagoView : public Ficha, private Ui_PagoBase, public Pago {
    Q_OBJECT

public:
    PagoView(company *, QWidget *);
    ~PagoView();
    void manageArticle(int);
    void pintafechapago(QString id);
    void pintacomentpago(QString id);
    void pintaidproveedor(QString id);
    void pintarefpago(QString id);
    void pintacantpago(QString id);
    void pintaidbanco(QString id);
    void pintaprevisionpago(QString id);
    virtual int guardar() {return Pago::guardar();};
    virtual int borrar() {return Pago::borrar();};
    virtual int cargar(QString id);
    virtual int sacaWindow();

public slots:
    virtual void on_mui_comentpago_textChanged(const QString &str);
    virtual void on_mui_refpago_valueChanged(const QString &str);
    virtual void on_mui_cantpago_textChanged(const QString &str);
    virtual void on_mui_previsionpago_stateChanged(int i);
    virtual void on_mui_proveedor_valueChanged(QString id);
    virtual void on_mui_fechapago_valueChanged(QString id);
    virtual void on_mui_idbanco_valueChanged(QString id);
};

#endif

