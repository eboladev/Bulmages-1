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
#include "configuracion.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qmessagebox.h>
#include <qlabel.h>
#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>

#include "funcaux.h"
//#include "postgresiface2.h"

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
#define COL_TIPO_IVA 11

#define COL_DESCUENTO_IDDPRESUPUESTO 0
#define COL_DESCUENTO_CONCEPTDPRESUPUESTO 1
#define COL_DESCUENTO_PROPORCIONDPRESUPUESTO 2
#define COL_DESCUENTO_REMOVE 3

Budget::Budget(company *comp, QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   m_idpresupuesto = "0";
   m_idclient = "";
   inicialize();
   companyact->meteWindow(caption(),this);
}// end Budget

Budget::~Budget() {
	companyact->refreshBudgets();
	companyact->sacaWindow(this);
}// end ~Budget


void Budget::inicialize() {
	installEventFilter(this);
	m_list->installEventFilter( this );
	m_listDiscounts->installEventFilter( this );
	m_nomalmacen->setText("");
	m_comboformapago->clear();
	QString query = "SELECT * FROM prfp WHERE idpresupuesto="+m_idpresupuesto;
	companyact->begin();
	cursor2 * cur1= companyact->cargacursor(query, "querypresupuesto");
	companyact->commit();
	if (!cur1->eof()) {
		cargarcomboformapago(cur1->valor("idforma_pago"));
	} else {
		cargarcomboformapago("0");	
	}// end if
	delete cur1;

	
	
// Inicializamos la tabla de lineas de presupuesto
	m_list->setNumRows( 0 );
	m_list->setNumCols( 0 );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setSorting( TRUE );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setColumnMovingEnabled( TRUE );
	m_list->setNumCols(12);
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
	m_list->horizontalHeader()->setLabel( COL_TIPO_IVA, tr( "Tipo IVA" ) );
   
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
	m_list->setColumnWidth(COL_TIPO_IVA,50);

	
	m_list->hideColumn(COL_IDLPRESUPUESTO);
	m_list->hideColumn(COL_IDPRESUPUESTO);
	m_list->hideColumn(COL_IDARTICULO);
	m_list->hideColumn(COL_REMOVE);
	m_list->hideColumn(COL_TASATIPO_IVA);
	m_list->hideColumn(COL_TIPO_IVA);
	
	m_list->setNumRows(10);
	
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	m_list->setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	m_list->setPaletteBackgroundColor("#AFFAFA");   
	m_list->setReadOnly(FALSE);
	
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
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_CONCEPTDPRESUPUESTO,400);
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
	
	if (m_idpresupuesto=="0") {	 
		companyact->begin();
		cursor2 * cur0= companyact->cargacursor("SELECT * FROM configuracion where nombre='AlmacenDefecto'","queryconfig");
		companyact->commit(); 
		if (!cur0->eof()) {
			if (cur0->valor("valor")!="") {
				m_codigoalmacen->setText(cur0->valor("valor"));
			}
		}
		delete cur0;
	}
	
	if (confpr->valor(CONF_MOSTRAR_ALMACEN)!="YES") {
		m_codigoalmacen->setDisabled(true);
		m_buscaralmacen->setDisabled(true);
		m_lblalmacen->setDisabled(true);
		m_nomalmacen->setDisabled(true);
	}
	
	if (confpr->valor(CONF_NUM_AUT_PRESUP)=="YES") {
		m_numpresupuesto->setReadOnly(true);
		if (m_idpresupuesto!="0") {
		  	m_codigoalmacen->setReadOnly(true);
		}
	}
	
	buscarAlmacen();
}// end inicialize


// Esta función carga un presupuesto.
void Budget::chargeBudget(QString idbudget) {
	m_idpresupuesto = idbudget;
	inicialize();

	QString query = "SELECT * FROM presupuesto LEFT JOIN cliente ON cliente.idcliente = presupuesto.idcliente LEFT JOIN almacen ON  presupuesto.idalmacen = almacen.idalmacen WHERE idpresupuesto="+idbudget;
	companyact->begin();
	cursor2 * cur= companyact->cargacursor(query, "querypresupuesto");
	companyact->commit();
	if (!cur->eof()) {
		m_idclient = cur->valor("idcliente");	
		m_idalmacen = cur->valor("idalmacen");
		m_numpresupuesto->setText(cur->valor("numpresupuesto"));
		m_fpresupuesto->setText(cur->valor("fpresupuesto"));
		m_vencpresupuesto->setText(cur->valor("vencpresupuesto"));
		m_contactpresupuesto->setText(cur->valor("contactpresupuesto"));
		m_telpresupuesto->setText(cur->valor("telpresupuesto"));
		m_comentpresupuesto->setText(cur->valor("comentpresupuesto"));
		m_nomclient->setText(cur->valor("nomcliente"));
		m_cifclient->setText(cur->valor("cifcliente"));
		m_codigoalmacen-> setText(cur->valor("codigoalmacen"));
		m_nomalmacen-> setText(cur->valor("nomalmacen"));
   
		chargeBudgetLines(idbudget);
		chargeBudgetDiscounts(idbudget);
		calculateImports();
    }// end if
     delete cur;   
	  
	m_initialValues = calculateValues();
}// end chargeBudget


