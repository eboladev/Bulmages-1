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

#ifndef LISTDESCPEDIDOCLIENTEVIEW_H
#define LISTDESCPEDIDOCLIENTEVIEW_H

#include "subform2bf.h"
#include "company.h"


class ListDescuentoPedidoClienteView : public SubForm2Bf {
    Q_OBJECT
public:
    QString mdb_idpedidocliente;
    ListDescuentoPedidoClienteView(QWidget *parent = 0);
    ~ListDescuentoPedidoClienteView() {};

public slots:
    virtual void cargar(QString idpedidocliente) {
        _depura("ListDescuentoPedidoClienteView::cargar\n",0);
        mdb_idpedidocliente = idpedidocliente;
        cursor2 * cur= companyact()->cargacursor("SELECT * FROM dpedidocliente WHERE idpedidocliente="+mdb_idpedidocliente);
        SubForm3::cargar(cur);
        delete cur;
    };

};








/*
#include <Q3Table>

#include "listdescpedidocliente.h"
#include "company.h"
#include "descpedidocliente.h"


class ListDescuentoPedidoClienteView : public Q3Table , public ListDescuentoPedidoCliente
{
	Q_OBJECT

public:
	ListDescuentoPedidoClienteView(QWidget *parent = 0, const char *name = 0);
	~ListDescuentoPedidoClienteView();
	virtual void pintaListDescuentoPedidoCliente();
	virtual void pintadescListDescuentoPedidoCliente(int);
	DescuentoPedidoCliente *lineaat(int);
	DescuentoPedidoCliente *lineaact();

public slots:
	virtual void valueBudgetLineChanged(int row, int col);
	virtual void contextMenu (int, int, const QPoint &);
	virtual void borradescpedidoclienteact();
};

*/

#endif
