/***************************************************************************
                          nuevaempresaview.cpp  -  description
                             -------------------
    begin                : Mon Jan 20 2003
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

#include "nuevaempresaview.h"
#include <unistd.h>

nuevaempresaview::nuevaempresaview(QWidget *parent, const char *name, bool modal) : nuevaempresadlg(parent,name, modal) {
  empresacreada=0;
  usuarios->addColumn("idusuario",0);
  usuarios->addColumn("login",0);
  usuarios->addColumn("Nombre",-1);
  usuarioslectura->addColumn("idusuario",0);
  usuarioslectura->addColumn("login",0);
  usuarioslectura->addColumn("Nombre",-1);
  usuariostotal->addColumn("idusuario",0);
  usuariostotal->addColumn("login",0);
  usuariostotal->addColumn("Nombre",-1);
  
  QListViewItem * it;
  postgresiface2 *metabase;
  cursor2 *cursoraux;
  metabase = new postgresiface2();
  metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
  metabase->begin();
  cursoraux = metabase->cargacursor("SELECT * FROM  usuario","cursorusuario");
  metabase->commit();
  while (!cursoraux->eof()) {
    it =new QListViewItem(usuarios);
    it->setText(1,cursoraux->valor(4));
    it->setText(2,(cursoraux->valor(1)+cursoraux->valor(2)));
    it->setText(0,cursoraux->valor(0));
    cursoraux->siguienteregistro();
  }// end while
  delete cursoraux;
  delete metabase;   
}// end nuevaempresaview


nuevaempresaview::~nuevaempresaview(){
}


void nuevaempresaview::accept() {
  crearempresa();
}// end accept

/***************************************************************
 * Esta funcion se encarga de crear la empresa a partir de los *
 * datos que estan en el formulario.                           *
 ***************************************************************/
void nuevaempresaview::crearempresa() {
  char *args[4];
  string nombredb=bdempresa->text().ascii();
  string nombre=nombreempresa->text().ascii();
  int ano=atoi(ejercicioempresa->text().ascii());

  args[0]="nuevaemp";
  args[1]=(char *) nombredb.c_str();
  args[2]="/usr/share/bulmages/dbmodels/bgplangcont.pgdump";
  args[3]=NULL;
  
  int pid, error;  
  fprintf(stderr,"Creacion de la empresa\n");
  if ((pid=fork()) < 0) {
        perror ("Fork failed");
        exit(errno);
  }// end if
  if (pid==0) {
     error = execvp("/usr/local/bin/nuevaemp",args);
     exit(0);
  } else {
        waitpid (pid, NULL, 0);
  }// end if

   // Aqui vamos a insertar la columna correspondiente en la metabd
   char query[400];
   postgresiface2 *metabase;
   cursor2 *cursoraux;
   metabase = new postgresiface2();
   metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
   metabase->begin();
   sprintf(query,"INSERT INTO  empresa (nombre, ano, nombredb) VALUES('%s',%d,'%s')",nombre.c_str(),ano,nombredb.c_str());
   metabase->ejecuta(query);
   sprintf(query,"SELECT max(idempresa) from empresa");
   cursoraux = metabase->cargacursor(query,"cursorempresa");
   int idempresa = atoi(cursoraux->valor(0).ascii());

   // Hacemos la lectura de las listas de usuarios y los metemos en la base de datos.
   QListViewItem *item;
   int idusuario;
   item = usuarioslectura->firstChild();
   while (item) {
     idusuario = atoi(item->text(0).ascii()   );
     sprintf(query,"INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,2)",idusuario,idempresa);
     metabase->ejecuta(query);
     item = item->nextSibling();
   }// end while
   item = usuariostotal->firstChild();
   while (item) {
     idusuario = atoi(item->text(0).ascii()   );
     sprintf(query,"INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,1)",idusuario,idempresa);
     metabase->ejecuta(query);
     item = item->nextSibling();
   }// end while

      
   metabase->commit();
   delete cursoraux;
   delete metabase;
   empresacreada=1;
   close();
}// end crearempresa


void nuevaempresaview::boton_usalectura() {
  QListViewItem *it;
  it =usuarios->currentItem();
  usuarios->takeItem(it);
  usuarioslectura->insertItem(it);
}// end boton_usalectura

void nuevaempresaview::boton_lecturaaus() {
  QListViewItem *it;
  it =usuarioslectura->currentItem();
  usuarioslectura->takeItem(it);
  usuarios->insertItem(it);
}// end boton_lecturaaus

void nuevaempresaview::boton_usatotal() {
  QListViewItem *it;
  it =usuarios->currentItem();
  usuarios->takeItem(it);
  usuariostotal->insertItem(it);
}// end boton_usatotal

void nuevaempresaview::boton_totalaus() {
  QListViewItem *it;
  it =usuariostotal->currentItem();
  usuariostotal->takeItem(it);
  usuarios->insertItem(it);
}// end boton_totalaus
