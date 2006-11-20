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

#ifndef COBROVIEW_H
#define COBROVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_cobrobase.h"
#include "postgresiface2.h"
#include "cobro.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "dialogchanges.h"
#include "ficha.h"


class company;

/// Administra la ventana de con los datos de un cobro.
/** */
class CobroView : public Ficha, private Ui_CobroBase, public Cobro {
    Q_OBJECT

public:
    CobroView(company *, QWidget *);
    ~CobroView();
    void manageArticle(int);
    void pintafechacobro(QString id) {
        mui_fechacobro->setText(id);
    };
    void pintacomentcobro(QString id) {
        mui_comentcobro->setText(id);
    };
    void pintaidcliente(QString id) {
        mui_cliente->setidcliente(id);
    };
    void pintarefcobro(QString id) {
        mui_refcobro->setText(id);
    };
    void pintacantcobro(QString id) {
        mui_cantcobro->setText(id);
    };
    void pintaprevisioncobro(QString id) {
        if (id == "t" || id == "TRUE") {
            mui_previsioncobro->setChecked(TRUE);
        } else {
            mui_previsioncobro->setChecked(FALSE);
        }
    };
    virtual int guardar() {return Cobro::guardar();};
    virtual int borrar()  {return Cobro::borrar();};

public slots:
    virtual int cargar(QString id);

    virtual void on_mui_comentcobro_textChanged(const QString &str) {
        setcomentcobro(str);
    };
    virtual void on_mui_refcobro_valueChanged(const QString &str) {
        setrefcobro(str);
    };
    virtual void on_mui_cantcobro_textChanged(const QString &str) {
        setcantcobro(str);
    };
    virtual void on_mui_previsioncobro_stateChanged(int i) {
        if (i) {
            setprevisioncobro("TRUE");
        } else {
            setprevisioncobro("FALSE");
        }
    }
    virtual void on_mui_cliente_valueChanged(QString id) {
        setidcliente(id);
    };
    virtual void on_mui_fechacobro_valueChanged(QString id) {
        setfechacobro(id);
    };
};

#endif

