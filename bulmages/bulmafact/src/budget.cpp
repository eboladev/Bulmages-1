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

/*
-- Linea de presupuesto
-- Numero
-- Descripcio: Descripció de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en línia.
-- Linia de pressupost a clients.
CREATE TABLE lpresupuesto (
   idlpresupuesto integer PRIMARY KEY,
   desclpresupuesto character varying(150),
   cantlpresupuesto float,
   pvplpresupuesto float,
   descuentolpresupuesto float,
   
   idpresupuesto integer NOT NULL REFERENCES presupuesto(idpresupuesto),
   idarituclo integer REFERENCES articulo(idarticulo)
);
*/

#include "budget.h"
#include "company.h"
#include "division.h"
#include "clientslist.h"
#include "articleslist.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>

#include "funcaux.h"

#define COL_IDLPRESUPUESTO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPRESUPUESTO 4
#define COL_CANTLPRESUPUESTO 5
#define COL_PVPLPRESUPUESTO 6
#define COL_DESCUENTOLPRESUPUESTO 7
#define COL_IDPRESUPUESTO 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10

#define COL_DESCUENTO_IDDPRESUPUESTO 0
#define COL_DESCUENTO_CONCEPTDPRESUPUESTO 1
#define COL_DESCUENTO_PROPORCIONDPRESUPUESTO 2
#define COL_DESCUENTO_REMOVE 3

Budget::Budget(company *comp, QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   m_idpresupuesto = "";
   m_idclient = "";
}// end Budget

Budget::~Budget() {
}// end ~Budget


void Budget::inicialize() {

	m_modified = false;
	installEventFilter(this);
	
	installEventFilters("QTable");
	installEventFilters("QLineEdit");
	installEventFilters("QTextEdit");
	
// Inicializamos la tabla de lineas de presupuestoil
	m_list->setNumRows( 0 );
	m_list->setNumCols( 0 );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setSorting( TRUE );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setColumnMovingEnabled( TRUE );
	m_list->setNumCols(11);
	m_list->horizontalHeader()->setLabel( COL_IDLPRESUPUESTO, tr( "Nº Línea" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCLPRESUPUESTO, tr( "Descripción" ) );
	m_list->horizontalHeader()->setLabel( COL_CANTLPRESUPUESTO, tr( "Cantidad" ) );
	m_list->horizontalHeader()->setLabel( COL_PVPLPRESUPUESTO, tr( "Precio" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCUENTOLPRESUPUESTO, tr( "Descuento" ) );
	m_list->horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "Nº Pedido" ) );
	m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
   
	m_list->setColumnWidth(COL_IDLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_DESCLPRESUPUESTO,300);
	m_list->setColumnWidth(COL_CANTLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_PVPLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_DESCUENTOLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_IDPRESUPUESTO,100);
	m_list->setColumnWidth(COL_IDARTICULO,100);
	m_list->setColumnWidth(COL_CODARTICULO,100);
	m_list->setColumnWidth(COL_NOMARTICULO,300);
	m_list->setColumnWidth(COL_TASATIPO_IVA,50);

	
	m_list->hideColumn(COL_IDLPRESUPUESTO);
	m_list->hideColumn(COL_IDPRESUPUESTO);
	m_list->hideColumn(COL_IDARTICULO);
	m_list->hideColumn(COL_REMOVE);
	m_list->hideColumn(COL_TASATIPO_IVA);
	
	m_list->setNumRows(10);
	
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	m_list->setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	m_list->setPaletteBackgroundColor("#AFFAFA");   
	m_list->setReadOnly(FALSE);
	m_list->installEventFilter( this );
	
	
// Inicializamos la tabla de descuentos del presupuesto
	m_listDiscounts->setNumRows( 0 );
	m_listDiscounts->setNumCols( 0 );
	m_listDiscounts->setSelectionMode( QTable::SingleRow );
	m_listDiscounts->setSorting( TRUE );
	m_listDiscounts->setSelectionMode( QTable::SingleRow );
	m_listDiscounts->setColumnMovingEnabled( TRUE );
	m_listDiscounts->setNumCols(4);
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_IDDPRESUPUESTO, tr( "id" ) );
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_CONCEPTDPRESUPUESTO, tr( "Concepto" ) );
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_PROPORCIONDPRESUPUESTO, tr( "Proporción" ) );
   
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_IDDPRESUPUESTO,100);
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_CONCEPTDPRESUPUESTO,500);
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_PROPORCIONDPRESUPUESTO,100);
	m_listDiscounts->hideColumn(COL_DESCUENTO_IDDPRESUPUESTO);
	m_listDiscounts->hideColumn(COL_DESCUENTO_REMOVE);
	
	m_listDiscounts->setNumRows(10);

