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

#ifndef LISTDESCPEDIDOPROVEEDOR_H
#define LISTDESCPEDIDOPROVEEDOR_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "descpedidoproveedor.h"

class ListDescuentoPedidoProveedor
{

public:
	company *companyact;
	QString mdb_idpedidoproveedor;
	Q3PtrList<DescuentoPedidoProveedor> m_lista;

public:
	ListDescuentoPedidoProveedor(company *comp);
	ListDescuentoPedidoProveedor();
	void setcompany(company *c)
	{
		_depura("ListDescuentoPedidoProveedor setCompany.", 0);
		companyact = c;
		_depura("ListDescuentoPedidoProveedor fin de setCompany.", 0);
	};
	virtual ~ListDescuentoPedidoProveedor();
	void guardaListDescuentoPedidoProveedor();
	void vaciar();
	virtual void pintaListDescuentoPedidoProveedor()
	{
		_depura("La funcion pintaListDescuentoPedidoProveedor aun \
				no ha sido implementada.", 0);
	};
	void cargaDescuentos(QString);
	void borrar();
	void nuevalinea(QString concept, QString propor);
	DescuentoPedidoProveedor *linpos(int);
	void borraDescuentoPedidoProveedor(int);
	void setidpedidoproveedor(QString id)
	{
		mdb_idpedidoproveedor = id;
		DescuentoPedidoProveedor *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidpedidoproveedor(mdb_idpedidoproveedor);
			i++;
		};
	};
};

#endif
