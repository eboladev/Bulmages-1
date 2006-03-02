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

#ifndef BULMAFACT_H
#define BULMAFACT_H

#include "bulmafactbase.h"
#include "company.h"
#include "listventanas.h"

#include <Q3MainWindow>
#include <QWorkspace>
#include <Q3VBox>

class bulmafact: public bulmafactbase
{
	Q_OBJECT
private:
	company *m_company;
	Q3VBox *view_back;
	QWorkspace *pWorkspace;
	listventanas *m_list;

public:
	bulmafact(QString bd);
	~bulmafact();
	QWorkspace *workspace()
	{
		return pWorkspace;
	};

	company *getcompany() { return m_company; };
public slots:
	virtual void closeEvent( QCloseEvent *);
	virtual void listproviders();
	virtual void s_newProveedor();
	virtual void caja();
	virtual void clientes();
	virtual void emitirfactura();
	virtual void recibirfactura();
	virtual void listClients();
	virtual void newClient();
	virtual void listarticles();
	virtual void listdelivnotes();
	virtual void newClientDelivNote();
	virtual void listBudgets();
	virtual void listClientDelivNotes();
	virtual void s_FPago();
	virtual void s_Familias();
	virtual void s_trabajadores();
	virtual void s_ventanaCompleta();
	virtual void s_seriesFactura();
	virtual void s_listPedidosCli();
	virtual void s_listFacturasCli() { m_company->s_listFacturasCli(); };
	virtual void s_listPedidosPro() { m_company->s_listPedidosPro(); };
	virtual void s_listFacturasPro() { m_company->s_listFacturasPro(); };
	virtual void s_newFacturaPro() { m_company->s_newFacturaPro(); };
	virtual void s_newAlbaranPro() { m_company->s_newAlbaranPro(); };
	virtual void s_newPedidoPro() { m_company->s_newPedidoPro(); };
	virtual void s_newPresupuestoCli() { m_company->s_newPresupuestoCli(); };
	virtual void s_newPedidoCli() { m_company->s_newPedidoCli(); };
	virtual void s_newAlbaranCli() { m_company->s_newAlbaranCli(); };
	virtual void s_newFacturaCli() { m_company->s_newFacturaCli(); };
	virtual void s_newArticulo() { m_company->s_newArticulo(); };
	virtual void s_provincias() { m_company->s_provincias(); };
	virtual void s_inventarios() { m_company->s_inventarios(); };
	virtual void s_indexador()

	{
		if ( m_list->isHidden() )
		{
			m_list->show();
			m_list->undock();
			m_list->dock();
		} else {
			m_list->hide();
		}
	};

	void about();
	void aboutQt();
	virtual void s_About();
};
#endif

