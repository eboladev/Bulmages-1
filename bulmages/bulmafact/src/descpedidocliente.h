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

#ifndef DESCPEDIDOCLIENTE_H
#define DESCPEDIDOCLIENTE_H

#include "company.h"
#include "dbrecord.h"


class DescuentoPedidoCliente : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	DescuentoPedidoCliente(company *);
	DescuentoPedidoCliente(company *, QString );
	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	DescuentoPedidoCliente(company *, QString , QString , QString , QString);
	virtual ~DescuentoPedidoCliente();
	virtual void pintaDescuentoPedidoCliente()
	{
	};
	void guardaDescuentoPedidoCliente();
	void vaciaDescuentoPedidoCliente();
	inline QString iddpedidocliente()
	{
		return DBvalue("iddpedidocliente");
	};
	inline QString conceptdpedidocliente()
	{
		return DBvalue("conceptdpedidocliente");
	};
	inline QString proporciondpedidocliente()
	{
		return DBvalue("proporciondpedidocliente");
	};
	inline QString idpedidocliente()
	{
		return DBvalue("idpedidocliente");
	};
	inline void setiddpedidocliente(QString val)
	{
		setDBvalue("iddpedidocliente",val);
	};
	inline void setconceptdpedidocliente(QString val)
	{
		setDBvalue("conceptdpedidocliente",val);
	};
	inline void setproporciondpedidocliente(QString val)
	{
		setDBvalue("proporciondpedidocliente",val);
	};
	inline void setidpedidocliente(QString val)
	{
		setDBvalue("idpedidocliente",val);
	};
};

#endif