//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	m_listDiscounts->setPaletteBackgroundColor("#AFFAFA");   
	m_listDiscounts->setReadOnly(FALSE);    
	m_listDiscounts->installEventFilter( this );
	
	m_totalBases->setReadOnly(TRUE);
	m_totalBases->setAlignment(Qt::AlignRight);
	m_totalTaxes->setReadOnly(TRUE);
	m_totalTaxes->setAlignment(Qt::AlignRight);
	m_totalDiscounts->setReadOnly(TRUE);
	m_totalDiscounts->setAlignment(Qt::AlignRight);
	m_totalBudget->setReadOnly(TRUE);
	m_totalBudget->setAlignment(Qt::AlignRight);
}// end inicialize


// Esta función carga un presupuesto.
void Budget::chargeBudget(QString idbudget) {
	inicialize();
	
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
   
		chargeBudgetLines(idbudget);
		chargeBudgetDiscounts(idbudget);
		calculateImports();
    }// end if
     delete cur;   
}// end chargeBudget


// Carga líneas de presupuesto
void Budget::chargeBudgetLines(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo, tipo_iva WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo and articulo.idtipo_iva = tipo_iva.idtipo_iva","unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		m_list->setText(i,COL_IDLPRESUPUESTO,cur->valor("idlpresupuesto"));
		m_list->setText(i,COL_DESCLPRESUPUESTO,cur->valor("desclpresupuesto"));
		m_list->setText(i,COL_CANTLPRESUPUESTO,cur->valor("cantlpresupuesto"));
		m_list->setText(i,COL_PVPLPRESUPUESTO,cur->valor("pvplpresupuesto"));
		m_list->setText(i,COL_DESCUENTOLPRESUPUESTO,cur->valor("descuentolpresupuesto"));
		m_list->setText(i,COL_IDPRESUPUESTO,cur->valor("idpresupuesto"));
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		m_list->setText(i,COL_TASATIPO_IVA,cur->valor("tasatipo_iva"));
		i++;
		cur->siguienteregistro();
	}// end while
	if (i>0) m_list->setNumRows(i);
	
	delete cur;
}// end chargeBudgetLines


// Carga líneas descuentos presupuesto
void Budget::chargeBudgetDiscounts(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM dpresupuesto WHERE idpresupuesto="+idbudget,"unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		m_listDiscounts->setText(i,COL_DESCUENTO_IDDPRESUPUESTO,cur->valor("iddpresupuesto"));
		m_listDiscounts->setText(i,COL_DESCUENTO_CONCEPTDPRESUPUESTO,cur->valor("conceptdpresupuesto"));
		m_listDiscounts->setText(i,COL_DESCUENTO_PROPORCIONDPRESUPUESTO,cur->valor("proporciondpresupuesto"));
		i++;
		cur->siguienteregistro();
	}// end while
	if (i>0) m_listDiscounts->setNumRows(i);
	
	delete cur;
}// end chargeBudgetDiscounts


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


void Budget::budgetDateLostFocus() {
	m_fpresupuesto->setText(normalizafecha(m_fpresupuesto->text()).toString("dd/MM/yyyy"));
}


void Budget::budgetExpiryLostFocus() {
	m_vencpresupuesto->setText(normalizafecha(m_vencpresupuesto->text()).toString("dd/MM/yyyy"));
}


void Budget::newBudgetLine() {
	m_list->setNumRows( m_list->numRows()+1 );
	m_list->editCell(m_list->numRows()-1, COL_CODARTICULO);
	
}


