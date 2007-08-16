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
Empresa  "bvisorempresas.h"

QString * seleccion;

BVisorEmpresas::BVisorEmpresas(QString * ptrRetorno, QWidget * parent, const char * name, WFlags f)
 : UIvisorEmpresas(parent,name,f)
{
   seleccion = ptrRetorno; //puntero al QString que sera retornado al cerrar.
   listView1->setColumnWidth(0,(int)((listView1->width()-20)*0.75));
   listView1->setColumnWidth(1,listView1->width() - listView1->columnWidth(0)-20);
//   listView1->hideColumn(2);
   QListViewItem *linea;
}


BVisorEmpresas::~BVisorEmpresas()
{

}

void BVisorEmpresas::botonAceptar()
{
*seleccion = listView1->currentItem()->text(2);
close();
}

void BVisorEmpresas::botonCancelar()
{
seleccion=NULL;
close();
}
