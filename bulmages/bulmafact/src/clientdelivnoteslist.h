/***************************************************************************
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

#ifndef CLIENTDELIVNOTESLIST_H
#define CLIENTDELIVNOTESLIST_H

// Listado de albaranes de clientes.
// Clients delivery notes list.

#include "clientdelivnoteslistbase.h"
#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"


class AlbaranClienteList : public AlbaranClienteListBase
{
	Q_OBJECT

public:
	enum edmode
	{
		EditMode = 0,
		SelectMode = 1
	};

private:
	company *companyact;
	/// m_modo == 0 es modo edicion
	/// m_modo == 1 es modo selector.
	int m_modo;
	QString m_idclidelivnote;
	void inicializa();

public:
	AlbaranClienteList(QWidget *parent = 0, const char *name = 0,
				Qt::WFlags flag = 0, edmode editmodo = EditMode);
	AlbaranClienteList(company *, QWidget *parent = 0, const char *name = 0,
				Qt::WFlags flag = 0, edmode editmodo = EditMode);
	~AlbaranClienteList();
	void presenta();
	void modoseleccion()
	{
		m_modo = 1;
	};
	void modoedicion()
	{
		m_modo=0;
	};
	void imprimir();
	void setcompany (company *comp)
	{
		companyact = comp;
		m_cliente->setcompany(comp);
	};
	void hideBotonera()
	{
		m_botonera->hide();
	};
	void showBotonera()
	{
		m_botonera->show();
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
	void setidcliente(QString val)
	{
		m_cliente->setidcliente(val);
	};
	QString idCliDelivNote()
	{
		return m_idclidelivnote;
	};
	void meteWindow(QString nom, QObject *obj)
	{
		if (companyact != NULL)
		{
			companyact->meteWindow(nom, obj);
		}
	};
	QString generarFiltro();

	/// Funciones que se encarga en guardar y cargar la configuracion del listado.
	void guardaconfig();
	void cargaconfig();

public slots:
	virtual void s_doubleclicked(int, int, int, const QPoint &);
	virtual void s_newClientDelivNote()
	{
		companyact->s_newAlbaranCli();
	};
	virtual void s_removeClientDelivNote();
	virtual void s_contextMenu(int, int, int, const QPoint &);
	virtual void s_configurar();
	virtual void s_edit();
	virtual void s_searchClientDelivNote()
	{
		presenta();
	};
	virtual void s_printClientDelivNote()
	{
		imprimir();
	};
	virtual void s_filtrar()
	{
		presenta();
	};
	virtual void s_mostrarBusqueda()
	{
		if (m_busqueda->isVisible())
		{
			hideBusqueda();
		} else {
			showBusqueda();
		}
	};
	virtual void s_mostrarConfiguracion()
	{
		if (m_configuracion->isVisible())
		{
			hideConfiguracion();
		} else {
			showConfiguracion();
		}
	};
};

#endif
