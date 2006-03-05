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

#ifndef LINPEDIDOPROVEEDOR_H
#define LINPEDIDOPROVEEDOR_H

#include "company.h"

class LinPedidoProveedor
{

private:
	QString mdb_numlpedidoproveedor;
	QString mdb_desclpedidoproveedor;
	QString mdb_cantlpedidoproveedor;
	QString mdb_pvplpedidoproveedor;
	QString mdb_prevlpedidoproveedor;
	QString mdb_ivalpedidoproveedor;
	QString mdb_descuentolpedidoproveedor;
	QString mdb_idpedidoproveedor;
	QString mdb_idarticulo;
	QString mdb_codigocompletoarticulo;
	QString mdb_nomarticulo;
	QString mdb_puntlpedidoproveedor;
	company *companyact;

public:
	LinPedidoProveedor(company *);
	LinPedidoProveedor(company *, QString);
	/// La carga rapida tiene un comportamiento poco restrictivo para aumentar
	/// la eficiencia.
	LinPedidoProveedor(company *comp, QString numlpedidoproveedor,
				QString desclpedidoproveedor, QString cantlpedidoproveedor,
				QString pvplpedidoproveedor, QString prevlpedidoproveedor,
				QString ivalpedidoproveedor, QString descuentolpedidoproveedor,
				QString idpedidoproveedor, QString idarticulo,
				QString codigocompletoarticulo, QString nomarticulo,
				QString puntlpedidoproveedor);
	virtual ~LinPedidoProveedor();
	virtual void pintaLinPedidoProveedor()
	{
	};
	inline QString numlpedidoproveedor()
	{
		return mdb_numlpedidoproveedor;
	};
	inline QString desclpedidoproveedor()
	{
		return mdb_desclpedidoproveedor;
	};
	inline QString cantlpedidoproveedor()
	{
		return mdb_cantlpedidoproveedor;
	};
	inline QString pvplpedidoproveedor()
	{
		return mdb_pvplpedidoproveedor;
	};
	inline QString prevlpedidoproveedor()
	{
		return mdb_prevlpedidoproveedor;
	};
	inline QString ivalpedidoproveedor()
	{
		return mdb_ivalpedidoproveedor;
	};
	inline QString descuentolpedidoproveedor()
	{
		return mdb_descuentolpedidoproveedor;
	};
	inline QString idpedidoproveedor()
	{
		return mdb_idpedidoproveedor;
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
	inline QString puntlpedidoproveedor()
	{
		return mdb_puntlpedidoproveedor;
	};
	inline void setnumlpedidoproveedor(QString val)
	{
		mdb_numlpedidoproveedor = val;
	};
	inline void setdesclpedidoproveedor(QString val)
	{
		mdb_desclpedidoproveedor = val;
	};
	inline void setcantlpedidoproveedor(QString val)
	{
		mdb_cantlpedidoproveedor = val;
	};
	inline void setpvplpedidoproveedor(QString val)
	{
		mdb_pvplpedidoproveedor = val;
	};
	inline void setprevlpedidoproveedor(QString val)
	{
		mdb_prevlpedidoproveedor = val;
	};
	inline void setivalpedidoproveedor(QString val)
	{
		mdb_ivalpedidoproveedor = val;
	};
	inline void setdescuentolpedidoproveedor(QString val)
	{
		mdb_descuentolpedidoproveedor = val;
	};
	inline void setidpedidoproveedor(QString val)
	{
		mdb_idpedidoproveedor = val;
	};
	inline void setnomarticulo(QString val)
	{
		mdb_nomarticulo = val;
	};
	inline void setpuntlpedidoproveedor(QString val)
	{
		mdb_puntlpedidoproveedor = val;
	};
	/*
	void setcodigocompletoarticulo(QString);  
	/// Estas funciones no estan como debe, el uso de cada una de ellas debe hacer
	/// cambios sobre la base de datos.
	void setidarticulo(QString);    
	inline void setnomarticulo(QString val) {mdb_nomarticulo=val;};
	*/
	/// Hace el calculo de la base imponible de la linea.
	float calculabase();
	/// Hace el calculo del IVA de la linea.
	float calculaiva();
	void borrar();
	void setidarticulo(QString val);
	void setcodigocompletoarticulo(QString val);
	void guardaLinPedidoProveedor();
	void vaciaLinPedidoProveedor();
};

#endif
