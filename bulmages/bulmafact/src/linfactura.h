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

#ifndef LINFACTURA_H
#define LINFACTURA_H

#include "company.h"


class LinFactura
{

private:
	QString mdb_idlfactura;
	QString mdb_desclfactura;
	QString mdb_cantlfactura;
	QString mdb_pvplfactura;
	QString mdb_descuentolfactura;
	QString mdb_idfactura;
	QString mdb_idarticulo;
	QString mdb_ivalfactura;
	/// Aunque no es un elemento de la tabla, si es importante que este incluido
	/// porque se pinta en muchas partes.
	QString mdb_codigocompletoarticulo; 
	/// Aunque no es un elemento de la tabla, si es importante que este incluido
	/// porque se pinta en muchas partes.
	QString mdb_nomarticulo;
	QString mdb_idforma_pago;  
	company *companyact;

public:
	LinFactura(company *);
	LinFactura(company *, QString);
	LinFactura(company *, cursor2 *);

	/// La carga rapida tiene un comportamiento poco restrictivo para aumentar
	/// la eficiencia.
	LinFactura(company *comp, QString idlfactura, QString desclfactura,
			QString cantlfactura, QString pvplfactura, QString descuentolfactura,
			QString idfactura, QString idarticulo, QString codigocompletoarticulo,
			QString nomarticulo, QString ivalfactura);
	virtual ~LinFactura();
	virtual void pintaLinFactura()
	{
	};
	void guardaLinFactura();
	void vaciaLinFactura();
	inline QString idlfactura()
	{
		return mdb_idlfactura;
	};
	inline QString desclfactura()
	{
		return mdb_desclfactura;
	};
	inline QString cantlfactura()
	{
		return mdb_cantlfactura;
	};
	inline QString pvplfactura()
	{
		return mdb_pvplfactura;
	};
	inline QString descuentolfactura()
	{
		return mdb_descuentolfactura;
	};
	inline QString idfactura()
	{
		return mdb_idfactura;
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
	inline QString ivalfactura()
	{
		return mdb_ivalfactura;
	};
	inline void setidlFactura(QString val)
	{
		mdb_idlfactura = val;
	};
	inline void setdesclFactura(QString val)
	{
		mdb_desclfactura = val;
	};
	inline void setcantlFactura(QString val)
	{
		mdb_cantlfactura = val;
	};
	inline void setpvplFactura(QString val)
	{
		mdb_pvplfactura = val;
	};
	inline void setdescuentolFactura(QString val)
	{
		mdb_descuentolfactura = val;
	};
	inline void setidfactura(QString val)
	{
		mdb_idfactura = val;
	};
	inline void setivalFactura(QString val)
	{
		mdb_ivalfactura = val;
	};
	void setcodigocompletoarticulo(QString);  
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
};

#endif
