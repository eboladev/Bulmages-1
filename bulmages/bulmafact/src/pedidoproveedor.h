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

#ifndef PEDIDOPROVEEDOR_H
#define PEDIDOPROVEEDOR_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "listlinpedidoproveedorview.h"
#include "listdescpedidoproveedorview.h"
#include "fixed.h"
#include "company.h"
#include "dbrecord.h"
/// Clase que hace de intermediaria entre la tabla de factura de la base de datos y
/// el programa.


class PedidoProveedor : public DBRecord
{

protected:
	ListLinPedidoProveedorView *listalineas;
	ListDescuentoPedidoProveedorView *listadescuentos;
	company *companyact;

public:	
	PedidoProveedor(company *);
	virtual ~PedidoProveedor();
	/// Establece cual es la lista subformulario del presupuesto. Normalmente
	/// para apuntar listlinpresupuestoview.
	void setListLinPedidoProveedor(ListLinPedidoProveedorView *a)
	{
		listalineas = a;
		listalineas->setcompany(companyact);
	};
	void setListDescuentoPedidoProveedor(ListDescuentoPedidoProveedorView *a)
	{
		listadescuentos = a;
		listadescuentos->setcompany(companyact);
	};
	virtual int cargar(QString);
	virtual void pintar();
	virtual int guardar();
	virtual int borrar();
	void imprimirPedidoProveedor();
	void calculaypintatotales();   
	ListLinPedidoProveedorView* getlistalineas()
	{
		return listalineas;
	};
	ListDescuentoPedidoProveedorView* getlistadescuentos()
	{
		return listadescuentos;
	};
	virtual void pintaidproveedor(QString)
	{
	};
	virtual void pintaidalmacen(QString)
	{
	};
	virtual void pintaidpedidoproveedor(QString)
	{
	};
	virtual void pintanumpedidoproveedor(QString)
	{
	};
	virtual void pintafechapedidoproveedor(QString)
	{
	};
	virtual void pintadescpedidoproveedor(QString)
	{
	};
	virtual void pintanomalmacen(QString)
	{
	};
	virtual void pintaidforma_pago(QString)
	{
	};
	virtual void pintacodigoalmacen(QString)
	{
	};
	virtual void pintacomentpedidoproveedor(QString)
	{
	};
	virtual void pintaidpresupuesto(QString)
	{
	};
	virtual void pintadescpresupuesto(QString)
	{
	};
	virtual void pintarefpresupuesto(QString)
	{
	};
	virtual void pintarefpedidoproveedor(QString)
	{
	};
	virtual void pintaprocesadopedidoproveedor(QString)
	{
	};
	virtual void pintacontactpedidoproveedor(QString)
	{
	};
	virtual void pintatelpedidoproveedor(QString)
	{
	};
	virtual void pintaidtrabajador(QString)
	{
	};
	virtual void pintatotales(Fixed, Fixed, Fixed, Fixed)
	{
	};
	void setidproveedor(QString val)
	{
		setDBvalue("idproveedor", val);
	};
	void setidalmacen(QString val)
	{
		setDBvalue("idalmacen", val);
	};
	void setidpedidoproveedor(QString val)
	{
		setDBvalue("idpedidoproveedor", val);
		listalineas->setColumnValue( "idpedidoproveedor",val);
		listadescuentos->setColumnValue( "idpedidoproveedor",val);
	};
	void setnumpedidoproveedor(QString val)
	{
		setDBvalue("numpedidoproveedor", val);
	};
	void setfechapedidoproveedor(QString val)
	{
		setDBvalue("fechapedidoproveedor", val);
	};
	void setdescpedidoproveedor(QString val)
	{
		setDBvalue("descpedidoproveedor", val);
	};
	void setidforma_pago(QString val)
	{
		setDBvalue("idforma_pago", val);
	};
	void setcomentpedidoproveedor(QString val)
	{
		setDBvalue("comentpedidoproveedor", val);
	};
	void setrefpedidoproveedor(QString val)
	{
		setDBvalue("refpedidoproveedor", val);
	};
	void setprocesadopedidoproveedor(QString val)
	{
		setDBvalue("procesadopedidoproveedor", val);
	};
	void setcontactpedidoproveedor(QString val)
	{
		setDBvalue("contactpedidoproveedor", val);
	};
	void settelpedidoproveedor(QString val)
	{
		setDBvalue("telpedidoproveedor", val);
	};
	void setidtrabajador(QString val)
	{
		setDBvalue("idtrabajador", val);
	};
	void vaciaPedidoProveedor();
};

#endif
