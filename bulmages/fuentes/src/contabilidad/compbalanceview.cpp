/***************************************************************************
                          compbalanceview.cpp  -  description
                             -------------------
    begin                : sáb sep 13 2003
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

#include "compbalanceview.h"
#include <qradiobutton.h>
#include "mpatrimonialesview.h"
#include "mpatrimonialview.h"

#include "balancesprintview.h"

#include <qlineedit.h>
#include <qtable.h>

#define COL_IDCOMPBALANCE  0
#define COL_DESCRIPCION    2
#define COL_IDMPATRIMONIAL 1
#define COL_ORDEN          3
#define COL_TABULACION     4
#define COL_SALDO          5

compbalanceview::compbalanceview(QWidget *parent, const char *name ) : compbalancedlg(parent,name) {
}


compbalanceview::~compbalanceview(){
}


void compbalanceview::inicializa1(string idbalancep) {
   idbalance = idbalancep;
   string query = "SELECT * FROM balance WHERE idbalance="+idbalance;
   fprintf(stderr,"%s\n",query.c_str());
   conexionbase->begin();
   cursor2 *mycursor = conexionbase->cargacursor(query.c_str(),"balancequery");
   conexionbase->commit();

   if (!mycursor->eof()) {
      titbalance->setText(mycursor->valor("nombrebalance"));
   }// end if
   delete mycursor;

   // Hacemos la inicializacion del listado.
//  listado->setNumRows(0);
  listado->setNumCols(6);
  
  listado->horizontalHeader()->setLabel( COL_IDCOMPBALANCE, tr( "identificador" ) );
  listado->horizontalHeader()->setLabel( COL_DESCRIPCION, tr( "concepto" ) );
  listado->horizontalHeader()->setLabel( COL_IDMPATRIMONIAL, tr( "nmasa" ) );
  listado->horizontalHeader()->setLabel( COL_ORDEN, tr( "orden" ) );
  listado->horizontalHeader()->setLabel( COL_TABULACION, tr( "tabulacion" ) );
  listado->horizontalHeader()->setLabel( COL_SALDO, tr( "saldo") );

  listado->setColumnWidth(COL_IDCOMPBALANCE,25);   
  listado->setColumnWidth(COL_DESCRIPCION,450);
  listado->setColumnWidth(COL_IDMPATRIMONIAL,35);
  listado->setColumnWidth(COL_ORDEN,25);
  listado->setColumnWidth(COL_TABULACION,25);
  listado->setColumnWidth(COL_SALDO,25);

  listado->hideColumn(COL_IDCOMPBALANCE);
  listado->hideColumn(COL_ORDEN);
  listado->hideColumn(COL_TABULACION);
  listado->hideColumn(COL_SALDO);
  
//  listado->setColumnWidth(2,10);
   
//   query = "SELECT *, saldompatrimonial (idmpatrimonial,'01/01/2003','31/12/2003') AS saldot FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
   query = "SELECT *, compbalance.idbalance AS saldot FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
   fprintf(stderr,"%s\n",query.c_str());
   conexionbase->begin();
   mycursor = conexionbase->cargacursor(query.c_str(), "compbalancequery");
   conexionbase->commit();
   listado->setNumRows(mycursor->numregistros());
   int i=0;
   while (!mycursor->eof()) {
      int orden = atoi (mycursor->valor("tabulacion").latin1());
      QString texto = "";
      for (int j=0; j<orden; j++)
         texto += "   ";
      listado->setText(i,COL_IDCOMPBALANCE,mycursor->valor("idcompbalance"));
      listado->setText(i,COL_DESCRIPCION,(texto + mycursor->valor("concepto")));
      listado->setText(i,COL_IDMPATRIMONIAL,mycursor->valor("idmpatrimonial"));
      listado->setText(i,COL_ORDEN,mycursor->valor("orden"));
      listado->setText(i,COL_TABULACION,mycursor->valor("tabulacion"));
      listado->setText(i,COL_SALDO,mycursor->valor("saldot"));
      i++;
      mycursor->siguienteregistro();
   }// end while
   delete mycursor;
   
}// end inicializa1



void compbalanceview::borralinea() {
   int row = listado->currentRow();
   if (row > 0 && listado->numRows() > 0) {
      string idcompbalance = listado->text(row, COL_IDCOMPBALANCE).latin1();
      string idmasa = listado->text(row, COL_IDMPATRIMONIAL).latin1();
      QString query ;
      query.sprintf("DELETE FROM compmasap WHERE masaperteneciente = %s",idmasa.c_str());
      conexionbase->begin();
      conexionbase->ejecuta(query);
      query.sprintf("DELETE FROM mpatrimonial WHERE idmpatrimonial = %s",idmasa.c_str());
      conexionbase->ejecuta(query);
      query.sprintf("DELETE FROM compbalance WHERE idcompbalance = %s", idcompbalance.c_str());
      conexionbase->ejecuta(query);
      conexionbase->commit();
      inicializa1(idbalance);
      if (row > 1) listado->setCurrentCell(row-1,0);      
   }// end if
}// end borralinea

void compbalanceview::agregalinea() {
   string elorden;
   int row = listado->currentRow();
   if (row > 0 && listado->numRows() > 0) {
      elorden = listado->text(row,COL_ORDEN).latin1();
   } else {
      elorden = "0";
   }// end if
   if (idmpatrimonial == "") {
      idmpatrimonial = "NULL";
   }// end if
   
      conexionbase->begin();
      QString comentario = concepto->text();
      fprintf(stderr,"INSERCION POR FORMULA\n");
      // Lo primero es crear una masa patrimonial.
      QString query;
      query.sprintf("INSERT INTO mpatrimonial (idbalance, descmpatrimonial) VALUES (%s,'%s')",idbalance.c_str(),comentario.latin1());
      conexionbase->ejecuta(query);
      query.sprintf("SELECT max(idmpatrimonial) AS maxim FROM mpatrimonial");
      cursor2 * curs= conexionbase->cargacursor(query, "micursorcillo");
      QString idmpatrimonialnueva = curs->valor("maxim");
      delete curs;
      fprintf(stderr,"%s\n",(char *)query.latin1());
      // Hay que interpretar la linea de la formula y insertar la masa patrimonial.
      // es una formula, hay que insertar una masa patrimonial
      //string query = "INSERT INTO mpatrinonial (idbalance, descmpatrimonial) VALUES("++","++")"
      QString formula1 = formula->text();
      formula1.replace(" ","");
      int iant=0;
      QString componente;
      for (unsigned int i=1; i <= formula1.length(); i++) {
         if(formula1[i] == '+' || formula1[i] == '-' || i == formula1.length()) {
            componente = formula1.mid(iant,i-iant);
            fprintf(stderr,"Hemos localizado el componente: %s\n", componente.latin1());
            // Comprobamos cual es el signo del componente
            QString signo;
            if (componente[0] == '-') {
               signo = "FALSE";
               componente = componente.mid(1,componente.length());
            } else if (componente[0] == '+') {
               signo = "TRUE";
               componente = componente.mid(1,componente.length());
            } else {
               signo = "TRUE";
            }// end if

            // Comprobamos si se trata de una masa patrimonial o de una cuenta.
            QString idmpatrimonial;
            QString codcuenta;
            if (componente[0] == 'M') {
               idmpatrimonial =  componente.mid(1,componente.length());
               query.sprintf("INSERT INTO compmasap(idmpatrimonial, masaperteneciente, signo) VALUES (%s,%s,%s)",idmpatrimonial.latin1(), idmpatrimonialnueva.latin1(), signo.latin1());
            } else {
               codcuenta = componente;
               query.sprintf("INSERT INTO compmasap(idcuenta, masaperteneciente, signo) VALUES (id_cuenta('%s'),%s,%s)",codcuenta.latin1(), idmpatrimonialnueva.latin1(), signo.latin1());
            }// end if
            fprintf(stderr,"%s\n",query.latin1());
            conexionbase->ejecuta(query);
            iant = i;
         }// end if
      }// end for
      string query1 = "UPDATE compbalance SET orden = orden +1 WHERE idbalance="+idbalance+" AND orden >= "+elorden+"+1";
      fprintf(stderr,"%s\n",query1.c_str());
      conexionbase->ejecuta(query1.c_str());
      query.sprintf("INSERT INTO compbalance (concepto, idbalance, idmpatrimonial, tabulacion, orden) VALUES ('%s',%s,%s, 0, %s+1)", comentario.latin1(), idbalance.c_str(), idmpatrimonialnueva.latin1(), elorden.c_str());
      fprintf(stderr,"%s\n",query.latin1());
//      query1 = "INSERT INTO compbalance (concepto, idbalance, idmpatrimonial, tabulacion, orden) VALUES ('"+comentario+"',"+idbalance+","+idmpatrimonialnueva+", 0, "+elorden+"+1)";
      conexionbase->ejecuta(query);
      
      conexionbase->commit();
   
   inicializa1(idbalance);
   listado->setCurrentCell(row+1,0);
   
}// end agregalinea


void compbalanceview::botonizq() {
   int row = listado->currentRow();
   string idcompbalance = listado->text(listado->currentRow(),0).latin1();
   string query = "UPDATE compbalance set tabulacion=tabulacion-1 WHERE idcompbalance="+idcompbalance;
   conexionbase->begin();
   conexionbase->ejecuta(query.c_str());
   conexionbase->commit();
   inicializa1(idbalance);
   listado->setCurrentCell(row,0);
}// end botonizq


void compbalanceview::botondrcha() {
   int row = listado->currentRow();
   string idcompbalance = listado->text(listado->currentRow(),COL_IDCOMPBALANCE).latin1();
   string query = "UPDATE compbalance set tabulacion=tabulacion+1 WHERE idcompbalance="+idcompbalance;
   conexionbase->begin();
   conexionbase->ejecuta(query.c_str());
   conexionbase->commit();
   inicializa1(idbalance);
   listado->setCurrentCell(row,0);
}// end botondrcha


void compbalanceview::botonarriba() {
   int row = listado->currentRow();
   if (row >0) {
      string idcompbalance = listado->text(row,COL_IDCOMPBALANCE).latin1();
      string idcompbalance1 = listado->text(row-1,COL_IDCOMPBALANCE).latin1();
      string query = "UPDATE compbalance set orden = "+(string)listado->text(row-1,3).latin1()+" WHERE idcompbalance = "+ idcompbalance;
      fprintf(stderr,"%s\n",query.c_str());
      conexionbase->begin();
      conexionbase->ejecuta(query.c_str());
      query = "UPDATE compbalance set orden = "+(string)listado->text(row,3).latin1()+" WHERE idcompbalance = "+idcompbalance1;
      fprintf(stderr,"%s\n",query.c_str());
      conexionbase->ejecuta(query.c_str());
      conexionbase->commit();
   }// end if
   inicializa1(idbalance);
   listado->setCurrentCell(row-1,0);

}// end botonarriba


void compbalanceview::botonabajo() {
   int row = listado->currentRow();
   if (row < listado->numRows()-1) {
      string idcompbalance = listado->text(row,COL_IDCOMPBALANCE).latin1();
      string idcompbalance1 = listado->text(row+1,COL_IDCOMPBALANCE).latin1();
      string query = "UPDATE compbalance set orden = "+(string)listado->text(row+1,3).latin1()+" WHERE idcompbalance = "+ idcompbalance;
      fprintf(stderr,"%s\n",query.c_str());
      conexionbase->begin();
      conexionbase->ejecuta(query.c_str());
      query = "UPDATE compbalance set orden = "+(string)listado->text(row,3).latin1()+" WHERE idcompbalance = "+idcompbalance1;
      fprintf(stderr,"%s\n",query.c_str());
      conexionbase->ejecuta(query.c_str());
      conexionbase->commit();
   }// end if
   inicializa1(idbalance);
   listado->setCurrentCell(row+1,0);
}// end botonarriba


void compbalanceview::listadopulsado(int row, int col, int a, const QPoint &mouse) {
      concepto->setText("");
      string query = "SELECT * FROM compbalance WHERE idcompbalance = "+(string) listado->text(row,COL_IDCOMPBALANCE).latin1();
      fprintf(stderr,"%s\n",query.c_str());
      conexionbase->begin();
      cursor2 *curs = conexionbase->cargacursor(query.c_str(),"supmycursor");
      conexionbase->commit();
      if (!curs->eof()) {
         concepto->setText(curs->valor("concepto"));
//         idmpatrimonial = listado->text(row,COL_IDMPATRIMONIAL).latin1();
         idmpatrimonial = curs->valor("idmpatrimonial").latin1();

         query = "SELECT * FROM compmasap LEFT JOIN cuenta ON compmasap.idcuenta = cuenta.idcuenta LEFT JOIN mpatrimonial ON compmasap.idmpatrimonial = mpatrimonial.idmpatrimonial WHERE masaperteneciente = "+idmpatrimonial;
         QString formula1;
         conexionbase->begin();
         cursor2 *curs1 = conexionbase->cargacursor(query.c_str(),"cursdf");
         conexionbase->commit();
         while (!curs1->eof()) {
            if (curs1->valor("signo") == "t") {
               if (curs1->valor("codigo") == "") {
                  formula1 += " + M"+curs1->valor("idmpatrimonial");
               } else {
                  formula1 += " + "+curs1->valor("codigo");
               }// end if               
            } else {
               if (curs1->valor("codigo") == "") {
                  formula1 += " - M"+curs1->valor("idmpatrimonial");
               } else {
                  formula1 += " - "+curs1->valor("codigo");
               }// end if
            }// end if
            curs1->siguienteregistro();
         }// end while
         delete curs1;
         formula->setText(formula1);
      }// end if
      delete curs;
      
      // Para quitar el warning
      col=a=0;
      mouse.isNull();
}// end listadopulsado



void compbalanceview::modificamasa() {
     // Creamos el objeto mpatrimonialview, y lo lanzamos.
     mpatrimonialview *masa=new mpatrimonialview(this,0);
     masa->inicializa(conexionbase);
     masa->inicializa1(idmpatrimonial);
     masa->exec();
//     mpatrimonial->setText((char *) (masa->getnommasa()).c_str());
     idmpatrimonial = masa->getidmasa();
     delete masa;
     // Como existe la posibilidad de que hayan cambiado las cosas forzamos un repintado
}// end modificamasa





void compbalanceview::listadodpulsado(int row, int col, int but, const QPoint &butt) {
   listadopulsado ( row, col, but, butt);
   if (idmpatrimonial != "") {
      modificamasa();
   } else {
      nuevamasa();
   }// end if
}// end listadopulsado


void compbalanceview::cambiado(int row, int col) {
   QPoint aux;
   listadopulsado(row, col,0, aux);
}// end cambiado


void compbalanceview::modificalinea() {
   int row = listado->currentRow();
   QString query;                            // Utilizado para hacer los querys
//   string query;
   string idcompbalance = listado->text(row, COL_IDCOMPBALANCE).latin1();
   string descripcion = concepto->text().latin1();
   if (idmpatrimonial == "") {
      idmpatrimonial="NULL";
   }// end if
   conexionbase->begin();
      QString comentario = concepto->text();    // El concepto de la línea
      QString formula1 = formula->text();       // La fórmula de la linea de balance
      QString componente;                       // Esta variable es utilizada para desglosar la fórmula

      // es una formula, hay que insertar una masa patrimonial
      query.sprintf("UPDATE mpatrimonial SET idbalance=%s WHERE idmpatrimonial=%s", idbalance.c_str(), idmpatrimonial.c_str());
      fprintf(stderr,"%s\n",query.latin1());
      conexionbase->ejecuta(query);

      query.sprintf("DELETE FROM compmasap WHERE masaperteneciente = %s", idmpatrimonial.c_str());
      fprintf(stderr,"%s\n",(char *)query.latin1());
      conexionbase->ejecuta(query);
      // Hay que interpretar la linea de la formula y insertar la masa patrimonial.
      // es una formula, hay que insertar una masa patrimonial
      //string query = "INSERT INTO mpatrinonial (idbalance, descmpatrimonial) VALUES("++","++")"
      formula1.replace(" ","");
      int iant=0;
      for (unsigned int i=1; i <= formula1.length(); i++) {
         if(formula1[i] == '+' || formula1[i] == '-' || i == formula1.length()) {
            componente = formula1.mid(iant,i-iant);
            fprintf(stderr,"Hemos localizado el componente: %s\n", componente.latin1());
            // Comprobamos cual es el signo del componente
            QString signo;
            if (componente[0] == '-') {
               signo = "FALSE";
               componente = componente.mid(1,componente.length());
            } else if (componente[0] == '+') {
               signo = "TRUE";
               componente = componente.mid(1,componente.length());
            } else {
               signo = "TRUE";
            }// end if

            // Comprobamos si se trata de una masa patrimonial o de una cuenta.
            QString idmpatrimonialcomp;
            QString codcuenta;
            if (componente[0] == 'M') {
               idmpatrimonialcomp =  componente.mid(1,componente.length());
               query.sprintf("INSERT INTO compmasap(idmpatrimonial, masaperteneciente, signo) VALUES (%s,%s,%s)",idmpatrimonialcomp.latin1(), idmpatrimonial.c_str(), signo.latin1());
            } else {
               codcuenta = componente;
               query.sprintf("INSERT INTO compmasap(idcuenta, masaperteneciente, signo) VALUES (id_cuenta('%s'),%s,%s)",codcuenta.latin1(), idmpatrimonial.c_str(), signo.latin1());
            }// end if
            fprintf(stderr,"%s\n",query.latin1());
            conexionbase->ejecuta(query);
            iant = i;
         }// end if
      }// end for

   query.sprintf("UPDATE compbalance SET idmpatrimonial=%s, concepto='%s' WHERE idcompbalance=%s",idmpatrimonial.c_str(), descripcion.c_str(), idcompbalance.c_str());
   fprintf(stderr,"%s\n",query.latin1());
   conexionbase->ejecuta(query);
   conexionbase->commit();
   inicializa1(idbalance);
//   listado->setCurrentCell(row,0);
}// end modificalinea


void compbalanceview::botonimprimir() {
   balancesprintview *b = new balancesprintview(0,0);
   b->inicializa(conexionbase);
   b->setidbalance(idbalance);
   b->exec();
   delete b;
}// end botonimprimir

void compbalanceview::accept() {
   QString query;
   query.sprintf("UPDATE balance SET nombrebalance='%s' WHERE idbalance=%s",titbalance->text().latin1(),idbalance.c_str());
   conexionbase->begin();
   conexionbase->ejecuta(query);
   conexionbase->commit();
   done(1);
}// end accept

