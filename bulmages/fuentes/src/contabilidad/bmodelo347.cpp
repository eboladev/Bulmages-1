/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#include "bmodelo347.h"


BModelo347::BModelo347(postgresiface2 *DBconnect,QString ejerActual, QWidget * parent, const char * name, WFlags f) : UImodelo347(parent,name,f) 
{
  importe->setText("3005.06");
  finicial->setText(normalizafecha("01/01/"+ejerActual).toString("dd/MM/yyyy"));
  ffinal->setText(normalizafecha("31/12/"+ejerActual).toString("dd/MM/yyyy"));
  DBConn = DBconnect;
  click_recargar(); //carga las tablas en pantalla;
}


BModelo347::~BModelo347()
{
}


void BModelo347::click_recargar() {
  int i =0;
  QString query;
  //A por la tabla de Ventas...pedazo de consulta SQL
  
  //query= QString("SELECT codigo, descripcion, cifent_cuenta as cif, importe FROM (SELECT  registroiva.contrapartida, SUM(baseimp+(baseimp*iva*0.01)) AS importe FROM registroiva, borrador WHERE borrador.idborrador=registroiva.idborrador AND borrador.fecha >= '")+finicial->text() + QString("' AND borrador.fecha <= '") + ffinal->text() + QString("' GROUP BY registroiva.contrapartida) AS parcial, cuenta WHERE parcial.contrapartida=cuenta.idcuenta AND cuenta.codigo LIKE '430%%' AND importe >='") + importe->text() + QString("'");
  
  query= QString("SELECT codigo, descripcion, cifent_cuenta as cif, cpent_cuenta as cp, importe FROM cuenta INNER JOIN (SELECT idcuenta, sum(apunte.debe) as importe FROM apunte WHERE idasiento IN (SELECT idasiento FROM (SELECT idcuenta FROM cuenta WHERE (codigo='4770016' OR codigo='4770007' OR codigo='4770004')) AS iva INNER JOIN apunte USING (idcuenta) WHERE fecha<='")+ffinal->text()+QString("' AND fecha>='")+finicial->text()+QString("' GROUP BY idasiento) AND idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo LIKE '430%') GROUP BY idcuenta) AS facturado USING(idcuenta) WHERE importe>")+importe->text()+QString(" ORDER BY descripcion");
  DBConn->begin();
  cursor2 *recordSet = DBConn->cargacursor(query,"recordSet");
  DBConn->commit();
  tablaventas->setNumRows(recordSet->numregistros());
  i=0;
  while (!recordSet->eof()) {
      tablaventas->setText(i,0,recordSet->valor("codigo"));
      tablaventas->setText(i,1,recordSet->valor("descripcion"));
      tablaventas->setText(i,2,recordSet->valor("cif"));
      tablaventas->setText(i,3,recordSet->valor("cp"));
      query.setNum(recordSet->valor("importe").toFloat(),'f',2); //round a 2 decimales
      tablaventas->setText(i,4,query);
      ++i;
      recordSet->siguienteregistro();
  }

  //A por la tabla de Compras...
  
  //query= QString("SELECT codigo, descripcion, cifent_cuenta as cif, importe FROM (SELECT  registroiva.contrapartida, SUM(baseimp+(baseimp*iva*0.01)) AS importe FROM registroiva, borrador WHERE borrador.idborrador=registroiva.idborrador AND borrador.fecha >= '")+finicial->text() + QString("' AND borrador.fecha <= '") + ffinal->text() + QString("' GROUP BY registroiva.contrapartida) AS parcial, cuenta WHERE parcial.contrapartida=cuenta.idcuenta AND cuenta.codigo LIKE '400%%' AND importe >='") + importe->text() + QString("'");
  
  query= QString("SELECT codigo, descripcion, cifent_cuenta as cif, cpent_cuenta as cp, importe FROM cuenta INNER JOIN (SELECT idcuenta, sum(apunte.haber) as importe FROM apunte WHERE idasiento IN (SELECT idasiento FROM (SELECT idcuenta FROM cuenta WHERE (codigo='4720016' OR codigo='4720007' OR codigo='4720004')) AS iva INNER JOIN apunte USING (idcuenta) WHERE fecha<='")+ffinal->text()+QString("' AND fecha>='")+finicial->text()+QString("' GROUP BY idasiento) AND idcuenta IN (SELECT idcuenta FROM cuenta WHERE codigo SIMILAR TO '4(0|1)0%') GROUP BY idcuenta) AS facturado USING(idcuenta) WHERE importe>")+importe->text()+QString(" ORDER BY descripcion");
  DBConn->begin();
  recordSet = DBConn->cargacursor(query,"recordSet");
  DBConn->commit();
  tablacompras->setNumRows(recordSet->numregistros());
  i=0;
  while (!recordSet->eof()) {
      tablacompras->setText(i,0,recordSet->valor("codigo"));
      tablacompras->setText(i,1,recordSet->valor("descripcion"));
      tablacompras->setText(i,2,recordSet->valor("cif"));
      tablacompras->setText(i,3,recordSet->valor("cp"));
      query.setNum(recordSet->valor("importe").toFloat(),'f',2); //round a 2 decimales
      tablacompras->setText(i,4,query);
      ++i;
      recordSet->siguienteregistro();
  }
}


void BModelo347::click_imprimir() {
  int i, numventas, numcompras, error, pid;
  char *args[]={"listado347.txt","listado347.txt",NULL};
  QString codigo, descripcion, cif, importe, cp;
  
  if (fichero != NULL) {
      args[0]= fichero;
      args[1]= fichero;
   }// end if
   FILE *mifile;
   mifile = fopen(args[0],"wt");
   if (mifile != NULL) {   
   	fprintf(mifile, "LISTADO 347\n");
	fprintf(mifile, "\nCuenta	 Deudor 				 	    CIF/NIF     CP	Importe\n");
	fprintf(mifile, "________________________________________________________________________________________\n");
   	numventas = tablaventas->numRows();
  	for(i=0;i<numventas;i++) {
    	  codigo = tablaventas->text(i,0);
      	  descripcion = tablaventas->text(i,1);
      	  cif = tablaventas->text(i,2);
	  cp = tablaventas->text(i,3);
      	  importe = tablaventas->text(i,4);
	  fprintf(mifile, "%s %-50s %9s %6s %12.2f\n", codigo.ascii(), descripcion.ascii(), cif.ascii(), cp.ascii(), importe.toFloat());
  	}
	numcompras = tablacompras->numRows();
	fprintf(mifile, "\nCuenta	 Acreedor				 	    CIF/NIF     CP	Importe\n");
	fprintf(mifile, "________________________________________________________________________________________\n");
   	numcompras = tablacompras->numRows();
  	for(i=0;i<numcompras;i++) {
    	  codigo = tablacompras->text(i,0);
      	  descripcion = tablacompras->text(i,1);
      	  cif = tablacompras->text(i,2);
	  cp = tablacompras->text(i,3);
      	  importe = tablacompras->text(i,4);
	  fprintf(mifile, "%s %-50s %9s %6s %12.2f\n", codigo.ascii(), descripcion.ascii(), cif.ascii(), cp.ascii(), importe.toFloat());
  	}

	fclose(mifile);
   }// end if

#ifndef WIN32
   if ((pid=fork()) < 0) {
        perror ("Fork failed");
        exit(errno);
   }// end if
   if (!pid) {
	error = execvp(confpr->valor(CONF_EDITOR).ascii(),args);
   }// end if
#endif
}
