/***************************************************************************
                          usuariosview.cpp  -  description
                             -------------------
    begin                : vie abr 4 2003
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

#include "usuariosview.h"

#define COL_IDUSUARIO 0
#define COL_NOMBRE    1
#define COL_APELLIDO1 2
#define COL_APELLIDO2 3
#define COL_APELLIDOS 4
#define COL_LOGIN     5
#define COL_PASSWORD  6

usuariosview::usuariosview(QWidget *parent, const char *name ) : usuariosdlg(parent,name) {
   semaforo= 0;
   listusuarios->addColumn("idusuario", 0);
   listusuarios->addColumn("Nombre", -1);
   listusuarios->addColumn("apellido1", 0);
   listusuarios->addColumn("apellido2", 0);
   listusuarios->addColumn("Apellidos", -1);
   listusuarios->addColumn("Login", -1);
   listusuarios->addColumn("Password", 0);   
   inicializa();
}// ebd usuariosview


usuariosview::~usuariosview(){
}// end ~usuariosview


void usuariosview::inicializa() {
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa(confpr->valor(CONF_METABASE).c_str());
   QListViewItem *it;
   cursor2 *a;
   listusuarios->clear();


   apuestatealgo->begin();
   a=apuestatealgo->cargacursor("SELECT * from usuario","cursor");
   apuestatealgo->commit();
   while (!a->eof()) {
         it =new QListViewItem(listusuarios);
         it->setText(COL_IDUSUARIO,a->valor("idusuario"));
         it->setText(COL_NOMBRE,a->valor("nombre"));
         it->setText(COL_APELLIDO1,a->valor("apellido1"));
         it->setText(COL_APELLIDO2,a->valor("apellido2"));
         it->setText(COL_APELLIDOS,a->valor("apellido1")+" "+a->valor("apellido2"));
         it->setText(COL_LOGIN, a->valor("login"));
         it->setText(COL_PASSWORD, a->valor("password"));
         a->siguienteregistro();
   }// end while
   delete a;
   delete apuestatealgo;   
}// end inicializa

void usuariosview::listusuarios_currentChanged(QListViewItem *it) {
  semaforo=1;
  nombreedit->setText(it->text(COL_NOMBRE).latin1());
  apellido1edit->setText(it->text(COL_APELLIDO1).latin1());
  apellido2edit->setText(it->text(COL_APELLIDO2).latin1());
  loginedit->setText(it->text(COL_LOGIN).latin1());
  passwordedit->setText(it->text(COL_PASSWORD).latin1());
  semaforo=0;
}// end listusuarios_currentChanged


void usuariosview::accept() {
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa(confpr->valor(CONF_METABASE).c_str());
   QString query;
   QListViewItem *it;
   fprintf(stderr,"Aceptamos\n");
   it = listusuarios->firstChild();
   while (it != 0) {
      query.sprintf("UPDATE usuario SET nombre='%s', apellido1='%s', apellido2='%s', login='%s', password='%s' WHERE idusuario=%s", it->text(COL_NOMBRE).latin1(), it->text(COL_APELLIDO1).latin1(), it->text(COL_APELLIDO2).latin1(), it->text(COL_LOGIN).latin1(), it->text(COL_PASSWORD).latin1(), it->text(COL_IDUSUARIO).latin1());
//      sprintf(cadena,"UPDATE usuario SET nombre='%s', 1apellido='%s' WHERE idusuario=%s", it->text(COL_NOMBRE).latin1(), it->text(COL_1APELLIDO).latin1(),it->text(COL_IDUSUARIO).latin1());
      apuestatealgo->begin();
      apuestatealgo->ejecuta(query);
      apuestatealgo->commit();
      it = it->nextSibling();
   }// end while
   delete apuestatealgo;
   done(1);
}// end accept


void usuariosview::datos_textChanged(const QString &st) {
   fprintf(stderr,"cambios en el nombre\n");
   if(!semaforo) {
      QListViewItem *it = listusuarios->currentItem();
      it->setText(COL_NOMBRE, nombreedit->text());
      it->setText(COL_APELLIDO1, apellido1edit->text());
      it->setText(COL_APELLIDO2, apellido2edit->text());
      it->setText(COL_LOGIN, loginedit->text());
      it->setText(COL_PASSWORD, passwordedit->text());
      QString cadena;
      cadena.sprintf("%s %s",(char *) apellido1edit->text().latin1(), (char *) apellido2edit->text().latin1());
      it->setText(COL_APELLIDOS, cadena);
   }// end if
}// end nombreedit_textChanged


void usuariosview::nuevo_usuario() {
   fprintf(stderr,"Nuevo usuario\n");
   QString query;
   query.sprintf("INSERT INTO usuario (nombre) VALUES ('%s')",tr("Nuevo Usuario").latin1());
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa(confpr->valor(CONF_METABASE).c_str());
   apuestatealgo->begin();
   apuestatealgo->ejecuta(query);
   apuestatealgo->commit();
   delete apuestatealgo;
   inicializa();
}// end nuevo_usuario


void usuariosview::borrar_usuario() {
   QListViewItem *it = listusuarios->currentItem();
   QString query;
   query.sprintf("DELETE FROM usuario WHERE idusuario=%s",it->text(COL_IDUSUARIO).latin1());
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa(confpr->valor(CONF_METABASE).c_str());
   apuestatealgo->begin();
   apuestatealgo->ejecuta(query);
   apuestatealgo->commit();
   listusuarios->takeItem(it);
   delete apuestatealgo;
   delete it;
//   inicializa();
}// end borrar_usuario

