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

#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qtable.h>
#include <qwidget.h>
#include "funcaux.h"

#define COL_IDLPRESUPUESTO 0
#define COL_DESCLPRESUPUESTO 1
#define COL_CANTLPRESUPUESTO 2
#define COL_PVPLPRESUPUESTO 3
#define COL_DESCUENTOLPRESUPUESTO 4
#define COL_IDPRESUPUESTO 5
#define COL_IDARTICULO 6
#define COL_CODARTICULO 7
#define COL_NOMARTICULO 8
#define COL_REMOVE 9

Budget::Budget(company *comp, QWidget *parent, const char *name) : BudgetBase(parent, name, Qt::WDestructiveClose) {
   companyact = comp;
   m_idpresupuesto = "";
   m_idclient = "";
}// end Budget

Budget::~Budget() {
}// end ~Budget


void Budget::inicialize() {
// Inicializamos la tabla de lineas de presupuesto
	m_list->setNumRows( 0 );
	m_list->setNumCols( 0 );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setSorting( TRUE );
	m_list->setSelectionMode( QTable::SingleRow );
	m_list->setColumnMovingEnabled( TRUE );
	m_list->setNumCols(9);
	m_list->horizontalHeader()->setLabel( COL_IDLPRESUPUESTO, tr( "Nº Línea" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCLPRESUPUESTO, tr( "Descripción" ) );
	m_list->horizontalHeader()->setLabel( COL_CANTLPRESUPUESTO, tr( "Cantidad" ) );
	m_list->horizontalHeader()->setLabel( COL_PVPLPRESUPUESTO, tr( "Precio" ) );
	m_list->horizontalHeader()->setLabel( COL_DESCUENTOLPRESUPUESTO, tr( "Fecha Prevista Entrega" ) );
	m_list->horizontalHeader()->setLabel( COL_IDPRESUPUESTO, tr( "Nº Pedido" ) );
	m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_CODARTICULO, tr( "Código Artículo" ) );
	m_list->horizontalHeader()->setLabel( COL_NOMARTICULO, tr( "Descripción Artículo" ) );
   
	m_list->setColumnWidth(COL_IDLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_DESCLPRESUPUESTO,300);
	m_list->setColumnWidth(COL_CANTLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_PVPLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_DESCUENTOLPRESUPUESTO,100);
	m_list->setColumnWidth(COL_IDPRESUPUESTO,100);
	m_list->setColumnWidth(COL_IDARTICULO,100);
	m_list->setColumnWidth(COL_CODARTICULO,100);
	m_list->setColumnWidth(COL_NOMARTICULO,300);

	//m_list->hideColumn(COL_NUMLPRESUPUESTO);
	//m_list->hideColumn(COL_IDPRESUPUESTO);
	//m_list->hideColumn(COL_IDARTICULO);
	m_list->hideColumn(COL_REMOVE);
	
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
	m_list->setColumnReadOnly(COL_NOMARTICULO,true);
	// Establecemos el color de fondo de la rejilla. El valor lo tiene la clase configuracion que es global.
	m_list->setPaletteBackgroundColor("#AFFAFA");   
	m_list->setReadOnly(FALSE);        
	
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
    }// end if
     delete cur;   
}// end chargeBudget


// Carga líneas de presupuesto
void Budget::chargeBudgetLines(QString idbudget) {
	companyact->begin();
	cursor2 * cur= companyact->cargacursor("SELECT * FROM lpresupuesto, articulo WHERE idpresupuesto="+idbudget+" AND articulo.idarticulo=lpresupuesto.idarticulo","unquery");
	companyact->commit();
	m_list->setNumRows( cur->numregistros() );
	int i=0;
	while (!cur->eof()) {
		m_list->setText(i,COL_IDLPRESUPUESTO,cur->valor("numlpresupuesto"));
		m_list->setText(i,COL_DESCLPRESUPUESTO,cur->valor("desclpresupuesto"));
		m_list->setText(i,COL_CANTLPRESUPUESTO,cur->valor("cantlpresupuesto"));
		m_list->setText(i,COL_PVPLPRESUPUESTO,cur->valor("pvplpresupuesto"));
		m_list->setText(i,COL_DESCUENTOLPRESUPUESTO,cur->valor("descuentolpresupuesto"));
		m_list->setText(i,COL_IDPRESUPUESTO,cur->valor("idpresupuesto"));
		m_list->setText(i,COL_IDARTICULO,cur->valor("idarticulo"));
		m_list->setText(i,COL_CODARTICULO,cur->valor("codarticulo"));
		m_list->setText(i,COL_NOMARTICULO,cur->valor("nomarticulo"));
		i++;
		cur->siguienteregistro();
	}// end while
	
	delete cur;
}


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
