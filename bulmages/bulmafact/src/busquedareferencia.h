/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef BUSQUEDAREFERENCIA_H
#define BUSQUEDAREFERENCIA_H

#include <QLineEdit>
#include <QLabel>

#include "ui_busquedareferenciabase.h"


class company;


/// Permite buscar y seleccionar una referencia.
/** Muestra la parte del formulario que permite buscar y seleccionar una
    referencia en cualquier documento. */
class BusquedaReferencia : public QWidget, public Ui_BusquedaReferenciaBase {
    Q_OBJECT

private:
    company *m_companyact;
    bool m_semaforo;

public:
    BusquedaReferencia(QWidget *parent = 0);
    ~BusquedaReferencia();
    void setcompany(company *comp) {
        m_companyact = comp;
    };
    virtual void setText(QString val) {
        mui_referencia->setText(val);
    };
    virtual QString text() {
        return mui_referencia->text();
    };

public slots:
    virtual void on_mui_informe_clicked();
    virtual void on_mui_referencia_editingFinished();
    virtual void on_mui_abrirtodo_clicked();

signals:
    void valueChanged(QString);
};

#endif

