/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef COMPANY_H
#define COMPANY_H

#include <QObject>
#include <QWorkspace>

#include "postgresiface2.h"
#include "listventanas.h"


class ProveedorList;
class ClientsList;
class ClienteView;
class ArticuloList;
class orderslist;
class PresupuestoList;
class AlbaranClienteList;
class FacturasList;
class PedidosClienteList;
class PedidosProveedorList;
class AlbaranesProveedor;
class CobrosList;
class FacturasProveedorList;
class PagosList;
class PresupuestoView;
class FacturaView;
class FacturaProveedorView;
class ArticuloView;
class CobroView;
class AlbaranClienteView;
class PedidoClienteView;

class company : public postgresiface2
{

private:
	ProveedorList *m_providerslist;
	ClientsList *m_clientsList;
	ArticuloList *m_articleslist;
	PresupuestoList *m_budgetsList;
	AlbaranClienteList *m_clientDelivNotesList;
	FacturasList *m_facturasList;
	PedidosClienteList *m_pedidosclienteList;
	PedidosProveedorList *m_pedidosproveedorList;
	listventanas *m_listventanas;
	AlbaranesProveedor *m_albaranesproveedor;
	CobrosList *m_cobrosList;
	FacturasProveedorList *m_facturasproveedorlist;
	PagosList *m_pagosList;

public:
	QWorkspace *m_pWorkspace;

public:
	company();
	~company();
	void init(QString);
	void setListVentanas(listventanas *doc)
	{
		m_listventanas = doc;
	};
	void setWorkspace(QWorkspace *qw)
	{
		m_pWorkspace = qw;
	}
	void listproviders();
	void listClients();
	void s_newProveedor();
	void listarticles();
	void listorders();
	void lAlbaranesProveedor();
	void listBudgets();
	void listClientDelivNotes();
	void newClientDelivNote();
	void newPedidoCliente();
	void newPedidoProveedor();
	ClienteView * newClienteView();
	ArticuloView * newArticuloView();
	void createMainWindows();
	void refreshArticles();
	void refreshBudgets();
	void refreshPedidosCliente();
	void refreshPedidosProveedor();
	void refreshClientDelivNotes();
	void refreshAlbaranesCliente();
	void refreshAlbaranesProveedor();
	void refreshFacturas();
	void refreshClientes();
	int meteWindow(QString nom, QObject *obj)
	{
		return m_listventanas->meteWindow(nom, obj);
	};
	void sacaWindow(QObject *nom)
	{
		m_listventanas->sacaWindow(nom);
	};
	void s_FPago();
	void s_Familias();
	void s_trabajadores();
	void s_seriesFactura();
	void s_listPedidosCli();
	void s_listFacturasCli();
	void s_listPedidosPro();
	void s_listFacturasPro();
	void s_newFacturaPro();
	void s_newAlbaranPro();
	void s_newPedidoPro();
	void s_newPresupuestoCli();
	void s_newPedidoClienteView();
	void s_newAlbaranClienteView();
	void s_newFacturaCli();
	void s_newArticulo();
        void s_newCobroView();
	void s_newClienteView();
	void s_provincias();
	void s_almacenes();
	void s_inventarios();
	void s_newInventario();
	QString searchCompany();

	FacturaProveedorView* newFacturaProveedorView();
	PresupuestoView *newBudget();
	FacturaView *newFacturaView();
        CobroView   *newCobroView();
	AlbaranClienteView *newAlbaranClienteView();
	PedidoClienteView *newPedidoClienteView();
};

#endif
