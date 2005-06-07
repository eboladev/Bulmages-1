/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borrï¿½ Riera                              *
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
-- Entendemos que un presupuesto es una relaciï¿½ de materiales y trabajos cuantificada que
-- hacemos a peticiï¿½ de un cliente determinado
-- Numero
-- Data: Data d'emisiï¿½del presupost.
-- PersContacte: Nom de persona de contacte (si cal).
-- TelfContacte: Telï¿½on.
-- Venciment: Data mï¿½ima de validesa del presupost.
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
-- Descripcio: Descripciï¿½de l'article en el moment de ser presupostat.
-- Quantitat
-- PVP: Preu de l'article en el moment de ser pressupostat
-- Descompte: Percentatge de descompte en lï¿½ia.
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

#include "listlinpresupuestoview.h"

#include "budget.h"
#include "company.h"
#include "division.h"
#include "clientslist.h"
#include "articleslist.h"
#include "configuracion.h"


#include <qmessagebox.h>

#include <qtable.h>
#include <qwidget.h>
#include <qobjectlist.h>
#include <qcombobox.h>
#include <qpopupmenu.h>
#include <qtoolbutton.h>

#include <qlayout.h>

#include <fstream>
using namespace std;

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
#define COL_TIPO_IVA 11

#define COL_DESCUENTO_IDDPRESUPUESTO 0
#define COL_DESCUENTO_CONCEPTDPRESUPUESTO 1
#define COL_DESCUENTO_PROPORCIONDPRESUPUESTO 2
#define COL_DESCUENTO_REMOVE 3

#define coma "'"

Budget::Budget( company *comp , QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) , presupuesto(comp) {
	/// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
	subform2->setcompany(comp);
	setlislinpresupuesto(subform2);
   inicialize();
   comp->meteWindow(caption(),this);
   	fprintf(stderr,"Fin de la inicialización de Budget\n");
}// end Budget


Budget::~Budget() {
	companyact->refreshBudgets();
	companyact->sacaWindow(this);
}// end ~Budget


void Budget::inicialize() {

	m_nomalmacen->setText("");
	// Tratamos la forma de pago.
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
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_PROPORCIONDPRESUPUESTO, tr( "Proporciï¿½" ) );
   
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_IDDPRESUPUESTO,100);
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_CONCEPTDPRESUPUESTO,400);
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_PROPORCIONDPRESUPUESTO,100);
	m_listDiscounts->hideColumn(COL_DESCUENTO_IDDPRESUPUESTO);
	m_listDiscounts->hideColumn(COL_DESCUENTO_REMOVE);
	
	m_listDiscounts->setNumRows(10);

	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	m_listDiscounts->setPaletteBackgroundColor("#AFFAFA");   
	m_listDiscounts->setReadOnly(FALSE);    
	
	m_totalBases->setReadOnly(TRUE);
	m_totalBases->setAlignment(Qt::AlignRight);
	m_totalTaxes->setReadOnly(TRUE);
	m_totalTaxes->setAlignment(Qt::AlignRight);
	m_totalDiscounts->setReadOnly(TRUE);
	m_totalDiscounts->setAlignment(Qt::AlignRight);
	m_totalBudget->setReadOnly(TRUE);
	m_totalBudget->setAlignment(Qt::AlignRight);
	
	if (m_idpresupuesto=="0") {	 
		cursor2 * cur0= companyact->cargacursor("SELECT * FROM configuracion where nombre='AlmacenDefecto'","queryconfig");
		if (!cur0->eof()) {
			if (cur0->valor("valor")!="") {
				m_codigoalmacen->setText(cur0->valor("valor"));
			}
		}
		delete cur0;
	}
/*	
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
*/
}// end inicialize



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




// Carga lï¿½eas descuentos presupuesto
void Budget::chargeBudgetDiscounts(QString ) {
/*
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
*/
}// end chargeBudgetDiscounts


// Bsqueda de Clientes.
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
	setFPresupuesto(m_fpresupuesto->text());
}


void Budget::budgetExpiryLostFocus() {
	m_vencpresupuesto->setText(normalizafecha(m_vencpresupuesto->text()).toString("dd/MM/yyyy"));
	setVencPresupuesto(m_vencpresupuesto->text());
}


void Budget::s_almacenLostFocus() {
	setCodigoAlmacen(m_codigoalmacen->text());
}//end s_almacenLostFocus


void Budget::s_printBudget() {
	presentaReports();
}//end s_printBudget


void Budget::buscarAlmacen() {
	QMessageBox::warning( this, "BulmaFact - Presupuestos", "Aun no estï¿½implementado.", "Si", "No");
	cursor2 * cur= companyact->cargacursor("SELECT * FROM almacen where codigoalmacen ="+ m_codigoalmacen->text(),"unquery");
	delete cur;
} // end buscarAlmacen


void Budget::newBudgetLine() {
/*
	m_list->setNumRows( m_list->numRows()+1 );
	m_list->setCurrentCell(m_list->numRows()-1, COL_CODARTICULO);	
	m_list->editCell(m_list->numRows()-1, COL_CODARTICULO);
*/
}


