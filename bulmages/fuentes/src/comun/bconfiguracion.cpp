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
#include "bconfiguracion.h"


BConfiguracion::BConfiguracion(QWidget * parent, const char * name, WFlags f)
 : UIconfiguracion(parent,name,f)
{
   postgresiface2 * conexionDB;
   conexionDB = new postgresiface2();
   conexionDB->inicializa(confpr->valor(CONF_METABASE).c_str());
   cursor2 * recordSet;
   QListViewItem *it;

   conexionDB->begin();
   recordSet=conexionDB->cargacursor("SELECT * from usuario","recordSet");
   conexionDB->commit();
   while (!recordSet->eof()) {
         it =new QListViewItem(listView1);
         it->setText(0,recordSet->valor("login"));
         recordSet->siguienteregistro();
   }// end while
   delete recordSet;
   delete conexionDB;   

}

BConfiguracion::~BConfiguracion()
{
}

void BConfiguracion::listView1_currentChanged(QListViewItem *it) {
  lineEdit1->setText(it->text(0).latin1());
  lineEdit2->setText("login");
  lineEdit3->setText("login");
  lineEdit4->setText("login");
  lineEdit5->setText("login");
  textEdit1->setText("Comentarios Varios...");

}// end listView1_currentChanged

void BConfiguracion::listView2_clickBotonDerecho(QListViewItem* item,const QPoint& coord ,int col){
   QPopupMenu *popup;
   popup = new QPopupMenu;
        popup->insertItem(tr("S - Acceso Permitido"),101);
        popup->insertItem(tr("N - Acceso Denegado"),102);
        popup->insertItem(tr("V - Acceso Restringido"),103);
        //popup->insertSeparator();
   col = popup->exec(coord);
   delete popup;
   item=listView2->firstChild();
   while (item != 0) {
       if ( item->isSelected() ) {
           switch(col) {
               case 101: item->setText(0,"S"); break;
               case 102: item->setText(0,"N"); break;
               case 103: item->setText(0,"V"); break;
          }
       }       
       item = item->itemBelow();
   }
}


void BConfiguracion::cerrar()
{
delete(this);
}
