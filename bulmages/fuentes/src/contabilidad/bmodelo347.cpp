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
  click_boto3(); //carga las tablas en pantalla;
}


BModelo347::~BModelo347()
{
}

void BModelo347::click_boto3() {
  int i =0;
  QString query;
  //A por la tabla de Ventas...pedazo de consulta SQL
  query= QString("SELECT codigo, descripcion, cifent_cuenta as cif, importe FROM (SELECT  registroiva.contrapartida, SUM(baseimp+(baseimp*iva*0.01)) AS importe FROM registroiva, borrador WHERE borrador.idborrador=registroiva.idborrador AND borrador.fecha >= '")+finicial->text() + QString("' AND borrador.fecha <= '") + ffinal->text() + QString("' GROUP BY registroiva.contrapartida) AS parcial, cuenta WHERE parcial.contrapartida=cuenta.idcuenta AND cuenta.codigo LIKE '430%' AND importe >='") + importe->text() + QString("'");
  DBConn->begin();
  cursor2 *recordSet = DBConn->cargacursor(query,"recordSet");
  DBConn->commit();
  tablaventas->setNumRows(recordSet->numregistros());
  i=0;
  while (!recordSet->eof()) {
      tablaventas->setText(i,0,recordSet->valor("codigo"));
      tablaventas->setText(i,1,recordSet->valor("descripcion"));
      tablaventas->setText(i,2,recordSet->valor("cif"));
      query.setNum(recordSet->valor("importe").toFloat(),'f',2); //round a 2 decimales
      tablaventas->setText(i,3,query);
      ++i;
      recordSet->siguienteregistro();
  }

  //A por la tabla de Compras...
  query= QString("SELECT codigo, descripcion, cifent_cuenta as cif, importe FROM (SELECT  registroiva.contrapartida, SUM(baseimp+(baseimp*iva*0.01)) AS importe FROM registroiva, borrador WHERE borrador.idborrador=registroiva.idborrador AND borrador.fecha >= '")+finicial->text() + QString("' AND borrador.fecha <= '") + ffinal->text() + QString("' GROUP BY registroiva.contrapartida) AS parcial, cuenta WHERE parcial.contrapartida=cuenta.idcuenta AND cuenta.codigo LIKE '400%' AND importe >='") + importe->text() + QString("'");
  DBConn->begin();
  recordSet = DBConn->cargacursor(query,"recordSet");
  DBConn->commit();
  tablacompras->setNumRows(recordSet->numregistros());
  i=0;
  while (!recordSet->eof()) {
      tablacompras->setText(i,0,recordSet->valor("codigo"));
      tablacompras->setText(i,1,recordSet->valor("descripcion"));
      tablacompras->setText(i,2,recordSet->valor("cif"));
      query.setNum(recordSet->valor("importe").toFloat(),'f',2); //round a 2 decimales
      tablacompras->setText(i,3,query);
      ++i;
      recordSet->siguienteregistro();
  }
}


