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

#ifndef LINALBARANPROVEEDOR_H
#define LINALBARANPROVEEDOR_H

#include "company.h"


class LinAlbaranProveedor
{

private:
	QString mdb_numlalbaranp;
	QString mdb_desclalbaranp;
	QString mdb_cantlalbaranp;
	QString mdb_pvplalbaranp;
	QString mdb_descontlalbaranp;
	QString mdb_idarticulo;
	QString mdb_ivalalbaranp;

	/// Aunque no es un elemento de la tabla, si es importante que este incluido
	/// porque se pinta en muchas partes.
	QString mdb_codigocompletoarticulo; 
	/// Aunque no es un elemento de la tabla, si es importante que este incluido
	/// porque se pinta en muchas partes.
	QString mdb_nomarticulo; 
	QString mdb_idalbaranp;
	company *companyact;

public:
	LinAlbaranProveedor(company *);
	LinAlbaranProveedor(company *, QString);

	/// La carga rapida tiene un comportamiento poco restrictivo para
	/// aumentar la eficiencia.
	LinAlbaranProveedor(company *comp, QString numlalbaranp, QString desclalbaranp,
				QString cantlalbaranp, QString pvplalbaranp,
				QString descontlalbaranp, QString idarticulo,
				QString codigocompletoarticulo, QString nomarticulo,
				QString idalbaranp, QString ivalalbaranp);
	virtual ~LinAlbaranProveedor();
	virtual void pintaLinAlbaranProveedor()
	{
	};
	inline QString numlalbaranp()
	{
		return mdb_numlalbaranp;
	};
	inline QString desclalbaranp()
	{
		return mdb_desclalbaranp;
	};
	inline QString cantlalbaranp()
	{
		return mdb_cantlalbaranp;
	};
	inline QString pvplalbaranp()
	{
		return mdb_pvplalbaranp;
	};
	inline QString descontlalbaranp()
	{
		return mdb_descontlalbaranp;
	};
	inline QString idarticulo()
	{
		return mdb_idarticulo;
	};
	inline QString codigocompletoarticulo()
	{
		return mdb_codigocompletoarticulo;
	};
	inline QString nomarticulo()
	{
		return mdb_nomarticulo;
	};
	inline QString idalbaranp()
	{
		return mdb_idalbaranp;
	};
	inline QString ivalalbaranp()
	{
		return mdb_ivalalbaranp;
	};
	inline void setnumlalbaranp(QString val)
	{
		mdb_numlalbaranp = val;
	};
	inline void setdesclalbaranp(QString val)
	{
		mdb_desclalbaranp = val;
	};
	inline void setcantlalbaranp(QString val)
	{
		mdb_cantlalbaranp = val;
	};
	inline void setpvplalbaranp(QString val)
	{
		mdb_pvplalbaranp = val;
	};
	inline void setdescontlalbaranp(QString val)
	{
		mdb_descontlalbaranp = val;
	};
	inline void setnomarticulo(QString val)
	{
		mdb_nomarticulo = val;
	};
	inline void setidalbaranp(QString val)
	{
		mdb_idalbaranp = val;
	};
	inline void setivalalbaranp(QString val)
	{
		mdb_ivalalbaranp = val;
	};
	void setcodigocompletoarticulo(QString val);
	void guardaLinAlbaranProveedor();
	void vaciaLinAlbaranProveedor();
	void setidarticulo(QString);
	/// Hace el calculo de la base imponible de la linea.
	float calculabase();
	/// Hace el calculo del IVA de la linea.
	float calculaiva();
	void borrar();
};

#endif
