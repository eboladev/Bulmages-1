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

#ifndef LISTLINPEDIDOCLIENTE_H
#define LISTLINPEDIDOCLIENTE_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "linpedidocliente.h"
#include "funcaux.h"

class ListLinPedidoCliente
{

public:
	company *companyact;
	QString mdb_idpedidocliente;
	Q3PtrList<LinPedidoCliente> m_lista;

public:
	ListLinPedidoCliente(company *comp);
	ListLinPedidoCliente();
	virtual ~ListLinPedidoCliente();
	void setcompany(company *c)
	{
		companyact = c;
	};
	void guardaListLinPedidoCliente();
	void vaciar();
	void setidpedidocliente(QString id)
	{
		mdb_idpedidocliente = id;
		LinPedidoCliente *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidpedidocliente(mdb_idpedidocliente);
			i++;
		};
	};
	virtual void pintaListLinPedidoCliente()
	{
		_depura("La funcion pintaListLinPedidoCliente aun no ha sido implementada\n", 2);
	};
	void cargaListLinPedidoCliente(QString);
	void borrar();
	void nuevalinea(QString desclpedidocliente, QString cantlpedidocliente,
				QString pvplpedidocliente, QString prevlpedidocliente,
				QString ivalpedidocliente, QString descuentolpedidocliente,
				QString idarticulo, QString codigocompletoarticulo,
				QString nomarticulo, QString puntlpedidocliente);
	LinPedidoCliente *linpos(int);
	float calculabase();
	float calculaiva();
	void borraLinPedidoCliente(int);
};

#endif
