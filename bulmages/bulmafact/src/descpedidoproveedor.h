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

#ifndef DESCPEDIDOPROVEEDOR_H
#define DESCPEDIDOPROVEEDOR_H

#include "company.h"
#include "dbrecord.h"


class DescuentoPedidoProveedor : public DBRecord
{

private:
	company *companyact;
	void definetabla();
public:
	DescuentoPedidoProveedor(company *);
	DescuentoPedidoProveedor(company *, QString);
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoPedidoProveedor(company *, QString , QString , QString , QString);
	virtual ~DescuentoPedidoProveedor();
	virtual void pintaDescuentoPedidoProveedor()
	{
	};
	void guardaDescuentoPedidoProveedor();
	void vaciaDescuentoPedidoProveedor();
	inline QString iddpedidoproveedor()
	{
		return DBvalue("iddpedidoproveedor");
	};
	inline QString conceptdpedidoproveedor()
	{
		return DBvalue("conceptdpedidoproveedor");
	};
	inline QString proporciondpedidoproveedor()
	{
		return DBvalue("proporciondpedidoproveedor");
	};
	inline QString idpedidoproveedor()
	{
		return DBvalue("idpedidoproveedor");
	};
	inline void setiddpedidoproveedor(QString val)
	{
		setDBvalue("iddpedidoproveedor",val);
	};
	inline void setconceptdpedidoproveedor(QString val)
	{
		setDBvalue("conceptdpedidoproveedor",val);
	};
	inline void setproporciondpedidoproveedor(QString val)
	{
		setDBvalue("proporciondpedidoproveedor",val);
	};
	inline void setidpedidoproveedor(QString val)
	{
		setDBvalue("idpedidoproveedor",val);
	};
	void borrar();
};

#endif
