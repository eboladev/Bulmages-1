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
    columnaRetorno = cadRetorno->toInt();
    tablaBusqueda = *tabla; //Tabla Base de Datos
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
    connect(tablaResultados, SIGNAL(doubleClicked(int, int, int, const QPoint&)), this, SLOT(dblClickRaton(int, int, int, const QPoint&)));
    setCaption("Buscar en la Tabla: " + tablaBusqueda);
    *cadRetorno="";
   
}


Bbuscador::~Bbuscador(){
}

void Bbuscador::llenarTabla(const QString &cadena) {
    int i;
    for (i=tablaResultados->numRows()-1; i>=0; i--) {
        tablaResultados->removeRow(i);
    }
    
    QString condicionBusqueda = "ILIKE '";
    if (checkMayMin->isChecked()) condicionBusqueda = "LIKE '";
    if (checkBuscarIntermedio->isChecked()) condicionBusqueda = condicionBusqueda + "%";
    
    i=0;
    BfCursor* recordset;
    recordset = EmpresaTrabajo->buscarParecidos(tablaBusqueda,cadena,condicionBusqueda);
    while (! recordset->eof() ) { 
        tablaResultados->insertRows(i);
        if (tablaBusqueda=="cliente") {
            tablaResultados->setText(i,0,recordset->valor("nomcliente"));
            tablaResultados->setText(i,1,recordset->valor("poblcliente"));
            tablaResultados->setText(i,2,recordset->valor("telcliente"));
            tablaResultados->setText(i,3,recordset->valor("idcliente"));
        }
        if (tablaBusqueda=="articulo") {
            tablaResultados->setText(i,0,recordset->valor("nomarticulo"));
            tablaResultados->setText(i,1,recordset->valor("codarticulo"));
            tablaResultados->setText(i,2,recordset->valor("descarticulo"));
            tablaResultados->setText(i,3,recordset->valor("idarticulo"));
        }
        if (tablaBusqueda=="proveedor") {
            tablaResultados->setText(i,0,recordset->valor("nomproveedor"));
            tablaResultados->setText(i,1,recordset->valor("poblproveedor"));
            tablaResultados->setText(i,2,recordset->valor("telproveedor"));
            tablaResultados->setText(i,3,recordset->valor("idproveedor"));
        }
        i++;
        recordset->siguienteregistro();
    }
}

void Bbuscador::aceptarBusqueda() {
    tablaResultados->setFocus();
}

void Bbuscador::keyPressEvent(QKeyEvent * e) {
    if (e->key() == Qt::Key_Down) aceptarBusqueda();
}

void Bbuscador::dblClickRaton(int row,int col,int button,const QPoint & mousePos) {
    pulsadoEnter(row, col);
}

void Bbuscador::pulsadoEnter(int fila, int columna){
    columna=columnaRetorno;
    *cadRetorno = tablaResultados->text(fila,columna);
    delete this;
}

