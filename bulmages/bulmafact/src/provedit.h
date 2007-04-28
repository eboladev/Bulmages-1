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
#include "fichabf.h"


class company;


/// Muestra y administra la ventana con la informaci&oacute;n de un proveedor.
/** */
class ProveedorView : public FichaBf, public Ui_ProveedorBase {
    Q_OBJECT

private:
    company *m_companyact;

public:
    ProveedorView(company *comp, QWidget *parent = 0);
    ~ProveedorView();

public:
    virtual int cargar(QString);
    virtual int guardar();

public slots:
    virtual void on_mui_guardar_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_borrar_clicked();
};

#endif

