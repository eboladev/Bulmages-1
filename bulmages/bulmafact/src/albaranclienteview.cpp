/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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


#include "albaranclienteview.h"


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
#include "postgresiface2.h"
#include "listlinalbaranclienteview.h"

AlbaranClienteView::AlbaranClienteView(company *comp, QWidget *parent, const char *name) : AlbaranClienteBase(parent, name, Qt::WDestructiveClose), AlbaranCliente(comp) {
    subform2->setcompany(comp);
    setListLinAlbaranCliente(subform2);
    m_cursorcombo=NULL;
    inicialize();
    comp->meteWindow(caption(),this);
    fprintf(stderr,"Fin de la inicialización de AlbaranClienteView\n");
}// end ClientDelivNote

AlbaranClienteView::~AlbaranClienteView() {
	companyact->refreshClientDelivNotes();
	companyact->sacaWindow(this);
}// end ~ClientDelivNote




void AlbaranClienteView::inicialize() {
	m_totalBases->setReadOnly(TRUE);
	m_totalBases->setAlignment(Qt::AlignRight);
	m_totalTaxes->setReadOnly(TRUE);
	m_totalTaxes->setAlignment(Qt::AlignRight);
	m_totalDiscounts->setReadOnly(TRUE);
	m_totalDiscounts->setAlignment(Qt::AlignRight);
	m_totalalbaran->setReadOnly(TRUE);
	m_totalalbaran->setAlignment(Qt::AlignRight);
	
	if (mdb_idalbaran=="0") {	 
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
		m_numalbaran->setReadOnly(true);
		if (mdb_idalbaran!="0") {
		  	m_codigoalmacen->setReadOnly(true);
		}
	}
}// end inicialize