void Budget::s_removeBudget() {
	fprintf(stderr,"Iniciamos el boton_borrar\n");
	if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este presupuesto", "Si", "No") == 0) {
		borraPresupuesto();
	}// end if	
}// end boton_borrar


void Budget::removeBudgetLine() {
/*
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
*/
}





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





void Budget::s_contextMenuDiscount(int, int, int button, const QPoint &poin) {
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
/*	
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
*/	
} //end accept




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




int Budget::deleteBudgetLine(int line) {
/*
	QString SQLQuery = "DELETE FROM lpresupuesto WHERE idlpresupuesto ="+m_list->text(line,COL_IDLPRESUPUESTO)+" AND idpresupuesto="+m_idpresupuesto;
	return companyact->ejecuta(SQLQuery);
*/
} //end deleteBudgetLine


int Budget::updateBudgetDiscountLine(int) {
/*
	QString SQLQuery = "UPDATE dpresupuesto SET conceptdpresupuesto='"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDPRESUPUESTO)+"'";
	SQLQuery += " , proporciondpresupuesto="+ m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDPRESUPUESTO);
	SQLQuery += " WHERE idpresupuesto ="+m_idpresupuesto+" AND iddpresupuesto="+m_listDiscounts->text(i,COL_DESCUENTO_IDDPRESUPUESTO);
	return companyact->ejecuta(SQLQuery);
*/
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
/*
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
*/
} // end calculateImports


QString Budget::calculateValues() {
	QString values = retrieveValues("QTable");
	values += retrieveValues("QLineEdit");
	values += retrieveValues("QTextEdit");
	return values;
}




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
/*
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
*/
}


QString Budget::newBudgetNumber() {
	QString rtnNumber;
	cursor2 * cur4= companyact->cargacursor("SELECT max(numpresupuesto) FROM presupuesto WHERE idalmacen="+m_idalmacen,"unquery1");
	if (!cur4->eof()) {
		rtnNumber = QString().sprintf("%d",cur4->valor(0).toInt()+1);
	} else {
		rtnNumber = "1";
	}
	delete cur4;
	return rtnNumber;
}


void   Budget::pintatotales(float base, float iva) {
	m_totalBases->setText(QString::number(base));
	m_totalTaxes->setText(QString::number(iva));
	m_totalBudget->setText(QString::number(iva+base));
};// end pintatotales


void Budget::presentaReports() {

	system ("cp /home/tborras/Desktop/prueba.rml /tmp/prueba.rml");
	char *argstxt[]={"/tmp/presupuesto.rml","/tmp/presupuesto.rml",NULL};      //presentaciï¿½txt normal
	ofstream fitxersortidatxt(argstxt[0]);     // creem els fitxers de sordida
	if (!fitxersortidatxt) return;    // verifiquem que s'hagin creat correctament els fitxers

	
	
	// Lï¿½ea de totales del presupuesto
fitxersortidatxt << "<story>";
fitxersortidatxt << "<blockTable style=\"tabla\" colWidths=\"10cm, 2cm, 2cm, 3cm\">";
fitxersortidatxt << "<tr>";
fitxersortidatxt << "	<td>"+mdb_nomcliente+"</td>";
fitxersortidatxt << "	<td>Cantidad</td>";
fitxersortidatxt << "	<td>Precio Und.</td>";
fitxersortidatxt << "	<td>Total</td>";
fitxersortidatxt << "</tr>";
fitxersortidatxt << "<tr>";
fitxersortidatxt << "	<td></td>";
fitxersortidatxt << "	<td></td>";
fitxersortidatxt << "	<td>Base</td>";
fitxersortidatxt << "	<td>6.00</td>";
fitxersortidatxt << "</tr>";
fitxersortidatxt << "<tr>";
fitxersortidatxt << "	<td></td>";
fitxersortidatxt << "	<td></td>";
fitxersortidatxt << "	<td>Iva</td>";
fitxersortidatxt << "	<td>6.00</td>";
fitxersortidatxt << "</tr>";
fitxersortidatxt << "<tr>";
fitxersortidatxt << "	<td></td>";
fitxersortidatxt << "	<td></td>";
fitxersortidatxt << "	<td>Total</td>";
fitxersortidatxt << "	<td>6.00</td>";
fitxersortidatxt << "</tr>";
fitxersortidatxt << "</blockTable>";
fitxersortidatxt << "<nextFrame/>";
fitxersortidatxt << "<blockTable colWidths=\"5cm\">";
fitxersortidatxt << "<tr><td>"+mdb_nomcliente+"</td></tr>";
fitxersortidatxt << "<tr><td>"+mdb_cifcliente+"</td></tr>";
fitxersortidatxt << "<tr><td>Precio Und.</td></tr>";
fitxersortidatxt << "<tr><td>Total</td></tr>";
fitxersortidatxt << "</blockTable>";
fitxersortidatxt << "</story>";
fitxersortidatxt << "</document>";
	
	fitxersortidatxt.close();
	system("cat /tmp/prueba.rml /tmp/presupuesto.rml > /tmp/pressupost.rml");	
	system("trml2pdf.py /tmp/pressupost.rml > /tmp/pressupost.pdf");
	system("kpdf /tmp/pressupost.pdf"); 
} //end presentaReports

