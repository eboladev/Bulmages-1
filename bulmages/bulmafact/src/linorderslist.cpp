/***************************************************************************
 *   Copyright (C) 2004 by J. M. Estopa Rey                                *
 *   pepma@telefonica.net                                                  *
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

// Para laura con cariño
// Esta línea en honor a bruli y a la asociación de jovenes empresarios de Valencia
 
 /*
-- Linea de pedido
-- Numero: Número de línia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsió: Data prevista de recepció
CREATE TABLE lpedido (
   numlpedido integer PRIMARY KEY,
   desclpedido character varying(150),
   cantlpedido integer,
   pvdlpedido float,
   prevlpedido date,
   
   idpedido integer NOT NULL REFERENCES pedido(idpedido),
   idalb_pro integer REFERENCES alb_pro(idalb_pro),
   idarticulo integer REFERENCES articulo(idarticulo)
);*/

#include "linorderslist.h"
#include "providerslist.h"
#include "articleslist.h"
#include <qtable.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include <qwidget.h>
#include "company.h"
#include "funcaux.h"

#define COL_NUMLPEDIDO 0
#define COL_IDARTICULO 1
#define COL_CODARTICULO 2
#define COL_NOMARTICULO 3
#define COL_DESCLPEDIDO 4
#define COL_CANTLPEDIDO 5
#define COL_PVDLPEDIDO 6
#define COL_PREVLPEDIDO 7
#define COL_IDPEDIDO 8
#define COL_IDALB_PRO 9
#define COL_REMOVE 10

linorderslist::linorderslist(company *comp, QWidget *parent, const char *name, int flag)
 : linorderslistbase(parent, name, flag) {
      companyact = comp;
}// end linorderslist

// Cargamos de la tabla de pedidos los datos del pedido seleccionado y los ponemos en la pantalla.

void linorderslist::chargeorder(QString idpedido_) {
   QString idproveedor;
   QString iddivision;
   QString idalmacen;
	
	idpedido = idpedido_;
   
   if (idpedido != "0") {
   	companyact->begin();
   	cursor2 * cur= companyact->cargacursor("SELECT * FROM pedido, division WHERE idpedido="+idpedido+" and pedido.iddivision=division.iddivision","unquery");
   	companyact->commit();
   	if (!cur->eof()) {
	  	 	m_numpedido->setText(cur->valor("numpedido"));
			m_fechapedido->setText(cur->valor("fechapedido"));
			m_descpedido->setText(cur->valor("descpedido"));
			idproveedor = cur->valor("idproveedor");
			iddivision = cur->valor("iddivision");
			idalmacen = cur->valor("idalmacen");
   	}
   	delete cur;
   	providerChanged(idproveedor);
   	cargarcombodivision(idproveedor, iddivision);
	} //endif (pedido !=0)
	cargarcomboalmacen(idalmacen);
  	
}

