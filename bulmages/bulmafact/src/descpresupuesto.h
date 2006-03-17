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

#ifndef DESCPRESUPUESTO_H
#define DESCPRESUPUESTO_H

#include "company.h"
#include "dbrecord.h"

class DescuentoPresupuesto : public DBRecord  {
private:
	company *companyact;
	void definetabla();
public:
	DescuentoPresupuesto(company *);
	DescuentoPresupuesto(company *, QString );
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoPresupuesto(company *, QString , QString , QString , QString);
	virtual ~DescuentoPresupuesto();
	virtual void pintaDescuentoPresupuesto()
	{
	};
	void guardaDescuentoPresupuesto();
	void vaciaDescuentoPresupuesto();
	inline QString iddpresupuesto()
	{
		return DBvalue("iddpresupuesto");
	};
	inline QString conceptdpresupuesto()
	{
		return DBvalue("conceptdpresupuesto");
	};
	inline QString proporciondpresupuesto()
	{
		return DBvalue("proporciondpresupuesto");
	};
	inline QString idpresupuesto()
	{
		return DBvalue("idpresupuesto");
	};
	inline void setiddpresupuesto(QString val)
	{
		setDBvalue("iddpresupuesto",val);
	};
	inline void setconceptdpresupuesto(QString val)
	{
		setDBvalue("conceptdpresupuesto",val);
	};
	inline void setproporciondpresupuesto(QString val)
	{
		setDBvalue("proporciondpresupuesto",val);
	};
	inline void setidpresupuesto(QString val)
	{
		setDBvalue("idpresupuesto",val);
	};
};

#endif
