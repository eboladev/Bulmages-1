/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                              *
 *   alvaro.demiguel@gmail.com                                                  *
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

 // ALBARANES DE CLIENTES
 // CLIENT DELIVERY NOTES
/*
-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- Tots els albarans d'una factura corresponen al mateix client.
-- FACTURACIO>Albarans:
-- Albarans pendents: S'entendran com albarans pendents tots aquells dels quals no existeixi ticket, factura ni nofactura.
-- Numero
-- Data
-- Albarà a clients.
CREATE TABLE albaran (
   numalbaran integer PRIMARY KEY,
   fechaalbaran date,
   idusuario integer,

--   numalbaran integer REFERENCES presupuesto(numalbaran),
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   numfactura integer REFERENCES factura(numfactura),
   numnofactura integer REFERENCES nofactura(numnofactura)
);



-- Descuento albaran
-- Numero
-- Concepte: Descripció del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albarà a clients.
CREATE TABLE dalbaran (
   numdalbaran integer PRIMARY KEY,
   conceptdalbaran character varying(500),
   propordalbaran float,
   
   numalbaran integer REFERENCES albaran(numalbaran)
);


-- Numero
-- Descripcio
-- Quantitat
-- PVP: Preu de l'article en el moment de la compra o de ser presupostat.
-- Descompte
-- Línia d'albarà a clients.
CREATE TABLE lalbaran (
   numlalbaran integer PRIMARY KEY,
   desclalbaran character varying(100),
   cantlalbaran float,
   pvplalbaran float,
   descontlalbaran float,
   
   numalbaran integer REFERENCES albaran(numalbaran),
   idarticulo integer REFERENCES articulo(idarticulo)
);
*/

#include "clientdelivnote.h"
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
#include <qcombobox.h>

#include "funcaux.h"
//#include "postgresiface2.h"

#define COL_NUMLALBARAN 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLALBARAN 4
#define COL_CANTLALBARAN 5
#define COL_PVPLALBARAN 6
#define COL_DESCUENTOLALBARAN 7
#define COL_NUMALBARAN 8
#define COL_REMOVE 9
#define COL_TASATIPO_IVA 10

#define COL_DESCUENTO_NUMDALBARAN 0
#define COL_DESCUENTO_CONCEPTDALBARAN 1
#define COL_DESCUENTO_PROPORCIONDALBARAN 2
#define COL_DESCUENTO_REMOVE 3

ClientDelivNote::ClientDelivNote(company *comp, QWidget *parent, const char *name) : ClientDelivNoteBase(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   m_idalbaran = "0";
   m_idclient = "";
	inicialize();
}// end ClientDelivNote

ClientDelivNote::~ClientDelivNote() {
	companyact->refreshClientDelivNotes();
}// end ~ClientDelivNote


