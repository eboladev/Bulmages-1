/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borrás                                    *
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
#include "amortizacionesview.h"
#include "amortizacionview.h"
#include <empresa.h>


#define COL_CODIGO 0
#define COL_NOMBRE 1

amortizacionesview::amortizacionesview(empresa *emp,QWidget *parent, const char *name, bool flag ) : amortizacionesdlg(parent,name,flag) {
   empresaactual = emp;
   conexionbase = empresaactual->bdempresa();
   inicializatabla();
   modo = 0;
}// end amortizacionesview


amortizacionesview::~amortizacionesview() {
}// end ~amortizacionesview

void amortizacionesview::inicializatabla()  {
  listado->setNumRows(0);
  listado->setNumCols(2);
  listado->horizontalHeader()->setLabel( COL_CODIGO, tr( "CODIGO" ) );
  listado->horizontalHeader()->setLabel( COL_NOMBRE, tr( "Nombre Bien Amortizado" ) );
  listado->hideColumn(2);
  listado->hideColumn(0);
  listado->setColumnWidth(1,400);
  string query = "SELECT * FROM amortizacion";
  conexionbase->begin();
  cursor2 *cursoraux1=conexionbase->cargacursor(query.c_str(),"elquery");
  conexionbase->commit();
  listado->setNumRows(cursoraux1->numregistros());
  int i=0;
   while (!cursoraux1->eof()) {
      listado->setText(i,COL_CODIGO, cursoraux1->valor("idamortizacion"));
      listado->setText(i,COL_NOMBRE, cursoraux1->valor("nomamortizacion"));
      cursoraux1->siguienteregistro ();
      i++;
   }// end while
   delete cursoraux1;
   listado->setReadOnly(TRUE);
}// end inicializatabla


void amortizacionesview::dbtabla(int row, int colummn, int button,const QPoint &mouse) {
  fprintf(stderr,"Se ha hecho doble click sobre la tabla\n");
  // Dependiendo del modo hacemos una cosa u otra
  if (modo == 0) {
     idamortizacion = listado->text(row,COL_CODIGO).latin1();
     // Creamos el objeto mpatrimonialview, y lo lanzamos.
     amortizacionview *amor=new amortizacionview(empresaactual, 0,"", true);
//     bal->inicializa1(idbalance);
     amor->exec();
     delete amor;
     // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
     inicializatabla();
   } else {
      idamortizacion = listado->text(listado->currentRow(),COL_CODIGO);
      nomamortizacion = listado->text(listado->currentRow(),COL_NOMBRE);
      close();
   }// end if
   
   // Para quitar el warning
   colummn=button=0;
   mouse.isNull();
}// end dbtabla
