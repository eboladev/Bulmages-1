/***************************************************************************
                          balancesview.cpp  -  description
                             -------------------
    begin                : sáb oct 18 2003
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

#include "balancesview.h"
#include <qtable.h>
#include "compbalanceview.h"
#include "balancesprintview.h"


#define COL_CODIGO 0
#define COL_NOMBRE 1

balancesview::balancesview(QWidget *parent, const char *name ) : balancesdlg(parent,name) {
   modo = 0;
}

balancesview::~balancesview(){
}


/*************************************************************
 * Esta funcion se encarga de hacer las inicializaciones de todo el formulario.
 * Se llama asi y no desde el constructor pq asi la podemos llamar desde dentro
 * de la misma clase, etc etc etc
 *************************************************************/
int balancesview::inicializa(postgresiface2 *conn ) {
//   cursor1 *cursoraux1;

   conexionbase = conn;
   conexionbase->begin();

/*   
   // Vamos a cargar el número de digitos de cuenta para poder hacer una introduccion de numeros de cuenta mas practica.
   conexionbase->begin();
   string query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursoraux1 = conexionbase->cargacursor(query,"codcuenta");
   conexionbase->commit();
   numdigitos=cursoraux1->valor(2).size();
   delete cursoraux1;
   fprintf(stderr,"las cuentas tienen %d digitos\n",numdigitos);
*/
   inicializatabla();
   return(0);
}// end inicializa


void balancesview::inicializatabla()  {
  listado->setNumRows(0);
  listado->setNumCols(2);
  listado->horizontalHeader()->setLabel( COL_CODIGO, tr( "CODIGO" ) );
  listado->horizontalHeader()->setLabel( COL_NOMBRE, tr( "Nombre Balance" ) );
  listado->hideColumn(2);
  listado->hideColumn(0);
  listado->setColumnWidth(1,400);
  string query = "SELECT * FROM balance";
  conexionbase->begin();
  cursor2 *cursoraux1=conexionbase->cargacursor(query.c_str(),"elquery");
  conexionbase->commit();
  listado->setNumRows(cursoraux1->numregistros());
  int i=0;
   while (!cursoraux1->eof()) {
      listado->setText(i,COL_CODIGO, cursoraux1->valor("idbalance"));
      listado->setText(i,COL_NOMBRE, cursoraux1->valor("nombrebalance"));
      cursoraux1->siguienteregistro ();
      i++;
   }// end while
   delete cursoraux1;
   listado->setReadOnly(TRUE);
}// end inicializatabla


void balancesview::nuevo() {
   QString query;
   // Insertamos el balance en su sitio
   query.sprintf("INSERT INTO balance (nombrebalance) VALUES ('Nuevo Balance')");
   conexionbase->begin();
   conexionbase->ejecuta(query);
   conexionbase->commit();
   // Cogemos el identificador del balance insertado y abrimos la ventana de edición de balances.
   query.sprintf("SELECT max(idbalance) AS idbalance FROM balance");
   conexionbase->begin();
   cursor2 *cursoraux = conexionbase->cargacursor(query, "cursor1");
   conexionbase->commit();
   
   compbalanceview * nuevae = new compbalanceview(0,"compbalance");
   nuevae->inicializa(conexionbase);
   nuevae->inicializa1(cursoraux->valor("idbalance").latin1());
   nuevae->exec();
   delete nuevae;   
   delete cursoraux;
   // Cuando hemos terminado de trabajar con el nuevo balance repintamos para que aparezcan bien las cosas
   inicializatabla();
}// end nuevo


void balancesview::borrar() {
   // Borrar un balance es borrar tanto la entrada de balance como todas las lineas asociadas a éste.
   // Borramos primero todas las compbalance y luego la entrada en balance.
   QString idbalance = listado->text(listado->currentRow(), COL_CODIGO);
   QString query;
   conexionbase->begin();
   query.sprintf("DELETE FROM compbalance WHERE idbalance=%s", idbalance.latin1());
   conexionbase->ejecuta(query.latin1());
   query.sprintf("DELETE FROM balance WHERE idbalance=%s",idbalance.latin1());
   conexionbase->ejecuta(query);
   conexionbase->commit();
   inicializatabla();
}// end borrar


void balancesview::dbtabla(int row, int colummn, int button,const QPoint &mouse) {
  fprintf(stderr,"Se ha hecho doble click sobre la tabla\n");
  // Dependiendo del modo hacemos una cosa u otra
  if (modo == 0) {
     string idbalance = listado->text(row,COL_CODIGO).latin1();
     // Creamos el objeto mpatrimonialview, y lo lanzamos.
     compbalanceview *bal=new compbalanceview(this,0);
     bal->inicializa(conexionbase);
     bal->inicializa1(idbalance);
     bal->exec();
     delete bal;
     // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
     inicializatabla();
   } else {
      idbalance = listado->text(listado->currentRow(),COL_CODIGO).latin1();
      nombalance = listado->text(listado->currentRow(),COL_NOMBRE).latin1();
      close();
   }// end if
   
   // Para quitar el warning
   colummn=button=0;
   mouse.isNull();
}// end dbtabla

void balancesview::imprimir() {
   string idbalance = listado->text(listado->currentRow(),COL_CODIGO).latin1();
   balancesprintview *b = new balancesprintview(0,0);
   b->inicializa(conexionbase);
   b->setidbalance(idbalance);
   b->exec();
   delete b;
}// end imprimir


