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
-- Albaran de proveedor
-- Any: Any en que s'efectua la comanda.
-- NumCompra: Numero de Compra (Clau artificial per poder registrar recepcions que ens arribin sense l'albarà postposant la cumplimentació del número d'albarà).
-- NumAlbara: Número d'albarà.
-- Data: Data de l'albarà
-- Recepcio: Data de recepció.
-- Comentaris
CREATE TABLE alb_pro (
   idalb_pro serial PRIMARY KEY,
   anoalb_pro integer,
   ncompraalb_pro integer,
   nalbalb_pro integer,
   fcrealb_pro date,
   frecepalb_pro date,
   comentalb_pro character varying(2000),
   
   idfra_pro integer REFERENCES fra_pro(idfra_pro)
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen)
);
*/

#include "delivnoteslist.h"
#include <qtable.h>
#include "company.h"
// #include "lindelivnoteslist.h"

#define COL_IDALB_PRO 0
#define COL_ANOALB_PRO 1
#define COL_NCOMPRAALB_PRO 2
#define COL_NALBALB_PRO 3
#define COL_FCREALB_PRO 4
#define COL_FRECEPALB_PRO 5
#define COL_COMENTALB_PRO 6
#define COL_IDFRA_PRO 7
#define COL_IDALMACEN 8

delivnoteslist::delivnoteslist(company *comp, QWidget *parent, const char *name, int flag)
 : delivnoteslistbase(parent, name, flag) {
      companyact = comp;
      inicializa();
      showMaximized();
}// end delivnoteslist

void delivnoteslist::inicializa() {
   m_list->setNumRows( 0 );
   m_list->setNumCols( 0 );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setSorting( TRUE );
   m_list->setSelectionMode( QTable::SingleRow );
   m_list->setColumnMovingEnabled( TRUE );
   m_list->setNumCols(9);
   m_list->horizontalHeader()->setLabel( COL_IDALB_PRO, tr( "Identificador" ) );
   m_list->horizontalHeader()->setLabel( COL_ANOALB_PRO, tr( "Año" ) );
   m_list->horizontalHeader()->setLabel( COL_NCOMPRAALB_PRO, tr( "Nº Compra" ) );
   m_list->horizontalHeader()->setLabel( COL_NALBALB_PRO, tr( "Nº Albarán" ) );
   m_list->horizontalHeader()->setLabel( COL_FCREALB_PRO, tr( "Fecha Alta" ) );
   m_list->horizontalHeader()->setLabel( COL_FRECEPALB_PRO, tr( "Fecha Recepción" ) );
   m_list->horizontalHeader()->setLabel( COL_COMENTALB_PRO, tr( "Observaciones" ) );
   m_list->horizontalHeader()->setLabel( COL_IDFRA_PRO, tr( "Factura" ) );
   m_list->horizontalHeader()->setLabel( COL_IDALMACEN, tr( "Almacén" ) );
   
   m_list->setColumnWidth(COL_IDALB_PRO,100);
   m_list->setColumnWidth(COL_ANOALB_PRO,100);
   m_list->setColumnWidth(COL_NCOMPRAALB_PRO,100);
   m_list->setColumnWidth(COL_NALBALB_PRO,200);
   m_list->setColumnWidth(COL_FCREALB_PRO,100);
   m_list->setColumnWidth(COL_FRECEPALB_PRO,100);
   m_list->setColumnWidth(COL_COMENTALB_PRO,300);
   m_list->setColumnWidth(COL_IDFRA_PRO,100);
   m_list->setColumnWidth(COL_IDALMACEN,100);
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#AAAAAA");   
    m_list->setReadOnly(TRUE);        
       companyact->begin();
       cursor2 * cur= companyact->cargacursor("SELECT * FROM alb_pro","unquery");
       companyact->commit();
       m_list->setNumRows( cur->numregistros() );
       int i=0;
       while (!cur->eof()) {
         m_list->setText(i,COL_IDALB_PRO,cur->valor("idalb_pro"));
	 m_list->setText(i,COL_ANOALB_PRO,cur->valor("anoalb_pro"));
         m_list->setText(i,COL_NCOMPRAALB_PRO,cur->valor("ncompraalb_pro"));
         m_list->setText(i,COL_NALBALB_PRO,cur->valor("nalbalb_pro"));
	 m_list->setText(i,COL_FCREALB_PRO,cur->valor("fcrealb_pro"));
         m_list->setText(i,COL_FRECEPALB_PRO,cur->valor("frecepalb_pro"));
	 m_list->setText(i,COL_COMENTALB_PRO,cur->valor("comentalb_pro"));
	 m_list->setText(i,COL_IDFRA_PRO,cur->valor("idfra_pro"));
	 m_list->setText(i,COL_IDALMACEN,cur->valor("idalmacen"));
         i++;
         cur->siguienteregistro();
       }// end while
      
      delete cur;
}// end delivnoteslist


void delivnoteslist::dobleclick(int a, int b, int c, const QPoint &d) {
   QString idpedido = m_list->text(a, COL_IDALB_PRO);
   fprintf(stderr, "parm a: %d  parm b: %d  parm c %d \n", a, b, c);
  /* lindelivnoteslist *linea = new lindelivnoteslist(companyact,0,theApp->translate("Detalle Pedido", "company"));
   linea->chargelinorders(idpedido);
   linea->exec();
   delete linea; */
   inicializa();
}



delivnoteslist::~delivnoteslist() {
}// end ~delivnoteslist


