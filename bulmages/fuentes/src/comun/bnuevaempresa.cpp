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
#include "bnuevaempresa.h"


BNuevaEmpresa::BNuevaEmpresa(QWidget * parent, const char * name, WFlags f)
 : nuevaempresadlg(parent,name,f)
{
}


BNuevaEmpresa::~BNuevaEmpresa()
{
}

void BNuevaEmpresa::accept()
{
  QString nombredb;
  QString nombreEmp;
  int ejercicio;

  nombredb = bdempresa->text().stripWhiteSpace()+ejercicioempresa->text().stripWhiteSpace();
  nombreEmp = nombreempresa->text().stripWhiteSpace();
  ejercicio = ejercicioempresa->text().stripWhiteSpace().toInt();
  //Creamos una base de datos a imagen y semejanza de bgplangcont.
  postgresiface2 *conexionbase;
  conexionbase = new postgresiface2();
  conexionbase->inicializa(confpr->valor(CONF_METABASE).c_str());
  //conexionbase->ejecuta("DROP DATABASE " + nombredb);
  conexionbase->ejecuta("CREATE DATABASE " + nombredb + " WITH TEMPLATE=bgplangcont");
   // Aqui vamos a insertar la fila correspondiente en la metabd
   QString query;
   cursor2 *cursoraux;
   conexionbase->begin();
   query.sprintf("INSERT INTO  empresa (nombre, ano, nombredb) VALUES('%s',%d,'%s')",nombreEmp.ascii(),ejercicio,nombredb.ascii());
   conexionbase->ejecuta(query);
   // Agregamos el SUPERUSUARIO "BULMAGES" en la nueva empresa.
   //Los demas usuarios tendran que ser entrados manualmente por el SUPERUSUARIO.
   query.sprintf("SELECT idempresa from empresa where nombredb='%s'",nombredb.ascii());
   cursoraux = conexionbase->cargacursor(query,"cursoraux");
   int idempresa = cursoraux->valor(0).toInt();
   query.sprintf("SELECT idusuario from usuario where login='bulmages'");
   cursoraux = conexionbase->cargacursor(query,"cursoraux");
   int idSuperUser= cursoraux->valor(0).toInt();
   query.sprintf("INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,1)",idSuperUser,idempresa);
   conexionbase->ejecuta(query);

   conexionbase->commit();
   delete conexionbase;
   delete this;
}
