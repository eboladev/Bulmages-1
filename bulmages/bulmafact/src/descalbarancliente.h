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

#ifndef DESCALBARANCLIENTE_H
#define DESCALBARANCLIENTE_H

#include "company.h"
#include "dbrecord.h"


class DescuentoAlbaranCliente : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	DescuentoAlbaranCliente(company *);
	DescuentoAlbaranCliente(company *, QString );
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoAlbaranCliente(company *, QString , QString , QString , QString);
	virtual ~DescuentoAlbaranCliente();
	virtual void pintaDescuentoAlbaranCliente()
	{
	};
	void guardaDescuentoAlbaranCliente();
	void vaciaDescuentoAlbaranCliente();
	inline QString iddalbaran()
	{
		return DBvalue("iddalbaran");
	};
	inline QString conceptdalbaran()
	{
		return DBvalue("conceptdalbaran");
	};
	inline QString proporciondalbaran()
	{
		return DBvalue("proporciondalbaran");
	};
	inline QString idalbaran()
	{
		return DBvalue("idalbaran");
	};
	inline void setiddalbaran(QString val)
	{
		setDBvalue("iddalbaran",val);
	};
	inline void setconceptdalbaran(QString val)
	{
		setDBvalue("conceptdalbaran",val);
	};
	inline void setproporciondalbaran(QString val)
	{
		setDBvalue("proporciondalbaran",val);
	};
	inline void setidalbaran(QString val)
	{
		setDBvalue("idalbaran",val);
	};
};

#endif