void Budget::cargarcomboformapago(QString idformapago) {
	m_cursorcombo = NULL;
	companyact->begin();
  	if (m_cursorcombo != NULL) delete m_cursorcombo;
   	m_cursorcombo = companyact->cargacursor("SELECT * FROM forma_pago","unquery");
   	companyact->commit();
   	int i = 0;
   	int i1 = 0;
   	while (!m_cursorcombo->eof()) {
   		i ++;
			if (m_cursorcombo->valor("idforma_pago") == idformapago) {
			   i1 = i;
			}
   		m_comboformapago->insertItem(m_cursorcombo->valor("descforma_pago"));
			m_cursorcombo->siguienteregistro();
   	}
	if (i1 != 0 ) {
   		m_comboformapago->setCurrentItem(i1-1);
 	} 
} //end cargarcombodformapago

// Carga líneas de presupuesto
void Budget::chargeBudgetLines(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo, tipo_iva WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo and articulo.idtipo_iva = tipo_iva.idtipo_iva","unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		m_list->setText(i,COL_IDLPRESUPUESTO,cur->valor("idlpresupuesto"));
		m_list->setText(i,COL_DESCLPRESUPUESTO,cur->valor("desclpresupuesto"));
		m_list->setText(i,COL_CANTLPRESUPUESTO,QString().sprintf("%0.3f",cur->valor("cantlpresupuesto").toFloat()));
		m_list->setText(i,COL_PVPLPRESUPUESTO,QString().sprintf("%0.2f",cur->valor("pvplpresupuesto").toFloat()));
		m_list->setText(i,COL_DESCUENTOLPRESUPUESTO,QString().sprintf("%0.2f",cur->valor("descuentolpresupuesto").toFloat()));
		m_list->setText(i,COL_IDPRESUPUESTO,cur->valor("idpresupuesto"));
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		m_list->setText(i,COL_TASATIPO_IVA,cur->valor("tasatipo_iva"));
		m_list->setText(i,COL_TIPO_IVA,cur->valor("idtipo_iva"));
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
		m_listDiscounts->setText(i,COL_DESCUENTO_PROPORCIONDPRESUPUESTO,QString().sprintf("%0.2f",cur->valor("proporciondpresupuesto").toFloat()));
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
   
	if (clients->cifclient() !="" && clients->cifclient() !=NULL) {
   	m_idclient = clients->idclient();
   	m_cifclient->setText(clients->cifclient());
   	m_nomclient->setText(clients->nomclient());
	}
   delete clients;
}// end searchClient


void Budget::budgetDateLostFocus() {
	m_fpresupuesto->setText(normalizafecha(m_fpresupuesto->text()).toString("dd/MM/yyyy"));
}


void Budget::budgetExpiryLostFocus() {
	m_vencpresupuesto->setText(normalizafecha(m_vencpresupuesto->text()).toString("dd/MM/yyyy"));
}


void Budget::s_almacenLostFocus() {
	buscarAlmacen();
}//end s_almacenLostFocus


void Budget::s_printBudget() {
	presentaOpenReports();
}//end s_printBudget


void Budget::buscarAlmacen() {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM almacen where codigoalmacen ="+ m_codigoalmacen->text(),"unquery");
	companyact->commit();
	if (!cur->eof()) {
		m_idalmacen = cur->valor("idalmacen");
		m_nomalmacen->setText(cur->valor("nomalmacen"));
	} else {
		m_nomalmacen->setText(""); 
		m_idalmacen = "";
	}
	delete cur;
} // end buscarAlmacen


void Budget::newBudgetLine() {
	m_list->setNumRows( m_list->numRows()+1 );
	m_list->setCurrentCell(m_list->numRows()-1, COL_CODARTICULO);	
	m_list->editCell(m_list->numRows()-1, COL_CODARTICULO);
}


