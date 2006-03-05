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

#ifndef FACTURASPROVEEDORLIST_H
#define FACTURASPROVEEDORLIST_H

#include <Q3Frame>

#include "facturasplistbase.h"
#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"


class FacturasProveedorList : public FacturasProveedorListBase
{
	Q_OBJECT

private:
	company *companyact;
	/// m_modo == 0 es modo edicion
	/// m_modo == 1 es modo selector.
	int m_modo;
	QString m_idfacturap;
	void inicializa();

public:
	FacturasProveedorList(QWidget *parent = 0, const char *name = 0, Qt::WFlags flag = 0);
	FacturasProveedorList(company *,QWidget *parent = 0, const char *name = 0);
	~FacturasProveedorList();
	void presenta();
	void setcompany (company *comp)
	{
		companyact = comp;
		m_proveedor->setcompany(comp);
		m_articulo->setcompany(comp);
	};
	void meteWindow(QString nom, QObject *obj)
	{
		if (companyact != NULL)
		{
			companyact->meteWindow(nom, obj);
		}
	};
	void modoseleccion()
	{
		m_modo = 1;
	};
	void modoedicion()
	{
		m_modo = 0;
	};
	QString idfacturap()
	{
		return m_idfacturap;
	};
	void hideBusqueda()
	{
		m_busqueda->hide();
	};
	void showBusqueda()
	{
		m_busqueda->show();
	};
	void hideConfiguracion()
	{
		m_configuracion->hide();
	};
	void showConfiguracion()
	{
		m_configuracion->show();
	};
	void hideBotonera()
	{
		m_botonera->hide();
	};
	void showBotonera()
	{
		m_botonera->show();
	};
	void setidproveedor(QString val)
	{
		m_proveedor->setidproveedor(val);
	};
	void setidarticulo(QString val)
	{
		m_articulo->setidarticulo(val);
	};
	QString generaFiltro();

	/// Funciones que se encargan de guardar y cargar la configuracion del listado.
	void guardaconfig();
	void cargaconfig();

public slots:
	virtual void doubleclicked(int, int , int , const QPoint &);
	virtual void s_filtrar()
	{
		presenta();
	};
	virtual void s_nuevaFacturaProveedor()
	{
		companyact->s_newFacturaPro();
	};
	virtual void s_editarFacturaProveedor();
	virtual void s_borrarFacturaProveedor();
	virtual void s_actualizarFacturaProveedor()
	{
		presenta();
	};
	virtual void s_mostrarBusqueda()
	{
		_depura("s_mostrarBusqueda", 0);
		if (m_busqueda->isVisible())
		{
			hideBusqueda();
		} else {
			showBusqueda();
		}
	};
	virtual void s_mostrarConfiguracion()
	{
		_depura("s_mostrarConfiguracion", 0);
		if (m_configuracion->isVisible())
		{
			hideConfiguracion();
		} else {
			showConfiguracion();
		}
	};
	virtual void s_configurar();
	virtual void s_imprimir();
};

#endif
