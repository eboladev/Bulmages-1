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
 *                                                                         *
 ***************************************************************************/
#include "bbloqfecha.h"

BbloqFecha::BbloqFecha(QWidget * parent, const char * name, WFlags f) : UIbloqFecha(parent,name,f) {
  QListViewItem *listMain, *listAux=0;
  QString query;
  listView1->setSorting(-1);
  
  postgresiface2 *DBconn = new postgresiface2();
  DBconn->inicializa( "bulmages2003" );
  DBconn->begin();
  query.sprintf("SELECT * FROM ejercicios WHERE periodo=0 ORDER BY ejercicio DESC");
  cursor2 *curPeri,*curEjer = DBconn->cargacursor(query,"curEjer");
  DBconn->commit();
  while (!curEjer->eof()) {
        listMain = new QListViewItem(listView1,curEjer->valor("ejercicio"));
        curEjer->valor("bloqueado")=="t" ? listMain->setText(1,"Bloqueado") : listMain->setText(1,"Abierto");
        DBconn->begin();
        query.sprintf("SELECT * FROM ejercicios WHERE ejercicio='%s' ORDER BY periodo DESC",curEjer->valor("ejercicio").ascii());
        curPeri = DBconn->cargacursor(query,"curPeri");
        DBconn->commit();
        while (!curPeri->eof()) {
            switch (curPeri->valor("periodo").toInt()) {
                case 12: listAux = new QListViewItem(listMain,tr("Diciembre ")); break;
                case 11: listAux = new QListViewItem(listMain,tr("Noviembre ")); break;
                case 10: listAux = new QListViewItem(listMain,tr("Octubre"   )); break;
                case  9: listAux = new QListViewItem(listMain,tr("Septiembre")); break;
                case  8: listAux = new QListViewItem(listMain,tr("Agosto    ")); break;
                case  7: listAux = new QListViewItem(listMain,tr("Julio     ")); break;
                case  6: listAux = new QListViewItem(listMain,tr("Junio     ")); break;
                case  5: listAux = new QListViewItem(listMain,tr("Mayo      ")); break;
                case  4: listAux = new QListViewItem(listMain,tr("Abril     ")); break;
                case  3: listAux = new QListViewItem(listMain,tr("Marzo     ")); break;
                case  2: listAux = new QListViewItem(listMain,tr("Febrero   ")); break;
                case  1: listAux = new QListViewItem(listMain,tr("Enero     ")); break;
            }
            curPeri->valor("bloqueado")=="t" ? listAux->setText(1,"Bloqueado") : listAux->setText(1,"Abierto");   
            curPeri->siguienteregistro();
        }
        curEjer->siguienteregistro();
    }
    
}

BbloqFecha::~BbloqFecha()
{
}

void BbloqFecha::boto1_click()
{
delete this;
}

void BbloqFecha::dobleClick (QListViewItem *item) {
if (item->text(1)=="Bloqueado") item->setText(1,"Abierto");
else item->setText(1,"Bloqueado");
}



