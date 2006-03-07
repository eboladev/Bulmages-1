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

#ifndef PEDIDOCLIENTEVIEW_H
#define PEDIDOCLIENTEVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include <pedidoclientebase.h>
#include "factura.h"
#include "listlinpedidoclienteview.h"
#include "listdescpedidoclienteview.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaalmacen.h"
#include "busquedatrabajador.h"
#include "dialogchanges.h"
#include "fixed.h"
#include "pedidocliente.h"
#include "postgresiface2.h"


class PedidoClienteView : public PedidoClienteBase, public PedidoCliente, public dialogChanges
{
	Q_OBJECT

public:
	PedidoClienteView(company *, QWidget *parent = 0, const char *name = 0);
	~PedidoClienteView();
	void closeEvent(QCloseEvent *);
	void generarAlbaran();
	void pintaidcliente(QString id)
	{
		m_cliente->setidcliente(id);
	};
	void pintaidalmacen(QString id)
	{
		m_almacen->setidalmacen(id);
	};
	void pintaidpedidocliente(QString)
	{
	};
	void pintanumpedidocliente(QString id)
	{
		m_numpedidocliente->setText(id);
	};
	void pintafechapedidocliente(QString id)
	{
		m_fechapedidocliente->setText(id);
	};
	void pintadescpedidocliente(QString id)
	{
		m_descpedidocliente->setText(id);
	};
	void pintaidforma_pago(QString id)
	{
		m_forma_pago->setidforma_pago(id);
	};
	void pintaidtrabajador(QString id)
	{
		m_trabajador->setidtrabajador(id);
	};
	void pintacomentpedidocliente(QString id)
	{
		m_comentpedidocliente->setText(id);
	};
	void pintarefpedidocliente(QString id)
	{
		m_refpedidocliente->setText(id);
	};
	void pintacontactpedidocliente(QString id)
	{
		m_contactpedidocliente->setText(id);
	};
	void pintatelpedidocliente(QString id)
	{
		m_telpedidocliente->setText(id);
	};
	void pintatotales(Fixed iva, Fixed base, Fixed total, Fixed desc);
	void pintaprocesadopedidocliente(QString id)
	{
		if (id == "t" || id == "TRUE") {
			m_procesadopedidocliente->setChecked(TRUE);
		} else {
			m_procesadopedidocliente->setChecked(FALSE);
		}
	};

public slots:
	virtual bool close(bool);
	virtual void s_comentpedidoclientetextChanged()
	{
		setcomentpedidocliente(m_comentpedidocliente->text());
	};
	virtual void s_numpedidoclientetextChanged(const QString &val)
	{
		setnumpedidocliente(val);
	};
	virtual void s_clientevalueChanged(QString val)
	{
		setidcliente(val);
	};
	virtual void s_fechapedidoclientevalueChanged(QString val)
	{
		setfechapedidocliente(val);
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
	virtual void s_refpedidoclientetextChanged(const QString &val)
	{
		setrefpedidocliente(val);
	};
	virtual void s_descpedidoclientetextChanged(const QString &val)
	{
		setdescpedidocliente(val);
	};
	virtual void s_savePedidoCliente()
	{
			guardaPedidoCliente();
	};
	virtual int cargaPedidoCliente(QString id);
	virtual void s_deletePedidoCliente()
	{
		borraPedidoCliente();
	};
	virtual void s_printPedidoCliente()
	{
		imprimirPedidoCliente();
	};
	virtual void s_contactpedidoclientetextChanged(const QString &val)
	{
		setcontactpedidocliente(val);
	};
	virtual void s_telpedidoclientetextChanged(const QString &val)
	{
		settelpedidocliente(val);
	};
	virtual void s_procesadopedidoclientestateChanged(int i)
	{
		if (i)
		{
			setprocesadopedidocliente("TRUE");
		} else {
			setprocesadopedidocliente("FALSE");
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
	virtual void s_informeReferencia();
};

#endif
