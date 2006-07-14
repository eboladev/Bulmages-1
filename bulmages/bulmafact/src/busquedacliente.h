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

#ifndef BUSQUEDACLIENTE_H
#define BUSQUEDACLIENTE_H

#include <QLineEdit>
#include <QLabel>

#include "ui_busquedaclientebase.h"


class company;

class BusquedaCliente : public QWidget, public Ui_BusquedaClienteBase {
    Q_OBJECT

private:
    company *companyact;
    QString mdb_idcliente;
    QString mdb_nomcliente;
    QString mdb_cifcliente;
    QString mdb_codcliente;
    bool m_semaforo;

public:
    BusquedaCliente(QWidget *parent = 0, const char *name = 0);
    ~BusquedaCliente();
    void setcompany(company *comp) {
        companyact = comp;
    };
    void pinta();
    virtual QString cifcliente() {
        return m_cifcliente->text();
    };
    virtual QString idcliente() {
        return mdb_idcliente;
    };
    virtual QString nomcliente() {
        return nomcliente();
    };
    virtual void setidcliente(QString val);
    virtual void setcifcliente(QString val);

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_m_cifcliente_textChanged(const QString &);
    virtual void on_m_cifcliente_editingFinished();
signals:
    void valueChanged(QString);
};

#endif

