/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#ifndef INFORMEQTOOLBUTTON1_H
#define INFORMEQTOOLBUTTON1_H

#include <QWidget>
#include <QToolButton>
#include <QString>
#include "bfcompany.h"
#include "blfunctions.h"
#include <QtXml/QDomDocument>
#include <QMap>
#include <QString>


#include "clientslist.h"
#include "providerslist.h"
#include "blwidget.h"

class InformeQToolButton1 : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

private:
    ClientsList *m_clientsList;

public:
    InformeQToolButton1 ( ClientsList * , QWidget *parent = NULL );
    ~InformeQToolButton1();
    void setBoton();
    QString generarCliente ( QString idcliente, int row, int anyo );

public slots:
    virtual void click();
};


class InformeProveedorQToolButton1 : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

private:
    ProveedorList *m_proveedorList;

public:
    InformeProveedorQToolButton1 ( ProveedorList * , QWidget *parent = NULL );
    ~InformeProveedorQToolButton1();
    void setBoton();
    QString generarProveedor ( QString idproveedor, int row, int anyo );

public slots:
    virtual void click();
};

#endif
