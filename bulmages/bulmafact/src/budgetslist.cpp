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
#include <qmessagebox.h>

#define COL_IDPRESUPUESTO 0
#define COL_NUMPRESUPUESTO 1
#define COL_NOMCLIENTE 2
#define COL_FPRESUPUESTO 3
#define COL_CONTACTPRESUPUESTO 4
#define COL_TELPRESUPUESTO 5
#define COL_COMENTPRESUPUESTO 6
#define COL_IDUSUARI 7
#define COL_IDCLIENTE 8


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
   m_list->setNumCols(9);
   m_list->horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "COL_IDPRESUPUESTO" ) );
	m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
   m_list->horizontalHeader()->setLabel( COL_NUMPRESUPUESTO, tr( "Nº Presupuesto" ) );
   m_list->horizontalHeader()->setLabel( COL_FPRESUPUESTO, tr( "Fecha" ) );
   m_list->horizontalHeader()->setLabel( COL_CONTACTPRESUPUESTO, tr( "Persona Contacto" ) );
   m_list->horizontalHeader()->setLabel( COL_TELPRESUPUESTO, tr( "Teléfono" ) );
   m_list->horizontalHeader()->setLabel( COL_COMENTPRESUPUESTO, tr( "Comentarios" ) );
   m_list->horizontalHeader()->setLabel( COL_IDUSUARI, tr("COL_IDUSUARI") );
   m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
   m_list->setColumnWidth(COL_IDPRESUPUESTO,75);
   m_list->setColumnWidth(COL_NUMPRESUPUESTO,75);
   m_list->setColumnWidth(COL_FPRESUPUESTO,100);
   m_list->setColumnWidth(COL_CONTACTPRESUPUESTO,200);
   m_list->setColumnWidth(COL_TELPRESUPUESTO,150);
   m_list->setColumnWidth(COL_COMENTPRESUPUESTO,300);
   m_list->setColumnWidth(COL_IDUSUARI,75);
   m_list->setColumnWidth(COL_IDCLIENTE,75);
	m_list->setColumnWidth(COL_NOMCLIENTE,200);
	m_list->hideColumn(COL_IDPRESUPUESTO);
	m_list->hideColumn(COL_IDCLIENTE);
	//m_list->hideColumn(COL_IDUSUARI);
         
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");   
    m_list->setReadOnly(TRUE);        
    companyact->begin();
    cursor2 * cur= companyact->cargacursor("SELECT * FROM presupuesto, cliente where presupuesto.idcliente=cliente.idcliente","querypresupuesto");
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
			m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
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


void BudgetsList::s_removeBudget() {
	fprintf(stderr,"Iniciamos el boton_borrar\n");
	if (m_list->currentRow() >= 0) {
		if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar el presupuesto seleccionado", "Sí", "No") == 0) {
			companyact->begin();
			QString SQLQuery = "DELETE FROM lpresupuesto WHERE idpresupuesto ="+m_list->text(m_list->currentRow(),COL_IDPRESUPUESTO);
			if (companyact->ejecuta(SQLQuery)==0){
				QString SQLQuery = "DELETE FROM dpresupuesto WHERE idpresupuesto ="+m_list->text(m_list->currentRow(),COL_IDPRESUPUESTO);
					if (companyact->ejecuta(SQLQuery)==0){
					QString SQLQuery = "DELETE FROM presupuesto WHERE idpresupuesto ="+m_list->text(m_list->currentRow(),COL_IDPRESUPUESTO);
					if (companyact->ejecuta(SQLQuery)==0){
						companyact->commit();
					} else {
						companyact->rollback();
					}
				} else {
					companyact->rollback();
				}
			} else {
				companyact->rollback();
			}
		}	
	}
	inicializa();
}// end boton_borrar
