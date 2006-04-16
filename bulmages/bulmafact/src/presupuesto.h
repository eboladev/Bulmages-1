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

#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "fixed.h"
#include "listlinpresupuestoview.h"
#include "listdescpresupuestoview.h"
#include "company.h"
#include "linpresupuesto.h"
#include "dbrecord.h"
/// Clase que hace de intermediaria entre la tabla de presupuesto de la base de
/// datos y el programa.


class presupuesto : public DBRecord
{

protected:
	listlinpresupuestoview *listalineas;
	ListDescuentoPresupuestoView *listadescuentos;
	company *companyact;

public:
	presupuesto(company *);
	/// Establece cual es la lista subformulario del presupuesto. Normalmente
	/// para apuntar listlinpresupuestoview.
	void setlislinpresupuesto(listlinpresupuestoview *a)
	{
		listalineas = a;
		listalineas->setcompany(companyact);
	};
	void setlisdescpresupuesto(ListDescuentoPresupuestoView *a)
	{
		listadescuentos = a;
		listadescuentos->setcompany(companyact);
	};
	listlinpresupuestoview* _listalineas()
	{
		return listalineas;
	};
	ListDescuentoPresupuestoView * _listadescuentos()
	{
		return listadescuentos;
	};
	company * _company()
	{
		return companyact;
	};
	QString idcliente()
	{
		return DBvalue("idcliente");
	};
	QString idalmacen()
	{
		return DBvalue("idalmacen");
	};
	QString numpresupuesto()
	{
		return DBvalue("numpresupuesto");
	};
	QString fpresupuesto()
	{
		return DBvalue("fpresupuesto");
	};
	QString vencpresupuesto()
	{
		return DBvalue("vencpresupuesto");
	};
	QString contactpresupuesto()
	{
		return DBvalue("contactpresupuesto");
	};
	QString telpresupuesto()
	{
		return DBvalue("telpresupuesto");
	};
	QString comentpresupuesto()
	{
		return DBvalue("comentpresupuesto");
	};
	QString idpresupuesto()
	{
		return DBvalue("idpresupuesto");
	};
	QString idusuari()
	{
		return DBvalue("idusuari");
	};
	QString procesadopresupuesto()
	{
		return DBvalue("procesadopresupuesto");
	};
	QString descpresupuesto()
	{
		return DBvalue("descpresupuesto");
	};
	QString refpresupuesto()
	{
		return DBvalue("refpresupuesto");
	};
	QString idforma_pago()
	{
		return DBvalue("idforma_pago");
	};
	QString idtrabajador()
	{
		return DBvalue("idtrabajador");
	};
	virtual ~presupuesto();
	virtual int cargar(QString);
	virtual void pintaPresupuesto();
	virtual int guardar();
	virtual int borrar();
	virtual void imprimirPresupuesto();
	virtual void calculaypintatotales();
	virtual QString detalleArticulos();
	virtual void pintaidcliente(QString)
	{
	};
	virtual void pintaIdAlmacen(QString)
	{
	};
	virtual void pintaNumPresupuesto(QString)
	{
	};
	virtual void pintaFPresupuesto(QString)
	{
	};
	virtual void pintaVencPresupuesto(QString)
	{
	};
	virtual void pintaContractPresupuesto(QString)
	{
	};
	virtual void pintaTelPresupuesto(QString)
	{
	};
	virtual void pintaComentPresupuesto(QString)
	{
	};
	virtual void pintaidalmacen(QString)
	{
	};
	virtual void pintaCodigoAlmacen(QString)
	{
	};
	virtual void pintaNomAlmacen(QString)
	{
	};
	virtual void pintaprocesadopresupuesto(QString)
	{
	};
	virtual void pintadescpresupuesto(QString)
	{
	};
	virtual void pintarefpresupuesto(QString)
	{
	};
	virtual void pintaidforma_pago(QString)
	{
	};
	virtual void pintaidtrabajador(QString)
	{
	};
	virtual void pintatotales(Fixed, Fixed, Fixed, Fixed)
	{
	};
	void setidcliente(QString val)
	{
		setDBvalue("idcliente", val);
	};
	void setidalmacen(QString val)
	{
		setDBvalue("idalmacen", val);
	};
	void setidtrabajador(QString val)
	{
		setDBvalue("idtrabajador", val);
	};
	void setnumpresupuesto(QString val)
	{
		setDBvalue("numpresupuesto", val);
	};
	void setfpresupuesto(QString val)
	{
		setDBvalue("fpresupuesto", val);
	};
	void setvencpresupuesto(QString val)
	{
		setDBvalue("vencpresupuesto", val);
	};
	void setcontactpresupuesto(QString val)
	{
		setDBvalue("contactpresupuesto", val);
	};
	void settelpresupuesto(QString val)
	{
		setDBvalue("telpresupuesto", val);
	};
	void setcomentpresupuesto(QString val)
	{
		setDBvalue("comentpresupuesto", val);
	};
	void setidpresupuesto(QString val)
	{
		setDBvalue("idpresupuesto", val);
		listalineas->setColumnValue("idpresupuesto",val);
		listadescuentos->setColumnValue("idpresupuesto",val);
	};
	void setprocesadopresupuesto(QString val)
	{
		setDBvalue("procesadopresupuesto", val);
	};
	void setdescpresupuesto(QString val)
	{
		setDBvalue("descpresupuesto", val);
	};
	void setrefpresupuesto(QString val)
	{
		setDBvalue("refpresupuesto", val);
	};
	void setidforma_pago(QString val)
	{
		setDBvalue("idforma_pago", val);
	};
	void vaciaPresupuesto();
};

#endif
