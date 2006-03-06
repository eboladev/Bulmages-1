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

#ifndef LISTLINALBARANCLIENTE_H
#define LISTLINALBARANCLIENTE_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "linalbarancliente.h"
#include "funcaux.h"

class ListLinAlbaranCliente {
public:
	company *companyact;
	QString mdb_idalbaran;
	Q3PtrList<LinAlbaranCliente> m_lista;

public:
	ListLinAlbaranCliente(company *comp);
	ListLinAlbaranCliente();
	virtual ~ListLinAlbaranCliente();
	void setcompany(company *c)
	{
		_depura("ListLinAlbaranCliente setCompany.", 0);
		companyact = c;
		_depura("ListLinAlbaranCliente fin de setCompany.", 0);
	};
	void guardaListLinAlbaranCliente();
	void vaciar();
	virtual void pintaListLinAlbaranCliente()
	{
		_depura("La funcion pintaListLinAlbaranCliente aun \
				no ha sido implementada.", 0);
	};
	void setidalbaran(QString id)
	{
		mdb_idalbaran = id;
		LinAlbaranCliente *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidalbaran(mdb_idalbaran);
			i++;
		};
	};
	void cargaListLinAlbaranCliente(QString);
	void borrar();
	void nuevalinea(QString desclalbaran, QString cantlalbaran, QString pvplalbaran,
				QString descontlalbaran, QString idarticulo,
				QString codigocompletoarticulo, QString nomarticulo, QString iva);
	LinAlbaranCliente *linpos(int);
	void borraLinAlbaranCliente(int);
};

#endif
