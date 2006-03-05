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

#ifndef DESCALBARANPROV_H
#define DESCALBARANPROV_H

#include "company.h"
#include "dbrecord.h"


class DescuentoAlbaranProv : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	DescuentoAlbaranProv(company *);
	DescuentoAlbaranProv(company *, QString);
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoAlbaranProv(company *, QString , QString , QString , QString);
	virtual ~DescuentoAlbaranProv();
	virtual void pintaDescuentoAlbaranProv()
	{
	};
	void guardaDescuentoAlbaranProv();
	void vaciaDescuentoAlbaranProv();
	inline QString iddalbaranp()
	{
		return DBvalue("iddalbaranp");
	};
	inline QString conceptdalbaranp()
	{
		return DBvalue("conceptdalbaranp");
	};
	inline QString proporciondalbaranp()
	{
		return DBvalue("proporciondalbaranp");
	};
	inline QString idalbaranp()
	{
		return DBvalue("idalbaranp");
	};
	inline void setiddalbaranp(QString val)
	{
		setDBvalue("iddalbaranp",val);
	};
	inline void setconceptdalbaranp(QString val)
	{
		setDBvalue("conceptdalbaranp",val);
	};
	inline void setproporciondalbaranp(QString val)
	{
		setDBvalue("proporciondalbaranp",val);
	};
	inline void setidalbaranp(QString val)
	{
		setDBvalue("idalbaranp",val);
	};
	void borrar();
};

#endif
