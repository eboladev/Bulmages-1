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

#ifndef FACTURAVIEW_H
#define FACTURAVIEW_H

#include <QLineEdit>
#include <Q3TextEdit>
#include <QLabel>
#include <QCheckBox>

#include <facturabase.h>
#include "factura.h"
#include "listlinpresupuestoview.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "busquedaformapago.h"
#include "busquedaseriefactura.h"
#include "busquedaalmacen.h"
#include "dialogchanges.h"
#include "postgresiface2.h"


class FacturaView : public FacturaBase, public Factura, public dialogChanges
{
	Q_OBJECT

public:
	FacturaView(company *, QWidget *parent = 0, const char *name = 0);
	~FacturaView();
	void inicialize();
	void pintaidcliente(QString id)
	{
		m_cliente->setidcliente(id);
	};
	void pintaidalmacen(QString id)
	{
		m_almacen->setidalmacen(id);
	};
	void pintaNumFactura(QString id)
	{
		m_numfactura->setText(id);
	};
	void pintacodigoserie_factura(QString id)
	{
		m_codigoserie_factura->setcodigoserie_factura(id);
	};
	void pintafechafactura(QString id)
	{
		m_fechafactura->setText(id);
	};
	void pintadescfactura(QString id)
	{
		m_descfactura->setText(id);
	};
	void pintaComentFactura(QString id)
	{
		m_comentfactura->setText(id);
	};
	void pintareffactura(QString id)
	{
		m_reffactura->setText(id);
	};
	void pintaidforma_pago(QString id)
	{
		m_forma_pago->setidforma_pago(id);
	};
	void pintaprocesadafactura(QString id)
	{
		//fprintf(stderr,"pintaprocesadafactura(%s)\n",id.ascii());
		if (id == "t" || id == "TRUE")
		{
			m_procesadafactura->setChecked(TRUE);
		} else {
			m_procesadafactura->setChecked(FALSE);
		}
	};
	void pintatotales(Fixed , Fixed , Fixed , Fixed);
	void closeEvent( QCloseEvent *);

public slots:
	virtual void s_comentfacturatextChanged()
	{
		setcomentfactura(m_comentfactura->text());
	};
	virtual void s_almacenvalueChanged(QString val)
	{
		setidalmacen(val);
	};
	virtual void s_numfacturatextChanged(const QString &val)
	{
		setNumFactura(val);
	};
	virtual void s_reffacturatextChanged(const QString &val)
	{
		setreffactura(val);
	};
	virtual void s_clientevalueChanged(QString val)
	{
		setidcliente(val);
	};
	virtual void s_fechafacturavalueChanged(QString val)
	{
		setfechafactura(val);
	};
	virtual void s_descfacturatextChanged(const QString &val)
	{
		setdescfactura(val);
	};
	virtual void s_forma_pagovalueChanged(QString val)
	{
		setidforma_pago(val);
	};
	virtual void s_serie_facturavalueChanged(QString val)
	{
		setcodigoserie_factura(val);
	};
	virtual void s_saveFactura()
	{
		guardaFactura();
	};
	virtual int cargaFactura(QString id);
	virtual void s_deleteFactura()
	{
		borraFactura();
	};
	virtual void s_printFactura()
	{
		imprimirFactura();
	};
	virtual void s_agregaAlbaran();

	/// Este slot se activa cuando hay cambios en los subformularios.
	virtual void s_pintaTotales()
	{
		calculaypintatotales();
	}
	virtual void s_procesadafacturastateChanged(int i)
	{
		//fprintf(stderr,"s_procesadafacturastateChanged(%d)\n",i);
		if (i)
		{
			setprocesadafactura("TRUE");
		} else {
			setprocesadafactura("FALSE");
		}
	};
	virtual void s_nuevoCobro();
	virtual void s_informeReferencia();
};

#endif
