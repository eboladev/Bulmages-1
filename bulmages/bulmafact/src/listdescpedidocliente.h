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

#ifndef LISTDESCPEDIDOCLIENTE_H
#define LISTDESCPEDIDOCLIENTE_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "descpedidocliente.h"
#include "funcaux.h"


class ListDescuentoPedidoCliente
{

public:
	company *companyact;
	QString mdb_idpedidocliente;
	Q3PtrList<DescuentoPedidoCliente> m_lista;

public:
	ListDescuentoPedidoCliente(company *comp);
	ListDescuentoPedidoCliente();
	virtual ~ListDescuentoPedidoCliente();
	void setcompany(company *c)
	{
		_depura("ListDescuentoPedidoCliente setCompany.", 0);
		companyact = c;
		_depura("ListDescuentoPedidoCliente fin de setCompany.", 0);
	};
	void guardaListDescuentoPedidoCliente();
	void vaciar();
	virtual void pintaListDescuentoPedidoCliente()
	{
		_depura("La funcion pintaListDescuentoPedidoCliente aun \
				no ha sido implementada.", 0);
	};
	void cargaDescuentos(QString);
	void borrar();
	void nuevalinea(QString concept, QString propor);
	DescuentoPedidoCliente *linpos(int);
	void borraDescuentoPedidoCliente(int);
	void setidpedidocliente(QString id)
	{
		mdb_idpedidocliente = id;
		DescuentoPedidoCliente *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidpedidocliente(mdb_idpedidocliente);
			i++;
		};
	};
};

#endif
