/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

#ifndef clientslist_H
#define clientslist_H

#include <Q3Frame>

#include "clientslistbase.h"
#include "pgimportfiles.h"
#include "company.h"

class ClientsList : public ClientsListBase, public pgimportfiles
{
	Q_OBJECT

public:
	enum edmode
	{
		EditMode = 0,
		SelectMode = 1
	};

private:
	edmode m_mode;
	QString m_idclient;
	QString m_nomclient;
	QString m_cifclient;

public:
	company *companyact;
   
public:
	ClientsList(company *, QWidget *parent = 0, const char *name = 0,
			Qt::WFlags flag = 0, edmode editmode = EditMode);
	~ClientsList();
	void selectMode()
	{
		m_mode = SelectMode;
	};
	void editMode()
	{
		m_mode = EditMode;
	};
	void presenta();
	QString idclient()
	{
		return m_idclient;
	};
	QString nomclient()
	{
		return m_nomclient;
	};
	QString cifclient()
	{
		return m_cifclient;
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

	/// Funciones que se encarga en guardar y cargar la configuracion del listado.
	void guardaconfig();
	void cargaconfig();

public slots:
	virtual void newClient()
	{
		companyact->newClient();
	};
	virtual void editClient();
	virtual void m_clientList_clicked(int, int, int, const QPoint &);
	virtual void m_clientList_doubleClicked(int, int, int, const QPoint &);
	virtual void s_printClients();
	virtual void s_findClients();
	virtual void s_refreshClientes()
	{
		presenta();
	};
	virtual void s_exportar();
	virtual void s_importar();
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
	virtual void s_filtrar()
	{
		presenta();
	};
	virtual void s_borrarCliente();
	virtual void s_configurar();
};

#endif
