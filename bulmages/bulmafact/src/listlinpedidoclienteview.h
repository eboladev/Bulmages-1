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

#ifndef LISTLINPEDIDOCLIENTEVIEW_H
#define LISTLINPEDIDOCLIENTEVIEW_H

#include <QEvent>

#include "company.h"
#include "subform2bf.h"
#include "fixed.h"

class ListLinPedidoClienteView : public SubForm2Bf {
    Q_OBJECT

public:
    QString mdb_idpedidocliente;
    ListLinPedidoClienteView(QWidget *parent = 0);
    ~ListLinPedidoClienteView() {}
    ;

public slots:
    virtual void cargar(QString idpedidocliente);
    Fixed calculabase();
    Fixed calculaiva();
    virtual void on_mui_list_editFinished(int, int);

};

#endif

