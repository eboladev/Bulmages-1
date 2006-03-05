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

#ifndef LINPRESUPUESTO_H
#define LINPRESUPUESTO_H

#include "company.h"
#include "dbrecord.h"

class linpresupuesto : public DBRecord
{

private:
	company *companyact;
	void definetabla();

public:
	linpresupuesto(company *);
	linpresupuesto(company *, QString);
	/// La carga rapida tiene un comportamiento poco restrictivo para aumentar
	/// la eficiencia.
	linpresupuesto(company *, QString, QString, QString, QString, QString, QString,
				QString, QString, QString, QString);
	linpresupuesto(company *, cursor2 *);
	virtual ~linpresupuesto();
	virtual void pintalinpresupuesto()
	{
	};
	void guardalinpresupuesto();
	void vacialinpresupuesto();
	inline QString idlpresupuesto()
	{
		return DBvalue("idlpresupuesto");
	};
	inline QString desclpresupuesto()
	{
		return DBvalue("desclpresupuesto");
	};
	inline QString cantlpresupuesto()
	{
		return DBvalue("cantlpresupuesto");
	};
	inline QString pvplpresupuesto()
	{
		return DBvalue("pvplpresupuesto");
	};
	inline QString descuentolpresupuesto()
	{
		return DBvalue("descuentolpresupuesto");
	};
	inline QString idpresupuesto()
	{
		return DBvalue("idpresupuesto");
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
	inline QString ivalpresupuesto()
	{
		return DBvalue("ivalpresupuesto");
	};
	inline void setidlpresupuesto(QString val)
	{
		setDBvalue("idlpresupuesto", val);
	};
	inline void setdesclpresupuesto(QString val)
	{
		setDBvalue("desclpresupuesto", val);
	};
	inline void setcantlpresupuesto(QString val)
	{
		setDBvalue("cantlpresupuesto", val);
	};
	inline void setpvplpresupuesto(QString val)
	{
		setDBvalue("pvplpresupuesto", val);
	};
	inline void setdescuentolpresupuesto(QString val)
	{
		setDBvalue("descuentolpresupuesto", val);
	};
	inline void setidpresupuesto(QString val)
	{
		setDBvalue("idpresupuesto", val);
	};
	inline void setivalpresupuesto(QString val)
	{
		setDBvalue("ivalpresupuesto", val);
	};
	void setcodigocompletoarticulo(QString);
	/// Estas funciones no estan como debe, el uso de cada una de ellas debe hacer
	/// cambios sobre la base de datos.
	void setidarticulo(QString);
	inline void setnomarticulo(QString val)
	{
		setDBvalue("nomarticulo", val);
	};
	/// Hace el calculo de la base imponible de la linea.
	float calculabase();
	/// Hace el calculo del IVA de la linea.
	float calculaiva();
	void borrar();
};

#endif
