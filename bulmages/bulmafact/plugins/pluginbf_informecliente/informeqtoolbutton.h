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

#ifndef INFORMEQTOOLBUTTON_H
#define INFORMEQTOOLBUTTON_H

#include <QtWidgets/QWidget>
#include <QtWidgets/QToolButton>
#include <QtCore/QString>
#include "bfcompany.h"
#include "blfunctions.h"
#include <QtXml/QDomDocument>
#include <QtCore/QMap>
#include <QtCore/QString>

#include "articulolist.h"
#include "clientslist.h"
#include "blwidget.h"

class InformeQToolButton : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

private:
    ClientsList *m_clientsList;

public:
    InformeQToolButton ( ClientsList * , QWidget *parent = NULL );
    ~InformeQToolButton();
    void setBoton();
    QString generarCliente ( QString idcliente );

public slots:
    virtual void click();

};


class InformeArtQToolButton : public QToolButton, BlMainCompanyPointer
{
    Q_OBJECT

private:
    ArticuloList *m_articuloList;

public:
    InformeArtQToolButton ( ArticuloList * , QWidget *parent = NULL );
    ~InformeArtQToolButton();
    void setBoton();
    QString generarArticulos();

public slots:
    virtual void click();

};

#endif
