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
comboBoxFuente->insertStringList( (new QFontDatabase)->families() );
cargarFichaUtilidades();
cargarFichaUsuarios();
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


void BConfiguracion::FontChanged(const QString & fuente) {
muestra->setFont(QFont(fuente,spinBoxFuente->value()));
}

void BConfiguracion::FontSizeChanged(int tamano) {
muestra->setFont(QFont(comboBoxFuente->currentText(),tamano));
}

void BConfiguracion::BotonA_10aceptar() {
//Salvar en la base de datos las preferencias del usuario: Idioma, Fuente, ...
  string codigoPais;
  switch (comboBoxIdioma->currentItem()) {
      case 0: codigoPais = "ca"; break;
      case 1: codigoPais = "es"; break;
      case 2: codigoPais = "fr"; break;
      case 3: codigoPais = "en"; break;
  }
  confpr->setValor(CONF_TRADUCCION,codigoPais);
  confpr->setValor(CONF_FONTFAMILY_BULMAGES,comboBoxFuente->currentText().latin1());
  confpr->setValor(CONF_FONTSIZE_BULMAGES,spinBoxFuente->text().latin1());

}

void BConfiguracion::BotonA_11rechazar() {
//poner el comboBoxFuente y el comboBoxIdioma a sus valores anteriores.
cargarFichaUtilidades();
}

void BConfiguracion::cerrar()
{
  //Cargo el nuevo Idioma
  string archivo;
  theApp->removeTranslator( traductor );
  switch (comboBoxIdioma->currentItem()) {
      case 0: archivo = "bulmages_ca.qm"; break;
      case 1: archivo = "bulmages_es.qm"; break;
      case 2: archivo = "bulmages_fr.qm"; break;
      case 3: archivo = "bulmages_en.qm"; break;
  }
  traductor->load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
  theApp->installTranslator( traductor );
  
  //Cargo la nueva fuente
  theApp->setFont(QFont(comboBoxFuente->currentText(),spinBoxFuente->value()), TRUE);
  
  //Cierro la ventana de Configuración
  delete(this);
}

void BConfiguracion::cargarFichaUtilidades() {
//Carga idioma y fuente de la aplicacion 
  if (confpr->valor(CONF_TRADUCCION)=="ca") comboBoxIdioma->setCurrentItem(0);
  if (confpr->valor(CONF_TRADUCCION)=="es") comboBoxIdioma->setCurrentItem(1);
  if (confpr->valor(CONF_TRADUCCION)=="fr") comboBoxIdioma->setCurrentItem(2);
  if (confpr->valor(CONF_TRADUCCION)=="en") comboBoxIdioma->setCurrentItem(3);
  
  int i=0;
  while ( comboBoxFuente->text(i) != confpr->valor(CONF_FONTFAMILY_BULMAGES).c_str() &&  i < comboBoxFuente->count()) ++i;
  comboBoxFuente->setCurrentItem(i);
  spinBoxFuente->setValue(atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).c_str()));
}

void BConfiguracion::cargarFichaUsuarios() {
//Carga los Usuarios de la Base de Datos   
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
