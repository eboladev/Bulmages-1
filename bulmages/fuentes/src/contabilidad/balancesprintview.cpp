/***************************************************************************
                          balancesprintview.cpp  -  description
                             -------------------
    begin                : jue oct 9 2003
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

#include "balancesprintview.h"
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qprogressbar.h>
#include "funcaux.h"
#include "calendario.h"
#include "empresa.h"

#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

balancesprintview::balancesprintview(empresa *emp,QWidget *parent, const char *name ) : balancesprintdlg(parent,name) {
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   fichero = NULL;
}// end balancesprintview

balancesprintview::~balancesprintview(){
}// end ~balancesprintview



/*********************************************************************************
 * Se ha pulsado sobre el boton de aceptar del formulario con lo que podemos     *
 * pasar a hacer la impresión.                                                   *
 *********************************************************************************/
void balancesprintview::accept() {
   char *args[]={"balance.txt","balance.txt",NULL};
   if (fichero != NULL) {
      args[0]= fichero;
      args[1]= fichero;
   }// end if
   string cad;
   string fechainicial = fechain->text().ascii();
   string fechafinal = fechafin->text().ascii();
   
   FILE *mifile;
   mifile = fopen(args[0],"wt");
   if (mifile != NULL) {
      string query1 = "SELECT * FROM compbalance WHERE idbalance = "+idbalance+" ORDER BY orden";
       conexionbase->begin();
       cursor2 *cursor = conexionbase->cargacursor(query1.c_str(), "compbalancequery9");
       conexionbase->commit();
       int numregistros = cursor->numregistros();
       progreso->reset();
       progreso->setTotalSteps(numregistros);
       progreso->setProgress(1);
       while (!cursor->eof()) {
                QString query = "SELECT saldototalmpatrimonial("+cursor->valor("idmpatrimonial")+") AS saldot";
                conexionbase->begin();
                cursor2 *mycursor = conexionbase->cargacursor(query, "compbalancequery");
                conexionbase->commit();
                int i=0;
		double valor;
                while (!mycursor->eof()) {
                   int orden = atoi (cursor->valor("tabulacion").ascii());
                   QString texto = "";
                   for (int j=0; j<orden; j++)
                      texto += "   ";
                   texto += cursor->valor("concepto");
		   valor = mycursor->valor("saldot").toDouble();
//		   if (valor > 0.001 || valor < -0.001)
                   	fprintf(mifile, "%-60.60s %10.2f\n", texto.ascii(), valor);
                   i++;
                   mycursor->siguienteregistro();
                }// end while
                delete mycursor;
                // Actualizamos la barra de progreso
                progreso->setProgress(progreso->progress()+1);
                cursor->siguienteregistro();
       }// end while
       delete cursor;
       fclose(mifile);
   }// end if

   QString cadena = confpr->valor(CONF_EDITOR)+" balances.txt &";
   system(cadena.ascii());
}// end accept


void balancesprintview::setidbalance(QString id) {
   idbalance = id;
   QString query ="SELECT * FROM balance WHERE idbalance="+idbalance;
   conexionbase->begin();
   cursor2 *cur = conexionbase->cargacursor(query, "micurs");
   conexionbase->commit();
   if (!cur->eof()) {
      m_nomBalance->setText(cur->valor("nombrebalance"));
   }// end if
   delete cur;
}// end setidbalance



