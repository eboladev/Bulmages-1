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

 /*
-- El cliente siempre tiene la razón, bueno, o por lo menos eso cree.
--Codi: Clau artificial.
--Nom: Nom comercial o fiscal.
--Nom_alternatiu: Nom comercial o fiscal.
--CIF: Codi d'Identificació Fiscal.
--C_Banc: Compte Bancari.
--Adr: Adreça.
--Pobl: Població.
--CProv: Codi de provincia (dos primers dígits del codi postal).
--sCP: Tres darrers dígits del codi postal.
--Telf: Telèfon.
--Fax: Fax.
--Email: eMail.
--Url: Url.
--Data_alta
--Data_Baixa
---Comentaris
CREATE TABLE cliente (
   idcliente serial PRIMARY KEY,
   nomcliente character varying(100),
   nomaltcliente character varying(300),
   cifcliente character varying(200),
   bancocliente character varying(35),
   dircliente character varying(100),
   poblcliente character varying(40),
   cpcliente character varying(10),
   telcliente character varying(20),
   faxcliente character varying(20),
   mailcliente character varying(100),
   urlcliente character varying(150),
   faltacliente date DEFAULT NOW(),
   fbajacliente date,
   comentcliente character varying(2000),
   
   idrecargo integer NOT NULL REFERENCES recargo(idrecargo)
);
*/

#include "clientslist.h"
#include <qtable.h>
#include "company.h"

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



clientslist::clientslist(company *comp, QWidget *parent, const char *name, int flag)
 : clientslistbase(parent, name, flag) {
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
    m_list->setPaletteBackgroundColor("#AAAAAA");   
    m_list->setReadOnly(TRUE);        
       companyact->begin();
       cursor2 * cur= companyact->cargacursor("SELECT * FROM cliente","unquery");
       companyact->commit();
       m_list->setNumRows( cur->numregistros() );
       int i=0;
       while (!cur->eof()) {
         m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idcliente"));
         m_list->setText(i,COL_NOMPROVEEDOR,cur->valor("nomcliente"));
         m_list->setText(i,COL_CIFPROVEEDOR,cur->valor("cifcliente"));
         i++;
         cur->siguienteregistro();
       }// end while
      
      delete cur;
      
      showMaximized();
}// end clientslist

clientslist::~clientslist() {
}// end ~clientslist


