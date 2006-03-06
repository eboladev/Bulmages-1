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

#ifndef LISTLINPEDIDOPROVEEDOR_H
#define LISTLINPEDIDOPROVEEDOR_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "linpedidoproveedor.h"
#include "funcaux.h"

class ListLinPedidoProveedor
{

public:
	company *companyact;
	QString mdb_idpedidoproveedor;
	Q3PtrList<LinPedidoProveedor> m_lista;

public:
	ListLinPedidoProveedor(company *comp);
	ListLinPedidoProveedor();
	virtual ~ListLinPedidoProveedor();
	void setcompany(company *c)
	{
		_depura("ListLinPedidoProveedor setCompany.", 0);
		companyact = c;
		_depura("ListLinPedidoProveedor fin de setCompany.", 0);
	};
	void guardaListLinPedidoProveedor();
	void vaciar();
	void setidpedidoproveedor(QString id)
	{
		mdb_idpedidoproveedor = id;
		LinPedidoProveedor *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidpedidoproveedor(mdb_idpedidoproveedor);
			i++;
		};
	};
	virtual void pintaListLinPedidoProveedor()
	{
		_depura("La funcion pintaListLinPedidoProveedor aun \
				no ha sido implementada.", 0);
	};
	void cargaListLinPedidoProveedor(QString);
	void borrar();
	void nuevalinea(QString desclpedidoproveedor, QString cantlpedidoproveedor,
				QString pvplpedidoproveedor, QString prevlpedidoproveedor,
				QString ivalpedidoproveedor, QString descuentolpedidoproveedor,
				QString idarticulo, QString codigocompletoarticulo,
				QString nomarticulo, QString puntlpedidoproveedor);
	LinPedidoProveedor *linpos(int);
	float calculabase();
	float calculaiva();
	void borraLinPedidoProveedor(int);
};

#endif
