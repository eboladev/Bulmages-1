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
#ifndef BUSQUEDAARTICULO_H
#define BUSQUEDAARTICULO_H

#include <QLineEdit>
#include <QLabel>

#include "ui_busquedaarticulobase.h"


class company;

class BusquedaArticulo : public QWidget, public Ui_BusquedaArticuloBase {
    Q_OBJECT

private:
    company *companyact;
    QString mdb_idarticulo;
    QString mdb_nomarticulo;
    QString mdb_codigocompletoarticulo;

public:
    BusquedaArticulo(QWidget *parent = 0, const char *name = 0);
    ~BusquedaArticulo();
    void setcompany(company *comp) {
        companyact = comp;
    };
    virtual QString codigocompletoarticulo() {
        return m_codigocompletoarticulo->text();
    };
    virtual QString idarticulo() {
        return mdb_idarticulo;
    };
    virtual QString nomarticulo() {
        return m_nomarticulo->text();
    };
    virtual void setidarticulo(QString val);
    virtual void setcodigocompletoarticulo(QString val);

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void on_m_codigocompletoarticulo_textChanged(const QString &);

signals:
    void valueChanged(QString);
};

#endif

