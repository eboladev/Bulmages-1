/***************************************************************************
                          libromayorprint.cpp  -  description
                             -------------------
    begin                : Tue Jan 7 2003
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

#include "libromayorprint.h"
#include <unistd.h>
#include "fixed.h"

libromayorprint::libromayorprint(){
   fichero = NULL;
}


libromayorprint::~libromayorprint(){
}


int libromayorprint::inicializa(postgresiface2 *conn ) {
    conexionbase = conn;
    return(0);
}// end inicializa


void libromayorprint::inicializa1(QString codinicial1, QString codfinal1, QString finicial1, QString ffinal1) {
   codinicial= codinicial1;
   codfinal = codfinal1;
   finicial = finicial1;
   ffinal = ffinal1;
}// end if


void libromayorprint::inicializa2(char *fich) {
   fichero = fich;
}// end inicializa2


/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void libromayorprint::accept() {


   Fixed debe("0"), haber("0"), saldo("0");
   Fixed debeinicial("0"), haberinicial("0"), saldoinicial("0");
   Fixed debefinal("0"), haberfinal("0"), saldofinal("0");
   
   int idcuenta;
   int idasiento;
   char *textasiento;
   int activo;
   string cad;
   cursor2 *cursoraux, *cursoraux1, *cursoraux2;
   
   FILE *mifile;
   mifile = fopen("mayor.txt","wt");
   if (mifile != NULL) {
      conexionbase->begin();
      cursoraux = conexionbase->cargacuentascodigo(-1,codinicial,codfinal);
      while(!cursoraux->eof()) {
         idcuenta = atoi(cursoraux->valor(0).ascii());
         cursoraux1 = conexionbase->cargaapuntesctafecha(idcuenta, finicial, ffinal);
         if (!cursoraux1->eof()) {
            activo = strcmp((char *) cursoraux->valor(13).ascii() , "f");
            fprintf(mifile,"\n\n%12s %50s",cursoraux->valor(1).ascii(), cursoraux->valor(2).ascii());
            if (activo) {
               fprintf(mifile," Cuenta de Activo\n");
            } else {
               fprintf(mifile," Cuenta de Pasivo\n");
            }// end if
            cursoraux2 = conexionbase->cargasaldoscuentafecha(idcuenta, finicial);
            if (!cursoraux2->eof()) {
               debeinicial = Fixed(cursoraux2->valor(0).ascii());
               haberinicial = Fixed(cursoraux2->valor(1).ascii());
               if (activo) {
                  saldoinicial = debeinicial - haberinicial;
               } else {
                  saldoinicial = haberinicial-debeinicial;
               }// end if
               fprintf(mifile,"%5.5s %10.10s %10.10s %-40.40s %10.10s %10.10s %10.10s\n","Asiento","Fecha","Asiento","Descripcion","Debe","Haber","Saldo");
               fprintf(mifile, "                                                 SUMAS ANTERIORES... %10.2f %10.2f %10.2f\n",debeinicial, haberinicial, saldoinicial);
               fprintf(mifile,"---------------------------------------------------------------------------------------------------------\n");
            }// end if
            saldo = saldoinicial;
            debefinal = debeinicial;
            haberfinal = haberinicial;
            for(;!cursoraux1->eof();cursoraux1->siguienteregistro()) {
               idasiento=atoi(cursoraux1->valor(2).ascii());
               textasiento = (char *) cursoraux1->valor(5).ascii();
               debe=Fixed(cursoraux1->valor(8).ascii());
               haber=Fixed(cursoraux1->valor(9).ascii());
               if (activo) {
                  saldo = saldo + debe - haber;
               } else {
                  saldo = saldo +haber -debe;
               }// end if
               debefinal = debefinal + debe;
               haberfinal = haberfinal + haber;
               cad = cursoraux1->valor(4).ascii();
               fprintf(mifile,"%5.5d %10.10s %10.10s %-40.40s %10.2f %10.2f %10.2f\n",idasiento, cad.substr(1,10).c_str(),textasiento, cursoraux1->valor(5).ascii(),debe.toQString().ascii(),haber.toQString().ascii(),saldo.toQString().ascii());
            }// end for
            if (activo) {
               saldofinal = debefinal - haberfinal;
            } else {
               saldofinal = haberfinal -debefinal;
            }// end if
            fprintf(mifile,"                                       -----------------------------------------------------------------\n");
            fprintf(mifile, "                                                  TOTAL SUBCUENTA... %10.2f %10.2f %10.2f\n",debefinal.toQString().ascii(), haberfinal.toQString().ascii(), saldofinal.toQString().ascii());
            cursoraux2->cerrar();
            delete cursoraux2;
         }// end if
         cursoraux1->cerrar();
         delete cursoraux1;
         cursoraux->siguienteregistro();
      }// end while
      fclose(mifile);
      conexionbase->commit();
      delete cursoraux;
   }// end if

	QString cadaux = confpr->valor(CONF_EDITOR)+" mayor.txt";
	system (cadaux.ascii());
}// end accept
