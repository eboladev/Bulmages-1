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
-- Any: Any en que s'efectua la comanda.
-- Numero: Número de comanda (començant de 1 cada any).
-- Descripcio: Breu descripció o comentari opcional.
-- Data: Data d'emisió de la comanda.
CREATE TABLE pedido (
   idpedido serial PRIMARY KEY,
   numpedido integer,
   anopedido integer,
   fechapedido date,
   descpedido character varying(500),
   
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor)
);
*/

#include "orderslist.h"
#include <qtable.h>
#include "company.h"
#include "linorderslist.h"

#define COL_IDPEDIDO 0
#define COL_NUMPEDIDO 1
#define COL_ANOPEDIDO 2
#define COL_FECHAPEDIDO 3
#define COL_DESCPEDIDO 4
#define COL_IDPROVEEDOR 5

orderslist::orderslist(company *comp, QWidget *parent, const char *name, int flag)
 : orderslistbase(parent, name, flag) {
      companyact = comp;
      inicializa();
      showMaximized();
}// end orderslist

void orderslist::inicializa() {
   m_list->setNumRows( 0 );
   m_list->setNumCols( 0 );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setSorting( TRUE );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setColumnMovingEnabled( TRUE );
   m_list->setNumCols(6);
   m_list->horizontalHeader()->setLabel( COL_IDPEDIDO, tr( "Identificador" ) );
   m_list->horizontalHeader()->setLabel( COL_NUMPEDIDO, tr( "Número de pedido" ) );
   m_list->horizontalHeader()->setLabel( COL_ANOPEDIDO, tr( "Año" ) );
   m_list->horizontalHeader()->setLabel( COL_FECHAPEDIDO, tr( "Fecha Emisión" ) );
   m_list->horizontalHeader()->setLabel( COL_DESCPEDIDO, tr( "Descripción" ) );
   m_list->horizontalHeader()->setLabel( COL_IDPROVEEDOR, tr( "Proveedor" ) );
   
   m_list->setColumnWidth(COL_IDPEDIDO,100);
   m_list->setColumnWidth(COL_NUMPEDIDO,100);
   m_list->setColumnWidth(COL_ANOPEDIDO,75);
   m_list->setColumnWidth(COL_FECHAPEDIDO,100);
   m_list->setColumnWidth(COL_DESCPEDIDO,300);
   m_list->setColumnWidth(COL_IDPROVEEDOR,100);
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#AAAAAA");   
    m_list->setReadOnly(TRUE);        
       companyact->begin();
       cursor2 * cur= companyact->cargacursor("SELECT * FROM pedido","unquery");
       companyact->commit();
       m_list->setNumRows( cur->numregistros() );
       int i=0;
       while (!cur->eof()) {
         m_list->setText(i,COL_IDPEDIDO,cur->valor("idpedido"));
	 m_list->setText(i,COL_NUMPEDIDO,cur->valor("numpedido"));
         m_list->setText(i,COL_ANOPEDIDO,cur->valor("anopedido"));
         m_list->setText(i,COL_FECHAPEDIDO,cur->valor("fechapedido"));
	 m_list->setText(i,COL_DESCPEDIDO,cur->valor("descpedido"));
         m_list->setText(i,COL_IDPROVEEDOR,cur->valor("idproveedor"));
         i++;
         cur->siguienteregistro();
       }// end while
      
      delete cur;
}// end orderslist


void orderslist::dobleclick(int a, int b, int c, const QPoint &d) {
   QString idpedido = m_list->text(a, COL_IDPEDIDO);
   fprintf(stderr, "parm a: %d  parm b: %d  parm c %d \n", a, b, c);
   linorderslist *linea = new linorderslist(companyact,0,theApp->translate("Detalle Pedido", "company"));
   linea->chargelinorders(idpedido);
//   linea->exec();
   delete linea;
   inicializa();
}



orderslist::~orderslist() {
}// end ~orderslist


