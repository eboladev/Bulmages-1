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
    tpropiedades->setNumRows(100);
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
    fprintf(stderr,"%s\n",query.ascii());
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
    
   //Este bloque de código realiza la consulta para obtener los datos fiscales de la empresa 
    QString empresa, ano;
   

  query="select nombre,valor from configuracion";//Tiene que usar la empresa elegida, no bulmages!!!! TODO

  metabase = new postgresiface2();
  metabase->inicializa("bulmages");//[TODO] CAMBIAR!!!
  metabase->begin();
      

   cursor2 *cur = metabase->cargacursor(query,"datos");
    
QString n,v;
  int nTuples=cur->numregistros();
  QLineEdit *p;
    for (int i=0;i<nTuples;i++)
    {
    p=NULL;
    n=cur->valor("nombre");
    v=cur->valor("valor");
    cur->siguienteregistro();
    
   if (n=="NombreEmpresa") p=lineNombreEmpresa;
    if (n=="CIF") p=lineCIF;
    if (n=="TipoVia") p=lineTipoVia;
    if (n=="NombreVia") p=lineNombreVia;
    if (n=="NumeroVia") p=lineNumeroVia;
    if (n=="Escalera") p=lineEscalera;
    if (n=="Piso") p=linePiso;
    if (n=="Puerta") p=linePuerta;
    if (n=="CodPostal") p=lineCodPostal;
    if (n=="Municipio") p=lineMunicipio;
    if (n=="Provincia") p=lineProvincia;
    if (n=="Pais") p=linePais;
    
    if (p) 
    {
    cout << n << " = " << v << "\n";
    p->setText(v);
    }
    }
    delete cur;
    delete metabase;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    return(0);
}// end inicializa


void propiedadesempresa::accept() {
    QString query = "DELETE FROM configuracion";
    conexionbase->begin();
    conexionbase->ejecuta(query);
    conexionbase->commit();
    int i=0;
    while (tpropiedades->text(i,0) != "") {
      QString SQLQuery;
      SQLQuery.sprintf("INSERT INTO configuracion (idconfiguracion, nombre, valor) VALUES (%d,'%s','%s')",i,tpropiedades->text(i,0).ascii(),tpropiedades->text(i,1).ascii());
      conexionbase->begin();
      conexionbase->ejecuta(SQLQuery);
      conexionbase->commit();
      i++;
    }// end while
    
   postgresiface2 *metabase;
   cursor2 *cursoraux;

   metabase = new postgresiface2();
   metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
   metabase->begin();
   query.sprintf("DELETE FROM usuario_empresa WHERE idempresa IN (SELECT idempresa FROM empresa WHERE nombredb='%s')",nombredb.ascii());
   metabase->ejecuta(query);

   query.sprintf("SELECT idempresa from empresa WHERE nombredb='%s'",nombredb.ascii());
   cursoraux = metabase->cargacursor(query,"cursorempresa");
   int idempresa = atoi(cursoraux->valor(0).ascii());
   delete cursoraux;

   
   // Hacemos la lectura de las listas de usuarios y los metemos en la base de datos.
   QListViewItem *item;
   int idusuario;
   item = usuarioslectura->firstChild();
   while (item) {
     idusuario = atoi(item->text(0).ascii()   );
     query.sprintf("INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,2)",idusuario,idempresa);
     metabase->ejecuta(query);
     item = item->nextSibling();
   }// end while
   item = usuariostotal->firstChild();
   while (item) {
     idusuario = atoi(item->text(0).ascii()   );
     query.sprintf("INSERT INTO usuario_empresa(idusuario,idempresa,permisos) VALUES (%d,%d,1)",idusuario,idempresa);
     metabase->ejecuta(query);
     item = item->nextSibling();
   }// end while


   metabase->commit();
   delete metabase;

   //Este bloque de codigo guarda los datos fiscales en la tabla configuracion
   
   
conexionbase->begin();
update_value(conexionbase,"NombreEmpresa",lineNombreEmpresa->text());
update_value(conexionbase,"CIF",lineCIF->text());
update_value(conexionbase,"TipoVia",lineTipoVia->text());
update_value(conexionbase,"NombreVia",lineNombreVia->text());
update_value(conexionbase,"NumeroVia",lineNumeroVia->text());
update_value(conexionbase,"Escalera",lineEscalera->text());
update_value(conexionbase,"Piso",linePiso->text());
update_value(conexionbase,"Puerta",linePuerta->text());
update_value(conexionbase,"CodPostal",lineCodPostal->text());
update_value(conexionbase,"Municipio",lineMunicipio->text());
update_value(conexionbase,"Provincia",lineProvincia->text());
update_value(conexionbase,"Pais",linePais->text());
conexionbase->commit();
   done(1);
}// end accept




/*
void propiedadesempresa::modificacodcuenta(char *plantilla) {
}// end modificacodcuenta
*/

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




void propiedadesempresa::update_value(postgresiface2 *m,QString n,QString v)
{
QString query="SELECT * from configuracion where nombre='"+n+"'";
 cursor2 *cur = m->cargacursor(query,"configuracion");
 if (cur->numregistros()==0) 
 {
 query.sprintf("INSERT into configuracion (idconfiguracion,nombre,valor) values ((select max(idconfiguracion)+1 from configuracion),'%s','%s')",n.ascii(),v.ascii());
 }
 else
query="UPDATE configuracion set valor='"+v+"' where nombre='"+n+"'";

delete cur;
  cout << m->ejecuta(query) << "\n";
  
}