void ClientDelivNote::inicialize() {
	installEventFilter(this);
	m_list->installEventFilter( this );
	m_listDiscounts->installEventFilter( this );
	
/*	m_comboformapago->clear();
	QString query = "SELECT * FROM prfp WHERE numalbaran="+m_numalbaran;
	companyact->begin();
	cursor2 * cur1= companyact->cargacursor(query, "querypresupuesto");
	companyact->commit();
	if (!cur1->eof()) {
		cargarcomboformapago(cur1->valor("idforma_pago"));
	} else {
		cargarcomboformapago("0");	
	}// end if
	delete cur1; */

	
	
// Inicializamos la tabla de lineas de presupuesto
	m_list->setNumRows( 0 );
	m_list->setNumCols( 0 );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setSorting( TRUE );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setColumnMovingEnabled( TRUE );
	m_list->setNumCols(11);
	m_list->horizontalHeader()->setLabel( COL_NUMLALBARAN, tr( "Nº Línea" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCLALBARAN, tr( "Descripción" ) );
	m_list->horizontalHeader()->setLabel( COL_CANTLALBARAN, tr( "Cantidad" ) );
	m_list->horizontalHeader()->setLabel( COL_PVPLALBARAN, tr( "Precio" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCUENTOLALBARAN, tr( "Descuento" ) );
	m_list->horizontalHeader()->setLabel( COL_NUMALBARAN, tr( "Nº Albarán" ) );
	m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
   
	m_list->setColumnWidth(COL_NUMLALBARAN,100);
	m_list->setColumnWidth(COL_DESCLALBARAN,300);
	m_list->setColumnWidth(COL_CANTLALBARAN,100);
	m_list->setColumnWidth(COL_PVPLALBARAN,100);
	m_list->setColumnWidth(COL_DESCUENTOLALBARAN,100);
	m_list->setColumnWidth(COL_NUMALBARAN,100);
	m_list->setColumnWidth(COL_IDARTICULO,100);
	m_list->setColumnWidth(COL_CODARTICULO,100);
	m_list->setColumnWidth(COL_NOMARTICULO,300);
	m_list->setColumnWidth(COL_TASATIPO_IVA,50);

	
	m_list->hideColumn(COL_NUMLALBARAN);
	m_list->hideColumn(COL_NUMALBARAN);
	m_list->hideColumn(COL_IDARTICULO);
	m_list->hideColumn(COL_REMOVE);
	m_list->hideColumn(COL_TASATIPO_IVA);
	
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
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_NUMDALBARAN, tr( "id" ) );
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_CONCEPTDALBARAN, tr( "Concepto" ) );
	m_listDiscounts->horizontalHeader()->setLabel( COL_DESCUENTO_PROPORCIONDALBARAN, tr( "Proporción" ) );
   
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_NUMDALBARAN,100);
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_CONCEPTDALBARAN,400);
	m_listDiscounts->setColumnWidth(COL_DESCUENTO_PROPORCIONDALBARAN,100);
	m_listDiscounts->hideColumn(COL_DESCUENTO_NUMDALBARAN);
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
	m_totalClientDelivNote->setReadOnly(TRUE);
	m_totalClientDelivNote->setAlignment(Qt::AlignRight);
}// end inicialize


// Esta función carga un presupuesto.
void ClientDelivNote::chargeClientDelivNote(QString idbudget) {
	m_idalbaran = idbudget;
	inicialize();
	
	QString query = "SELECT * FROM presupuesto LEFT JOIN cliente ON cliente.idcliente = presupuesto.idcliente WHERE numalbaran="+idbudget;
	companyact->begin();
	cursor2 * cur= companyact->cargacursor(query, "querypresupuesto");
	companyact->commit();
	if (!cur->eof()) {
		m_idclient = cur->valor("idcliente");	
		m_numalbaran->setText(cur->valor("numpresupuesto"));
		m_fpresupuesto->setText(cur->valor("fpresupuesto"));
		m_comentpresupuesto->setText(cur->valor("comentpresupuesto"));
		m_nomclient->setText(cur->valor("nomcliente"));
		m_cifclient->setText(cur->valor("cifcliente"));
   
		chargeClientDelivNoteLines(idbudget);
		chargeClientDelivNoteDiscounts(idbudget);
		calculateImports();
    }// end if
     delete cur;   
	  
	m_initialValues = calculateValues();
}// end chargeClientDelivNote


void ClientDelivNote::cargarcomboformapago(QString idformapago) {
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
void ClientDelivNote::chargeClientDelivNoteLines(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lalbaran, articulo, tipo_iva WHERE numalbaran="+idbudget+" AND articulo.idarticulo=lalbaran.idarticulo and articulo.idtipo_iva = tipo_iva.idtipo_iva","unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		m_list->setText(i,COL_NUMLALBARAN,cur->valor("numlalbaran"));
		m_list->setText(i,COL_DESCLALBARAN,cur->valor("desclalbaran"));
		m_list->setText(i,COL_CANTLALBARAN,QString().sprintf("%0.3f",cur->valor("cantlalbaran").toFloat()));
		m_list->setText(i,COL_PVPLALBARAN,QString().sprintf("%0.2f",cur->valor("pvplalbaran").toFloat()));
		m_list->setText(i,COL_DESCUENTOLALBARAN,QString().sprintf("%0.2f",cur->valor("descontlalbaran").toFloat()));
		m_list->setText(i,COL_NUMALBARAN,cur->valor("numalbaran"));
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		m_list->setText(i,COL_TASATIPO_IVA,cur->valor("tasatipo_iva"));
		i++;
		cur->siguienteregistro();
	}// end while
	if (i>0) m_list->setNumRows(i);
	
	delete cur;
}// end chargeClientDelivNoteLines


