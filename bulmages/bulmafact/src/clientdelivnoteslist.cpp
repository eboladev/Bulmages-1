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
 
 // Implementación del listado de albaranes.
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

--   idpresupuesto integer REFERENCES presupuesto(idpresupuesto),
   idcliente integer REFERENCES cliente(idcliente),
   idforma_pago integer REFERENCES forma_pago(idforma_pago),
   numfactura integer REFERENCES factura(numfactura),
   numnofactura integer REFERENCES nofactura(numnofactura)
);

*/
#include "clientdelivnoteslist.h"
#include "company.h"
#include "clientdelivnote.h"
#include <qtable.h>
#include <qmessagebox.h>

#define COL_NUMALBARAN 0
#define COL_FECHAALBARAN 1
#define COL_NOMCLIENTE 2
#define COL_IDFORMA_PAGO 3
#define COL_DESCFORMA_PAGO 4
#define COL_NUMFACTURA 5
#define COL_NUMNOFACTURA 6
#define COL_IDUSUARIO 7
#define COL_IDCLIENTE 8
#define COL_IDALBARAN 9
#define COL_COMENTALBARAN 10

ClientDelivNotesList::ClientDelivNotesList(company *comp, QWidget *parent, const char *name, int flag)
 : ClientDelivNotesListBase(parent, name, flag) {
      companyact = comp;
      inicializa();
      m_modo=0;
      m_idclidelivnote="";
}// end providerslist

ClientDelivNotesList::~ClientDelivNotesList() {
}// end ~providerslist

void ClientDelivNotesList::inicializa() {
   m_list->setNumRows( 0 );
   m_list->setNumCols( 0 );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setSorting( TRUE );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setColumnMovingEnabled( TRUE );
   m_list->setNumCols(11);
	m_list->horizontalHeader()->setLabel( COL_NOMCLIENTE, tr( "Cliente" ) );
   m_list->horizontalHeader()->setLabel( COL_NUMALBARAN, tr( "Nº Presupuesto" ) );
   m_list->horizontalHeader()->setLabel( COL_FECHAALBARAN, tr( "Fecha" ) );
   m_list->horizontalHeader()->setLabel( COL_IDFORMA_PAGO, tr( "COL_IDFORMA_PAGO" ) );
   m_list->horizontalHeader()->setLabel( COL_NUMFACTURA, tr( "Nº Factura" ) );
   m_list->horizontalHeader()->setLabel( COL_NUMNOFACTURA, tr( "Nº No Fra." ) );
   m_list->horizontalHeader()->setLabel( COL_IDUSUARIO, tr("COL_IDUSUARIO") );
   m_list->horizontalHeader()->setLabel( COL_IDCLIENTE, tr("COL_IDCLIENTE") );
	m_list->horizontalHeader()->setLabel( COL_IDALBARAN, tr("COL_IDALBARAN") );
	m_list->horizontalHeader()->setLabel( COL_COMENTALBARAN, tr("Comentario") );
	m_list->horizontalHeader()->setLabel( COL_DESCFORMA_PAGO, tr("Forma de Pago") );
   m_list->setColumnWidth(COL_NUMALBARAN,75);
   m_list->setColumnWidth(COL_FECHAALBARAN,100);
   m_list->setColumnWidth(COL_IDFORMA_PAGO,75);
   m_list->setColumnWidth(COL_NUMFACTURA,75);
   m_list->setColumnWidth(COL_NUMNOFACTURA,75);
   m_list->setColumnWidth(COL_IDUSUARIO,75);
   m_list->setColumnWidth(COL_IDCLIENTE,75);
	m_list->setColumnWidth(COL_NOMCLIENTE,200);
	m_list->setColumnWidth(COL_IDALBARAN,75);
	m_list->setColumnWidth(COL_COMENTALBARAN,200);
	m_list->setColumnWidth(COL_DESCFORMA_PAGO,200);
	
	m_list->hideColumn(COL_IDCLIENTE);
	m_list->hideColumn(COL_IDALBARAN);
	m_list->hideColumn(COL_IDFORMA_PAGO);
	//m_list->hideColumn(COL_IDUSUARI);
         
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");   
    m_list->setReadOnly(TRUE);        
    companyact->begin();
    cursor2 * cur= companyact->cargacursor("SELECT * FROM albaran, cliente where albaran.idcliente=cliente.idcliente","queryalbaran");
    companyact->commit();
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
		m_list->setText(i,COL_NUMALBARAN,cur->valor("numalbaran"));
		m_list->setText(i,COL_FECHAALBARAN,cur->valor("fechaalbaran"));
		m_list->setText(i,COL_IDFORMA_PAGO,cur->valor("idforma_pago"));
		m_list->setText(i,COL_NUMFACTURA,cur->valor("numfactura"));
		m_list->setText(i,COL_NUMNOFACTURA,cur->valor("numnofactura"));
		m_list->setText(i,COL_IDUSUARIO,cur->valor("idusuario"));
		m_list->setText(i,COL_IDCLIENTE,cur->valor("idcliente"));
		m_list->setText(i,COL_NOMCLIENTE,cur->valor("nomcliente"));
		m_list->setText(i,COL_IDALBARAN,cur->valor("idalbaran"));
		m_list->setText(i,COL_COMENTALBARAN,cur->valor("comentalbaran"));
		
		companyact->begin();
		cursor2 * cur2= companyact->cargacursor("SELECT * FROM forma_pago where idforma_pago="+cur->valor("idforma_pago"),"qryforma_pago");
		companyact->commit();
		m_list->setText(i,COL_DESCFORMA_PAGO,cur2->valor("descforma_pago"));
		delete cur2;
		i++;
		cur->siguienteregistro();
	}// end while
	delete cur;
}// end inicializa



