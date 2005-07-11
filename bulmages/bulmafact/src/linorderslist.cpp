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

// Para laura con cari�
// Esta l�ea en honor a bruli y a la asociaci� de jovenes empresarios de Valencia
 
 /*
-- Linea de pedido
-- Numero: Nmero de l�ia.
-- Descripcio: Descripcio de l'article.
-- Quantitat
-- PVD
-- Previsi� Data prevista de recepci�CREATE TABLE lpedido (
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
#include <qmessagebox.h>
#include <qdatetimeedit.h>
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
#define COL_TASATIPO_IVA 11

linorderslist::linorderslist(company *comp, QWidget *parent, const char *name, int flag)
 : linorderslistbase(parent, name, flag) {
      companyact = comp;
         companyact->meteWindow(caption(),this);

}// end linorderslist


linorderslist::~linorderslist() {
   companyact->refreshOrders();
   companyact->sacaWindow(this);
}// end ~linorderslist


void linorderslist::chargeorder(QString idpedido_) {
// Cargamos de la tabla de pedidos los datos del pedido seleccionado y los ponemos en la pantalla.
   QString idproveedor;
   QString iddivision;
   QString idalmacen;
	
	idpedido = idpedido_;
	
	m_netImport->setReadOnly(TRUE);
	m_taxImport->setReadOnly(TRUE);
	m_totalImport->setReadOnly(TRUE);
	m_netImport->setAlignment(Qt::AlignRight);
	m_taxImport->setAlignment(Qt::AlignRight);
	m_totalImport->setAlignment(Qt::AlignRight);
	m_previsionDate->setText(QDate::currentDate().toString("dd/MM/yyyy"));
	
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
} //end chargeorder

void linorderslist::chargelinorders(QString idpedido) {
// Cargamos datos generales pedido y proveedor
	chargeorder(idpedido);
	
// Cargamos la tabla con las l�eas del pedido
	m_list->setNumRows( 0 );
	m_list->setNumCols( 0 );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setSorting( TRUE );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setColumnMovingEnabled( TRUE );
	m_list->setNumCols(12);
	m_list->horizontalHeader()->setLabel( COL_NUMLPEDIDO, tr( "N L�ea" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCLPEDIDO, tr( "Descripci�" ) );
	m_list->horizontalHeader()->setLabel( COL_CANTLPEDIDO, tr( "Cantidad" ) );
	m_list->horizontalHeader()->setLabel( COL_PVDLPEDIDO, tr( "Precio" ) );
	m_list->horizontalHeader()->setLabel( COL_PREVLPEDIDO, tr( "Fecha Prevista Entrega" ) );
	m_list->horizontalHeader()->setLabel( COL_IDPEDIDO, tr( "N Pedido" ) );
	m_list->horizontalHeader()->setLabel( COL_IDALB_PRO, tr( "Albar�" ) );
	m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Art�ulo" ) );
	m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "C�igo Art�ulo" ) );
	m_list->horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripci� Art�ulo" ) );
	m_list->horizontalHeader()->setLabel( COL_TASATIPO_IVA, tr( "% IVA" ) );
   
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
	m_list->setColumnWidth(COL_TASATIPO_IVA,50);

	m_list->hideColumn(COL_NUMLPEDIDO);
	m_list->hideColumn(COL_IDPEDIDO);
	m_list->hideColumn(COL_IDARTICULO);
	m_list->hideColumn(COL_REMOVE);
	m_list->hideColumn(COL_TASATIPO_IVA);
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	m_list->setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	m_list->setPaletteBackgroundColor("#AFFAFA");   
	m_list->setReadOnly(FALSE);        
	 
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpedido, articulo, tipo_iva WHERE idpedido="+idpedido+" AND articulo.idarticulo=lpedido.idarticulo and articulo.idtipo_iva = tipo_iva.idtipo_iva","unquery");
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
		m_list->setText(i,COL_TASATIPO_IVA,cur->valor("tasatipo_iva"));
		i++;
		cur->siguienteregistro();
	}// end while
	
	delete cur;
	calculateImports();
	
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
	
	companyact->begin();
	if (saveOrder()!=0){
		companyact->rollback();
	} else {
		if (saveOrderLines()==0) {
			companyact->commit();
			close();
		} else {
			companyact->rollback();
		}
	}	
} //end accept


int linorderslist::saveOrder() {
	QString SQLQuery;
	
	if (idpedido != "0") {
		SQLQuery = "UPDATE pedido SET numpedido='"+companyact->sanearCadena(m_numpedido->text())+"'";
      SQLQuery += " , anopedido="+ companyact->sanearCadena(m_fechapedido->text().right(4));
      SQLQuery += " , fechapedido='"+companyact->sanearCadena(m_fechapedido->text())+"'";
      SQLQuery += " , descpedido='"+companyact->sanearCadena(m_descpedido->text())+"'";
      SQLQuery += " , iddivision="+companyact->sanearCadena(m_cursorcombo->valor("iddivision",m_combodivision->currentItem()));
      SQLQuery += " , idalmacen="+companyact->sanearCadena(m_cursorcombo2->valor("idalmacen",m_comboalmacen->currentItem()));
      SQLQuery += " WHERE idpedido ="+idpedido;
	} else {
		SQLQuery = "INSERT INTO pedido (numpedido, anopedido, fechapedido, descpedido, iddivision, idalmacen)";
		SQLQuery += " VALUES (";
		SQLQuery += "'"+companyact->sanearCadena(m_numpedido->text())+"'";
		SQLQuery += " , "+companyact->sanearCadena(m_fechapedido->text().right(4));
		SQLQuery += " , '"+companyact->sanearCadena(m_fechapedido->text())+"'";
		SQLQuery += " , '"+companyact->sanearCadena(m_descpedido->text())+"'";
		SQLQuery += " , "+companyact->sanearCadena(m_cursorcombo->valor("iddivision",m_combodivision->currentItem()));
		SQLQuery += " , "+companyact->sanearCadena(m_cursorcombo2->valor("idalmacen",m_comboalmacen->currentItem()));
		SQLQuery += " ) ";
	}
	return companyact->ejecuta(SQLQuery);
} //end saveOrder


int linorderslist::saveOrderLines() {
	int i = 0;
	int error = 0;
	while (i < m_list->numRows() && error==0) {
		if (m_list->text(i,COL_REMOVE)=="S") {
			if (m_list->text(i,COL_NUMLPEDIDO)!="") {
				error = deleteOrderLine(i);
			}
		} else {
			if (m_list->text(i,COL_NUMLPEDIDO)!="") {
				error = updateOrderLine(i);
			} else {
				error = insertOrderLine(i);
			}
		}
		i ++;
 	}// end while
	return error;
} // end saveOrderLines


int linorderslist::updateOrderLine(int i) {
	QString SQLQuery = "UPDATE lpedido SET desclpedido='"+companyact->sanearCadena(m_list->text(i,COL_DESCLPEDIDO))+"'";
	SQLQuery += " , cantlpedido="+ companyact->sanearCadena(m_list->text(i,COL_CANTLPEDIDO));
	SQLQuery += " , pvdlpedido="+companyact->sanearCadena(m_list->text(i,COL_PVDLPEDIDO));
	SQLQuery += " , prevlpedido='"+companyact->sanearCadena(m_list->text(i,COL_PREVLPEDIDO))+"'";
	SQLQuery += " , idarticulo="+companyact->sanearCadena(m_list->text(i,COL_IDARTICULO));
	if (m_list->text(i,COL_IDALB_PRO) != "") {
		SQLQuery += " , idalb_pro="+companyact->sanearCadena(m_list->text(i,COL_IDALB_PRO));
	}
	SQLQuery += " WHERE idpedido ="+idpedido+" AND numlpedido="+m_list->text(i,COL_NUMLPEDIDO);
	return companyact->ejecuta(SQLQuery);
} //end updateOrderLine


int linorderslist::insertOrderLine(int i) {
	QString SQLQuery ="";
	if (m_list->text(i,COL_IDALB_PRO)!="") {
		SQLQuery = "INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo, idalb_pro)";
	} else {
		SQLQuery = "INSERT INTO lpedido (desclpedido, cantlpedido, pvdlpedido, prevlpedido, idpedido, idarticulo)";
	}
	SQLQuery += " VALUES (";
	SQLQuery += "'"+companyact->sanearCadena(m_list->text(i,COL_DESCLPEDIDO))+"'";
	SQLQuery += " , "+companyact->sanearCadena(m_list->text(i,COL_CANTLPEDIDO));
	SQLQuery += " , "+companyact->sanearCadena(m_list->text(i,COL_PVDLPEDIDO));
	SQLQuery += " , '"+companyact->sanearCadena(m_list->text(i,COL_PREVLPEDIDO))+"'";
	SQLQuery += " , "+idpedido;
	SQLQuery += " , "+m_list->text(i,COL_IDARTICULO);
	if (m_list->text(i,COL_IDALB_PRO) != "") {
		SQLQuery += " , "+m_list->text(i,COL_IDALB_PRO);
	}
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertOrderLine


int linorderslist::deleteOrderLine(int line) {
	QString SQLQuery = "DELETE FROM lpedido WHERE numlpedido ="+m_list->text(line,COL_NUMLPEDIDO);
	return companyact->ejecuta(SQLQuery);
} //end deleteOrderLine


void linorderslist::neworderlin() {
	m_list->setNumRows( m_list->numRows()+1 );
	//m_list->setCurrentCell(m_list->numRows()-1, COL_CODARTICULO);
	m_list->editCell(m_list->numRows()-1, COL_CODARTICULO);
} // end neworderlin



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
   
	QString tmpProvider = provlist->idprovider();
	if (tmpProvider!="") {
   	m_idprovider = tmpProvider;
   	m_cifprovider->setText(provlist->cifprovider());
		providerChanged(m_idprovider);
		m_combodivision->clear();
		cargarcombodivision(m_idprovider, NULL);
	}
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


void linorderslist::orderDateLostFocus() {
   fprintf(stderr, "orderDate Lost Focus");
   m_fechapedido->setText(normalizafecha(m_fechapedido->text()).toString("dd/MM/yyyy"));
}// end neworder


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
			calculateImports();
		}
		case COL_CANTLPEDIDO: {
			m_list->setText(row, COL_CANTLPEDIDO, m_list->text(row, COL_CANTLPEDIDO).replace(",","."));
			calculateImports();
		}
		case COL_PVDLPEDIDO: {
			m_list->setText(row, COL_PVDLPEDIDO, m_list->text(row, COL_PVDLPEDIDO).replace(",","."));
			calculateImports();
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


void linorderslist::searchArticle() {
   fprintf(stderr,"Busqueda de un art�ulo\n");
   articleslist *artlist = new articleslist(companyact, NULL, theApp->translate("Seleccione Art�ulo","company"));
   
// , WType_Dialog| WShowModal   
   artlist->modoseleccion();
   
   // Esto es convertir un QWidget en un sistema modal de dialogo.
   this->setEnabled(false);
   artlist->show();
   while(!artlist->isHidden()) theApp->processEvents();
   this->setEnabled(true);
   
   m_idArticle = artlist->idArticle();
	
   delete artlist;
}// end searchArticle


void linorderslist::removeOrderLin() {	
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
}// end removeOrderLin


void linorderslist::cancelOrderLinChanges() {
	if (QMessageBox::warning( this, "BulmaFact - Pedidos",
    "Se perder� los cambios que haya realizado", "Aceptar", "Cancelar") == 0) {
		close();
	}
}


void linorderslist::calculateImports() {
	int i = 0;
	float netImport = 0;
	float taxImport = 0;
	while (i < m_list->numRows()) {
		if (m_list->text(i,COL_PVDLPEDIDO)!="" and m_list->text(i,COL_CANTLPEDIDO)!="") {
			netImport += m_list->text(i,COL_PVDLPEDIDO).toFloat() * m_list->text(i,COL_CANTLPEDIDO).toFloat();
			taxImport += (m_list->text(i,COL_PVDLPEDIDO).toFloat() * m_list->text(i,COL_CANTLPEDIDO).toFloat() * m_list->text(i,COL_TASATIPO_IVA).toFloat())/100;
		}
		i ++;
   }
	
	m_netImport->setText(QString().sprintf("%0.2f", netImport));
	m_taxImport->setText(QString().sprintf("%0.2f", taxImport));
	m_totalImport->setText(QString().sprintf("%0.2f", netImport+taxImport));
} // end calculateImports


void linorderslist::prevDateLostFocus() {
   fprintf(stderr, "Prevision Date Lost Focus\n");
   m_previsionDate->setText(normalizafecha(m_previsionDate->text()).toString("dd/MM/yyyy"));
}// end neworder


void linorderslist::applyPrevDate() {
	int i = 0;
	while (i < m_list->numRows()) {
		m_list->setText(i, COL_PREVLPEDIDO, m_previsionDate->text());
		i ++;
   }
} // end applyPrevDate
