/***************************************************************************
                          diarioprint.cpp  -  description
                             -------------------
    begin                : Wed Jan 8 2003
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

#include "diarioprint.h"

#include <unistd.h>

diarioprint::diarioprint(){
   fichero = NULL;
}// end diarioprint

diarioprint::~diarioprint(){
}// end diarioprint

int diarioprint::inicializa(postgresiface2 *conn) {
    conexionbase = conn;
    return(0);
}// end inicializa


void diarioprint::inicializa1( char *finicial1, char *ffinal1) {
   finicial = finicial1;
   ffinal = ffinal1;
}// end inicializa1

void diarioprint::inicializa2(char *fich) {
   fichero = fich;
}// end inicializa2

/**************************************************************
 * Se ha pulsado sobre el boton aceptar del formulario
 **************************************************************/
void diarioprint::accept() {
   char *args[]={"diario.txt","diario.txt",NULL};
   if (fichero != NULL) {
      args[0]=fichero;
      args[1]=fichero;
   }// end if
   int error;
   int pid;
   float debe, haber;
   int idcuenta;
   int idasiento;
   string fecha;
   string fechaasiento;
   string descripcion;
   string concepto;
   string codigocuenta;
   string cad;
   cursor2 *cursoraux, *cursoraux1, *cursoraux2;
   
   FILE *mifile;
   mifile = fopen(args[0],"wt");
   if (mifile != NULL) {
      fprintf(mifile,"Fecha Inicial: %s, Fecha Final: %s\n",finicial,ffinal);
      fprintf(mifile,"%5.5s %10.10s %10s %30.30s %9.2s %9.2s\n","ASIENTO", "FECHA","SUBCUENTA", "DESCRIPCION","DEBE","HABER");
      fprintf(mifile,"----------------------------------------------------------------------------------------------------------\n");
      conexionbase->begin();
      cursoraux = conexionbase->cargaasientosfecha(finicial,ffinal);
      for(;!cursoraux->eof();cursoraux->siguienteregistro()) {
         fprintf(stderr,"bucle\n");
         fechaasiento = cursoraux->valor(2).latin1();
         idasiento = atoi(cursoraux->valor(0).latin1());
         cursoraux1 = conexionbase->cargaapuntes(idasiento);
         for(;!cursoraux1->eof();cursoraux1->siguienteregistro()) {
            fecha = cursoraux1->valor(4).latin1();
            descripcion = cursoraux1->valor(5).latin1();
            concepto = cursoraux->valor(1).latin1();
            debe = atof(cursoraux1->valor(8).latin1());
            haber = atof(cursoraux1->valor(9).latin1());
            idcuenta = atoi(cursoraux1->valor(6).latin1());

            cursoraux2 = conexionbase->cargacuenta(idcuenta, NULL);
            if (!cursoraux2->eof()) {
               codigocuenta = cursoraux2->valor(1).latin1();
            }// end if
            cursoraux2->cerrar();
            delete cursoraux2;
            fprintf(mifile,"%5d %-10.10s %10s %-30.30s %9.2f %9.2f\n",idasiento, fecha.c_str(),codigocuenta.c_str(), descripcion.c_str(),debe,haber);
         }// end for
         cursoraux1->cerrar();
         delete cursoraux1;
         fprintf(mifile,"\n");
      }// end for
      delete cursoraux;
      conexionbase->commit();
      fclose(mifile);
   }// end if
   if ((pid=fork()) < 0) {
        perror ("Fork failed");
        exit(errno);
   }// end if
   if (!pid) {
      error = execvp("kedit",args);
   }// end if
}// end accept