void Budget::removeBudgetLine() {
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
}


void Budget::valueBudgetLineChanged(int row, int col) {
	/*if (m_list->text(row, col)=="*") {
		duplicateCell(m_list);
	}*/
	
	switch (col) {
		case COL_DESCUENTOLPRESUPUESTO: {
			m_list->setText(row, COL_DESCUENTOLPRESUPUESTO, m_list->text(row, COL_DESCUENTOLPRESUPUESTO).replace(",","."));
		}
		case COL_CODARTICULO: {
			manageArticle(row);
			calculateImports();
		}
		case COL_CANTLPRESUPUESTO: {
			m_list->setText(row, COL_CANTLPRESUPUESTO, m_list->text(row, COL_CANTLPRESUPUESTO).replace(",","."));
			calculateImports();
		}
		case COL_PVPLPRESUPUESTO: {
			m_list->setText(row, COL_PVPLPRESUPUESTO, m_list->text(row, COL_PVPLPRESUPUESTO).replace(",","."));
			calculateImports();
		}
	}
} //end valueBudgetLineChanged


void Budget::newBudgetDiscountLine() {
	m_listDiscounts->setNumRows( m_listDiscounts->numRows()+1 );
	m_listDiscounts->editCell(m_listDiscounts->numRows()-1, COL_DESCUENTO_CONCEPTDPRESUPUESTO);
	
}


void Budget::removeBudgetDiscountLine() {
	if (m_listDiscounts->currentRow() >= 0) {
		int row = m_listDiscounts->currentRow();
		m_listDiscounts->setText(row, COL_DESCUENTO_REMOVE, "S");
		m_listDiscounts->hideRow(row);
	}
}


void Budget::valueBudgetDiscountLineChanged(int row, int col) {
	/*	switch (col) {
		case COL_DESCUENTOLPRESUPUESTO: {
			m_list->setText(row, COL_DESCUENTOLPRESUPUESTO, m_list->text(row, COL_DESCUENTOLPRESUPUESTO).replace(",","."));
		}
		case COL_CODARTICULO: {
			manageArticle(row);
			calculateImports();
		}
		case COL_CANTLPRESUPUESTO: {
			m_list->setText(row, COL_CANTLPRESUPUESTO, m_list->text(row, COL_CANTLPRESUPUESTO).replace(",","."));
			calculateImports();
		}
		case COL_PVPLPRESUPUESTO: {
			m_list->setText(row, COL_PVPLPRESUPUESTO, m_list->text(row, COL_PVPLPRESUPUESTO).replace(",","."));
			calculateImports();
		}
	} */
} //end valueBudgetDiscountLineChanged

void Budget::manageArticle(int row) {
	QString articleCode = m_list->text(row, COL_CODARTICULO);
	if (articleCode == "+") {
		QString idArticle = "";
		idArticle = searchArticle();
		m_list->setText(row, COL_CODARTICULO, idArticle);
		articleCode = idArticle;
	}
	
	bool ok;
	if (articleCode.toInt(&ok, 10)>0) {
		m_list->setText(row, COL_NOMARTICULO, "");
		m_list->setText(row, COL_IDARTICULO, "");
		m_list->setText(row, COL_TASATIPO_IVA, "");
		
		companyact->begin();
		cursor2 * cur2= companyact->cargacursor("SELECT * FROM articulo WHERE codarticulo="+m_list->text(row, COL_CODARTICULO),"unquery");
		companyact->commit();
		if (!cur2->eof()) {
			m_list->setText(row, COL_NOMARTICULO, cur2->valor("nomarticulo"));
			m_list->setText(row, COL_IDARTICULO, cur2->valor("idarticulo"));
			QString taxType = cur2->valor("idtipo_iva");
			companyact->begin();
			cursor2 * cur3= companyact->cargacursor("SELECT * FROM tipo_iva WHERE idtipo_iva="+taxType,"unquery");
			companyact->commit();
			if (!cur3->eof()) {
				m_list->setText(row, COL_TASATIPO_IVA, cur3->valor("tasatipo_iva"));
			}
		}
	}
} //end manageArticle


