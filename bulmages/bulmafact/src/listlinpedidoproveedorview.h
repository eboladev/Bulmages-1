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

#ifndef LISTLINPEDIDOPROVEEDORVIEW_H
#define LISTLINPEDIDOPROVEEDORVIEW_H

#include <Q3Table>
#include <QEvent>

#include "listlinpedidoproveedor.h"
#include "company.h"
#include "linpedidoproveedor.h"


class ListLinPedidoProveedorView : public Q3Table , public ListLinPedidoProveedor
{
	Q_OBJECT

public:
	ListLinPedidoProveedorView(QWidget *parent = 0, const char *name = 0);
	~ListLinPedidoProveedorView();
	virtual void pintaListLinPedidoProveedor();
	virtual void pintalinListLinPedidoProveedor(int);
	virtual bool eventFilter(QObject *obj, QEvent *ev);
	LinPedidoProveedor *lineaat(int);
	LinPedidoProveedor *lineaact();
	void cargaconfig();
	void guardaconfig();

public slots:
	virtual void valueBudgetLineChanged(int row, int col);
	virtual QString searchArticle();
	virtual void manageArticle(int row);
	virtual void contextMenu (int, int, const QPoint &);
	virtual void borraLinPedidoProveedoract();
};

#endif