void linorderslist::chargelinorders(QString idpedido) {
// Cargamos datos generales pedido y proveedor
   chargeorder(idpedido);
	
// Cargamos la tabla con lasl íneas del pedido
	m_list->setNumRows( 0 );
	m_list->setNumCols( 0 );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setSorting( TRUE );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setColumnMovingEnabled( TRUE );
	m_list->setNumCols(11);
	m_list->horizontalHeader()->setLabel( COL_NUMLPEDIDO, tr( "Nº Línea" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCLPEDIDO, tr( "Descripción" ) );
	m_list->horizontalHeader()->setLabel( COL_CANTLPEDIDO, tr( "Cantidad" ) );
	m_list->horizontalHeader()->setLabel( COL_PVDLPEDIDO, tr( "Precio" ) );
	m_list->horizontalHeader()->setLabel( COL_PREVLPEDIDO, tr( "Fecha Prevista Entrega" ) );
	m_list->horizontalHeader()->setLabel( COL_IDPEDIDO, tr( "Nº Pedido" ) );
	m_list->horizontalHeader()->setLabel( COL_IDALB_PRO, tr( "Albarán" ) );
	m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
   
	m_list->setColumnWidth(COL_NUMLPEDIDO,100);
	m_list->setColumnWidth(COL_DESCLPEDIDO,300);
	m_list->setColumnWidth(COL_CANTLPEDIDO,100);
	m_list->setColumnWidth(COL_PVDLPEDIDO,100);
	m_list->setColumnWidth(COL_PREVLPEDIDO,100);
	m_list->setColumnWidth(COL_IDPEDIDO,100);
	m_list->setColumnWidth(COL_IDALB_PRO,100);
	m_list->setColumnWidth(COL_IDARTICULO,100);
	m_list->setColumnWidth(COL_CODARTICULO,100);
	m_list->setColumnWidth(COL_NOMARTICULO,300);

	m_list->hideColumn(COL_NUMLPEDIDO);
	m_list->hideColumn(COL_IDPEDIDO);
	m_list->hideColumn(COL_IDARTICULO);
	m_list->hideColumn(COL_REMOVE);
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
// Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
	m_list->setColumnReadOnly(COL_NOMARTICULO,true);
	m_list->setPaletteBackgroundColor("#AFFAFA");   
    //m_list->setReadOnly(TRUE);        
	m_list->setReadOnly(FALSE);        
	 
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpedido, articulo WHERE idpedido="+idpedido+" AND articulo.idarticulo=lpedido.idarticulo","unquery");
	companyact->commit();
	m_list->setNumRows( cur->numregistros() );
	int i=0;
	while (!cur->eof()) {
		m_list->setText(i,COL_NUMLPEDIDO,cur->valor("numlpedido"));
		m_list->setText(i,COL_DESCLPEDIDO,cur->valor("desclpedido"));
		m_list->setText(i,COL_CANTLPEDIDO,cur->valor("cantlpedido"));
		m_list->setText(i,COL_PVDLPEDIDO,cur->valor("pvdlpedido"));
		m_list->setText(i,COL_PREVLPEDIDO,cur->valor("prevlpedido"));
		m_list->setText(i,COL_IDPEDIDO,cur->valor("idpedido"));
		m_list->setText(i,COL_IDALB_PRO,cur->valor("idalb_pro"));
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		i++;
		cur->siguienteregistro();
	}// end while
	
	delete cur;
	
}// end chargelinorders

void linorderslist::cargarcomboalmacen(QString idalmacen) {
	m_cursorcombo2 = NULL;
   companyact->begin();
   if (m_cursorcombo2 != NULL) delete m_cursorcombo2;
   	m_cursorcombo2 = companyact->cargacursor("SELECT * FROM almacen","unquery");
   	companyact->commit();
   	int i = 0;
   	int i1 = 0;   
   	while (!m_cursorcombo2->eof()) {
   		i ++;
		if (idalmacen == m_cursorcombo2->valor("idalmacen")) {
		   i1 = i;
		}
   		m_comboalmacen->insertItem(m_cursorcombo2->valor("nomalmacen"));
		m_cursorcombo2->siguienteregistro();
   }
   if (i1 != 0 ) {
   	m_comboalmacen->setCurrentItem(i1-1);
   }
} // end cargarcomboalmacen

void linorderslist::cargarcombodivision(QString idproveedor, QString iddivision) {
	m_cursorcombo = NULL;
	companyact->begin();
  	if (m_cursorcombo != NULL) delete m_cursorcombo;
   	m_cursorcombo = companyact->cargacursor("SELECT * FROM division where idproveedor="+idproveedor,"unquery");
   	companyact->commit();
   	int i = 0;
   	int i1 = 0;
   	while (!m_cursorcombo->eof()) {
   		i ++;
			if (m_cursorcombo->valor("iddivision") == iddivision) {
			   i1 = i;
			}
   		m_combodivision->insertItem(m_cursorcombo->valor("descdivision"));
			m_cursorcombo->siguienteregistro();
   	}
	if (i1 != 0 ) {
   		m_combodivision->setCurrentItem(i1-1);
 	} 
} //end cargarcombodivision


void linorderslist::activated(int a) {
	fprintf(stderr,"id:%s\n", m_cursorcombo->valor("iddivision",a).ascii());
}


void linorderslist::almacenactivated(int a) {
	fprintf(stderr,"id:%s\n", m_cursorcombo2->valor("idalmacen",a).ascii());
}


void linorderslist::accept() {
	fprintf(stderr,"accept button activated\n");
	QDate orderDate = normalizafecha(m_fechapedido->text());
	
	if (idpedido != "0") {
		QString SQLQuery = "UPDATE pedido SET numpedido='"+m_numpedido->text()+"'";
      SQLQuery += " , anopedido="+ m_fechapedido->text().right(4);
      SQLQuery += " , fechapedido='"+m_fechapedido->text()+"'";
      SQLQuery += " , descpedido='"+m_descpedido->text()+"'";
      SQLQuery += " , iddivision="+m_cursorcombo->valor("iddivision",m_combodivision->currentItem());
      SQLQuery += " , idalmacen="+m_cursorcombo2->valor("idalmacen",m_comboalmacen->currentItem());
      SQLQuery += " WHERE idpedido ="+idpedido;
      companyact->begin();
      companyact->ejecuta(SQLQuery);
      companyact->commit();
	} else {
		QString SQLQuery = "INSERT INTO pedido (numpedido, anopedido, fechapedido, descpedido, iddivision, idalmacen)";
		SQLQuery += " VALUES (";
		SQLQuery += "'"+m_numpedido->text()+"'";
		SQLQuery += " , "+m_fechapedido->text().right(4);
      SQLQuery += " , '"+m_fechapedido->text()+"'";
      SQLQuery += " , '"+m_descpedido->text()+"'";
      SQLQuery += " , "+m_cursorcombo->valor("iddivision",m_combodivision->currentItem());
      SQLQuery += " , "+m_cursorcombo2->valor("idalmacen",m_comboalmacen->currentItem());
      SQLQuery += " ) ";
      companyact->begin();
      companyact->ejecuta(SQLQuery);
      companyact->commit();
	}

	saveOrderLines();
	
	close();
} //end accept


void linorderslist::neworderlin() {
	m_list->setNumRows( m_list->numRows()+1 );
} // end neworderlin


linorderslist::~linorderslist() {
}// end ~linorderslist


void linorderslist::searchProvider() {
   fprintf(stderr,"Busqueda de un proveedor\n");
   providerslist *provlist = new providerslist(companyact, NULL, theApp->translate("Seleccione proveedor","company"));
   
// , WType_Dialog| WShowModal   
   provlist->modoseleccion();
   
   // Esto es convertir un QWidget en un sistema modal de dialogo.
   this->setEnabled(false);
   provlist->show();
   while(!provlist->isHidden()) theApp->processEvents();
   this->setEnabled(true);
   
   m_idprovider = provlist->idprovider();
   m_cifprovider->setText(provlist->cifprovider());
	providerChanged(m_idprovider);
	m_combodivision->clear();
	cargarcombodivision(m_idprovider, NULL);
   delete provlist;
}// end searchProvider


void linorderslist::providerChanged(QString idProvider) {
	companyact->begin();
	cursor2 * cur2= companyact->cargacursor("SELECT * FROM proveedor WHERE idproveedor="+idProvider,"unquery");
	companyact->commit();
	if (!cur2->eof()) {
		m_cifprovider->setText(cur2->valor("cifproveedor"));
		m_nomproveedor->setText(cur2->valor("nomproveedor"));
	}
} //end providerChanged


void linorderslist::saveOrderLines() {
	int i = 0;
	while (i < m_list->numRows()) {
		if (m_list->text(i,COL_REMOVE)=="S") {
			if (m_list->text(i,COL_NUMLPEDIDO)!="") {
				deleteOrderLine(i);
			}
		} else {
			if (m_list->text(i,COL_NUMLPEDIDO)!="") {
				updateOrderLine(i);
			} else {
				insertOrderLine(i);
			}
		}
		i ++;
   }
} // end saveOrderLines


void linorderslist::orderDateLostFocus() {
   fprintf(stderr, "orderDate Lost Focus");
   m_fechapedido->setText(normalizafecha(m_fechapedido->text()).toString("dd/MM/yyyy"));
}// end neworder


void linorderslist::updateOrderLine(int i) {
	QString SQLQuery = "UPDATE lpedido SET desclpedido='"+m_list->text(i,COL_DESCLPEDIDO)+"'";
	SQLQuery += " , cantlpedido="+ m_list->text(i,COL_CANTLPEDIDO);
	SQLQuery += " , pvdlpedido="+m_list->text(i,COL_PVDLPEDIDO);
	SQLQuery += " , prevlpedido='"+m_list->text(i,COL_PREVLPEDIDO)+"'";
	SQLQuery += " , idarticulo="+m_list->text(i,COL_IDARTICULO);
	if (m_list->text(i,COL_IDALB_PRO) != "") {
		SQLQuery += " , idalb_pro="+m_list->text(i,COL_IDALB_PRO);
	}
	SQLQuery += " WHERE idpedido ="+idpedido+" AND numlpedido="+m_list->text(i,COL_NUMLPEDIDO);
	companyact->begin();
	companyact->ejecuta(SQLQuery);
	companyact->commit();
} //end updateOrderLine


void linorderslist::insertOrderLine(int i) {
	QString SQLQuery ="";
	if (m_list->text(i,COL_IDALB_PRO)!="") {
		SQLQuery = "INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo, idalb_pro)";
	} else {
		SQLQuery = "INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo)";
	}
	SQLQuery += " VALUES (";
	SQLQuery += "'"+m_list->text(i,COL_DESCLPEDIDO)+"'";
	SQLQuery += " , "+m_list->text(i,COL_CANTLPEDIDO);
	SQLQuery += " , "+m_list->text(i,COL_PVDLPEDIDO);
	SQLQuery += " , '"+m_list->text(i,COL_PREVLPEDIDO)+"'";
	SQLQuery += " , "+idpedido;
	SQLQuery += " , "+m_list->text(i,COL_IDARTICULO);
	if (m_list->text(i,COL_IDALB_PRO) != "") {
		SQLQuery += " , "+m_list->text(i,COL_IDALB_PRO);
	}
	SQLQuery += " ) ";
	companyact->begin();
	companyact->ejecuta(SQLQuery);
	companyact->commit();
} //end insertOrderLine


void linorderslist::deleteOrderLine(int line) {
	QString SQLQuery = "DELETE FROM lpedido WHERE numlpedido ="+m_list->text(line,COL_NUMLPEDIDO);
	companyact->begin();
	if (companyact->ejecuta(SQLQuery)==0) {
		m_list->removeRow(m_list->currentRow());
	}
	companyact->commit();
} //end deleteOrderLine


void linorderslist::valueOrderLineChanged(int row, int col) {
	if (m_list->text(row, col) == "*" && row>0) {
		m_list->setText(row, col, m_list->text(row-1, col));
	}
	
	switch (col) {
		case COL_PREVLPEDIDO: {
			m_list->setText(row, COL_PREVLPEDIDO,normalizafecha(m_list->text(row, col)).toString("dd/MM/yyyy"));
		}
		case COL_CODARTICULO: {
			manageArticle(row);
		}
	}
} //end valueOrderLineChanged


void linorderslist::manageArticle(int row) {
	QString articleCode = m_list->text(row, COL_CODARTICULO);
	if (articleCode == "+") {
		m_idArticle = "";
		searchArticle();
		m_list->setText(row, COL_CODARTICULO, m_idArticle);
		articleCode = m_idArticle;
	}
	
	bool ok;
	if (articleCode.toInt(&ok, 10)>0) {
		companyact->begin();
		cursor2 * cur2= companyact->cargacursor("SELECT * FROM articulo WHERE codarticulo="+m_list->text(row, COL_CODARTICULO),"unquery");
		companyact->commit();
		if (!cur2->eof()) {
			m_list->setText(row, COL_NOMARTICULO, cur2->valor("nomarticulo"));
			m_list->setText(row, COL_IDARTICULO, cur2->valor("idarticulo"));
		} else {
			m_list->setText(row, COL_NOMARTICULO, cur2->valor(""));
			m_list->setText(row, COL_IDARTICULO, cur2->valor(""));
		}
	}
} //end manageArticle


void linorderslist::searchArticle() {
   fprintf(stderr,"Busqueda de un artículo\n");
   articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Artículo","company"));
   
// , WType_Dialog| WShowModal   
   artlist->modoseleccion();
   
   // Esto es convertir un QWidget en un sistema modal de dialogo.
   this->setEnabled(false);
   artlist->show();
   while(!artlist->isHidden()) theApp->processEvents();
   this->setEnabled(true);
   
   m_idArticle = artlist->idArticle();
	
   delete artlist;
}// end searchProvider


void linorderslist::removeOrderLin() {
	
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
	
}// end removeOrderLin
