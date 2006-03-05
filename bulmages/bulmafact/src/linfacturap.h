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

#ifndef LINFACTURAP_H
#define LINFACTURAP_H

#include "company.h"


class LinFacturaProveedor
{

private:
	QString mdb_idlfacturap;
	QString mdb_desclfacturap;
	QString mdb_cantlfacturap;
	QString mdb_pvplfacturap;
	QString mdb_descuentolfacturap;
	QString mdb_idfacturap;
	QString mdb_idarticulo;
	QString mdb_ivalfacturap;
	/// Aunque no es un elemento de la tabla, si es importante que este incluido
	/// porque se pinta en muchas partes.
	QString mdb_codigocompletoarticulo; 
	/// Aunque no es un elemento de la tabla, si es importante que este incluido
	/// porque se pinta en muchas partes.
	QString mdb_nomarticulo;  
	QString mdb_idforma_pago;  
	company *companyact;

public:
	LinFacturaProveedor(company *);
	LinFacturaProveedor(company *, QString );
	/// La carga rapida tiene un comportamiento poco restrictivo para aumentar
	/// la eficiencia.
	LinFacturaProveedor(company *comp, QString idlfacturap, QString desclfacturap,
				QString cantlfacturap, QString pvplfacturap,
				QString descuentolfacturap, QString idfacturap,
				QString idarticulo, QString codigocompletoarticulo,
				QString nomarticulo, QString ivalfacturap);
	virtual ~LinFacturaProveedor();
	virtual void pintaLinFacturaProveedor()
	{
	};
	inline QString idlfacturap()
	{
		return mdb_idlfacturap;
	};
	inline QString desclfacturap()
	{
		return mdb_desclfacturap;
	};
	inline QString cantlfacturap()
	{
		return mdb_cantlfacturap;
	};
	inline QString pvplfacturap()
	{
		return mdb_pvplfacturap;
	};
	inline QString descuentolfacturap()
	{
		return mdb_descuentolfacturap;
	};
	inline QString idfacturap()
	{
		return mdb_idfacturap;
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
	inline QString ivalfacturap()
	{
		return mdb_ivalfacturap;
	};
	inline void setidlfacturap(QString val)
	{
		mdb_idlfacturap = val;
	};
	inline void setdesclfacturap(QString val)
	{
		mdb_desclfacturap = val;
	};
	inline void setcantlfacturap(QString val)
	{
		mdb_cantlfacturap = val;
	};
	inline void setpvplfacturap(QString val)
	{
		mdb_pvplfacturap = val;
	};
	inline void setdescuentolfacturap(QString val)
	{
		mdb_descuentolfacturap = val;
	};
	inline void setidfacturap(QString val)
	{
		mdb_idfacturap = val;
	};
	inline void setivalfacturap(QString val)
	{
		mdb_ivalfacturap=val;
	};
	/// Estas funciones no estan como debe, el uso de cada una de ellas debe hacer
	/// cambios sobre la base de datos.
	void setidarticulo(QString);
	inline void setnomarticulo(QString val)
	{
		mdb_nomarticulo = val;
	};
	/// Hace el calculo de la base imponible de la linea.
	float calculabase();
	/// Hace el calculo del IVA de la linea.
	float calculaiva();
	void borrar();
	void setcodigocompletoarticulo(QString);
	void guardaLinFacturaProveedor();
	void vaciaLinFacturaProveedor();
};

#endif