// Carga líneas descuentos presupuesto
void ClientDelivNote::chargeClientDelivNoteDiscounts(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM dalbaran WHERE numalbaran="+idbudget,"unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		m_listDiscounts->setText(i,COL_DESCUENTO_NUMDALBARAN,cur->valor("numdalbaran"));
		m_listDiscounts->setText(i,COL_DESCUENTO_CONCEPTDALBARAN,cur->valor("conceptdalbaran"));
		m_listDiscounts->setText(i,COL_DESCUENTO_PROPORCIONDALBARAN,QString().sprintf("%0.2f",cur->valor("proporciondalbaran").toFloat()));
		i++;
		cur->siguienteregistro();
	}// end while
	if (i>0) m_listDiscounts->setNumRows(i);
	
	delete cur;
}// end chargeClientDelivNoteDiscounts


// Búsqueda de Clientes.
void ClientDelivNote::s_searchClient() {
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


void ClientDelivNote::s_delivNoteDateLostFocus() {
	m_fpresupuesto->setText(normalizafecha(m_fpresupuesto->text()).toString("dd/MM/yyyy"));
}



void ClientDelivNote::s_newClientDelivNoteLine() {
	m_list->setNumRows( m_list->numRows()+1 );
	m_list->setCurrentCell(m_list->numRows()-1, COL_CODARTICULO);	
	m_list->editCell(m_list->numRows()-1, COL_CODARTICULO);
}


void ClientDelivNote::s_removeClientDelivNote() {
	fprintf(stderr,"Iniciamos el boton_borrar\n");
	if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este presupuesto", "Sí", "No") == 0) {
		companyact->begin();
		QString SQLQuery = "DELETE FROM lalbaran WHERE numalbaran ="+m_idalbaran;
		if (companyact->ejecuta(SQLQuery)==0){
			QString SQLQuery = "DELETE FROM dalbaran WHERE numalbaran ="+m_idalbaran;
				if (companyact->ejecuta(SQLQuery)==0){
					QString SQLQuery = "DELETE FROM prfp WHERE numalbaran ="+m_idalbaran;
					if (companyact->ejecuta(SQLQuery)==0){
						QString SQLQuery = "DELETE FROM presupuesto WHERE numalbaran ="+m_idalbaran;
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


void ClientDelivNote::s_removeClientDelivNoteLine() {
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
}


void ClientDelivNote::s_valueClientDelivNoteLineChanged(int row, int col) {
	
	switch (col) {
		case COL_DESCUENTOLALBARAN: {
			m_list->setText(row, COL_DESCUENTOLALBARAN, m_list->text(row, COL_DESCUENTOLALBARAN).replace(",","."));
			float discountLine = m_list->text(row, COL_DESCUENTOLALBARAN).toFloat();
			m_list->setText(row, COL_DESCUENTOLALBARAN, QString().sprintf("%0.2f", discountLine));
		}
		case COL_CODARTICULO: {
			manageArticle(row);
			calculateImports();
		}
		case COL_CANTLALBARAN: {
			m_list->setText(row, COL_CANTLALBARAN, m_list->text(row, COL_CANTLALBARAN).replace(",","."));
			float cantLine = m_list->text(row, COL_CANTLALBARAN).toFloat();
			m_list->setText(row, COL_CANTLALBARAN, QString().sprintf("%0.3f", cantLine));
			calculateImports();
		}
		case COL_PVPLALBARAN: {
			m_list->setText(row, COL_PVPLALBARAN, m_list->text(row, COL_PVPLALBARAN).replace(",","."));
			float pvpLine = m_list->text(row, COL_PVPLALBARAN).toFloat();
			m_list->setText(row, COL_PVPLALBARAN, QString().sprintf("%0.2f", pvpLine));
			calculateImports();
		}
	}
} //end valueClientDelivNoteLineChanged


void ClientDelivNote::s_newClientDelivNoteDiscountLine() {
	m_listDiscounts->setNumRows( m_listDiscounts->numRows()+1 );
	m_listDiscounts->editCell(m_listDiscounts->numRows()-1, COL_DESCUENTO_CONCEPTDALBARAN);
	
}


void ClientDelivNote::s_removeClientDelivNoteDiscountLine() {
	if (m_listDiscounts->currentRow() >= 0) {
		int row = m_listDiscounts->currentRow();
		m_listDiscounts->setText(row, COL_DESCUENTO_REMOVE, "S");
		m_listDiscounts->hideRow(row);
	}
}


void ClientDelivNote::s_valueClientDelivNoteDiscountLineChanged(int row, int col) {
	switch (col) {
		case COL_DESCUENTO_PROPORCIONDALBARAN: {
			m_listDiscounts->setText(row, COL_DESCUENTO_PROPORCIONDALBARAN, m_listDiscounts->text(row, COL_DESCUENTO_PROPORCIONDALBARAN).replace(",","."));
			float proporcionLine = m_listDiscounts->text(row, COL_DESCUENTO_PROPORCIONDALBARAN).toFloat();
			m_listDiscounts->setText(row, COL_DESCUENTO_PROPORCIONDALBARAN, QString().sprintf("%0.2f", proporcionLine));
			calculateImports();
		}
	}
} //end valueClientDelivNoteDiscountLineChanged

void ClientDelivNote::manageArticle(int row) {
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
			delete cur3;
		}
		delete cur2;
	}
} //end manageArticle


QString ClientDelivNote::searchArticle() {
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


void ClientDelivNote::s_saveClientDelivNote() {
	companyact->begin();
	if (saveClientDelivNote()==0) {
		if (m_idalbaran == "0") {
			cursor2 * cur4= companyact->cargacursor("SELECT max(numalbaran) FROM presupuesto","unquery1");
			if (!cur4->eof()) {
				m_idalbaran=cur4->valor(0);
				if (insertfpClientDelivNote()!=0) {
					companyact->rollback();
					return;
				}
			}
			delete cur4;
		} else {
			if (updatefpClientDelivNote()!=0) {
				companyact->rollback();
				return;
			}
		 }
		if (saveClientDelivNoteLines()==0 && saveClientDelivNoteDiscountLines()==0) {
			companyact->commit();
			m_initialValues = calculateValues();
		} else {
			companyact->rollback();
		}
	} else {
		companyact->rollback();
	}	
	chargeClientDelivNote(m_idalbaran);
}


void ClientDelivNote::s_accept() {
	fprintf(stderr,"accept button activated\n");
	companyact->begin();
	if (saveClientDelivNote()==0) {
		if (m_idalbaran == "0") {
			cursor2 * cur4= companyact->cargacursor("SELECT max(numalbaran) FROM presupuesto","unquery1");
			if (!cur4->eof()) {
				m_idalbaran=cur4->valor(0);
				if (insertfpClientDelivNote()!=0) {
					companyact->rollback();
					return;
				}
			}
			delete cur4;
		} else {
			if (updatefpClientDelivNote()!=0) {
				companyact->rollback();
				return;
			}
		}
		 
		if (saveClientDelivNoteLines()==0 && saveClientDelivNoteDiscountLines()==0) {
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


int ClientDelivNote::saveClientDelivNote() {
	QString SQLQuery;
	
	if (m_idalbaran != "0") {
		SQLQuery = "UPDATE presupuesto  SET numpresupuesto="+m_numalbaran->text();
      SQLQuery += " , fpresupuesto='"+ m_fpresupuesto->text()+"'";
      SQLQuery += " , comentpresupuesto='"+m_comentpresupuesto->text()+"'";
		SQLQuery += " , idcliente="+m_idclient;
      SQLQuery += " WHERE numalbaran ="+m_idalbaran;
	} else {
		SQLQuery = "INSERT INTO presupuesto (numpresupuesto, fpresupuesto, comentpresupuesto, idcliente)";
		SQLQuery += " VALUES (";
		SQLQuery += m_numalbaran->text();
		SQLQuery += " , '"+m_fpresupuesto->text()+"'";
      SQLQuery += " , '"+m_comentpresupuesto->text()+"'";
		SQLQuery += " , "+m_idclient;
      SQLQuery += " ) ";
	}
	return companyact->ejecuta(SQLQuery);
} //end saveClientDelivNote


int ClientDelivNote::insertfpClientDelivNote() {
	QString SQLQuery;
	SQLQuery = "INSERT INTO prfp (numalbaran, idforma_pago)";
	SQLQuery += " VALUES (";
	SQLQuery += m_idalbaran;
	SQLQuery += " , "+m_cursorcombo->valor("idforma_pago",m_comboformapago->currentItem());
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertfpClientDelivNote


int ClientDelivNote::updatefpClientDelivNote() {
	QString SQLQuery;
	SQLQuery = "UPDATE prfp SET idforma_pago="+m_cursorcombo->valor("idforma_pago",m_comboformapago->currentItem());
	SQLQuery += " WHERE numalbaran ="+m_idalbaran;
	return companyact->ejecuta(SQLQuery);
} //end saveClientDelivNote


int ClientDelivNote::saveClientDelivNoteLines() {
	int i = 0;
	int error = 0;
	while (i < m_list->numRows() && error==0) {
		if (m_list->text(i,COL_REMOVE)=="S") {
			if (m_list->text(i,COL_NUMLALBARAN)!="") {
				error = deleteClientDelivNoteLine(i);
			}
		} else {
			if (m_list->text(i,COL_IDARTICULO)!="" || m_list->text(i,COL_NOMARTICULO)!="") {
				if (m_list->text(i,COL_NUMLALBARAN)!="") {
					error = updateClientDelivNoteLine(i);
				} else {
					error = insertClientDelivNoteLine(i);
				}
			}
		}
		i ++;
   }
	return error;
} // end saveClientDelivNoteLines


int ClientDelivNote::saveClientDelivNoteDiscountLines() {
	int i = 0;
	int error = 0;
	while (i < m_listDiscounts->numRows() && error==0) {
		if (m_listDiscounts->text(i,COL_DESCUENTO_REMOVE)=="S") {
			if (m_listDiscounts->text(i,COL_DESCUENTO_NUMDALBARAN)!="") {
				error = deleteClientDelivNoteDiscountLine(i);
			}
		} else {
			if (m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDALBARAN)!="" || m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDALBARAN)!="") {
				if (m_listDiscounts->text(i,COL_DESCUENTO_NUMDALBARAN)!="") {
					error = updateClientDelivNoteDiscountLine(i);
				} else {
					error = insertClientDelivNoteDiscountLine(i);
				}
			}
		}
		i ++;
   }
	return error;
} // end saveClientDelivNoteDiscountLines


int ClientDelivNote::updateClientDelivNoteLine(int i) {
	QString SQLQuery = "UPDATE lalbaran SET desclalbaran='"+m_list->text(i,COL_DESCLALBARAN)+"'";
	SQLQuery += " , cantlalbaran="+ m_list->text(i,COL_CANTLALBARAN);
	SQLQuery += " , pvplalbaran="+m_list->text(i,COL_PVPLALBARAN);
	SQLQuery += " , descontlalbaran="+m_list->text(i,COL_DESCUENTOLALBARAN);
	SQLQuery += " , idarticulo="+m_list->text(i,COL_IDARTICULO);
	SQLQuery += " WHERE numalbaran ="+m_idalbaran+" AND numlalbaran="+m_list->text(i,COL_NUMLALBARAN);
	return companyact->ejecuta(SQLQuery);
} //end updateClientDelivNoteLine


int ClientDelivNote::insertClientDelivNoteLine(int i) {
	QString SQLQuery ="";
	SQLQuery = "INSERT INTO lalbaran (desclalbaran, cantlalbaran, pvplalbaran, desclalbaran, numalbaran, idarticulo)";
	SQLQuery += " VALUES (";
	SQLQuery += "'"+m_list->text(i,COL_DESCLALBARAN)+"'";
	SQLQuery += " , "+m_list->text(i,COL_CANTLALBARAN);
	SQLQuery += " , "+m_list->text(i,COL_PVPLALBARAN);
	SQLQuery += " , "+m_list->text(i,COL_DESCUENTOLALBARAN);
	SQLQuery += " , "+m_idalbaran;
	SQLQuery += " , "+m_list->text(i,COL_IDARTICULO);
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertClientDelivNoteLine


int ClientDelivNote::deleteClientDelivNoteLine(int line) {
	QString SQLQuery = "DELETE FROM lalbaran WHERE numlalbaran ="+m_list->text(line,COL_NUMLALBARAN);
	return companyact->ejecuta(SQLQuery);
} //end deleteClientDelivNoteLine


int ClientDelivNote::updateClientDelivNoteDiscountLine(int i) {
	QString SQLQuery = "UPDATE dalbaran SET conceptdalbaran='"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDALBARAN)+"'";
	SQLQuery += " , proporciondalbaran="+ m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDALBARAN);
	SQLQuery += " WHERE numalbaran ="+m_idalbaran+" AND numdalbaran="+m_listDiscounts->text(i,COL_DESCUENTO_NUMDALBARAN);
	return companyact->ejecuta(SQLQuery);
} //end updateClientDelivNoteDiscountLine


int ClientDelivNote::insertClientDelivNoteDiscountLine(int i) {
	QString SQLQuery ="";
	SQLQuery = "INSERT INTO dalbaran (conceptdalbaran, proporciondalbaran, numalbaran)";
	SQLQuery += " VALUES (";
	SQLQuery += "'"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDALBARAN)+"'";
	SQLQuery += " , "+m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDALBARAN);
	SQLQuery += " , "+m_idalbaran;
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertClientDelivNoteDiscountLine


int ClientDelivNote::deleteClientDelivNoteDiscountLine(int line) {
	QString SQLQuery = "DELETE FROM dalbaran WHERE numdalbaran ="+m_listDiscounts->text(line,COL_DESCUENTO_NUMDALBARAN);
	return companyact->ejecuta(SQLQuery);
} //end deleteClientDelivNoteDiscountLine

void ClientDelivNote::s_cancel() {
	close();
}//end cancel


void ClientDelivNote::calculateImports() {
	int i = 0;
	float netImport = 0;
	float taxImport = 0;
	float discountImport = 0;
	while (i < m_list->numRows()) {
		if (m_list->text(i,COL_PVPLALBARAN)!="" and m_list->text(i,COL_CANTLALBARAN)!="") {
			netImport += m_list->text(i,COL_PVPLALBARAN).toFloat() * m_list->text(i,COL_CANTLALBARAN).toFloat();
			taxImport += (m_list->text(i,COL_PVPLALBARAN).toFloat() * m_list->text(i,COL_CANTLALBARAN).toFloat() * m_list->text(i,COL_TASATIPO_IVA).toFloat())/100;
			discountImport += (m_list->text(i,COL_PVPLALBARAN).toFloat() * m_list->text(i,COL_CANTLALBARAN).toFloat() * m_list->text(i,COL_DESCUENTOLALBARAN).toFloat())/100;
		}
		i ++;
   }
	
	m_totalBases->setText(QString().sprintf("%0.2f", netImport));
	m_totalTaxes->setText(QString().sprintf("%0.2f", taxImport));
	m_totalDiscounts->setText(QString().sprintf("%0.2f", discountImport));
	m_totalClientDelivNote->setText(QString().sprintf("%0.2f", netImport+taxImport));
} // end calculateImports


QString ClientDelivNote::calculateValues() {
	QString values = retrieveValues("QTable");
	values += retrieveValues("QLineEdit");
	values += retrieveValues("QTextEdit");
	return values;
}


bool ClientDelivNote::eventFilter( QObject *obj, QEvent *ev ) {
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
						s_valueClientDelivNoteLineChanged(t->currentRow(), t->currentColumn());
					}
					if (QString(obj->name()).stripWhiteSpace() == "m_listDiscounts") {
						s_valueClientDelivNoteDiscountLineChanged(t->currentRow(), t->currentColumn());
					}
					nextCell(obj);
					return TRUE;
				}
				
				case Qt::Key_Asterisk: {
					duplicateCell(obj);
					if (QString(obj->name()).stripWhiteSpace() == "m_list") {
						s_valueClientDelivNoteLineChanged(t->currentRow(), t->currentColumn());
					}
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
	return FALSE;
} //end eventFilter


void ClientDelivNote::nextCell(QObject *obj) {
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


void ClientDelivNote::antCell(QObject *obj) {
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

void ClientDelivNote::duplicateCell(QObject *obj) {
	QTable *t = (QTable *)obj;
	int row = t->currentRow();
	int col = t->currentColumn();
	if (t->text(row, col) == "" && row>0) {
		t->setText(row, col, t->text(row-1, col));
	}
}


QString ClientDelivNote::retrieveValues(QString qsWidget) {
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

