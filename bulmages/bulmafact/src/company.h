/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrás Riera                              *
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

#include <qworkspace.h>
#include "postgresiface2.h"
#include <qobject.h>

#include "listventanas.h"

/**
Company class gives the application something like a standard interface to access each company in the same way.
@author Tomeu Borrás
*/

class providerslist;
class ClientsList;
class ClientEdit;
class articleslist;
class orderslist;
class delivnoteslist;
class BudgetsList;
class ClientDelivNotesList;
class linorderslist;


class company : public postgresiface2 {
private:
   providerslist *m_providerslist;
   ClientsList *m_clientsList;
   ClientEdit *m_clientEdit;
   articleslist *m_articleslist;
   orderslist *m_orderslist;
   delivnoteslist *m_delivnoteslist;
   linorderslist *m_linorderslist;
   BudgetsList *m_budgetsList;
   ClientDelivNotesList *m_clientDelivNotesList;
   listventanas *m_listventanas;
public:
   QWidget *m_pWorkspace;
public:
   company();
   ~company();
   void init(QString);
   void setListVentanas(listventanas *doc) {m_listventanas= doc;};
   void listproviders();
   void listClients();
   void newClient();
   void listarticles();
   void listorders();
   void listdelivnotes();
   void listBudgets();
   void listClientDelivNotes();
   void setWorkspace(QWidget *qw) {m_pWorkspace=qw;}
   void newBudget();
   void newClientDelivNote();
   void newOrder();
   void createMainWindows();
   void refreshOrders();
   void refreshArticles();
   void refreshBudgets();
   void refreshClientDelivNotes();
   void meteWindow(QString nom, QObject *obj) {m_listventanas->meteWindow(nom, obj);};
   void sacaWindow(QObject *nom) {m_listventanas->sacaWindow(nom);};
   void s_FPago();
   QString searchCompany();
};

#endif