void AlbaranClienteView::pintaIdForma_Pago(QString idformapago) {
    fprintf(stderr,"pintaFormaPago(%s)\n",idformapago.ascii());
    // Tratamos la forma de pago.
    m_comboformapago->clear();
    if (m_cursorcombo != NULL)
        delete m_cursorcombo;
    m_cursorcombo = companyact->cargacursor("SELECT * FROM forma_pago");
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

void   AlbaranClienteView::pintatotales(float base, float iva) {
    m_totalBases->setText(QString::number(base));
    m_totalTaxes->setText(QString::number(iva));
    m_totalalbaran->setText(QString::number(iva+base));
}// end pintatotales


// Bsqueda de Clientes.
void AlbaranClienteView::s_searchCliente() {
    fprintf(stderr,"Busqueda de un client\n");
    ClientsList *clients = new ClientsList(companyact, NULL, theApp->translate("Seleccione cliente","company"));
    clients->selectMode();
    // Esto es convertir un QWidget en un sistema modal de dialogo.
    this->setEnabled(false);
    clients->show();
    while(!clients->isHidden())
        theApp->processEvents();
    this->setEnabled(true);
    if (clients->cifclient() !="" && clients->cifclient() !=NULL) {
        m_cifcliente->setText(clients->cifclient());
    }// end if
    delete clients;
}// end searchClient


/*
// Esta función carga un albarán.
void AlbaranClienteView::chargeClientDelivNote(QString iddeliveryNote) {
	m_idalbaran = iddeliveryNote;
	inicialize();
	
	QString query = "SELECT * FROM albaran LEFT JOIN cliente ON cliente.idcliente = albaran.idcliente WHERE idalbaran="+iddeliveryNote;
	companyact->begin();
	cursor2 * cur= companyact->cargacursor(query, "queryalbaran");
	companyact->commit();
	if (!cur->eof()) {
		m_idclient = cur->valor("idcliente");	
		m_numalbaran->setText(cur->valor("numalbaran"));
		m_fechaalbaran->setText(cur->valor("fechaalbaran"));
		m_comentalbaran->setText(cur->valor("comentalbaran"));
		m_nomclient->setText(cur->valor("nomcliente"));
		m_cifclient->setText(cur->valor("cifcliente"));
   
		chargeClientDelivNoteLines(iddeliveryNote);
		chargeClientDelivNoteDiscounts(iddeliveryNote);
		calculateImports();
		m_comboformapago->clear();
		cargarcomboformapago(cur->valor("idforma_pago"));
	} else {
		cargarcomboformapago("0");
   }// end if
	delete cur;
	  
	m_initialValues = calculateValues();
}// end chargeClientDelivNote


void AlbaranClienteView::cargarcomboformapago(QString idformapago) {
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

// Carga líneas de albarán
void AlbaranClienteView::chargeClientDelivNoteLines(QString iddeliveryNote) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lalbaran, articulo WHERE idalbaran="+iddeliveryNote+" AND articulo.idarticulo=lalbaran.idarticulo","unquery");
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
		
		cursor2 * cur2= companyact->cargacursor("SELECT porcentasa_iva FROM tasa_iva WHERE idtipo_iva="+cur->valor("idtipo_iva")+" AND fechatasa_iva<='"+m_fechaalbaran->text()+"' ORDER BY fechatasa_iva DESC","unquery2");
		companyact->commit();
		if (!cur2->eof()) {
			m_list->setText(i,COL_TASATIPO_IVA,cur2->valor("porcentasa_iva"));
		}
		
		i++;
		cur->siguienteregistro();
	}// end while
	if (i>0) m_list->setNumRows(i);
	
	delete cur;
}// end chargeClientDelivNoteLines


// Carga líneas descuentos albarán
void AlbaranClienteView::chargeClientDelivNoteDiscounts(QString iddeliveryNote) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM dalbaran WHERE idalbaran="+iddeliveryNote,"unquery");
	companyact->commit();
	int i=0;
	while (!cur->eof()) {
		m_listDiscounts->setText(i,COL_DESCUENTO_NUMDALBARAN,cur->valor("numdalbaran"));
		m_listDiscounts->setText(i,COL_DESCUENTO_CONCEPTDALBARAN,cur->valor("conceptdalbaran"));
		m_listDiscounts->setText(i,COL_DESCUENTO_PROPORCIONDALBARAN,QString().sprintf("%0.2f",cur->valor("propordalbaran").toFloat()));
		i++;
		cur->siguienteregistro();
	}// end while
	if (i>0) m_listDiscounts->setNumRows(i);
	
	delete cur;
}// end chargeClientDelivNoteDiscounts


// Búsqueda de Clientes.
void AlbaranClienteView::s_searchClient() {
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
		qDebug("Identificador de cliente = %s",m_idclient.ascii());
   	m_cifclient->setText(clients->cifclient());
   	m_nomclient->setText(clients->nomclient());
	}
   delete clients;
}// end searchClient


void AlbaranClienteView::s_clientDelivNoteDateLostFocus() {
	m_fechaalbaran->setText(normalizafecha(m_fechaalbaran->text()).toString("dd/MM/yyyy"));
}



void AlbaranClienteView::s_newClientDelivNoteLine() {
	m_list->setNumRows( m_list->numRows()+1 );
	m_list->setCurrentCell(m_list->numRows()-1, COL_CODARTICULO);	
	m_list->editCell(m_list->numRows()-1, COL_CODARTICULO);
}


void AlbaranClienteView::s_removeClientDelivNote() {
	fprintf(stderr,"Iniciamos el boton_borrar\n");
	if (QMessageBox::warning( this, "BulmaFact - Presupuestos", "Desea borrar este albarán", "Sí", "No") == 0) {
		companyact->begin();
		QString SQLQuery = "DELETE FROM lalbaran WHERE idalbaran ="+m_idalbaran;
		if (companyact->ejecuta(SQLQuery)==0){
			QString SQLQuery = "DELETE FROM dalbaran WHERE idalbaran ="+m_idalbaran;
				if (companyact->ejecuta(SQLQuery)==0){
					QString SQLQuery = "DELETE FROM albaran WHERE idalbaran ="+m_idalbaran;
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
	}	
}// end boton_borrar


void AlbaranClienteView::s_removeClientDelivNoteLine() {
	if (m_list->currentRow() >= 0) {
		int row = m_list->currentRow();
		m_list->setText(row, COL_REMOVE, "S");
		m_list->hideRow(row);
	}
}


void AlbaranClienteView::s_valueClientDelivNoteLineChanged(int row, int col) {
	
	switch (col) {
		case COL_DESCUENTOLALBARAN: {
			m_list->setText(row, COL_DESCUENTOLALBARAN, m_list->text(row, COL_DESCUENTOLALBARAN).replace(",","."));
			float discountLine = m_list->text(row, COL_DESCUENTOLALBARAN).toFloat();
			m_list->setText(row, COL_DESCUENTOLALBARAN, QString().sprintf("%0.2f", discountLine));
			calculateImports();
			break;
		}
		case COL_CODARTICULO: {
			manageArticle(row);
			calculateImports();
			break;
		}
		case COL_CANTLALBARAN: {
			m_list->setText(row, COL_CANTLALBARAN, m_list->text(row, COL_CANTLALBARAN).replace(",","."));
			float cantLine = m_list->text(row, COL_CANTLALBARAN).toFloat();
			qDebug("Cantidad albaran = %s",m_list->text(row, COL_CANTLALBARAN).ascii());
			m_list->setText(row, COL_CANTLALBARAN, QString().sprintf("%0.3f", cantLine));
			calculateImports();
			break;
		}
		case COL_PVPLALBARAN: {
			m_list->setText(row, COL_PVPLALBARAN, m_list->text(row, COL_PVPLALBARAN).replace(",","."));
			float pvpLine = m_list->text(row, COL_PVPLALBARAN).toFloat();
			m_list->setText(row, COL_PVPLALBARAN, QString().sprintf("%0.2f", pvpLine));
			calculateImports();
			break;
		}
	}
} //end valueClientDelivNoteLineChanged


void AlbaranClienteView::s_newClientDelivNoteDiscountLine() {
	m_listDiscounts->setNumRows( m_listDiscounts->numRows()+1 );
	m_listDiscounts->editCell(m_listDiscounts->numRows()-1, COL_DESCUENTO_CONCEPTDALBARAN);
	
}


void AlbaranClienteView::s_removeClientDelivNoteDiscountLine() {
	if (m_listDiscounts->currentRow() >= 0) {
		int row = m_listDiscounts->currentRow();
		m_listDiscounts->setText(row, COL_DESCUENTO_REMOVE, "S");
		m_listDiscounts->hideRow(row);
	}
}


void AlbaranClienteView::s_valueClientDelivNoteDiscountLineChanged(int row, int col) {
	switch (col) {
		case COL_DESCUENTO_PROPORCIONDALBARAN: {
			m_listDiscounts->setText(row, COL_DESCUENTO_PROPORCIONDALBARAN, m_listDiscounts->text(row, COL_DESCUENTO_PROPORCIONDALBARAN).replace(",","."));
			float proporcionLine = m_listDiscounts->text(row, COL_DESCUENTO_PROPORCIONDALBARAN).toFloat();
			m_listDiscounts->setText(row, COL_DESCUENTO_PROPORCIONDALBARAN, QString().sprintf("%0.2f", proporcionLine));
			calculateImports();
			break;
		}
	}
} //end valueClientDelivNoteDiscountLineChanged

void AlbaranClienteView::manageArticle(int row) {
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
			cursor2 * cur3= companyact->cargacursor("SELECT idtipo_iva, porcentasa_iva FROM tasa_iva WHERE idtipo_iva="+taxType+" AND fechatasa_iva<='"+m_fechaalbaran->text()+"' ORDER BY fechatasa_iva DESC","unquery");
			companyact->commit();
			if (!cur3->eof()) {
				m_list->setText(row, COL_TASATIPO_IVA, cur3->valor("porcentasa_iva"));
			}
			delete cur3;
		}
		delete cur2;
	}
} //end manageArticle


QString AlbaranClienteView::searchArticle() {
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


void AlbaranClienteView::s_contextMenu(int, int, int button, const QPoint &poin) {
	qDebug("button = %d", button);
	if (button == 2) {
		QPopupMenu *popup;
		popup = new QPopupMenu;
		popup->insertItem(tr("Eliminar"),101);
		//popup->insertSeparator();
		int opcion = popup->exec(m_list->mapToGlobal(poin));
		switch(opcion) {
			case 101:
				s_removeClientDelivNoteLine();
				break;
		}// end switch
		delete popup;
	}
}// end contextmenu


void AlbaranClienteView::s_contextMenuDiscount(int, int, int button, const QPoint &poin) {
	qDebug("button = %d", button);
	if (button == 2) {
		QPopupMenu *popup;
		popup = new QPopupMenu;
		popup->insertItem(tr("Eliminar"),101);
		//popup->insertSeparator();
		int opcion = popup->exec(m_listDiscounts->mapToGlobal(poin));
		switch(opcion) {
			case 101:
				s_removeClientDelivNoteDiscountLine();
				break;
		}// end switch
		delete popup;
	}
}// end contextmenu


void AlbaranClienteView::s_saveClientDelivNote() {
	companyact->begin();
	if (saveClientDelivNote()==0) {
		if (m_idalbaran == "0") {
			cursor2 * cur4= companyact->cargacursor("SELECT max(idalbaran) FROM albaran","unquery1");
			if (!cur4->eof()) {
				m_idalbaran=cur4->valor(0);
			}
			delete cur4;
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


void AlbaranClienteView::s_accept() {
	fprintf(stderr,"accept button activated\n");
	companyact->begin();
	if (saveClientDelivNote()==0) {
		if (m_idalbaran == "0") {
			cursor2 * cur4= companyact->cargacursor("SELECT max(idalbaran) FROM albaran","unquery1");
			if (!cur4->eof()) {
				m_idalbaran=cur4->valor(0);
			}
			delete cur4;
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


int AlbaranClienteView::saveClientDelivNote() {
	buscarAlmacen();
	
	if (m_idalmacen == "")  return 1;

	
	if (m_numalbaran->text() == "") {
		m_numalbaran->setText(newDelivNoteNumber());
	}

	QString SQLQuery;
	
	if (m_idalbaran != "0") {
		SQLQuery = "UPDATE albaran SET numalbaran="+m_numalbaran->text();
		SQLQuery += " , fechaalbaran='"+ m_fechaalbaran->text()+"'";
		SQLQuery += " , comentalbaran='"+m_comentalbaran->text()+"'";
		SQLQuery += " , idcliente="+m_idclient;
		SQLQuery += " , idforma_pago="+m_cursorcombo->valor("idforma_pago",m_comboformapago->currentItem());
		SQLQuery += " , idalmacen="+m_idalmacen;
		SQLQuery += " WHERE idalbaran ="+m_idalbaran;
	} else {
		SQLQuery = "INSERT INTO albaran (idalmacen, numalbaran, fechaalbaran, comentalbaran, idcliente, idforma_pago)";
		SQLQuery += " VALUES (";
		SQLQuery += m_idalmacen;
		SQLQuery +=" , "+ m_numalbaran->text();
		SQLQuery += " , '"+m_fechaalbaran->text()+"'";
		SQLQuery += " , '"+m_comentalbaran->text()+"'";
		SQLQuery += " , "+m_idclient;
		SQLQuery += " , "+m_cursorcombo->valor("idforma_pago",m_comboformapago->currentItem());
		SQLQuery += " ) ";
	}
	return companyact->ejecuta(SQLQuery);
} //end saveClientDelivNote


int AlbaranClienteView::saveClientDelivNoteLines() {
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


int AlbaranClienteView::saveClientDelivNoteDiscountLines() {
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


int AlbaranClienteView::updateClientDelivNoteLine(int i) {
	QString SQLQuery = "UPDATE lalbaran SET desclalbaran='"+m_list->text(i,COL_DESCLALBARAN)+"'";
	SQLQuery += " , cantlalbaran="+ m_list->text(i,COL_CANTLALBARAN);
	SQLQuery += " , pvplalbaran="+m_list->text(i,COL_PVPLALBARAN);
	SQLQuery += " , descontlalbaran="+m_list->text(i,COL_DESCUENTOLALBARAN);
	SQLQuery += " , idarticulo="+m_list->text(i,COL_IDARTICULO);
	SQLQuery += " WHERE idalbaran ="+m_idalbaran+" AND numlalbaran="+m_list->text(i,COL_NUMLALBARAN);
	return companyact->ejecuta(SQLQuery);
} //end updateClientDelivNoteLine


int AlbaranClienteView::insertClientDelivNoteLine(int i) {
	QString SQLQuery ="";
	SQLQuery = "INSERT INTO lalbaran (desclalbaran, cantlalbaran, pvplalbaran, descontlalbaran, idalbaran, idarticulo)";
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


int AlbaranClienteView::deleteClientDelivNoteLine(int line) {
	QString SQLQuery = "DELETE FROM lalbaran WHERE numlalbaran ="+m_list->text(line,COL_NUMLALBARAN)+" and idalbaran="+m_idalbaran;
	return companyact->ejecuta(SQLQuery);
} //end deleteClientDelivNoteLine


int AlbaranClienteView::updateClientDelivNoteDiscountLine(int i) {
	QString SQLQuery = "UPDATE dalbaran SET conceptdalbaran='"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDALBARAN)+"'";
	SQLQuery += " , propordalbaran="+ m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDALBARAN);
	SQLQuery += " WHERE idalbaran ="+m_idalbaran+" AND numdalbaran="+m_listDiscounts->text(i,COL_DESCUENTO_NUMDALBARAN);
	return companyact->ejecuta(SQLQuery);
} //end updateClientDelivNoteDiscountLine


int AlbaranClienteView::insertClientDelivNoteDiscountLine(int i) {
	QString SQLQuery ="";
	SQLQuery = "INSERT INTO dalbaran (conceptdalbaran, propordalbaran, idalbaran)";
	SQLQuery += " VALUES (";
	SQLQuery += "'"+m_listDiscounts->text(i,COL_DESCUENTO_CONCEPTDALBARAN)+"'";
	SQLQuery += " , "+m_listDiscounts->text(i,COL_DESCUENTO_PROPORCIONDALBARAN);
	SQLQuery += " , "+m_idalbaran;
	SQLQuery += " ) ";
	return companyact->ejecuta(SQLQuery);
} //end insertClientDelivNoteDiscountLine


int AlbaranClienteView::deleteClientDelivNoteDiscountLine(int line) {
	QString SQLQuery = "DELETE FROM dalbaran WHERE numdalbaran ="+m_listDiscounts->text(line,COL_DESCUENTO_NUMDALBARAN)+" and idalbaran="+m_idalbaran;
	return companyact->ejecuta(SQLQuery);
} //end deleteClientDelivNoteDiscountLine

void AlbaranClienteView::s_cancel() {
	close();
}//end cancel


void AlbaranClienteView::calculateImports() {
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


QString AlbaranClienteView::calculateValues() {
	QString values = retrieveValues("QTable");
	values += retrieveValues("QLineEdit");
	values += retrieveValues("QTextEdit");
	return values;
}


bool AlbaranClienteView::eventFilter( QObject *obj, QEvent *ev ) {
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
					if (QString(obj->name()).stripWhiteSpace() == "m_listDiscounts") {
						s_valueClientDelivNoteDiscountLineChanged(t->currentRow(), t->currentColumn());
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


void AlbaranClienteView::nextCell(QObject *obj) {
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


void AlbaranClienteView::antCell(QObject *obj) {
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


void AlbaranClienteView::duplicateCell(QObject *obj) {
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


QString AlbaranClienteView::retrieveValues(QString qsWidget) {
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



void AlbaranClienteView::s_almacenLostFocus() {
	buscarAlmacen();
}//end s_almacenLostFocus


void AlbaranClienteView::buscarAlmacen() {
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


QString AlbaranClienteView::newDelivNoteNumber() {
	QString rtnNumber;
	companyact->begin();
	cursor2 * cur4= companyact->cargacursor("SELECT max(numalbaran) FROM albaran WHERE idalmacen="+m_idalmacen,"unquery2");
//	companyact->commit();
	if (!cur4->eof()) {
		rtnNumber = QString().sprintf("%d",cur4->valor(0).toInt()+1);
	} else {
		rtnNumber = "1";
	}
	delete cur4;
	return rtnNumber;
} // end newDelivNoteNumber
*/
