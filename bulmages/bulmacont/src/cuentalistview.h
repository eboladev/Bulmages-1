/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef CUENTALISTVIEW_H
#define CUENTALISTVIEW_H

#include "empresa.h"
#include "bcsubform.h"
#include "blformlist.h"


/**
*/
class CuentaListSubForm : public BcSubForm
{
    Q_OBJECT

public:
    CuentaListSubForm ( QWidget *parent = 0, const char *name = 0 );
    ~CuentaListSubForm();
};


#include "ui_cuentalistbase.h"


/**
*/
class CuentaListView : public BlFormList, public Ui_CuentaListBase
{
    Q_OBJECT

private:
    QString mdb_idcuenta;
    QString mdb_nombrecuenta;
    QString mdb_codigocuenta;

public:
    CuentaListView ( Empresa *, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    virtual ~CuentaListView();
    QString formaQuery();
    QString idcuenta();
    QString nombrecuenta();
    QString codigocuenta();
    void presentar();
    void editar ( int );
    void imprimir();
    void borrar();
    void crear();
    void submenu ( const QPoint & );

public slots:
    virtual void on_mui_importar_clicked();
    virtual void on_mui_exportar_clicked();

signals:
    void selected ( QString );
};

#endif

