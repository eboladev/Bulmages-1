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

 // PRESUPUESTOS
/*
-- Entendemos que un presupuesto es una relación de materiales y trabajos cuantificada que
-- hacemos a petición de un cliente determinado
-- Numero
-- Data: Data d'emisió del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Telèfon.
-- Venciment: Data màxima de validesa del presupost.
-- Comentaris
--  Pressupost a clients.
CREATE TABLE presupuesto (
   idpresupuesto serial PRIMARY KEY,
   numpresupuesto integer,
   fpresupuesto date,
   contactpresupuesto character varying(90),
   telpresupuesto character varying(20),
   vencpresupuesto date,
   comentpresupuesto character varying(3000),
   idusuari integer,
   
   idcliente integer REFERENCES cliente(idcliente)
);
*/  

#include "budget.h"
#include "company.h"
#include "division.h"
#include "clientslist.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>

Budget::Budget(company *comp, QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   m_idpresupuesto = "";
   m_idclient = "";
}// end Budget

Budget::~Budget() {
}// end ~Budget


// Esta función carga un presupuesto.
void Budget::chargeBudget(QString idbudget) {
    m_idpresupuesto = idbudget;
    QString query = "SELECT * FROM presupuesto LEFT JOIN cliente ON cliente.idcliente = presupuesto.idcliente WHERE idpresupuesto="+idbudget;
    companyact->begin();
    cursor2 * cur= companyact->cargacursor(query, "querypresupuesto");
    companyact->commit();
    if (!cur->eof()) {
      m_idclient = cur->valor("idcliente");
      m_numpresupuesto->setText(cur->valor("numpresupuesto"));
      m_fpresupuesto->setText(cur->valor("fpresupuesto"));
      m_vencpresupuesto->setText(cur->valor("vencpresupuesto"));
      m_contactpresupuesto->setText(cur->valor("contactpresupuesto"));
      m_telpresupuesto->setText(cur->valor("telpresupuesto"));
      m_comentpresupuesto->setText(cur->valor("comentpresupuesto"));
      m_nomclient->setText(cur->valor("nomcliente"));
      m_cifclient->setText(cur->valor("cifcliente"));
   
    }// end if
     delete cur;   
}// end chargeBudget


// Búsqueda de Clientes.
void Budget::searchClient() {
   fprintf(stderr,"Busqueda de un client\n");
   ClientsList *clients = new ClientsList(companyact, NULL, theApp->translate("Seleccione cliente","company"));
   
// , WType_Dialog| WShowModal   
   clients->selectMode();
   
   // Esto es convertir un QWidget en un sistema modal de dialogo.
   this->setEnabled(false);
   clients->show();
   while(!clients->isHidden()) theApp->processEvents();
   this->setEnabled(true);
   
   m_idclient = clients->idclient();
   m_cifclient->setText(clients->cifclient());
   m_nomclient->setText(clients->nomclient());
   delete clients;
}// end searchClient
