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

#ifndef ALBARANPROVEEDOR_H
#define ALBARANPROVEEDOR_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "company.h"
#include "dbrecord.h"
#include "listlinalbaranproveedor.h"
#include "listdescalbaranprov.h"


class AlbaranProveedor : public DBRecord
{
protected:
	ListLinAlbaranProveedor *listalineas;
	ListDescuentoAlbaranProv *listadescuentos;
	company *companyact;

public:
	AlbaranProveedor(company *);
	virtual ~AlbaranProveedor();

	/// Establece cual es la lista subformulario del presupuesto.
	/// Normalmente para apuntar listlinpresupuestoview.
	void setListLinAlbaranProveedor(ListLinAlbaranProveedor *a)
	{
		listalineas = a;
		listalineas->setcompany(companyact);
	};
	void setListDescuentoAlbaranProveedor(ListDescuentoAlbaranProv *a)
	{
		listadescuentos = a;
		listadescuentos->setcompany(companyact);
	};
	void pintaAlbaranProveedor();
	void guardaAlbaranProveedor();
	void borraAlbaranProveedor();
	ListLinAlbaranProveedor* getlistalineas()
	{
		return listalineas;
	};
	ListDescuentoAlbaranProv* getlistadescuentos()
	{
		return listadescuentos;
	};
	virtual int cargaAlbaranProveedor(QString);
	virtual void pintaidalbaranp(QString)
	{
	};
	virtual void pintanumalbaranp(QString)
	{
	};
	virtual void pintafechaalbaranp(QString)
	{
	};
	virtual void pintacomentalbaranp(QString)
	{
	};
	virtual void pintaidproveedor(QString)
	{
	};
	virtual void pintaidforma_pago(QString)
	{
	};
	virtual void pintaidalmacen(QString)
	{
	};
	virtual void pintadescalbaranp(QString)
	{
	};
	virtual void pintarefalbaranp(QString)
	{
	};
	virtual void pintatotales(float, float)
	{
	};
	void setidalbaranp(QString val)
	{
		setDBvalue("idalbaranp",val);
		listalineas->setidalbaranp(val);
		listadescuentos->setidalbaranp(val);
	};
	void setnumalbaranp(QString val)
	{
		setDBvalue("numalbaranp",val);
	};
	void setfechaalbaranp(QString val)
	{
		setDBvalue("fechaalbaranp",val);
	};
	void setloginusuario(QString val)
	{
		setDBvalue("loginusuario",val);
	};
	void setcomentalbaranp(QString val)
	{
		setDBvalue("comentalbaranp",val);
	};
	void setidproveedor(QString val)
	{
		setDBvalue("idproveedor",val);
	};
	void setidforma_pago(QString val)
	{
		setDBvalue("idforma_pago",val);
	};
	void setidalmacen(QString val)
	{
		setDBvalue("idalmacen",val);
	};
	void setrefalbaranp(QString val)
	{
		setDBvalue("refalbaranp",val);
	};
	void setdescalbaranp(QString val)
	{
		setDBvalue("descalbaranp",val);
	};
	void imprimirAlbaranProveedor();
	void vaciaAlbaranProveedor();    
};

#endif