QString Budget::searchArticle() {
   fprintf(stderr,"Busqueda de un artículo\n");
   articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Artículo","company"));
   
// , WType_Dialog| WShowModal   
   artlist->modoseleccion();
   
   // Esto es convertir un QWidget en un sistema modal de dialogo.
   this->setEnabled(false);
   artlist->show();
   while(!artlist->isHidden()) theApp->processEvents();
   this->setEnabled(true);
   
   QString idArticle = artlist->idArticle();
	
   delete artlist;
	
	return idArticle;
}// end searchArticle


void Budget::accept() {
	fprintf(stderr,"accept button activated\n");
	
	companyact->begin();
	if (saveBudget()==0 && saveBudgetLines()==0 && saveBudgetDiscountLines()==0) {
		companyact->commit();
		m_modified = false;
		close();
	} else {
		companyact->rollback();
	}	
} //end accept


int Budget::saveBudget() {
	QString SQLQuery;
	
	if (m_idpresupuesto != "0") {
		SQLQuery = "UPDATE presupuesto  SET numpresupuesto="+m_numpresupuesto->text();
      SQLQuery += " , fpresupuesto='"+ m_fpresupuesto->text()+"'";
      SQLQuery += " , contactpresupuesto='"+m_contactpresupuesto->text()+"'";
      SQLQuery += " , telpresupuesto='"+m_telpresupuesto->text()+"'";
      SQLQuery += " , vencpresupuesto='"+m_vencpresupuesto->text()+"'";
      SQLQuery += " , comentpresupuesto='"+m_comentpresupuesto->text()+"'";
		SQLQuery += " , idcliente="+m_idclient;
      SQLQuery += " WHERE idpresupuesto ="+m_idpresupuesto;
	} else {
		SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, contactpresupuesto, vencpresupuesto, comentpresupuesto, idcliente)";
		SQLQuery += " VALUES (";
		SQLQuery += m_numpresupuesto->text();
		SQLQuery += " , '"+m_fpresupuesto->text()+"'";
      SQLQuery += " , '"+m_contactpresupuesto->text()+"'";
      SQLQuery += " , '"+m_vencpresupuesto->text()+"'";
      SQLQuery += " , '"+m_comentpresupuesto->text()+"'";
		SQLQuery += " , "+m_idclient;
      SQLQuery += " ) ";
	}
	return companyact->ejecuta(SQLQuery);
} //end saveBudget


int Budget::saveBudgetLines() {
	int i = 0;
	int error = 0;
	while (i < m_list->numRows() && error==0) {
		if (m_list->text(i,COL_REMOVE)=="S") {
			if (m_list->text(i,COL_IDLPRESUPUESTO)!="") {
				error = deleteBudgetLine(i);
			}
		} else {
			if (m_list->text(i,COL_IDARTICULO)!="" || m_list->text(i,COL_NOMARTICULO)!="") {
				if (m_list->text(i,COL_IDLPRESUPUESTO)!="") {
					error = updateBudgetLine(i);
				} else {
					error = insertBudgetLine(i);
				}
			}
		}
		i ++;
   }
	return error;
} // end saveBudgetLines


int Budget::saveBudgetDiscountLines() {
	int i = 0;
	int error = 0;
	while (i < m_listDiscounts->numRows() && error==0) {
		if (m_listDiscounts->text(i,COL_DESCUENTO_REMOVE)=="S") {
			if (m_listDiscounts->text(i,COL_DESCUENTO_IDDPRESUPUESTO)!="") {
				error = deleteBudgetDiscountLine(i);
			}
		} else {
			if (m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDPRESUPUESTO)!="" || m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDPRESUPUESTO)!="") {
				if (m_listDiscounts->text(i,COL_DESCUENTO_IDDPRESUPUESTO)!="") {
					error = updateBudgetDiscountLine(i);
				} else {
					error = insertBudgetDiscountLine(i);
				}
			}
		}
		i ++;
   }
	return error;
} // end saveBudgetDiscountLines


