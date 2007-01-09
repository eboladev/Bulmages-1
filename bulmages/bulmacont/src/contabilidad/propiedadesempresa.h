/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
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

#ifndef PROPIEDADESEMPRESA_H
#define PROPIEDADESEMPRESA_H

#include <stdlib.h>

#include <QLineEdit>

#include "ui_propiedadesempresabase.h"
#include "postgresiface2.h"
#include "dialogchanges.h"
#include "ficha.h"


class empresa;


class propiedadesempresa : public Ficha, public Ui_PropiedadesEmpresaBase {
    Q_OBJECT

public:
    QString empresadb;
    empresa *m_companyact;

public:
    propiedadesempresa(empresa *emp, QWidget *parent = 0);
    ~propiedadesempresa();
    int inicializa();

public slots:
    virtual bool close();
    virtual int guardar();
    virtual void on_mui_modificarplan_clicked();

private:
    void update_value(QString, QString);
};

#endif

