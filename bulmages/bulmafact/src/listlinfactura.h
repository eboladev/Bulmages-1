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

#ifndef LISTLINFACTURA_H
#define LISTLINFACTURA_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "linfactura.h"
#include "funcaux.h"


class ListLinFactura
{

public:
	company *companyact;
	QString mdb_idfactura;
	Q3PtrList<LinFactura> m_lista;

public:
	ListLinFactura(company *comp);
	ListLinFactura();
	virtual ~ListLinFactura();
	void setidfactura(QString id)
	{
		mdb_idfactura = id;
		LinFactura *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidfactura(mdb_idfactura);
			i++;
		};
	};
	void setcompany(company *c)
	{
		companyact = c;
	};
	void guardaListLinFactura();
	void vaciar();
	virtual void pintaListLinFactura()
	{
		_depura("La funcion pintaListLinFactura aun no ha sido implementada.",2);
	};
	void cargaListLinFactura(QString);
	void borrar();
	void nuevalinea(QString desclfactura, QString cantlfactura, QString pvplfactura,
				QString descuentolfactura, QString idarticulo,
				QString codigocompletoarticulo, QString nomarticulo,
				QString ivalfactura);
	LinFactura *linpos(int);
	float calculabase();
	float calculaiva();
	void borraLinFactura(int);
};

#endif