void ClientDelivNotesList::s_doubleclicked(int a, int , int , const QPoint &) {
	m_idclidelivnote = m_list->text(a,COL_IDALBARAN);
	if (m_modo ==0 && m_idclidelivnote != "") {
		fprintf(stderr,"Iniciamos el boton_crear\n");
		ClientDelivNote *cDelivNote = new ClientDelivNote(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Albarán de Cliente", "company"));
		cDelivNote->chargeClientDelivNote(m_idclidelivnote);
		cDelivNote->show();
	} else {
		close();
	}// end if
}


void ClientDelivNotesList::s_contextMenu(int row , int , const QPoint &) {
/*
   QString idprov = m_list->text(a, COL_IDPROVEEDOR);
   provedit *prov = new provedit(companyact,0,theApp->translate("Edicion de Proveedores", "company"));
   prov->chargeprovider(idprov);
   prov->exec();
   delete prov;
   inicializa();
*/
}// end contextMenuRequested


void ClientDelivNotesList::s_newClientDelivNote() {
	fprintf(stderr,"Iniciamos el boton_crear\n");
	ClientDelivNote *cDelivNote = new ClientDelivNote(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Albarán de Cliente", "company"));
 	cDelivNote->show();
}// end boton_crear


void ClientDelivNotesList::s_removeClientDelivNote() {
	fprintf(stderr,"Iniciamos el boton_borrar\n");
	if (m_list->currentRow() >= 0) {
		if (QMessageBox::warning( this, "BulmaFact - Albaranes", "Desea borrar el albarán seleccionado", "Sí", "No") == 0) {
			companyact->begin();
			QString SQLQuery = "DELETE FROM lalbaran WHERE idalbaran ="+m_list->text(m_list->currentRow(),COL_IDALBARAN);
			if (companyact->ejecuta(SQLQuery)==0){
				QString SQLQuery = "DELETE FROM dalbaran WHERE idalbaran ="+m_list->text(m_list->currentRow(),COL_IDALBARAN);
					if (companyact->ejecuta(SQLQuery)==0){
						QString SQLQuery = "DELETE FROM albaran WHERE idalbaran ="+m_list->text(m_list->currentRow(),COL_IDALBARAN);
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
