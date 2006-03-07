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

#ifndef PEDIDOPROVEEDORVIEW_H
#define PEDIDOPROVEEDORVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include <pedidoproveedorbase.h>
#include "factura.h"
#include "listlinpedidoproveedorview.h"
#include "listdescpedidoproveedorview.h"
#include "busquedaproveedor.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "dialogchanges.h"
#include "fixed.h"
#include "pedidoproveedor.h"
#include "postgresiface2.h"


class PedidoProveedorView : public PedidoProveedorBase, public PedidoProveedor,
				public dialogChanges
{
	Q_OBJECT

public:
	PedidoProveedorView(company *, QWidget *parent = 0, const char *name = 0);
	~PedidoProveedorView();
	void closeEvent(QCloseEvent *);
	void generarAlbaran();
	void inicialize();
	void pintaidproveedor(QString id)
	{
		m_proveedor->setidproveedor(id);
	};
	void pintaidalmacen(QString id)
	{
		m_almacen->setidalmacen(id);
	};
	void pintaidpedidoproveedor(QString)
	{
	};
	void pintanumpedidoproveedor(QString id)
	{
		m_numpedidoproveedor->setText(id);
	};
	void pintafechapedidoproveedor(QString id)
	{
		m_fechapedidoproveedor->setText(id);
	};
	void pintadescpedidoproveedor(QString id)
	{
		m_descpedidoproveedor->setText(id);
	};
	void pintaidforma_pago(QString id)
	{
		m_forma_pago->setidforma_pago(id);
	};
	void pintaidtrabajador(QString id)
	{
		m_trabajador->setidtrabajador(id);
	};
	void pintacomentpedidoproveedor(QString id)
	{
		m_comentpedidoproveedor->setText(id);
	};
	void pintarefpedidoproveedor(QString id)
	{
		m_refpedidoproveedor->setText(id);
	};
	void pintacontactpedidoproveedor(QString id)
	{
		m_contactpedidoproveedor->setText(id);
	};
	void pintatelpedidoproveedor(QString id)
	{
		m_telpedidoproveedor->setText(id);
	};
	void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);
	void pintaprocesadopedidoproveedor(QString id)
	{
		if (id == "t" || id == "TRUE")
		{
			m_procesadopedidoproveedor->setChecked(TRUE);
		} else {
			m_procesadopedidoproveedor->setChecked(FALSE);
		}
	};    

public slots:
	virtual bool close(bool);
	virtual void s_comentpedidoproveedortextChanged()
	{
		setcomentpedidoproveedor(m_comentpedidoproveedor->text());
	};
	virtual void s_numpedidoproveedortextChanged(const QString &val)
	{
		setnumpedidoproveedor(val);
	};
	virtual void s_proveedorvalueChanged(QString val)
	{
		setidproveedor(val);
	};
	virtual void s_fechapedidoproveedorvalueChanged(QString val)
	{
		setfechapedidoproveedor(val);
	};
	virtual void s_almacenvalueChanged(QString val)
	{
		setidalmacen(val);
	};
	virtual void s_trabajadorvalueChanged(QString val)
	{
		setidtrabajador(val);
	};
	virtual void s_forma_pagovalueChanged(QString val)
	{
		setidforma_pago(val);
	};
	virtual void s_refpedidoproveedortextChanged(const QString &val)
	{
		setrefpedidoproveedor(val);
	};
	virtual void s_descpedidoproveedortextChanged(const QString &val)
	{
		setdescpedidoproveedor(val);
	};
	virtual void s_savePedidoProveedor()
	{
		guardaPedidoProveedor();
	};
	virtual int cargaPedidoProveedor(QString id);
	virtual void s_deletePedidoProveedor()
	{
		borraPedidoProveedor();
	};
	virtual void s_printPedidoProveedor()
	{
		imprimirPedidoProveedor();
	};
	virtual void s_contactpedidoproveedortextChanged(const QString &val)
	{
		setcontactpedidoproveedor(val);
	};
	virtual void s_telpedidoproveedortextChanged(const QString &val)
	{
		settelpedidoproveedor(val);
	};
	virtual void s_procesadopedidoproveedorstateChanged(int i)
	{
		if (i)
		{
			setprocesadopedidoproveedor("TRUE");
		} else {
			setprocesadopedidoproveedor("FALSE");
		}
	};
	/// Este slot se activa cuando hay cambios en los subformularios.
	virtual void s_pintaTotales()
	{
		calculaypintatotales();
	};
	virtual void s_verpresupuesto();
	virtual void s_generarAlbaran()
	{
		generarAlbaran();
	};
	virtual void s_nuevoCobro();
};

#endif
