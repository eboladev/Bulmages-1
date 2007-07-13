/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BUSQUEDACUENTA_H
#define BUSQUEDACUENTA_H

#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <QCompleter>

#include "ui_busquedacuentabase.h"
#include "postgresiface2.h"


class empresa;


class BusquedaCuentaDelegate : public QComboBox {
    Q_OBJECT

private:
    empresa *m_companyact;
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    cursor2 *m_cursorcombo;
    QCompleter *completar;

public:
    BusquedaCuentaDelegate(QWidget *parent = 0);
    ~BusquedaCuentaDelegate();
    void setcompany(empresa *comp);

public slots:
    virtual void s_editTextChanged(const QString &);
};


class BusquedaCuenta : public QWidget, public Ui_BusquedaCuentaBase {
    Q_OBJECT

private:
    empresa *m_companyact;
    QString mdb_idcuenta;
    QString mdb_nomcuenta;
    QString mdb_codigocuenta;
    QString mdb_tipocuenta;
    int m_numdigitos;

public:
    BusquedaCuenta(QWidget *parent = 0);
    ~BusquedaCuenta();
    void setempresa(empresa *comp);
    empresa *_empresa();
    QString text();
    virtual void setText(QString val);
    virtual QString codigocuenta();
    virtual QString idcuenta();
    virtual QString nomcuenta();
    virtual QString tipocuenta();
    virtual void setidcuenta(QString val);
    virtual void setcodigocuenta(QString val);
    void hideNombre();
    void showNombre();

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void s_searchCuenta();
    virtual void on_mui_codigocuenta_textChanged(const QString &q);
    virtual void s_codigocuentatextChanged(const QString &);
    virtual void on_lostFocus();
    virtual void s_lostFocus();
    virtual void on_returnPressed();
    virtual void s_returnPressed();
    virtual void selectAll();
    virtual void setFocus();

signals:
    void valueChanged(QString);
    void returnPressed();
};

#endif

