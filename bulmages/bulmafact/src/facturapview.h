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

#ifndef FACTURAPVIEW_H
#define FACTURAPVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include "facturapbase.h"
#include "facturap.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "dialogchanges.h"
#include "postgresiface2.h"
#include "funcaux.h"


class FacturaProveedorView : public FacturaProveedorBase, public FacturaProveedor,
	public dialogChanges
{
	Q_OBJECT

public:
	FacturaProveedorView(company *, QWidget *parent = 0, const char *name = 0);
	~FacturaProveedorView();
	void inicialize();
	void pintaidproveedor(QString id)
	{
		m_proveedor->setidproveedor(id);
	};
	void pintanumfacturap(QString id)
	{
		m_numfacturap->setText(id);
	};
	void pintafechafacturap(QString id)
	{
		m_fechafacturap->setText(id);
	};
	void pintadescfacturap(QString id)
	{
		m_descfacturap->setText(id);
	};
	void pintaComentFacturaProveedor(QString id)
	{
		m_comentfacturap->setText(id);
	};
	void pintareffacturap(QString id)
	{
		m_reffacturap->setText(id);
	};
	void pintaidforma_pago(QString id)
	{
		m_forma_pago->setidforma_pago(id);
	};
	void pintaprocesadafacturap(QString id)
	{
		//fprintf(stderr,"pintaprocesadafacturap(%s)\n",id.ascii());
		if (id == "t" || id == "TRUE")
		{
			m_procesadafacturap->setChecked(TRUE);
		} else {
			m_procesadafacturap->setChecked(FALSE);
		}
	};
	virtual void pintatotales(float base, float iva);
	void closeEvent(QCloseEvent *);

public slots:
	virtual void s_comentfacturaptextChanged()
	{
		setcomentfacturap(m_comentfacturap->text());
	};
	virtual void s_numfacturaptextChanged(const QString &val)
	{
		setnumfacturap(val);
	};
	virtual void s_reffacturaptextChanged(const QString &val)
	{
		setreffacturap(val);
	};
	virtual void s_proveedorvalueChanged(QString val)
	{
		setidproveedor(val);
	};
	virtual void s_fechafacturapvalueChanged(QString val)
	{
		setfechafacturap(val);
	};
	virtual void s_descfacturaptextChanged(const QString &val)
	{
		setdescfacturap(val);
	};
	virtual void s_forma_pagovalueChanged(QString val)
	{
		setidforma_pago(val);
	};
	virtual void s_saveFacturaProveedor()
	{
		guardaFacturaProveedor();
	};
	virtual int cargaFacturaProveedor(QString id);

	/// Este slot se activa cuando hay cambios en los subformularios.
	virtual void s_pintaTotales()
	{
		pintatotales(listalineas->calculabase(), listalineas->calculaiva());
	}
	virtual void s_procesadafacturapstateChanged(int i)
	{
		//fprintf(stderr,"s_procesadafacturapstateChanged(%d)\n",i);
		if (i)
		{
			setprocesadafacturap("TRUE");
		} else {
			setprocesadafacturap("FALSE");
		}
	};
	virtual void s_nuevoCobro();
};

#endif
