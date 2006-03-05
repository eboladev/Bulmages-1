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

#ifndef LISTCOMPARTICULO_H
#define LISTCOMPARTICULO_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "comparticulo.h"
#include "funcaux.h"


class ListCompArticulo
{

public:
	company *companyact;
	QString mdb_idarticulo;
	Q3PtrList<CompArticulo> m_lista;

public:
	ListCompArticulo(company *comp);
	ListCompArticulo();
	void setcompany(company *c)
	{
		_depura("ListCompArticulo setCompany", 0);
		companyact = c;
		_depura("ListCompArticulo  fin de setCompany", 0);
	};
	virtual ~ListCompArticulo();
	void guardaListCompArticulo();
	void vaciar();
	virtual void pintaListCompArticulo()
	{
		_depura("La funcion pintaListCompArticulo aun no ha sido implementada.", 0);
	};
	void cargaListCompArticulo(QString);
	void borrar();
	void nuevalinea(QString, QString, QString, QString);
	CompArticulo *linpos(int);
	void borraCompArticulo(int);
	void setidarticulo(QString id)
	{
		mdb_idarticulo = id;
		CompArticulo *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidarticulo(mdb_idarticulo);
			i++;
		}
	};
};

#endif
