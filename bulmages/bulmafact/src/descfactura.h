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

#ifndef DESCFACTURA_H
#define DESCFACTURA_H

#include "company.h"
#include "dbrecord.h"


class DescuentoFactura : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	DescuentoFactura(company *);
	DescuentoFactura(company *, QString );
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoFactura(company *, QString , QString , QString , QString);
	virtual ~DescuentoFactura();
	virtual void pintaDescuentoFactura()
	{
	};
	void guardaDescuentoFactura();
	void vaciaDescuentoFactura();
	inline QString iddfactura()
	{
		return DBvalue("iddfactura");
	};
	inline QString conceptdfactura()
	{
		return DBvalue("conceptdfactura");
	};
	inline QString proporciondfactura()
	{
		return DBvalue("proporciondfactura");
	};
	inline QString idfactura()
	{
		return DBvalue("idfactura");
	};
	inline void setiddfactura(QString val)
	{
		setDBvalue("iddfactura",val);
	};
	inline void setconceptdfactura(QString val)
	{
		setDBvalue("conceptdfactura",val);
	};
	inline void setproporciondfactura(QString val)
	{
		setDBvalue("proporciondfactura",val);
	};
	inline void setidfactura(QString val)
	{
		setDBvalue("idfactura",val);
	};
};

#endif
