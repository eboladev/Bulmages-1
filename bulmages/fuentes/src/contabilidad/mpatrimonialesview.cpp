/***************************************************************************
                          mpatrimonialesview.cpp  -  description
                             -------------------
    begin                : mié sep 10 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "mpatrimonialesview.h"
#include "mpatrimonialview.h"
#include <qtable.h>

mpatrimonialesview::mpatrimonialesview(QWidget *parent, const char *name, int fl ) : mpatrimonialesdlg(parent,name,fl) {
   modo = 0;
}

mpatrimonialesview::~mpatrimonialesview(){
}


/*************************************************************
 * Esta funcion se encarga de hacer las inicializaciones de todo el formulario.
 * Se llama asi y no desde el constructor pq asi la podemos llamar desde dentro
 * de la misma clase, etc etc etc
 *************************************************************/
int mpatrimonialesview::inicializa(postgresiface2 *conn ) {
   cursor2 *cursoraux1;

   conexionbase = conn;
   conexionbase->begin();
   // Vamos a cargar el número de digitos de cuenta para poder hacer una introduccion de numeros de cuenta mas practica.
   conexionbase->begin();
   string query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursoraux1 = conexionbase->cargacursor(query.c_str(),"codcuenta");
   conexionbase->commit();
   numdigitos=cursoraux1->valor(2).length();
   delete cursoraux1;
   fprintf(stderr,"las cuentas tienen %d digitos\n",numdigitos);
   inicializatabla();
   return(0);
}// end inicializa


void mpatrimonialesview::inicializatabla()  {
  tabla->setNumRows(0);
  tabla->setNumCols(2);
  tabla->horizontalHeader()->setLabel( 0, tr( "CODIGO" ) );
  tabla->horizontalHeader()->setLabel( 1, tr( "Masa patrimonial" ) );
  tabla->hideColumn(2);
  tabla->hideColumn(0);
  tabla->setColumnWidth(1,400);
  string query = "SELECT * FROM mpatrimonial WHERE idbalance ISNULL";
  conexionbase->begin();
  cursor2 *cursoraux1=conexionbase->cargacursor(query.c_str(),"elquery");
  conexionbase->commit();
  tabla->setNumRows(cursoraux1->numregistros());
  int i=0;
   while (!cursoraux1->eof()) {
      tabla->setText(i,0, cursoraux1->valor("idmpatrimonial"));
      tabla->setText(i,1, cursoraux1->valor("descmpatrimonial"));
      cursoraux1->siguienteregistro ();
      i++;
   }// end while
   delete cursoraux1;
   tabla->setReadOnly(TRUE);
}// end inicializatabla


void mpatrimonialesview::dbtabla(int row, int colummn, int button,const QPoint &mouse) {
  fprintf(stderr,"Se ha hecho doble click sobre la tabla\n");
  // Dependiendo del modo hacemos una cosa u otra
  if (modo == 0) {
     string idmpatrimonial = tabla->text(row,0).latin1();
     // Creamos el objeto mpatrimonialview, y lo lanzamos.
     mpatrimonialview *masa=new mpatrimonialview(this,0);
     masa->inicializa(conexionbase);
     masa->inicializa1(idmpatrimonial);
     masa->exec();
     delete masa;
     // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
     inicializatabla();
   } else {
      idmasa = tabla->text(tabla->currentRow(),0).latin1();
      nommasa = tabla->text(tabla->currentRow(),1).latin1();
      close();
   }// end if  
   
   // Para quitar el warning
   colummn=button=0;
   mouse.isNull();
}// end dbtabla


void mpatrimonialesview::editarmasa() {
   int row = tabla->currentRow();
   int col = tabla->currentColumn();
   QPoint mouse;
   dbtabla (row, col, 0, mouse);
   inicializatabla();
}// end editarmasa

void mpatrimonialesview::borrarmasa() {
   int row;
	row = tabla->currentRow();
   idmasa = tabla->text(tabla->currentRow(),0).latin1();
   QString query;
   query.sprintf("DELETE FROM compmasap WHERE idmpatrimonial   = %s",idmasa.c_str());
   conexionbase->begin();
   conexionbase->ejecuta(query);
   query.sprintf("DELETE FROM mpatrimonial WHERE idmpatrimonial = %s",idmasa.c_str());
   conexionbase->ejecuta(query);
   conexionbase->commit();
   inicializatabla();
}// end borrarmasa 

void mpatrimonialesview::nuevamasa() {
   mpatrimonialview *masa = new mpatrimonialview(this,0);
   masa->inicializa(conexionbase);
   masa->exec();
   delete masa;
   // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
   inicializatabla();
}// end nuevamasa
