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
 
 // Implementación del listado de presupuestos.
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
#include "budgetslist.h"
#include "company.h"
#include "budget.h"
#include <qtable.h>

#define COL_IDPRESUPUESTO 0
#define COL_NUMPRESUPUESTO 1
#define COL_FPRESUPUESTO 2
#define COL_CONTACTPRESUPUESTO 3
#define COL_TELPRESUPUESTO 4
#define COL_COMENTPRESUPUESTO 5
#define COL_IDUSUARI 6
#define COL_IDCLIENTE 7

BudgetsList::BudgetsList(company *comp, QWidget *parent, const char *name, int flag)
 : BudgetsListBase(parent, name, flag) {
      companyact = comp;
      inicializa();
      m_modo=0;
      m_idpresupuesto="";
}// end providerslist

BudgetsList::~BudgetsList() {
}// end ~providerslist

void BudgetsList::inicializa() {
   m_list->setNumRows( 0 );
   m_list->setNumCols( 0 );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setSorting( TRUE );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setColumnMovingEnabled( TRUE );
   m_list->setNumCols(8);
   m_list->horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "COL_IDPRESUPUESTO" ) );
   m_list->horizontalHeader()->setLabel( COL_NUMPRESUPUESTO, tr( "COL_NUMPRESUPUESTO" ) );
   m_list->horizontalHeader()->setLabel( COL_FPRESUPUESTO, tr( "COL_FPRESUPUESTO" ) );
   m_list->horizontalHeader()->setLabel( COL_CONTACTPRESUPUESTO, tr( "COL_CONTACTPRESUPUESTO" ) );
   m_list->horizontalHeader()->setLabel( COL_TELPRESUPUESTO, tr( "COL_TELPRESUPUESTO" ) );
   m_list->horizontalHeader()->setLabel( COL_COMENTPRESUPUESTO, tr( "COL_COMENTPRESUPUESTO" ) );
   m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
   m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
   m_list->setColumnWidth(COL_IDPRESUPUESTO,75);
   m_list->setColumnWidth(COL_NUMPRESUPUESTO,300);
   m_list->setColumnWidth(COL_FPRESUPUESTO,300);
   m_list->setColumnWidth(COL_CONTACTPRESUPUESTO,75);
   m_list->setColumnWidth(COL_TELPRESUPUESTO,75);
   m_list->setColumnWidth(COL_COMENTPRESUPUESTO,100);
   m_list->setColumnWidth(COL_IDUSUARI,1000);
   m_list->setColumnWidth(COL_IDCLIENTE,300);
         
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");   
    m_list->setReadOnly(TRUE);        
    companyact->begin();
    cursor2 * cur= companyact->cargacursor("SELECT * FROM presupuesto","querypresupuesto");
    companyact->commit();
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
         m_list->setText(i,COL_IDPRESUPUESTO,cur->valor("idpresupuesto"));
         m_list->setText(i,COL_NUMPRESUPUESTO,cur->valor("numpresupuesto"));
         m_list->setText(i,COL_FPRESUPUESTO,cur->valor("fpresupuesto"));
         m_list->setText(i,COL_CONTACTPRESUPUESTO,cur->valor("contactpresupuesto"));
         m_list->setText(i,COL_TELPRESUPUESTO,cur->valor("telpresupuesto"));
         m_list->setText(i,COL_COMENTPRESUPUESTO,cur->valor("comentpresupuesto"));
         m_list->setText(i,COL_IDUSUARI,cur->valor("idusuari"));
         m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
         i++;
         cur->siguienteregistro();
    }// end while
      delete cur;
}// end inicializa



void BudgetsList::doubleclicked(int a, int , int , const QPoint &) {
   m_idpresupuesto = m_list->text(a,COL_IDPRESUPUESTO);
   if (m_modo ==0 && m_idpresupuesto != "") {
      Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
      bud->chargeBudget(m_idpresupuesto);
      bud->show();
   } else {
      close();
   }// end if
}




void BudgetsList::contextMenu(int , int , const QPoint &) {
/*
   QString idprov = m_list->text(a, COL_IDPROVEEDOR);
   provedit *prov = new provedit(companyact,0,theApp->translate("Edicion de Proveedores", "company"));
   prov->chargeprovider(idprov);
   prov->exec();
   delete prov;
   inicializa();
*/
}// end contextMenuRequested


void BudgetsList::newBudget() {
   fprintf(stderr,"Iniciamos el boton_crear\n");
   Budget *bud = new Budget(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Presupuestos", "company"));
   bud->show();
}// end boton_crear

