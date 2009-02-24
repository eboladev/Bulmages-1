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
#include <QCompleter>

#include "blcombobox.h"
#include "blwidget.h"
#include "ui_busquedacuentabase.h"
#include "blpostgresqlclient.h"


class BcCompany;


class BusquedaCuentaDelegate : public BlComboBox
{
    Q_OBJECT

private:
    /// Este cursor almacena el listado de series de factura para poder trabajar con ellas.
    BlDbRecordSet *m_cursorcombo;
    QCompleter *completar;

public:
    BusquedaCuentaDelegate ( QWidget *parent = 0 );
    ~BusquedaCuentaDelegate();

public slots:
    virtual void s_editTextChanged ( const QString & );
};


class BusquedaCuenta : public BlWidget, public Ui_BusquedaCuentaBase
{
    Q_OBJECT

private:
    QString mdb_idcuenta;
    QString mdb_nomcuenta;
    QString mdb_codigocuenta;
    QString mdb_tipocuenta;
    int m_numdigitos;
    bool m_semaforo;

public:
    BusquedaCuenta ( QWidget *parent = 0 );
    ~BusquedaCuenta();
    QString text();
    virtual QString fieldValue();
    virtual void setText ( QString val );
    virtual void setFieldValue ( QString val );
    virtual QString codigocuenta();
    virtual QString idcuenta();
    virtual QString nomcuenta();
    virtual QString tipocuenta();
    virtual void setidcuenta ( QString val );
    virtual void setcodigocuenta ( QString val );
    virtual void setMainCompany ( BcCompany *emp );
    void hideNombre();
    void showNombre();
    void pinta();

public slots:
    virtual void on_mui_buscar_clicked();
    virtual void s_searchCuenta();
    virtual void on_mui_codigocuenta_textChanged ( const QString &q );
    virtual void on_mui_codigocuenta_editingFinished();
    virtual void s_codigocuentatextChanged ( const QString & );
    virtual void selectAll();
    virtual void setFocus();

signals:
    void valueChanged ( QString );
    void enterPressed();
};

#endif
