/***************************************************************************
                          propiedadesempresa.cpp  -  description
                             -------------------
    begin                : Fri Nov 29 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
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

#include "propiedadesempresa.h"

propiedadesempresa::propiedadesempresa(QWidget *parent, const char *name, bool modal) : propiedemp(parent,name,modal) {
  usuarios->addColumn(tr("idusuario"),0);
  usuarios->addColumn(tr("login"),0);
  usuarios->addColumn(tr("Nombre"),-1);
  usuarioslectura->addColumn(tr("idusuario"),0);
  usuarioslectura->addColumn(tr("login"),0);
  usuarioslectura->addColumn(tr("Nombre"),-1);
  usuariostotal->addColumn(tr("idusuario"),0);
  usuariostotal->addColumn(tr("login"),0);
  usuariostotal->addColumn(tr("Nombre"),-1);
}// end propiedadesempresa


propiedadesempresa::~propiedadesempresa(){
}// end ~propiedadesempresa


int propiedadesempresa::inicializa(postgresiface2 *conn, QString nomdb) {
    int num;
    int i;
    nombredb = nomdb;
    
    QString query = "SELECT * FROM configuracion WHERE nombre='CodCuenta'";
    conexionbase = conn;
    conexionbase->begin();
    cursor2 *curs = conexionbase->cargacursor(query,"query");
    conexionbase->commit();
    num = curs->numregistros();
    if (num >0) {
       modcodigo->setText(curs->valor(2));
    }// end if
    delete curs;

    tpropiedades->setNumCols(2);
    query = "SELECT * FROM configuracion";
    conexionbase->begin();
    curs = conexionbase->cargacursor(query,"queryconf");
    conexionbase->commit();
    num=curs->numregistros();
    // Hacemos que no haya ningún apunte como preestablecido.
    tpropiedades->setNumRows(num);
    i=0;
    while (!curs->eof()) {
      tpropiedades->setText(i,0,curs->valor("nombre"));
      tpropiedades->setText(i,1,curs->valor("valor"));
      curs->siguienteregistro();
      i++;
    }// end while
    delete curs;


    // Leemos los usuarios que no pertenecen a esta empresa y los introducimos.
    // Recordatorio de permisos:
    // 1.- Total
    // 2.- Lectura.
    QListViewItem * it;
    postgresiface2 *metabase;
    cursor2 *cursoraux;
    metabase = new postgresiface2();
	 QString nommetabase = confpr->valor(CONF_METABASE).c_str();
    metabase->inicializa(nommetabase);
    metabase->begin();
    query = "SELECT * FROM usuario WHERE idusuario NOT IN (SELECT idusuario FROM usuario_empresa, empresa WHERE usuario_empresa.idempresa=empresa.idempresa AND empresa.nombredb ='"+nombredb+"')";
    cursoraux = metabase->cargacursor(query,"cursorusuario");
    metabase->commit();
    while (!cursoraux->eof()) {
      it =new QListViewItem(usuarios);
      it->setText(1,cursoraux->valor(4));
      it->setText(2,(cursoraux->valor(1)+cursoraux->valor(2)));
      it->setText(0,cursoraux->valor(0));
      cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;

    // Leemos los usuarios que pertenecen a esta empresa, son de acceso lectura. y los introducimos.
    metabase->begin();
    query = "SELECT * FROM usuario WHERE idusuario IN (SELECT idusuario FROM usuario_empresa, empresa WHERE usuario_empresa.idempresa=empresa.idempresa AND empresa.nombredb ='"+nombredb+"' AND permisos=2)";
    cursoraux = metabase->cargacursor(query,"cursorusuario");
    metabase->commit();
    while (!cursoraux->eof()) {
      it =new QListViewItem(usuarioslectura);
      it->setText(1,cursoraux->valor(4));
      it->setText(2,(cursoraux->valor(1)+cursoraux->valor(2)));
      it->setText(0,cursoraux->valor(0));
      cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;

    // Leemos los usuarios que pertenecen a esta empresa, son de acceso total. y los introducimos.
    metabase->begin();
    query = "SELECT * FROM usuario WHERE idusuario IN (SELECT idusuario FROM usuario_empresa, empresa WHERE usuario_empresa.idempresa=empresa.idempresa AND empresa.nombredb ='"+nombredb+"' AND permisos=1)";
    fprintf(stderr,"%s\n",query.latin1());
    cursoraux = metabase->cargacursor(query,"cursorusuario");
    metabase->commit();
    while (!cursoraux->eof()) {
      it =new QListViewItem(usuariostotal);
      it->setText(1,cursoraux->valor(4));
      it->setText(2,(cursoraux->valor(1)+cursoraux->valor(2)));
      it->setText(0,cursoraux->valor(0));
      cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;

    
    delete metabase;     
    
    return(0);
}// end inicializa


void propiedadesempresa::accept() {
/*
   char *cadena;
   postgresiface *piface = new postgresiface();
   piface->inicializa(empresadb);
   piface->modificaconfiguracion("CodCuenta",(char *)modcodigo->text().latin1());
   fprintf(stderr,"accept;Pulsado\n");
   cadena =  (char *) modcodigo->text().latin1();
   modificacodcuenta(cadena);
   delete piface;
   done(1);
*/

   postgresiface2 *metabase;
   cursor2 *cursoraux;
   QString query;

   metabase = new postgresiface2();
   metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
   metabase->begin();
   query.sprintf("DELETE FROM usuario_empresa WHERE idempresa IN (SELECT idempresa FROM empresa WHERE nombredb='%s')",nombredb.latin1());
   metabase->ejecuta(query);

   query.sprintf("SELECT idempresa from empresa WHERE nombredb='%s'",nombredb.latin1());
   cursoraux = metabase->cargacursor(query,"cursorempresa");
   int idempresa = atoi(cursoraux->valor(0).latin1());
   delete cursoraux;

   
   // Hacemos la lectura de las listas de usuarios y los metemos en la base de datos.
   QListViewItem *item;
   int idusuario;
   item = usuarioslectura->firstChild();
   while (item) {
     idusuario = atoi(item->text(0).latin1()   );
     query.sprintf("INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,2)",idusuario,idempresa);
     metabase->ejecuta(query);
     item = item->nextSibling();
   }// end while
   item = usuariostotal->firstChild();
   while (item) {
     idusuario = atoi(item->text(0).latin1()   );
     query.sprintf("INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,1)",idusuario,idempresa);
     metabase->ejecuta(query);
     item = item->nextSibling();
   }// end while


   metabase->commit();
   delete metabase;


   done(1);
}// end accept