int Budget::updateBudgetLine(int i) {
	QString SQLQuery = "UPDATE lpresupuesto SET desclpresupuesto='"+m_list->text(i,COL_DESCLPRESUPUESTO)+"'";
	SQLQuery += " , cantlpresupuesto="+ m_list->text(i,COL_CANTLPRESUPUESTO);
	SQLQuery += " , pvplpresupuesto="+m_list->text(i,COL_PVPLPRESUPUESTO);
	SQLQuery += " , descuentolpresupuesto="+m_list->text(i,COL_DESCUENTOLPRESUPUESTO);
	SQLQuery += " , idarticulo="+m_list->text(i,COL_IDARTICULO);
	SQLQuery += " WHERE idpresupuesto ="+m_idpresupuesto+" AND idlpresupuesto="+m_list->text(i,COL_IDLPRESUPUESTO);
	return companyact->ejecuta(SQLQuery);
} //end updateBudgetLine


int Budget::insertBudgetLine(int i) {
	QString SQLQuery ="";
	SQLQuery = "INSERT INTO lpresupuesto (desclpresupuesto, cantlpresupuesto, pvplpresupuesto, descuentolpresupuesto, idpresupuesto, idarticulo)";
	SQLQuery += " VALUES (";
	SQLQuery += "'"+m_list->text(i,COL_DESCLPRESUPUESTO)+"'";
	SQLQuery += " , "+m_list->text(i,COL_CANTLPRESUPUESTO);
	SQLQuery += " , "+m_list->text(i,COL_PVPLPRESUPUESTO);
	SQLQuery += " , "+m_list->text(i,COL_DESCUENTOLPRESUPUESTO);
	SQLQuery += " , "+m_idpresupuesto;
	SQLQuery += " , "+m_list->text(i,COL_IDARTICULO);
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertBudgetLine


int Budget::deleteBudgetLine(int line) {
	QString SQLQuery = "DELETE FROM lpresupuesto WHERE idlpresupuesto ="+m_list->text(line,COL_IDLPRESUPUESTO);
	return companyact->ejecuta(SQLQuery);
} //end deleteBudgetLine


int Budget::updateBudgetDiscountLine(int i) {
	QString SQLQuery = "UPDATE dpresupuesto SET conceptdpresupuesto='"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDPRESUPUESTO)+"'";
	SQLQuery += " , proporciondpresupuesto="+ m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDPRESUPUESTO);
	SQLQuery += " WHERE idpresupuesto ="+m_idpresupuesto+" AND iddpresupuesto="+m_listDiscounts->text(i,COL_DESCUENTO_IDDPRESUPUESTO);
	return companyact->ejecuta(SQLQuery);
} //end updateBudgetDiscountLine


int Budget::insertBudgetDiscountLine(int i) {
	QString SQLQuery ="";
	SQLQuery = "INSERT INTO dpresupuesto (conceptdpresupuesto, proporciondpresupuesto, idpresupuesto)";
	SQLQuery += " VALUES (";
	SQLQuery += "'"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDPRESUPUESTO)+"'";
	SQLQuery += " , "+m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDPRESUPUESTO);
	SQLQuery += " , "+m_idpresupuesto;
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertBudgetDiscountLine


int Budget::deleteBudgetDiscountLine(int line) {
	QString SQLQuery = "DELETE FROM dpresupuesto WHERE iddpresupuesto ="+m_listDiscounts->text(line,COL_DESCUENTO_IDDPRESUPUESTO);
	return companyact->ejecuta(SQLQuery);
} //end deleteBudgetDiscountLine

void Budget::cancel() {
	close();
}//end cancel


