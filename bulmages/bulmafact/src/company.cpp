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
#include "clientdelivnote.h"
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




company::company(){
   inicializa("bulmafact");
}// end company

void company::createMainWindows() {
   m_providerslist = new providerslist(this, m_pWorkspace,theApp->translate("Listado de Proveedores","company"));   
   m_articleslist = new articleslist(this, m_pWorkspace,theApp->translate("Listado de Artículos","company"));
   m_orderslist= new orderslist(this, m_pWorkspace,theApp->translate("Listado de Pedidos","company"));
   m_delivnoteslist = new delivnoteslist(this, m_pWorkspace,theApp->translate("Listado de Albaranes","company"));   
   m_budgetsList = new BudgetsList(this, m_pWorkspace,theApp->translate("Presupuestos","company"));  
   m_clientDelivNotesList = new ClientDelivNotesList(this, m_pWorkspace,theApp->translate("Listado de Albaranes de Clientes","company"));
}// end createMainWindows


company::~company(){
}


void company::listproviders () {
   m_providerslist->hide();
   m_providerslist->show();
   m_providerslist->setActiveWindow();
}

void company::listClients () {
   m_clientsList = new ClientsList(this, m_pWorkspace,theApp->translate("Listado de Clientes","company"));
   m_clientsList->hide();
   m_clientsList->show();
   m_clientsList->setActiveWindow();
}

void company::newClient() {
   m_clientEdit = new ClientEdit(this, m_pWorkspace,theApp->translate("Editar/Añadir cliente","company"));
   m_clientEdit->hide();
   m_clientEdit->show();
   m_clientEdit->setActiveWindow();
}   

void company::listarticles () {
   m_articleslist->hide();
   m_articleslist->show();
   m_articleslist->setActiveWindow();
}

void company::refreshArticles() {
   m_articleslist->inicializa();
}// end refreshOrders


void company::listorders () {
   m_orderslist->hide();
   m_orderslist->show();
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
   m_delivnoteslist->show();
   m_delivnoteslist->setActiveWindow();
}


void company::listBudgets() {
   m_budgetsList->hide();
   m_budgetsList->show();
   m_budgetsList->setActiveWindow();
}// end listbudgets

void company::newBudget() {
   Budget *bud = new Budget(this,m_pWorkspace,theApp->translate("Edicion de Proveedores", "company"));
   bud->show();
}// end bud

void company::refreshBudgets() {
   m_budgetsList->inicializa();
}// end refreshBudgets


void company::listClientDelivNotes() {
   m_clientDelivNotesList->hide();
   m_clientDelivNotesList->show();
   m_clientDelivNotesList->setActiveWindow();
}// end listbudgets

void company::newClientDelivNote() {
   ClientDelivNote *cDelivNote = new ClientDelivNote(this,m_pWorkspace,theApp->translate("Edicion de Albaranes", "company"));
   cDelivNote->show();
}// end newClientDelivNote

void company::refreshClientDelivNotes() {
   m_clientDelivNotesList->inicializa();
}// end refreshClientDelivNotes

