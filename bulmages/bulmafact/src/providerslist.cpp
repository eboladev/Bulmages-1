/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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
 
 /*
 -- Los proveedores son los que nos suminstran articulos y/o servicios.
-- COMPROVACIONS D'INTEGRITAT>Genèriques:
-- 1 Article té 1 sol proveïdor principal.
-- 1 Article té 1 sol proveïdor referent.
-- CAMPOS
-- ======
-- Codi: Clau artificial.
-- Nom: Nom comercial o fiscal.
-- Nom_alternatiu: Nom comercial o fiscal.
-- CIF: Codi d'Identificació Fiscal.
-- CodiCli: Codi de client amb que ens facturen. Útil per a identificar-nos.
-- C_Banc
-- Comentaris
-- Adreça: Adreça.
-- Població: Població.
-- CProv: Codi de provincia (dos primers dígits del codi postal).
-- sCP: Tres darrers dígits del codi postal.
-- Telf: Telèfon.
-- Fax: Fax.
-- Email: eMail.
-- Url: Url.
-- CompteWeb: Dades de login si disposen de tenda o tarifes en línia
CREATE TABLE proveedor (
   idproveedor serial PRIMARY KEY,
   nomproveedor character varying(200),
   nomaltproveedor character varying(200),
   cifproveedor character varying(12),
   codicliproveedor character varying(30),
   cbancproveedor character varying(20),
   comentproveedor character varying(2000),
   dirproveedor character varying(50),
   poblproveedor character varying(50),
   cpproveedor character varying(9) NOT NULL,
   telproveedor character varying(12),
   faxproveedor character varying(12),
   emailproveedor character varying(100),
   urlproveedor character varying(100),
   clavewebproveedor character varying(100),
   iddivision integer NOT NULL REFERENCES division(iddivision)
);
*/


#include "providerslist.h"
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
#define COL_IDDIVISION 15



providerslist::providerslist(company *comp, QWidget *parent, const char *name, int flag)
 : providerslistbase(parent, name, flag) {
      companyact = comp;
   m_list->setNumRows( 0 );
   m_list->setNumCols( 0 );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setSorting( TRUE );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setColumnMovingEnabled( TRUE );
   m_list->setNumCols(16);
   m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr( "Cuenta" ) );
   m_list->horizontalHeader()->setLabel( COL_NOMPROVEEDOR, tr( "Denominacion" ) );
   m_list->horizontalHeader()->setLabel( COL_NOMALTPROVEEDOR, tr( "Saldo Ant." ) );
   m_list->horizontalHeader()->setLabel( COL_CIFPROVEEDOR, tr( "Debe Periodo" ) );
   m_list->horizontalHeader()->setLabel( COL_CODICLIPROVEEDOR, tr( "Haber Periodo" ) );
   m_list->horizontalHeader()->setLabel( COL_CBANCPROVEEDOR, tr( "Saldo Periodo" ) );
   m_list->horizontalHeader()->setLabel( COL_COMENTPROVEEDOR, tr("Debe Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_DIRPROVEEDOR, tr("Haber Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_POBLPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_CPPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_TELPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_FAXPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_EMAILPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_URLPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_CLAVEWEBPROVEEDOR, tr("Saldo Ejercicio") );
   m_list->horizontalHeader()->setLabel( COL_IDDIVISION, tr("Saldo Ejercicio") );

   m_list->setColumnWidth(COL_IDDIVISION,75);
   m_list->setColumnWidth(COL_NOMPROVEEDOR,300);
   m_list->setColumnWidth(COL_NOMALTPROVEEDOR,75);
   m_list->setColumnWidth(COL_CIFPROVEEDOR,75);
   m_list->setColumnWidth(COL_CODICLIPROVEEDOR,75);
   m_list->setColumnWidth(COL_CBANCPROVEEDOR,100);
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor(confpr->valor(CONF_BG_BALANCE).c_str());   
    m_list->setReadOnly(TRUE);        
       companyact->begin();
       cursor2 * cur= companyact->cargacursor("SELECT * FROM proveedor","unquery");
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
         i++;
         cur->siguienteregistro();
       }// end while
      
      delete cur;
      
      showMaximized();
}// end providerslist

providerslist::~providerslist() {
}// end ~providerslist

void providerslist::dobleclick(int a, int b, int c, const QPoint &d) {
QString idprov = m_list->text(a, COL_IDPROVEEDOR);

fprintf(stderr, "parm a: %d  parm b: %d  parm c %d \n", a, b, c);
   provedit *prov = new provedit(companyact,0,theApp->translate("Edicion de Proveedores", "company"));
   prov->chargeprovider(idprov);
   prov->exec();
   delete prov;
}
