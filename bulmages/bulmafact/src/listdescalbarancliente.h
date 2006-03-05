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

#ifndef LISTDESCALBARANCLIENTE_H
#define LISTDESCALBARANCLIENTE_H

#include <QObject>
#include <Q3PtrList>

#include "company.h"
#include "descalbarancliente.h"
#include "funcaux.h"


class ListDescuentoAlbaranCliente
{

public:
	company *companyact;
	QString mdb_idalbaran;
	Q3PtrList<DescuentoAlbaranCliente> m_lista;

public:
	ListDescuentoAlbaranCliente(company *comp);
	ListDescuentoAlbaranCliente();
	virtual ~ListDescuentoAlbaranCliente();
	void setcompany(company *c)
	{
		_depura("ListDescuentoAlbaranCliente setCompany.", 0);
		companyact = c;
		_depura("ListDescuentoAlbaranCliente fin de setCompany.", 0);
	};
	void guardaListDescuentoAlbaranCliente();
	void vaciar();
	virtual void pintaListDescuentoAlbaranCliente()
	{
		_depura("La funcion pintaListDescuentoAlbaranCliente aun no ha sido \
				implementada.", 0);
	};
	void cargaDescuentos(QString);
	void borrar();
	void nuevalinea(QString concept, QString propor);
	DescuentoAlbaranCliente *linpos(int);
	void borraDescuentoAlbaranCliente(int);
	void setidalbaran(QString id)
	{
		mdb_idalbaran = id;
		DescuentoAlbaranCliente *linea;
		uint i = 0;
		for (linea = m_lista.first(); linea; linea = m_lista.next())
		{
			linea->setidalbaran(mdb_idalbaran);
			i++;
		}
	};
};

#endif
