/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#ifndef PROVEDIT_H
#define PROVEDIT_H

#include "ui_proveditbase.h"
#include "dbrecord.h"
#include "dialogchanges.h"


class company;


class ProveedorView : public QWidget, public Ui_ProveedorBase, public DBRecord, public dialogChanges {
    Q_OBJECT

private:
    company *m_companyact;
    void closeEvent(QCloseEvent *);
public:
    ProveedorView(company *emp, QWidget *parent = 0);
    ~ProveedorView();
public:
    virtual int cargar(QString);
    virtual int guardar();

public slots:
    virtual void on_mui_guardar_clicked() {
	guardar();
    };
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
    virtual void on_mui_aceptar_clicked();
};

#endif

