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
 
 
#include "budgetslist.h"
#include <qtable.h>
#include "company.h"
#include "provedit.h"

#define COL_IDPROVEEDOR 0
#define COL_NOMPROVEEDOR 1
#define COL_NOMALTPROVEEDOR 2
#define COL_CIFPROVEEDOR 3
#define COL_CODICLIPROVEEDOR 4
#define COL_CBANCPROVEEDOR 5
#define COL_COMENTPROVEEDOR 6
#define COL_DIRPROVEEDOR 7
#define COL_POBLPROVEEDOR 8
#define COL_CPPROVEEDOR 9
#define COL_TELPROVEEDOR 10
#define COL_FAXPROVEEDOR 11
#define COL_EMAILPROVEEDOR 12
#define COL_URLPROVEEDOR 13
#define COL_CLAVEWEBPROVEEDOR 14



BudgetsList::BudgetsList(company *comp, QWidget *parent, const char *name, int flag)
 : BudgetsListBase(parent, name, flag) {
      companyact = comp;
      inicializa();
      m_modo=0;
      
      m_idprovider="";
      m_cifprovider="";
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
   m_list->setNumCols(15);
   m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr( "Código" ) );
   m_list->horizontalHeader()->setLabel( COL_NOMPROVEEDOR, tr( "Nombre Fiscal" ) );
   m_list->horizontalHeader()->setLabel( COL_NOMALTPROVEEDOR, tr( "Nombre Comercial" ) );
   m_list->horizontalHeader()->setLabel( COL_CIFPROVEEDOR, tr( "CIF/NIF" ) );
   m_list->horizontalHeader()->setLabel( COL_CODICLIPROVEEDOR, tr( "Código de cliente" ) );
   m_list->horizontalHeader()->setLabel( COL_CBANCPROVEEDOR, tr( "Cuenta Bancaria" ) );
   m_list->horizontalHeader()->setLabel( COL_COMENTPROVEEDOR, tr("Observaciones") );
   m_list->horizontalHeader()->setLabel( COL_DIRPROVEEDOR, tr("Domicilio") );
   m_list->horizontalHeader()->setLabel( COL_POBLPROVEEDOR, tr("Población") );
   m_list->horizontalHeader()->setLabel( COL_CPPROVEEDOR, tr("C.P.") );
   m_list->horizontalHeader()->setLabel( COL_TELPROVEEDOR, tr("Nº Teléfono") );
   m_list->horizontalHeader()->setLabel( COL_FAXPROVEEDOR, tr("Nº Fax") );
   m_list->horizontalHeader()->setLabel( COL_EMAILPROVEEDOR, tr("Correo Electrónico") );
   m_list->horizontalHeader()->setLabel( COL_URLPROVEEDOR, tr("Página Web") );
   m_list->horizontalHeader()->setLabel( COL_CLAVEWEBPROVEEDOR, tr("Clave propia web proveedor") );
   m_list->setColumnWidth(COL_IDPROVEEDOR,75);
   m_list->setColumnWidth(COL_NOMPROVEEDOR,300);
   m_list->setColumnWidth(COL_NOMALTPROVEEDOR,300);
   m_list->setColumnWidth(COL_CIFPROVEEDOR,75);
   m_list->setColumnWidth(COL_CODICLIPROVEEDOR,75);
   m_list->setColumnWidth(COL_CBANCPROVEEDOR,100);
   m_list->setColumnWidth(COL_COMENTPROVEEDOR,1000);
   m_list->setColumnWidth(COL_DIRPROVEEDOR,300);
   m_list->setColumnWidth(COL_POBLPROVEEDOR,200);
   m_list->setColumnWidth(COL_CPPROVEEDOR,75);
   m_list->setColumnWidth(COL_TELPROVEEDOR,75);
   m_list->setColumnWidth(COL_FAXPROVEEDOR,100);
   m_list->setColumnWidth(COL_EMAILPROVEEDOR,300);
   m_list->setColumnWidth(COL_URLPROVEEDOR,300);
   m_list->setColumnWidth(COL_CLAVEWEBPROVEEDOR,300);
         
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#EEFFFF");   
    m_list->setReadOnly(TRUE);        
    companyact->begin();
    cursor2 * cur= companyact->cargacursor("SELECT * FROM proveedor","unquery12");
    companyact->commit();
    m_list->setNumRows( cur->numregistros() );
    int i=0;
    while (!cur->eof()) {
         m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
         m_list->setText(i,COL_NOMPROVEEDOR,cur->valor("nomproveedor"));
         m_list->setText(i,COL_NOMALTPROVEEDOR,cur->valor("nomaltproveedor"));
         m_list->setText(i,COL_CIFPROVEEDOR,cur->valor("cifproveedor"));
         m_list->setText(i,COL_CODICLIPROVEEDOR,cur->valor("codicliproveedor"));
         m_list->setText(i,COL_CBANCPROVEEDOR,cur->valor("cbancproveedor"));
         m_list->setText(i,COL_COMENTPROVEEDOR,cur->valor("comentproveedor"));
         m_list->setText(i,COL_DIRPROVEEDOR,cur->valor("dirproveedor"));
         m_list->setText(i,COL_POBLPROVEEDOR,cur->valor("poblproveedor"));
	 m_list->setText(i,COL_CPPROVEEDOR,cur->valor("cpproveedor"));
         m_list->setText(i,COL_TELPROVEEDOR,cur->valor("telproveedor"));
         m_list->setText(i,COL_FAXPROVEEDOR,cur->valor("faxproveedor"));
         m_list->setText(i,COL_EMAILPROVEEDOR,cur->valor("emailproveedor"));
         m_list->setText(i,COL_URLPROVEEDOR,cur->valor("urlproveedor"));
         m_list->setText(i,COL_CLAVEWEBPROVEEDOR,cur->valor("clavewebproveedor"));
         i++;
         cur->siguienteregistro();
    }// end while
      delete cur;
}// end inicializa



void BudgetsList::doubleclicked(int a, int b, int c, const QPoint &) {
   m_idprovider = m_list->text(a,COL_IDPROVEEDOR);
   m_cifprovider = m_list->text(a,COL_CIFPROVEEDOR);
   if (m_modo ==0 ) {
      QString idprov = m_list->text(a, COL_IDPROVEEDOR);
      fprintf(stderr, "parm a: %d  parm b: %d  parm c %d \n", a, b, c);
      provedit *prov = new provedit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Proveedores", "company"));
      prov->chargeprovider(idprov);
      prov->show();
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


void BudgetsList::newprovider() {
   fprintf(stderr,"Iniciamos el boton_crear\n");
   provedit *prov = new provedit(companyact,companyact->m_pWorkspace,theApp->translate("Edicion de Proveedores", "company"));
   prov->show();
}// end boton_crear

/*
void providerslist::boton_editar() {}
void providerslist::boton_duplicar() {}
void providerslist::boton_borrar() {}
void providerslist::boton_imprimir() {}
void providerslist::boton_filtrar() {}
*/

