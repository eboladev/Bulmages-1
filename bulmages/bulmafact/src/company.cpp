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
 
 
#include "company.h"
#include "stdio.h"
#include <qnamespace.h>
#include "providerslist.h"
#include "provedit.h"
#include "qobject.h"
#include "clientslist.h"
#include "articleslist.h"
#include "orderslist.h"
#include "delivnoteslist.h"

company::company(){
//       inicializa(confpr->valor(CONF_METABASE).c_str());
       inicializa("bulmafact");
}


company::~company(){
}


void company::listproviders () {
/*
   providerslist *m_provlist = new providerslist(this, m_pWorkspace,theApp->translate("Hola mundo.","company"));
   m_provlist->show();
*/
   providerslist *m_provlist = new providerslist(this, NULL,theApp->translate("Hola mundo.","company"));
   m_provlist->modoseleccion();
   m_provlist->exec();
   fprintf(stderr,"El id seleccionado es: %s",m_provlist->idprovider().ascii());
   delete m_provlist;

}

void company::listclients () {
   clientslist *m_clielist = new clientslist(this, m_pWorkspace,theApp->translate("Hola mundo.","company"));
   m_clielist->show();
}

void company::listarticles () {
   articleslist *m_artilist = new articleslist(this, m_pWorkspace,theApp->translate("Hola mundo.","company"));
   m_artilist->show();
}

void company::listorders () {
   orderslist *m_orderlist = new orderslist(this, m_pWorkspace,theApp->translate("Hola mundo.","company"));
   m_orderlist->show();
}

void company::listdelivnotes () {
   delivnoteslist *m_delivnoteslist = new delivnoteslist(this, m_pWorkspace,theApp->translate("Hola mundo.","company"));
   m_delivnoteslist->show();
}

void company::provideraction() {
   provedit *prov = new provedit(this,0,theApp->translate("Edicion de Proveedores", "company"));
   prov->exec();
   delete prov;
}// end provideraction

