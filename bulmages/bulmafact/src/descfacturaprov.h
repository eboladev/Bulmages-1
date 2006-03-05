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

#ifndef DESCFACTURAPROV_H
#define DESCFACTURAPROV_H

#include "company.h"
#include "dbrecord.h"


class DescuentoFacturaProv : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	DescuentoFacturaProv(company *);
	DescuentoFacturaProv(company *, QString);
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoFacturaProv(company *, QString , QString , QString , QString);
	virtual ~DescuentoFacturaProv();
	virtual void pintaDescuentoFacturaProv()
	{
	};
	void guardaDescuentoFacturaProv();
	void vaciaDescuentoFacturaProv();
	inline QString iddfacturap()
	{
		return DBvalue("iddfacturap");
	};
	inline QString conceptdfacturap()
	{
		return DBvalue("conceptdfacturap");
	};
	inline QString proporciondfacturap()
	{
		return DBvalue("proporciondfacturap");
	};
	inline QString idfacturap()
	{
		return DBvalue("idfacturap");
	};
	inline void setiddfacturap(QString val)
	{
		setDBvalue("iddfacturap",val);
	};
	inline void setconceptdfacturap(QString val)
	{
		setDBvalue("conceptdfacturap",val);
	};
	inline void setproporciondfacturap(QString val)
	{
		setDBvalue("proporciondfacturap",val);
	};
	inline void setidfacturap(QString val)
	{
		setDBvalue("idfacturap",val);
	};
	void borrar();
};

#endif
