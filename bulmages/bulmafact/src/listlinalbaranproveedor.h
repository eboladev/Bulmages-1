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

#ifndef LISTLINALBARANPROVEEDOR_H
#define LISTLINALBARANPROVEEDOR_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "linalbaranproveedor.h"
#include "funcaux.h"


class ListLinAlbaranProveedor
{

public:
	company *companyact;
	QString mdb_idalbaranp;
	Q3PtrList<LinAlbaranProveedor> m_lista;

public:
	ListLinAlbaranProveedor(company *comp);
	ListLinAlbaranProveedor();
	virtual ~ListLinAlbaranProveedor();
	void setcompany(company *c)
	{
		_depura("ListLinAlbaranProveedor setCompany.", 0);
		companyact = c;
		_depura("ListLinAlbaranProveedor fin de setCompany.", 0);
	};
	void guardaListLinAlbaranProveedor();
	void vaciar();
	virtual void pintaListLinAlbaranProveedor()
	{
		_depura("La funcion pintaListLinAlbaranProveedor aun no \
				ha sido implementada.", 0);
	};
	void setidalbaranp(QString id)
	{
		mdb_idalbaranp = id;
		LinAlbaranProveedor *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidalbaranp(mdb_idalbaranp);
			i++;
		};
	};
	void cargaListLinAlbaranProveedor(QString);
	void borrar();
	void nuevalinea(QString desclalbaranp, QString cantlalbaranp, QString pvplalbaranp,
				QString descontlalbaranp, QString idarticulo,
				QString codigocompletoarticulo, QString nomarticulo,
				QString ivalalbaranp);
	LinAlbaranProveedor *linpos(int);
	float calculabase();
	float calculaiva();
	void borraLinAlbaranProveedor(int);
};

#endif
