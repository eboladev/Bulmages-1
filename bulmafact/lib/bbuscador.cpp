/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
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

#include <stdarg.h>

#include "bbuscador.h"

#define MAXCOLUMNS 10

Bbuscador::Bbuscador(QWidget* parent, const char* name, WFlags f,BfEmpresa* punteroEmpresaTrabajo, QString* cadenaRet,QString* tabla, QString* args, ...) : UIbuscador(parent,name,f)
//Bbuscador::Bbuscador(QString* cadenaRet, QWidget* parent, const char* name, WFlags f) : UIbuscador(parent,name,f)
{
    int i=0;
    QString taula[MAXCOLUMNS];
    va_list args_ptr;
    va_start(args_ptr, args);
    while ( args && i < MAXCOLUMNS ) {
        taula[i++] = *args;
        args = va_arg(args_ptr, QString *);
    }
    va_end(args_ptr);
    
    EmpresaTrabajo=punteroEmpresaTrabajo;
    cadRetorno=cadenaRet;
    tablaResultados=new MyQTable(this,"tablaResultados");
    tablaResultados->setGeometry(10,80,550,260);
    tablaResultados->setNumCols(i);
    textLabel1->setText(taula[0]);
    for (int k=0; k<i; k++){
        tablaResultados->horizontalHeader()->setLabel(k,taula[k]);
    }
    tablaResultados->setSelectionMode( QTable::SingleRow );
    tablaResultados->setReadOnly(TRUE);
    llenarTabla("");
    connect(tablaResultados, SIGNAL(keyEnterPressed(int, int)), this, SLOT(pulsadoEnter(int, int)));
    setCaption("Buscar en la Tabla: " + *tabla);
   
}


Bbuscador::~Bbuscador(){
}

void Bbuscador::llenarTabla(const QString &cadena) {
    /*
    int i=0;
    QString query;
    postgresiface2 *DBconn= new postgresiface2();
    cursor2 *recordSet;
    DBconn->inicializa("proves2004");
    DBconn->begin();
    query.sprintf("SELECT codigo, descripcion FROM cuenta WHERE codigo LIKE '%s%%' ORDER BY codigo",cadena.ascii());
    recordSet = DBconn->cargacursor(query,"cuentas");
    DBconn->commit();
    tablacuentas->setNumRows(recordSet->numregistros());
    while (!recordSet->eof()) {
        tablacuentas->setText(i,0,recordSet->valor("codigo"));
        tablacuentas->setText(i,1,recordSet->valor("descripcion"));
        i++;
        recordSet->siguienteregistro();
    }
    delete DBconn;
    delete recordSet;
    */
}


void Bbuscador::pulsadoEnter(int fila, int columna){
    columna=0;
    *cadRetorno = tablaResultados->text(fila,columna);
    delete this;
}


