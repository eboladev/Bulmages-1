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


class providerslist;
class ClientsList;
class ClienteView;
class articleslist;
class orderslist;
class BudgetsList;
class ClientDelivNotesList;
class FacturasList;
class PedidosClienteList;
class PedidosProveedorList;
class AlbaranesProveedor;
class CobrosList;
class FacturasProveedorList;
class PagosList;
class Budget;
class FacturaView;
class FacturaProveedorView;

class company : public postgresiface2
{

private:
	providerslist *m_providerslist;
	ClientsList *m_clientsList;
	articleslist *m_articleslist;
	BudgetsList *m_budgetsList;
	ClientDelivNotesList *m_clientDelivNotesList;
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
	void listproviders();
	void listClients();
	void s_newProveedor();
	void listarticles();
	void listorders();
	void lAlbaranesProveedor();
	void listBudgets();
	void listClientDelivNotes();
	void setWorkspace(QWorkspace *qw)
	{
		m_pWorkspace = qw;
	}
	void newClientDelivNote();
	void newPedidoCliente();
	void newPedidoProveedor();
	ClienteView * newClienteView();
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
	void s_newPedidoCli();
	void s_newAlbaranCli();
	void s_newFacturaCli();
	void s_newArticulo();
	void s_newClienteView();
	void s_provincias();
	void s_inventarios();
	void s_newInventario();
	QString searchCompany();
	FacturaProveedorView* newFacturaProveedorView();
	Budget *newBudget();
	FacturaView *newFacturaView();
};

#endif
