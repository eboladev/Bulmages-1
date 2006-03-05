/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef FACTURAP_H
#define FACTURAP_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "listlinfacturap.h"
#include "company.h"
#include "linfacturap.h"
#include "listdescfacturaprov.h"
#include "dbrecord.h"


class FacturaProveedor : public DBRecord
{

protected:
	ListLinFacturaProveedor *listalineas;
	ListDescuentoFacturaProv *listadescuentos;
	company *companyact;

public:
	FacturaProveedor(company *);
	virtual ~FacturaProveedor();
	ListLinFacturaProveedor* getlistalineas()
	{
		return listalineas;
	};
	ListDescuentoFacturaProv* getlistadescuentos()
	{
		return listadescuentos;
	};
	void borraFacturaProveedor();
	void vaciaFacturaProveedor();
	/// Establece cual es la lista subformulario del presupuesto. Normalmente para
	/// apuntar listlinpresupuestoview.
	void setListLinFacturaProveedor(ListLinFacturaProveedor *a)
	{
		listalineas = a;
		listalineas->setcompany(companyact);
	};
	void setListDescuentoFacturaProv(ListDescuentoFacturaProv *a)
	{
		listadescuentos = a;
		listadescuentos->setcompany(companyact);
	};
	void setidproveedor(QString val)
	{
		setDBvalue("idproveedor", val);
	};
	void setreffacturap(QString val)
	{
		setDBvalue("reffacturap", val);
	};
	void setnumfacturap(QString val)
	{
		setDBvalue("numfacturap", val);
	};
	void setfechafacturap(QString val)
	{
		setDBvalue("ffacturap", val);
	};
	void setdescfacturap(QString val)
	{
		setDBvalue("descfacturap", val);
	};
	void setcomentfacturap(QString val)
	{
		setDBvalue("comentfacturap", val);
	};
	void setidfacturap(QString val)
	{
		setDBvalue("idfacturap", val);
		listalineas->setidfacturap(val);
		listadescuentos->setidfacturap(val);
	};
	void setIdUsuari(QString val)
	{
		setDBvalue("idusuari", val);
	};
	void setidforma_pago(QString val)
	{
		setDBvalue("idforma_pago", val);
	};
	void setprocesadafacturap(QString val)
	{
		setDBvalue("procesadafacturap", val);
	};
	virtual void imprimirFacturaProveedor();
	virtual int cargaFacturaProveedor(QString);
	virtual void pintaFacturaProveedor();
	virtual void guardaFacturaProveedor();
	virtual void pintaidproveedor(QString)
	{
	};
	virtual void pintareffacturap(QString)
	{
	};
	virtual void pintanumfacturap(QString)
	{
	};
	virtual void pintafechafacturap(QString)
	{
	};
	virtual void pintadescfacturap(QString)
	{
	};
	virtual void pintacomentfacturap(QString)
	{
	};
	virtual void pintaidforma_pago(QString)
	{
	};
	virtual void pintaprocesadafacturap(QString)
	{
	};
	virtual void pintatotales(float, float)
	{
	};
	virtual void cargaFacturaProveedorDescuentas(QString)
	{
	};
	virtual void calculateImports()
	{
	};
	virtual void inicialize()
	{
	};
	virtual QString calculateValues()
	{
		return "";
	};
};
#endif