void Budget::s_removeBudget() {
	fprintf(stderr,"Iniciamos el boton_borrar\n");
	if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este presupuesto", "Sí", "No") == 0) {
		companyact->begin();
		QString SQLQuery = "DELETE FROM lpresupuesto WHERE idpresupuesto ="+m_idpresupuesto;
		if (companyact->ejecuta(SQLQuery)==0){
			QString SQLQuery = "DELETE FROM dpresupuesto WHERE idpresupuesto ="+m_idpresupuesto;
				if (companyact->ejecuta(SQLQuery)==0){
					QString SQLQuery = "DELETE FROM prfp WHERE idpresupuesto ="+m_idpresupuesto;
					if (companyact->ejecuta(SQLQuery)==0){
						QString SQLQuery = "DELETE FROM presupuesto WHERE idpresupuesto ="+m_idpresupuesto;
						if (companyact->ejecuta(SQLQuery)==0){
							companyact->commit();
							close();
						} else {
							companyact->rollback();
						}
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
}// end boton_borrar


void Budget::removeBudgetLine() {
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
}


void Budget::valueBudgetLineChanged(int row, int col) {
	
	switch (col) {
		case COL_DESCUENTOLPRESUPUESTO: {
			m_list->setText(row, COL_DESCUENTOLPRESUPUESTO, m_list->text(row, COL_DESCUENTOLPRESUPUESTO).replace(",","."));
			float discountLine = m_list->text(row, COL_DESCUENTOLPRESUPUESTO).toFloat();
			m_list->setText(row, COL_DESCUENTOLPRESUPUESTO, QString().sprintf("%0.2f", discountLine));
			break;
		}
		case COL_CODARTICULO: {
			manageArticle(row);
			calculateImports();
			break;
		}
		case COL_CANTLPRESUPUESTO: {
			m_list->setText(row, COL_CANTLPRESUPUESTO, m_list->text(row, COL_CANTLPRESUPUESTO).replace(",","."));
			float cantLine = m_list->text(row, COL_CANTLPRESUPUESTO).toFloat();
			m_list->setText(row, COL_CANTLPRESUPUESTO, QString().sprintf("%0.3f", cantLine));
			calculateImports();
			break;
		}
		case COL_PVPLPRESUPUESTO: {
			m_list->setText(row, COL_PVPLPRESUPUESTO, m_list->text(row, COL_PVPLPRESUPUESTO).replace(",","."));
			float pvpLine = m_list->text(row, COL_PVPLPRESUPUESTO).toFloat();
			m_list->setText(row, COL_PVPLPRESUPUESTO, QString().sprintf("%0.2f", pvpLine));
			calculateImports();
			break;
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
	switch (col) {
		case COL_DESCUENTO_PROPORCIONDPRESUPUESTO: {
			m_listDiscounts->setText(row, COL_DESCUENTO_PROPORCIONDPRESUPUESTO, m_listDiscounts->text(row, COL_DESCUENTO_PROPORCIONDPRESUPUESTO).replace(",","."));
			float proporcionLine = m_listDiscounts->text(row, COL_DESCUENTO_PROPORCIONDPRESUPUESTO).toFloat();
			m_listDiscounts->setText(row, COL_DESCUENTO_PROPORCIONDPRESUPUESTO, QString().sprintf("%0.2f", proporcionLine));
			calculateImports();
			break;
		}
	}
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
		m_list->setText(row, COL_TIPO_IVA, "");
		
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
				m_list->setText(row, COL_TIPO_IVA, cur3->valor("tipo_iva"));
			}
			delete cur3;
		}
		delete cur2;
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


void Budget::s_saveBudget() {
	companyact->begin();
	if (saveBudget()==0) {
		if (m_idpresupuesto == "0") {
			cursor2 * cur4= companyact->cargacursor("SELECT max(idpresupuesto) FROM presupuesto","unquery1");
			if (!cur4->eof()) {
				m_idpresupuesto=cur4->valor(0);
				if (insertfpBudget()!=0) {
					companyact->rollback();
					return;
				}
			}
			delete cur4;
		} else {
			if (updatefpBudget()!=0) {
				companyact->rollback();
				return;
			}
		 }
		if (saveBudgetLines()==0 && saveBudgetDiscountLines()==0) {
			companyact->commit();
			m_initialValues = calculateValues();
		} else {
			companyact->rollback();
		}
	} else {
		companyact->rollback();
	}	
	chargeBudget(m_idpresupuesto);
}


void Budget::s_contextMenu(int row, int col, int button, const QPoint &poin) {
	qDebug("button = %d", button);
	if (button == 2) {
		QPopupMenu *popup;
		popup = new QPopupMenu;
		popup->insertItem(tr("Eliminar"),101);
		//popup->insertSeparator();
		int opcion = popup->exec(m_list->mapToGlobal(poin));
		switch(opcion) {
			case 101:
				removeBudgetLine();
				break;
		}// end switch
		delete popup;
	}
}// end contextmenu


void Budget::s_contextMenuDiscount(int row, int col, int button, const QPoint &poin) {
	qDebug("button = %d", button);
	if (button == 2) {
		QPopupMenu *popup;
		popup = new QPopupMenu;
		popup->insertItem(tr("Eliminar"),101);
		//popup->insertSeparator();
		int opcion = popup->exec(m_listDiscounts->mapToGlobal(poin));
		switch(opcion) {
			case 101:
				removeBudgetDiscountLine();
				break;
		}// end switch
		delete popup;
	}
}// end contextmenu


void Budget::accept() {
	fprintf(stderr,"accept button activated\n");
	companyact->begin();
	if (saveBudget()==0) {
		if (m_idpresupuesto == "0") {
			cursor2 * cur4= companyact->cargacursor("SELECT max(idpresupuesto) FROM presupuesto","unquery1");
			if (!cur4->eof()) {
				m_idpresupuesto=cur4->valor(0);
				if (insertfpBudget()!=0) {
					companyact->rollback();
					return;
				}
			}
			delete cur4;
		} else {
			if (updatefpBudget()!=0) {
				companyact->rollback();
				return;
			}
		}
		 
		if (saveBudgetLines()==0 && saveBudgetDiscountLines()==0) {
			companyact->commit();
			m_initialValues = calculateValues();
			close();
		} else {
			companyact->rollback();
		}
	} else {
		companyact->rollback();
	}	
} //end accept


int Budget::saveBudget() {
	buscarAlmacen();
	if (m_idalmacen == "")  return 1;
	
	if (m_numpresupuesto->text() == "") {
		m_numpresupuesto->setText(newBudgetNumber());
	}
	QString SQLQuery;
	
	if (m_idpresupuesto != "0") {
		SQLQuery = "UPDATE presupuesto  SET numpresupuesto="+m_numpresupuesto->text();
      SQLQuery += " , fpresupuesto='"+ m_fpresupuesto->text()+"'";
      SQLQuery += " , contactpresupuesto='"+m_contactpresupuesto->text()+"'";
      SQLQuery += " , telpresupuesto='"+m_telpresupuesto->text()+"'";
      SQLQuery += " , vencpresupuesto='"+m_vencpresupuesto->text()+"'";
      SQLQuery += " , comentpresupuesto='"+m_comentpresupuesto->text()+"'";
		SQLQuery += " , idcliente="+m_idclient;
		SQLQuery += " , idalmacen="+m_idalmacen;
      SQLQuery += " WHERE idpresupuesto ="+m_idpresupuesto;
	} else {
		SQLQuery = "INSERT INTO presupuesto (idalmacen, numpresupuesto, fpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idcliente)";
		SQLQuery += " VALUES (";
		SQLQuery += m_idalmacen;
		SQLQuery += " , "+m_numpresupuesto->text();
		SQLQuery += " , '"+m_fpresupuesto->text()+"'";
      SQLQuery += " , '"+m_contactpresupuesto->text()+"'";
		SQLQuery += " , '"+m_telpresupuesto->text()+"'";
      SQLQuery += " , '"+m_vencpresupuesto->text()+"'";
      SQLQuery += " , '"+m_comentpresupuesto->text()+"'";
		SQLQuery += " , "+m_idclient;
      SQLQuery += " ) ";
	}
	return companyact->ejecuta(SQLQuery);
} //end saveBudget


int Budget::insertfpBudget() {
	QString SQLQuery;
	SQLQuery = "INSERT INTO prfp (idpresupuesto, idforma_pago)";
	SQLQuery += " VALUES (";
	SQLQuery += m_idpresupuesto;
	SQLQuery += " , "+m_cursorcombo->valor("idforma_pago",m_comboformapago->currentItem());
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertfpBudget


int Budget::updatefpBudget() {
	QString SQLQuery;
	SQLQuery = "UPDATE prfp SET idforma_pago="+m_cursorcombo->valor("idforma_pago",m_comboformapago->currentItem());
	SQLQuery += " WHERE idpresupuesto ="+m_idpresupuesto;
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
	QString SQLQuery = "DELETE FROM lpresupuesto WHERE idlpresupuesto ="+m_list->text(line,COL_IDLPRESUPUESTO)+" AND idpresupuesto="+m_idpresupuesto;
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
	QString SQLQuery = "DELETE FROM dpresupuesto WHERE iddpresupuesto ="+m_listDiscounts->text(line,COL_DESCUENTO_IDDPRESUPUESTO)+" AND idpresupuesto="+m_idpresupuesto;
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


QString Budget::calculateValues() {
	QString values = retrieveValues("QTable");
	values += retrieveValues("QLineEdit");
	values += retrieveValues("QTextEdit");
	return values;
}


bool Budget::eventFilter( QObject *obj, QEvent *ev ) {
	if ( obj->isA("QTable")) {
		QTable *t = (QTable *)obj;
		if ( ev->type() == QEvent::KeyRelease ) {
			QKeyEvent *k = (QKeyEvent *)ev;
			switch (k->key()) {
				//esta linea ha sido modificada por Javier
				case Qt::Key_Return:
				case Qt::Key_Enter:  {
					// Esto se hace porque en la última linea del qtable tiene un comportamiento raro. Se reportará como bug a trolltech.
					if (t->currentRow()==t->numRows()-1) {
						antCell(obj);
					}
					if (QString(obj->name()).stripWhiteSpace() == "m_list") {
						valueBudgetLineChanged(t->currentRow(), t->currentColumn());
					}
					if (QString(obj->name()).stripWhiteSpace() == "m_listDiscounts") {
						valueBudgetDiscountLineChanged(t->currentRow(), t->currentColumn());
					}
					nextCell(obj);
					return TRUE;
				}
				
				case Qt::Key_Asterisk: {
					duplicateCell(obj);
					if (QString(obj->name()).stripWhiteSpace() == "m_list") {
						valueBudgetLineChanged(t->currentRow(), t->currentColumn());
					}
					if (QString(obj->name()).stripWhiteSpace() == "m_listDiscount") {
						valueBudgetDiscountLineChanged(t->currentRow(), t->currentColumn());
					}
					nextCell(obj);
					return TRUE;
				}
			} 
		}
	} 
	
	if ( ev->type() == QEvent::Close ) {
		if ( calculateValues() != m_initialValues ) {
			if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Se perderán los cambios que haya realizado", "Aceptar", "Cancelar") != 0) {
				return TRUE;
			}
		}
	}
	return QWidget::eventFilter( obj, ev );
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
					t->editCell(row, col);
					break;
				}
			}
		}
	}
}


void Budget::antCell(QObject *obj) {
	QTable *t = (QTable *)obj;
	int row = t->currentRow();
	int col = t->currentColumn();
	int lastCol = t->numCols()-1;
	if (t->isReadOnly()==FALSE) {
		//qDebug( "Fila, Columna = %d, %d", row, col);
		col--;
		while (true) {
			if (col < 0) {
				col = lastCol;
				
			} else {
				if (t->isColumnHidden(col) || t->isRowHidden(row) ) {
					col--;
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
	int antRow = row - 1;
	while (antRow>=0 && t->isRowHidden(antRow)) {
		antRow--;
	}
	if ((t->text(row, col) == "" || t->text(row, col) == "*") && antRow>=0) {
		t->setText(row, col, t->text(antRow, col).ascii());
		qDebug("Valor = %s", t->text(antRow, col).ascii());
	} else {
		t->setText(row, col, "");
	}
}


QString Budget::retrieveValues(QString qsWidget) {
	QObjectList *l = queryList( qsWidget );
	QObjectListIt it( *l );
	QObject *obj;
	QString values="";
    while ( (obj = it.current() ) != 0 ) {
		++it;
		
		if (qsWidget=="QLineEdit") {
			//qDebug("QLineEdit Name = %s",obj->name());
			values += ((QLineEdit*)obj)->text();
		}
		
		if (qsWidget=="QTextEdit") {
			//qDebug("QTextEdit Name = %s",obj->name());
			values += ((QTextEdit*)obj)->text();
		}
		
		if (qsWidget=="QTable") {
			//qDebug("QTable Name = %s",obj->name());
			for (int i=0; i < m_list->numRows(); i++) {
				for (int j=0; j < m_list->numCols(); j++) {
					values += ((QTable*)obj)->text(i, j);
				}
			}
		}
	}
	delete l; // delete the list, not the objects
	return values;
}


QString Budget::newBudgetNumber() {
	QString rtnNumber;
	companyact->begin();
	cursor2 * cur4= companyact->cargacursor("SELECT max(numpresupuesto) FROM presupuesto WHERE idalmacen="+m_idalmacen,"unquery1");
	//companyact->commit();
	if (!cur4->eof()) {
		rtnNumber = QString().sprintf("%d",cur4->valor(0).toInt()+1);
	} else {
		rtnNumber = "1";
	}
	delete cur4;
	return rtnNumber;
}


void Budget::presentaOpenReports() {
	int txt=1;
	string codigoarticulo;
	string descripcionarticulo;
	string cantidadlinea;
	string preciolinea;
	string descuentolinea;
	string importelinea;
	
	string cif;
	string fecha;
	string numero;
	string observaciones;
	
	string nombre;
	string direccion;
	string poblacion;
	string cp;
	string telefono;
	string fax;
	
	float bases [99];
	float tasas [99];
	for (int i=0;i<100;i++) {
		bases[i]=0;
		tasas[i]=0;
	}
	

	cif = m_cifclient->text().ascii();
	fecha = m_fpresupuesto->text().ascii();
	numero = m_numpresupuesto->text().ascii();
	observaciones = m_comentpresupuesto->text().ascii();
	
	ifstream filestr((confpr->valor(CONF_DIR_OPENREPORTS)+"presupuesto.rml").c_str());
	string a;
	char c [1000];
	char *argstxt[]={"presupuesto.jm.rml","presupuesto.jm.rml",NULL};      //presentació txt normal
	ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
	if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
	while (filestr.good()) {
		filestr.getline(c,1000);// >> a;
		if (txt) {
			fitxersortidatxt.setf(ios::fixed);
			fitxersortidatxt.precision(2);
			fitxersortidatxt << c ;
			fitxersortidatxt << "\n" ;
		}
	}// end while
	fitxersortidatxt << "<story>\n" ;
	
	// dades del client
	QString query = "SELECT * FROM cliente WHERE idcliente="+m_idclient;
	companyact->begin();
	cursor2 * cur= companyact->cargacursor(query, "querypresupuesto");
	companyact->commit(); 
	if (!cur->eof()) {
		nombre = cur->valor("nomcliente").ascii();	
		direccion = cur->valor("dircliente").ascii();
		poblacion = cur->valor("poblcliente").ascii();
		cp = cur->valor("cpcliente").ascii();
		telefono = cur->valor("telcliente").ascii();
		fax = cur->valor("faxcliente").ascii();
    }// end if
	 delete cur;
	
	fitxersortidatxt << "<para style=\"name\">" << nombre.c_str() << "</para>\n" ;
	fitxersortidatxt << "<para>" << direccion.c_str() << "</para>\n" ;
	fitxersortidatxt << "<para>" << cp.c_str() << " " << poblacion.c_str() << "</para>\n" ;
	fitxersortidatxt << "<para>" << "Tfno: " << telefono.c_str() << "  Fax: " << fax.c_str() << "</para>\n" ;
	fitxersortidatxt << "\t<nextFrame/>\n" ;
	
	// Dades capcelera pressupost
	fitxersortidatxt << "\t<blockTable colWidths=\"2.5cm, 2.5cm, 2.5cm, 2.5cm\" style=\"datoscabecera\">\n" ;
	fitxersortidatxt << "\t\t<tr><td>NIF/CIF</td><td>Cod. Clien.</td><td>Fecha</td><td>N. Presup</td></tr>\n" ;
	fitxersortidatxt << "\t\t<tr>\n" ;
	fitxersortidatxt << "\t\t\t<td>" << cif.c_str() << "</td>\n" ;
	fitxersortidatxt << "\t\t\t<td></td>\n" ;
	fitxersortidatxt << "\t\t\t<td>" << fecha << "</td>\n" ;
	fitxersortidatxt << "\t\t\t<td>" << numero << "</td>\n" ;
	fitxersortidatxt << "\t\t</tr>\n" ;
	fitxersortidatxt << "\t</blockTable>\n" ;
	fitxersortidatxt << "\t<nextFrame/>\n";
	
	// Dades detall pressupost
	fitxersortidatxt << "\t<setNextTemplate name=\"others\"/>\n" ;
	fitxersortidatxt << "\t<spacer length=\"0.5cm\" width=\"1mm\"/>" ;
	fitxersortidatxt << "\t<blockTable colWidths=\"1.5cm, 10cm, 2cm, 2cm, 1.5cm, 2cm\" style=\"products\">" ;
	fitxersortidatxt << "\t<tr><td>Código</td><td>Descripción</td><td>Cantidad</td><td>Precio</td><td>%Desc</td><td>Importe</td></tr>\n" ;

	int i = 0;
	int error = 0;
	while (i < m_list->numRows() && error==0) {
		if (m_list->text(i,COL_REMOVE)!="S") {
			if (m_list->text(i,COL_IDARTICULO)!="" || m_list->text(i,COL_NOMARTICULO)!="") {
				codigoarticulo = m_list->text(i,COL_CODARTICULO).ascii();
				descripcionarticulo = m_list->text(i,COL_NOMARTICULO).ascii();
				preciolinea = m_list->text(i,COL_PVPLPRESUPUESTO).ascii();
				cantidadlinea = m_list->text(i,COL_CANTLPRESUPUESTO).ascii();
				descuentolinea = m_list->text(i,COL_DESCUENTOLPRESUPUESTO).ascii();
				float importe = m_list->text(i, COL_PVPLPRESUPUESTO).toFloat() * m_list->text(i, COL_CANTLPRESUPUESTO).toFloat();
				importe -=  (importe*m_list->text(i, COL_DESCUENTOLPRESUPUESTO).toFloat())/100;
				importelinea = QString().sprintf("%0.2f",importe).ascii();
				bases[m_list->text(i,COL_TIPO_IVA).toInt()]+=importe;
				tasas[m_list->text(i,COL_TIPO_IVA).toInt()]=m_list->text(i,COL_TASATIPO_IVA).toFloat();
		
				if (txt) {
					//presentació txt normal
					fitxersortidatxt << "\t<tr>" ;
					fitxersortidatxt << "\t\t<td>" << codigoarticulo.c_str() << "</td>\n" ;
					fitxersortidatxt << "\t\t<td><para>" << descripcionarticulo.c_str() << "</para></td>\n" ;
					fitxersortidatxt << "\t\t<td>" << cantidadlinea.c_str() << "</td>\n" ; 
					fitxersortidatxt << "\t\t<td>" << preciolinea.c_str() << "</td>\n" ;  
					fitxersortidatxt << "\t\t<td>" << descuentolinea.c_str() << "</td>\n" ; 
					fitxersortidatxt << "\t\t<td>" << importelinea.c_str() << "</td>\n" ; 
					fitxersortidatxt << "\t</tr>" ;
				}
			}
		}
		i ++;
   } //end while

	fitxersortidatxt << "\t</blockTable>\n" ;
	
	
	// Línea de totales del presupuesto
	
	fitxersortidatxt << "\t<spacer length=\"2cm\" width=\"50mm\"/>\n" ;
	
	fitxersortidatxt << "\t<blockTable colWidths=\"2.5cm, 2.5cm, 2.5cm, 2.5cm\" style=\"products2\">\n" ;
	fitxersortidatxt << "\t<tr><td>NETO</td><td>CUOTA IVA</td><td>IVA</td><td>TOTAL</td></tr>\n" ;
	
	for (int i=0;i<100;i++) {
		if (bases[i]!=0 && bases[i]!=NULL) {
			string base = QString().sprintf("%0.2f",bases[i]).ascii();
			string tasa = QString().sprintf("%0.2f",tasas[i]).ascii();
			string cuota = QString().sprintf("%0.2f",(bases[i]*tasas[i])/100).ascii();
			string total = QString().sprintf("%0.2f",(bases[i] + (bases[i]*tasas[i])/100)).ascii();
			fitxersortidatxt << "\t<tr>\n" ;
			fitxersortidatxt << "\t\t<td>" << base.c_str() << "</td>\n" ;
			fitxersortidatxt << "\t\t<td>" << tasa.c_str() << "</td>\n" ;
			fitxersortidatxt << "\t\t<td>" << cuota.c_str() << "</td>\n" ;
			fitxersortidatxt << "\t\t<td>" << total.c_str() << "</td>\n" ;
			fitxersortidatxt << "\t</tr>\n" ;
		}
	} //end for
	
	fitxersortidatxt << "\t</blockTable>\n" ;
	fitxersortidatxt << "\t<spacer length=\"2cm\" width=\"1mm\"/>\n" ;
	fitxersortidatxt << "\t<para style=\"payment\">\n" ;
	fitxersortidatxt << "\t" << observaciones.c_str() << "\n" ;
	fitxersortidatxt << "\t</para>\n" ;
	fitxersortidatxt << "</story>\n" ;
	fitxersortidatxt << "</document>\n" ;
	
	fitxersortidatxt.close();
   filestr.close();
	system("trml2pdf.py presupuesto.jm.rml > pressupost.pdf");
	system("kprinter pressupost.pdf");
} //end presentaOpenReports


// Esta función fue un intento de sacar un presupuesto con kugar. Lo dejo aquí de momento por si más adelante sirve de algo.
void Budget::presentakugar() {
	int txt=1;
	string codigoarticulo;
	string descripcionarticulo;
	string cantidadlinea;
	string preciolinea;
	string descuentolinea;
	string importelinea;
	cursor2 *cursoraux;
	
	char *argstxt[]={"pressupost.kud","pressupost.kud",NULL};      //presentació txt normal
	ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
	if (!fitxersortidatxt) txt=0;    // verifiquem que s'hagin creat correctament els fitxers
	
	if (txt) {
		//presentació txt normal
		fitxersortidatxt.setf(ios::fixed);
		fitxersortidatxt.precision(2);
		fitxersortidatxt << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" ;
		fitxersortidatxt << "<!DOCTYPE KugarData [\n" ;
		fitxersortidatxt << "\t<!ELEMENT KugarData (Row* )>\n" ;
		fitxersortidatxt << "\t\t<!ATTLIST KugarData\n";
		fitxersortidatxt << "\t\tTemplate CDATA #REQUIRED>\n";
		fitxersortidatxt << "\t<!ELEMENT Row EMPTY>\n";
		fitxersortidatxt << "\t<!ATTLIST Row \n";
		fitxersortidatxt << "\t\tlevel CDATA #REQUIRED\n";
		fitxersortidatxt << "\t\tcodigoarticulo CDATA #REQUIRED\n";
		fitxersortidatxt << "\t\tdescripcionarticulo CDATA #REQUIRED\n";
		fitxersortidatxt << "\t\tcantidadlinea CDATA #REQUIRED\n";
		fitxersortidatxt << "\t\tpreciolinea CDATA #REQUIRED\n";
		fitxersortidatxt << "\t\tdescuentolinea CDATA #REQUIRED\n";
		fitxersortidatxt << "\t\timportelinea CDATA #REQUIRED>\n";
		fitxersortidatxt << "]>\n\n";	 
		fitxersortidatxt << "<KugarData Template=\"" << confpr->valor(CONF_DIR_KUGAR).c_str()<<"pressupost.kut\">\n";
	}// end if
	companyact->begin();
	cursoraux = companyact->cargacursor("SELECT * FROM presupuesto LEFT JOIN  lpresupuesto ON presupuesto.idpresupuesto = lpresupuesto.idpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo ORDER BY idlpresupuesto ASC","elquery");
	companyact->commit();
	for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
		codigoarticulo = cursoraux->valor("codarticulo").ascii();
		descripcionarticulo = cursoraux->valor("nomarticulo").ascii();
		preciolinea = cursoraux->valor("pvplpresupuesto").ascii();
		cantidadlinea = QString().sprintf("%0.3f",cursoraux->valor("cantlpresupuesto").toFloat()).ascii(); //cursoraux->valor("cantlpresupuesto").ascii();
		descuentolinea = cursoraux->valor("descuentolpresupuesto").ascii();
		importelinea = "";
		
		if (txt) {
			//presentació txt normal
			fitxersortidatxt << "\t<Row level=\"0\" codigoarticulo=\""<< codigoarticulo.c_str() <<"\"";
			fitxersortidatxt << " descripcionarticulo=\""<< descripcionarticulo.c_str() <<"\""; 
			fitxersortidatxt << " preciolinea=\""<< preciolinea.c_str() <<"\""; 
			fitxersortidatxt << " cantidadlinea=\""<< cantidadlinea.c_str() <<"\""; 
			fitxersortidatxt << " descuentolinea=\""<< descuentolinea.c_str() <<"\""; 
			fitxersortidatxt << " importelinea=\""<< importelinea.c_str() <<"\"/>\n" ; 
		}
	}// end for

	delete cursoraux;
	fitxersortidatxt <<"</KugarData>\n";
	fitxersortidatxt.close();
	system("kugar pressupost.kud");
}// end presentakugar
