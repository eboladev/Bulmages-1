/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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
 
#include <qnamespace.h>

#include "company.h"
#include "stdio.h"
#include "budget.h"
#include "albaranclienteview.h"
#include "providerslist.h"
#include "provedit.h"
#include "qobject.h"
#include "clientslist.h"
#include "clientedit.h"
#include "articleslist.h"
#include "orderslist.h"
#include "delivnoteslist.h"
#include "budgetslist.h"
#include "clientdelivnoteslist.h"
#include "linorderslist.h"
#include "abreempresaview.h"
#include "fpago.h"
#include "familiasview.h"
#include "facturaslist.h"
#include "pedidosclientelist.h"
#include "pedidoclienteview.h"



company::company(){
}// end company


void company::init(QString bd) {
   if (bd == "") 
   	bd = searchCompany();
   inicializa(bd);
}// end init

/** \brief Se utiliza para mostrar un selector de empresas \ref abreempresaview
  * Al usuario debe seleccionar una empresa y el sistema empieza la inicialización de clases a partir de dicha inicialización.
  */
QString company::searchCompany() {
//El cambio de empresa se realiza desde el selector.
  fprintf(stderr,"empresa::searchCompany vamos a mostrar el abreempresaview\n");
  abreempresaview *nuevae = new abreempresaview(0,"BulmaFact" );
  nuevae->exec();
   fprintf(stderr,"Vamos a cambiar la empresa \n");
   QString bd= nuevae->nomDB();
   fprintf(stderr,"Empresa cambiada a %s\n", bd.ascii());
   delete nuevae;
  return(bd);
}// end searchCompany



void company::createMainWindows() {
   m_providerslist = new providerslist(this, m_pWorkspace,theApp->translate("Proveedores","company"));   
   m_articleslist = new articleslist(this, m_pWorkspace,theApp->translate("Artículos","company"));
   m_orderslist= new orderslist(this, m_pWorkspace,theApp->translate("Pedidos a Proveedores","company"));
   m_delivnoteslist = new delivnoteslist(this, m_pWorkspace,theApp->translate("Albaranes","company"));   
   m_budgetsList = new BudgetsList(this, m_pWorkspace,theApp->translate("Presupuestos","company"));  
   m_clientsList = new ClientsList(this, m_pWorkspace,theApp->translate("Clientes","company"));   
   m_clientDelivNotesList = new ClientDelivNotesList(this, m_pWorkspace,theApp->translate("Listado de Albaranes de Clientes","company"));
   m_facturasList = new FacturasList(this, m_pWorkspace,theApp->translate("Facturas","company"));
   m_pedidosclienteList = new PedidosClienteList(this, m_pWorkspace,theApp->translate("Pedidos de Clientes","company"));   
   fprintf(stderr,"Fin de createMainWindows\n");
}// end createMainWindows


company::~company(){
}


void company::listproviders () {
   m_providerslist->hide();
   m_providerslist->showMaximized();
   m_providerslist->setActiveWindow();
}

void company::listClients () {
   m_clientsList->hide();
   m_clientsList->showMaximized();
   m_clientsList->setActiveWindow();
}

void company::newClient() {
   m_clientEdit = new ClientEdit(this, m_pWorkspace,theApp->translate("Editar/Añadir cliente","company"));
   m_clientEdit->hide();
   m_clientEdit->showMaximized();
   m_clientEdit->setActiveWindow();
}   

void company::listarticles () {
   m_articleslist->hide();
   m_articleslist->showMaximized();
   m_articleslist->setActiveWindow();
}

void company::refreshArticles() {
   m_articleslist->inicializa();
}// end refreshOrders


void company::listorders () {
   m_orderslist->hide();
   m_orderslist->showMaximized();
   m_orderslist->setActiveWindow();
}

void company::refreshOrders() {
   m_orderslist->inicializa();
}// end refreshOrders

void company::newOrder() {
   linorderslist *order = new linorderslist(this,m_pWorkspace,theApp->translate("Nuevo Pedido", "company"));
	order->chargelinorders("0");
   order->show();
}// end newOrder


void company::listdelivnotes () {
   m_delivnoteslist->hide();
   m_delivnoteslist->showMaximized();
   m_delivnoteslist->setActiveWindow();
}


void company::listBudgets() {
   m_budgetsList->hide();
   m_budgetsList->showMaximized();
   m_budgetsList->setActiveWindow();
}// end listbudgets

void company::newBudget() {
   Budget *bud = new Budget(this , m_pWorkspace,theApp->translate("Edicion de Proveedores", "company"));
   bud->show();
}// end bud

void company::refreshBudgets() {
   m_budgetsList->inicializa();
}// end refreshBudgets


void company::listClientDelivNotes() {
   m_clientDelivNotesList->hide();
   m_clientDelivNotesList->showMaximized();
   m_clientDelivNotesList->setActiveWindow();
}// end listbudgets

void company::newClientDelivNote() {
   AlbaranClienteView *cDelivNote = new AlbaranClienteView(this,m_pWorkspace,theApp->translate("Edicion de Albaranes", "company"));
   cDelivNote->showMaximized();
}// end newClientDelivNote

void company::refreshClientDelivNotes() {
   m_clientDelivNotesList->inicializa();
}// end refreshClientDelivNotes



void company::newPedidoCliente() {
   PedidoClienteView *bud = new PedidoClienteView(this , m_pWorkspace,theApp->translate("Edicion de Pedidos de Cliente", "company"));
   bud->show();
}// end bud

void company::refreshPedidosCliente() {
   m_pedidosclienteList->inicializa();
}// end refreshPedidosCliente


/** Presenta la ventana de formas de pago y espera la ejecución de la misma */
void company::s_FPago() {
   fpago *f = new fpago(this,NULL,theApp->translate("Formas de Pago", "company"));
   f->exec();
   delete f;
}// end newOrder


void company::s_Familias() {
	familiasview *fam = new familiasview(this, 0,0);
	fam->exec();
	delete fam;
}// end s_Familias