void propiedadesempresa::modificacodcuenta(char *plantilla) {
/*
   int num, i,k;
   unsigned int j;
   char *cadena;
   char cadaux[20];
   int idcuenta;
   int numdigits = strlen(plantilla);
   postgresiface *piface = new postgresiface();
   postgresiface *piface1;
   piface->inicializa(empresadb);
   num = piface->cargacuentas(-1);
   fprintf(stderr,"Digitos %d",numdigits);
   for(i=0;i<num;i++)  {
      for (k=0;k<numdigits;k++) {
         cadaux[k]='0';
      }// end for
      cadaux[numdigits]=0;
      cadena = piface->siguientecampo(1,i);
      for(j=0;j<strlen(cadena);j++) {
         cadaux[j] = cadena[j];
      }// end for
      idcuenta= atoi(piface->siguientecampo(0,i));
      piface1 = new postgresiface();
      piface1->inicializa(empresadb);
      piface1->modificacodcuenta(idcuenta, cadaux);
      delete piface1;
      fprintf(stderr,"%s\n",cadaux);
   }// end for
*/
}// end modificacodcuenta


void propiedadesempresa::boton_usalectura() {
  QListViewItem *it;
  it =usuarios->currentItem();
  usuarios->takeItem(it);
  usuarioslectura->insertItem(it);
}// end boton_usalectura

void propiedadesempresa::boton_lecturaaus() {
  QListViewItem *it;
  it =usuarioslectura->currentItem();
  usuarioslectura->takeItem(it);
  usuarios->insertItem(it);
}// end boton_lecturaaus

void propiedadesempresa::boton_usatotal() {
  QListViewItem *it;
  it =usuarios->currentItem();
  usuarios->takeItem(it);
  usuariostotal->insertItem(it);
}// end boton_usatotal

void propiedadesempresa::boton_totalaus() {
  QListViewItem *it;
  it =usuariostotal->currentItem();
  usuariostotal->takeItem(it);
  usuarios->insertItem(it);
}// end boton_totalaus
