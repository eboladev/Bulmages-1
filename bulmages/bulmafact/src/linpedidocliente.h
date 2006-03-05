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

#ifndef LINPEDIDOCLIENTE_H
#define LINPEDIDOCLIENTE_H

#include "company.h"


class LinPedidoCliente
{

private:
	QString mdb_numlpedidocliente;
	QString mdb_desclpedidocliente;
	QString mdb_cantlpedidocliente;
	QString mdb_pvplpedidocliente;
	QString mdb_prevlpedidocliente;
	QString mdb_ivalpedidocliente;
	QString mdb_descuentolpedidocliente;
	QString mdb_idpedidocliente;
	QString mdb_idarticulo;
	QString mdb_codigocompletoarticulo;
	QString mdb_nomarticulo;
	QString mdb_puntlpedidocliente;
	company *companyact;

public:
	LinPedidoCliente(company *);
	LinPedidoCliente(company *, QString);
	/// La carga rapida tiene un comportamiento poco restrictivo para aumentar
	/// la eficiencia.
	LinPedidoCliente(company *comp, QString numlpedidocliente, QString desclpedidocliente,
				QString cantlpedidocliente, QString pvplpedidocliente,
				QString prevlpedidocliente, QString ivalpedidocliente,
				QString descuentolpedidocliente, QString idpedidocliente,
				QString idarticulo, QString codigocompletoarticulo,
				QString nomarticulo, QString puntlpedidocliente);
	virtual ~LinPedidoCliente();
	virtual void pintaLinPedidoCliente()
	{
	};
	void guardaLinPedidoCliente();
	void vaciaLinPedidoCliente();
	inline QString numlpedidocliente()
	{
		return mdb_numlpedidocliente;
	};
	inline QString desclpedidocliente()
	{
		return mdb_desclpedidocliente;
	};
	inline QString cantlpedidocliente()
	{
		return mdb_cantlpedidocliente;
	};
	inline QString pvplpedidocliente()
	{
		return mdb_pvplpedidocliente;
	};
	inline QString prevlpedidocliente()
	{
		return mdb_prevlpedidocliente;
	};
	inline QString ivalpedidocliente()
	{
		return mdb_ivalpedidocliente;
	};
	inline QString descuentolpedidocliente()
	{
		return mdb_descuentolpedidocliente;
	};
	inline QString idpedidocliente()
	{
		return mdb_idpedidocliente;
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
	inline QString puntlpedidocliente()
	{
		return mdb_puntlpedidocliente;
	};
	inline void setnumlpedidocliente(QString val)
	{
		mdb_numlpedidocliente = val;
	};
	inline void setdesclpedidocliente(QString val)
	{
		mdb_desclpedidocliente = val;
	};
	inline void setcantlpedidocliente(QString val)
	{
		mdb_cantlpedidocliente = val;
	};
	inline void setpvplpedidocliente(QString val)
	{
		mdb_pvplpedidocliente = val;
	};
	inline void setprevlpedidocliente(QString val)
	{
		mdb_prevlpedidocliente = val;
	};
	inline void setivalpedidocliente(QString val)
	{
		mdb_ivalpedidocliente = val;
	};
	inline void setdescuentolpedidocliente(QString val)
	{
		mdb_descuentolpedidocliente = val;
	};
	inline void setidpedidocliente(QString val)
	{
		mdb_idpedidocliente = val;
	};
	void setidarticulo(QString val);
	void setcodigocompletoarticulo(QString val);
	inline void setnomarticulo(QString val)
	{
		mdb_nomarticulo = val;
	};
	inline void setpuntlpedidocliente(QString val)
	{
		mdb_puntlpedidocliente = val;
	};
	/*
	void setcodigocompletoarticulo(QString);  
	/// Estas funciones no estan como debe, el uso de cada una de ellas debe hacer
	/// cambios sobre la base de datos.
	void setidarticulo(QString);    
	inline void setnomarticulo(QString val)
	{
		mdb_nomarticulo = val;
	};
	*/
	/// Hace el calculo de la base imponible de la linea.
	float calculabase();
	/// Hace el calculo del IVA de la linea.
	float calculaiva();
	void borrar();
};

#endif