void Budget::calculateImports() {
	int i = 0;
	float netImport = 0;
	float taxImport = 0;
	float discountImport = 0;
	while (i < m_list->numRows()) {
		if (m_list->text(i,COL_PVPLPRESUPUESTO)!="" and m_list->text(i,COL_CANTLPRESUPUESTO)!="") {
			netImport += m_list->text(i,COL_PVPLPRESUPUESTO).toFloat() * m_list->text(i,COL_CANTLPRESUPUESTO).toFloat();
			taxImport += (m_list->text(i,COL_PVPLPRESUPUESTO).toFloat() * m_list->text(i,COL_CANTLPRESUPUESTO).toFloat() * m_list->text(i,COL_TASATIPO_IVA).toFloat())/100;
			discountImport += (m_list->text(i,COL_PVPLPRESUPUESTO).toFloat() * m_list->text(i,COL_CANTLPRESUPUESTO).toFloat() * m_list->text(i,COL_DESCUENTOLPRESUPUESTO).toFloat())/100;
		}
		i ++;
   }
	
	m_totalBases->setText(QString().sprintf("%0.2f", netImport));
	m_totalTaxes->setText(QString().sprintf("%0.2f", taxImport));
	m_totalDiscounts->setText(QString().sprintf("%0.2f", discountImport));
	m_totalBudget->setText(QString().sprintf("%0.2f", netImport+taxImport));
} // end calculateImports


bool Budget::eventFilter( QObject *obj, QEvent *ev ) {
	if ( ev->type() == QEvent::KeyPress ) {
		m_modified = true;
	}
	
	if ( obj->isA("QTable")) {
		QTable *t = (QTable *)obj;
		if ( ev->type() == QEvent::KeyRelease ) {
			QKeyEvent *k = (QKeyEvent *)ev;
			qDebug( "Widget = %s",obj->name());
			switch (k->key()) {
				case Qt::Key_Enter:  {
					if (QString(obj->name()).stripWhiteSpace() == "m_list") {
						valueBudgetLineChanged(t->currentRow(), t->currentColumn());
					}
					nextCell(obj);
					return TRUE;
				}
				case Qt::Key_Return: {
					if (QString(obj->name()).stripWhiteSpace() == "m_list") {
						valueBudgetLineChanged(t->currentRow(), t->currentColumn());
					}
					nextCell(obj);
					return TRUE;
				}
				case Qt::Key_Asterisk: {
					duplicateCell(obj);
					if (QString(obj->name()).stripWhiteSpace() == "m_list") {
						valueBudgetLineChanged(t->currentRow(), t->currentColumn());
					}
					return TRUE;
				}
			} 
		}
	} 
	
	if ( ev->type() == QEvent::Close ) {
		if ( m_modified ) {
			if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Se perderán los cambios que haya realizado", "Aceptar", "Cancelar") != 0) {
				return TRUE;
			}
		}
	}
	return FALSE;
} //end eventFilter


void Budget::nextCell(QObject *obj) {
	QTable *t = (QTable *)obj;
	int row = t->currentRow();
	int col = t->currentColumn();
	int lastCol = t->numCols()-1;
	if (t->isReadOnly()==FALSE) {
		//qDebug( "Fila, Columna = %d, %d", row, col);
		col++;
		while (true) {
			qDebug( "Fila, Columna = %d, %d", row, col);
			if (col > lastCol) {
				col = 0;
				row++;
				if (row == (t->numRows())) {
					t->setNumRows(row+1);
				}
			} else {
				if (t->isColumnHidden(col) || t->isColumnReadOnly(col) || t->isRowHidden(row) || t->isRowReadOnly(row)) {
					col++;
				} else {
					t->setCurrentCell(row, col);
					//t->editCell(row, col);
					break;
				}
			}
		}
	}
}


void Budget::duplicateCell(QObject *obj) {
	QTable *t = (QTable *)obj;
	int row = t->currentRow();
	int col = t->currentColumn();
	if (t->text(row, col) == "" && row>0) {
		t->setText(row, col, t->text(row-1, col));
	}
}


void Budget::installEventFilters(QString qsWidget) {
	QObjectList *l = queryList( qsWidget );
	QObjectListIt it( *l ); // iterate over the buttons
	QObject *obj;

    while ( (obj = it.current()) != 0 ) {
		// for each found object...
		++it;
		qDebug("Widget = %s", ((QWidget*)obj)->name());
		((QWidget*)obj)->installEventFilter(this);
    }
    delete l; // delete the list, not the objects
}

