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
#include <qtable.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qcombobox.h>
#include "company.h"

#define COL_NUMLPEDIDO 0
#define COL_DESCLPEDIDO 1
#define COL_CANTLPEDIDO 2
#define COL_PVDLPEDIDO 3
#define COL_PREVLPEDIDO 4
#define COL_IDPEDIDO 5
#define COL_IDALB_PRO 6
#define COL_IDARTICULO 7

linorderslist::linorderslist(company *comp, QWidget *parent, const char *name, int flag)
 : linorderslistbase(parent, name, flag) {
      m_cursorcombo = NULL;
      m_cursorcombo2 = NULL;
      companyact = comp;
}// end linorderslist

// Cargamos de la tabla de pedidos los datos del pedido seleccionado y los ponemos en la pantalla.

void linorderslist::chargeorder(QString idpedido) {
   QString idproveedor;
   
   companyact->begin();
   cursor2 * cur= companyact->cargacursor("SELECT * FROM pedido, proveedor  WHERE idpedido="+idpedido+" and pedido.idproveedor=proveedor.idproveedor","unquery");
   companyact->commit();
   if (!cur->eof()) {
   	m_numpedido->setText(cur->valor("numpedido"));
	m_cifproveedor->setText(cur->valor("cifproveedor"));
	m_nomproveedor->setText(cur->valor("nomproveedor"));
	m_dirproveedor->setText(cur->valor("dirproveedor"));
	m_fechapedido->setText(cur->valor("fechapedido"));
	m_descpedido->setText(cur->valor("descpedido"));
	idproveedor = cur->valor("idproveedor");
   }
   delete cur;
   
   companyact->begin();
   
   if (m_cursorcombo != NULL) delete m_cursorcombo;
   m_cursorcombo = companyact->cargacursor("SELECT * FROM division where idproveedor="+idproveedor,"unquery");
   companyact->commit();
   while (!m_cursorcombo->eof()) {
   	m_combodivision->insertItem(m_cursorcombo->valor("descdivision"));
	m_cursorcombo->siguienteregistro();
   }
   
   companyact->begin();
   if (m_cursorcombo2 != NULL) delete m_cursorcombo2;
   m_cursorcombo2 = companyact->cargacursor("SELECT * FROM almacen","unquery");
   companyact->commit();
   while (!m_cursorcombo2->eof()) {
   	m_comboalmacen->insertItem(m_cursorcombo2->valor("nomalmacen"));
	m_cursorcombo2->siguienteregistro();
   }
   
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
   m_list->setNumCols(8);
   m_list->horizontalHeader()->setLabel( COL_NUMLPEDIDO, tr( "Nº Línea" ) );
   m_list->horizontalHeader()->setLabel( COL_DESCLPEDIDO, tr( "Descripción" ) );
   m_list->horizontalHeader()->setLabel( COL_CANTLPEDIDO, tr( "Cantidad" ) );
   m_list->horizontalHeader()->setLabel( COL_PVDLPEDIDO, tr( "Precio" ) );
   m_list->horizontalHeader()->setLabel( COL_PREVLPEDIDO, tr( "Fecha Prevista Entrega" ) );
   m_list->horizontalHeader()->setLabel( COL_IDPEDIDO, tr( "Nº Pedido" ) );
   m_list->horizontalHeader()->setLabel( COL_IDALB_PRO, tr( "Albarán" ) );
   m_list->horizontalHeader()->setLabel( COL_IDARTICULO, tr( "Artículo" ) );
   
   m_list->setColumnWidth(COL_NUMLPEDIDO,100);
   m_list->setColumnWidth(COL_DESCLPEDIDO,300);
   m_list->setColumnWidth(COL_CANTLPEDIDO,100);
   m_list->setColumnWidth(COL_PVDLPEDIDO,100);
   m_list->setColumnWidth(COL_PREVLPEDIDO,100);
   m_list->setColumnWidth(COL_IDPEDIDO,100);
   m_list->setColumnWidth(COL_IDALB_PRO,100);
   m_list->setColumnWidth(COL_IDARTICULO,100);
   
//   listado->setPaletteBackgroundColor(QColor(150,230,230));
    // Establecemos el color de fondo del extracto. El valor lo tiene la clase configuracion que es global.
    m_list->setPaletteBackgroundColor("#AAAAAA");   
    m_list->setReadOnly(TRUE);        
       companyact->begin();
       cursor2 * cur= companyact->cargacursor("SELECT * FROM lpedido WHERE idpedido="+idpedido,"unquery");
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
         i++;
         cur->siguienteregistro();
       }// end while
      
      delete cur;
    //showMaximized();
}// end linorderslist


void linorderslist::activated(int a) {
fprintf(stderr,"id:%s\n", m_cursorcombo->valor("iddivision",a-1).ascii());
}

void linorderslist::almacenactivated(int a) {
fprintf(stderr,"id:%s\n", m_cursorcombo2->valor("idalmacen",a-1).ascii());
}

linorderslist::~linorderslist() {
}// end ~linorderslist


