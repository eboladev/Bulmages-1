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

#ifndef LINALBARANCLIENTE_H
#define LINALBARANCLIENTE_H

#include "company.h"
#include "dbrecord.h"

class LinAlbaranCliente : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	LinAlbaranCliente(company *);
	LinAlbaranCliente(company *, QString );

	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	LinAlbaranCliente(company *comp, QString numlalbaran, QString desclalbaran,
				QString cantlalbaran, QString pvplalbaran,
				QString descontlalbaran, QString idarticulo,
				QString codigocompletoarticulo, QString nomarticulo,
				QString idalbaran, QString iva);
	virtual ~LinAlbaranCliente();
	virtual void pintaLinAlbaranCliente()
	{
	};
	inline QString numlalbaran()
	{
		return DBvalue("numlalbaran");
	};
	inline QString desclalbaran()
	{
		return DBvalue("desclalbaran");
	};
	inline QString cantlalbaran()
	{
		return DBvalue("cantlalbaran");
	};
	inline QString pvplalbaran()
	{
		return DBvalue("pvplalbaran");
	};
	inline QString descontlalbaran()
	{
		return DBvalue("descontlalbaran");
	};
	inline QString idarticulo()
	{
		return DBvalue("idarticulo");
	};
	inline QString codigocompletoarticulo()
	{
		return DBvalue("codigocompletoarticulo");
	};
	inline QString nomarticulo()
	{
		return DBvalue("nomarticulo");
	};
	inline QString idalbaran()
	{
		return DBvalue("idalbaran");
	};
	inline QString ivalalbaran()
	{
		return DBvalue("ivalalbaran");
	};
	inline void setnumlalbaran(QString val)
	{
		setDBvalue("numlalbaran", val);
	};
	inline void setdesclalbaran(QString val)
	{
		setDBvalue("desclalbaran", val);
	};
	inline void setcantlalbaran(QString val)
	{
		setDBvalue("cantlalbaran", val);
	};
	inline void setpvplalbaran(QString val)
	{
		setDBvalue("pvplalbaran", val);
	};
	inline void setdescontlalbaran(QString val)
	{
		setDBvalue("descontlalbaran", val);
	};
	inline void setivalalbaran(QString val)
	{
		setDBvalue("ivalalbaran", val);
	};
	inline void setnomarticulo(QString val)
	{
		setDBvalue("nomarticulo", val);
	};
	inline void setidalbaran(QString val)
	{
		setDBvalue("idalbaran", val);
	};
	void guardaLinAlbaranCliente();
	void vaciaLinAlbaranCliente();
	void setcodigocompletoarticulo(QString val);
	void setidarticulo(QString); 
};

#endif
