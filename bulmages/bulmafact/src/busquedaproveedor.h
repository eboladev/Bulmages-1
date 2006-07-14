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

#ifndef BUSQUEDAPROVEEDOR_H
#define BUSQUEDAPROVEEDOR_H

#include <QLineEdit>
#include <QLabel>

#include "ui_busquedaproveedorbase.h"


class company;

class BusquedaProveedor : public QWidget, public Ui_BusquedaProveedorBase {
    Q_OBJECT

private:
    company *companyact;
    QString mdb_idproveedor;
    QString mdb_nomproveedor;
    QString mdb_cifproveedor;
    QString mdb_codproveedor;
    bool m_semaforo;

public:
    BusquedaProveedor(QWidget *parent = 0, const char *name = 0);
    ~BusquedaProveedor();
    void setcompany(company *comp) {
        companyact = comp;
    };
    void pinta();

    virtual void setidproveedor(QString val);
    virtual void setcifproveedor(QString val);
    virtual QString cifproveedor() {
        return m_cifproveedor->text();
    };
    virtual QString idproveedor() {
        return mdb_idproveedor;
    };
    virtual QString nomproveedor() {
        return nomproveedor();
    };

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_m_cifproveedor_textChanged(const QString &);
    virtual void on_m_cifproveedor_editingFinished();

signals:
    void valueChanged(QString);
};

#endif

