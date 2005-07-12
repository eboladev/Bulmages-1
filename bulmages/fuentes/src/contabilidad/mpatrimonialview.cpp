/***************************************************************************
                          mpatrimonialview.cpp  -  description
                             -------------------
    begin                : mié sep 10 2003
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

#include "mpatrimonialview.h"
#include "mpatrimonialesview.h"
#include <qlineedit.h>
#include <qlistview.h>
//#include <qstring.h>

mpatrimonialview::mpatrimonialview(QWidget *parent, const char *name ) : mpatrimonialdlg(parent,name) {
  conexionbase=NULL;
  idmpatrimonial = "";

  componentessuma->addColumn(tr("identificador"),0);
  componentessuma->addColumn(tr("codigo"),0);
  componentessuma->addColumn(tr("descripcion"),-1);
  componentessuma->addColumn(tr("tipo"),0);
  componentesresta->addColumn(tr("identificador"),0);
  componentesresta->addColumn(tr("codigo"),0);
  componentesresta->addColumn(tr("descripcion"),-1);
  componentesresta->addColumn(tr("tipo"),-1);
}


mpatrimonialview::~mpatrimonialview(){
}

/*************************************************************
 * Esta funcion se encarga de hacer las inicializaciones de todo el formulario.
 * Se llama asi y no desde el constructor pq asi la podemos llamar desde dentro
 * de la misma clase, etc etc etc
 *************************************************************/
int mpatrimonialview::inicializa(postgresiface2 *conn ) {
   conexionbase = conn;
   return(0);
}// end inicializa


void mpatrimonialview::inicializa1(QString idmpatrimonial1) {
    idmpatrimonial = idmpatrimonial1;
    QString query = "SELECT * from mpatrimonial WHERE idmpatrimonial="+idmpatrimonial;
    conexionbase->begin();
    cursor2 *cursor = conexionbase->cargacursor(query,"micursor");
    conexionbase->commit();
    if (!cursor->eof() ) {
       descmpatrimonial->setText(cursor->valor("descmpatrimonial"));
    }// end if
    delete cursor;


    // Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente="+idmpatrimonial+" AND signo=true)";
    cursor2 *cursoraux = conexionbase->cargacursor(query,"cursorusuario");
    conexionbase->commit();
    QListViewItem *it;
    while (!cursoraux->eof()) {
      it =new QListViewItem(componentessuma);
      it->setText(1,cursoraux->valor("codigo"));
      it->setText(2,cursoraux->valor("descripcion"));
      it->setText(3,"cuenta");
      it->setText(0,cursoraux->valor("idcuenta"));
      cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;


    // Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente="+idmpatrimonial+" AND signo=true)";
    cursoraux = conexionbase->cargacursor(query,"cursorusuario");
    conexionbase->commit();
    while (!cursoraux->eof()) {
      it =new QListViewItem(componentessuma);
      it->setText(1,"");
      it->setText(2,cursoraux->valor("descmpatrimonial"));
      it->setText(3,"masa patrimonial");
      it->setText(0,cursoraux->valor("idmpatrimonial"));
      cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;
    
    // Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT idcuenta FROM compmasap WHERE masaperteneciente="+idmpatrimonial+" AND signo=false)";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query,"cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
      it =new QListViewItem(componentesresta);
      it->setText(1,cursoraux1->valor("codigo"));
      it->setText(2,cursoraux1->valor("descripcion"));
      it->setText(3,"cuenta");
      it->setText(0,cursoraux1->valor("idcuenta"));
      cursoraux1->siguienteregistro();
    }// end while   
    delete cursoraux1;

    // Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial IN (SELECT idmpatrimonial FROM compmasap WHERE masaperteneciente="+idmpatrimonial+" AND signo=false)";
    cursoraux = conexionbase->cargacursor(query,"cursorusuario");
    conexionbase->commit();
    while (!cursoraux->eof()) {
      it =new QListViewItem(componentesresta);
      it->setText(1,"");
      it->setText(2,cursoraux->valor("descmpatrimonial"));
      it->setText(3,"masa patrimonial");
      it->setText(0,cursoraux->valor("idmpatrimonial"));
      cursoraux->siguienteregistro();
    }// end while
    delete cursoraux;


}// end inicializa1


/********************************************************************************
 * Se ha pulsado sobre el boton de agregar donde está la suma                   *
 * Tenemos que añadir la masa patrimonial o la cuenta a la suma                 *
 ********************************************************************************/
void mpatrimonialview::nuevasuma() {
   QListViewItem *it;
   // Lo primero de todo es coger el código de cuenta.
   QString codcuenta = codigocta1->text();
   QString masapatrimonial = mpatrimonial->text();

    // Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '"+codcuenta+"'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query,"cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
      it =new QListViewItem(componentessuma);
      it->setText(1,cursoraux1->valor("codigo"));
      it->setText(2,cursoraux1->valor("descripcion"));
      it->setText(3,"cuenta");
      it->setText(0,cursoraux1->valor("idcuenta"));
      cursoraux1->siguienteregistro();
    }// end while
    delete cursoraux1;

    // Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial="+masapatrimonial;
    cursoraux1 = conexionbase->cargacursor(query,"cursormpatrimonial");
    conexionbase->commit();
    while(!cursoraux1->eof()) {
      it =new QListViewItem(componentessuma);
      it->setText(1,"");
      it->setText(2,cursoraux1->valor("descmpatrimonial"));
      it->setText(3,"masa patrimonial");
      it->setText(0,cursoraux1->valor("idmpatrimonial"));
      cursoraux1->siguienteregistro();
    }// end while
}// end nuevasuma

void mpatrimonialview::borrasuma() {
   QListViewItem *it;
   it = componentessuma->currentItem();
   componentessuma->takeItem(it);
}// end borrasuma


/********************************************************************************
 * Se ha pulsado sobre el boton de agregar donde está la suma                   *
 * Tenemos que añadir la masa patrimonial o la cuenta a la suma                 *
 ********************************************************************************/
void mpatrimonialview::nuevaresta() {
   QListViewItem *it;
   // Lo primero de todo es coger el código de cuenta.
   QString codcuenta = codigocta1->text();
   QString masapatrimonial = mpatrimonial->text();

    // Leemos las cuentas y las metemos en la lista que corresponda.
    conexionbase->begin();
    QString query = "SELECT * FROM cuenta WHERE codigo = '"+codcuenta+"'";
    cursor2 *cursoraux1 = conexionbase->cargacursor(query,"cursorusuario");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
      it =new QListViewItem(componentesresta);
      it->setText(1,cursoraux1->valor("codigo"));
      it->setText(2,cursoraux1->valor("descripcion"));
      it->setText(3,"cuenta");
      it->setText(0,cursoraux1->valor("idcuenta"));
      cursoraux1->siguienteregistro();
    }// end while
    delete cursoraux1;

    // Leemos las masas patrimoniales y las metemos en la lista que corresponda.
    conexionbase->begin();
    query = "SELECT * FROM mpatrimonial WHERE idmpatrimonial="+masapatrimonial;
    cursoraux1 = conexionbase->cargacursor(query,"cursormpatrimonial");
    conexionbase->commit();
    while(!cursoraux1->eof()) {
      it =new QListViewItem(componentesresta);
      it->setText(1,"");
      it->setText(2,cursoraux1->valor("descmpatrimonial"));
      it->setText(3,"masa patrimonial");
      it->setText(0,cursoraux1->valor("idmpatrimonial"));
      cursoraux1->siguienteregistro();
    }// end while    
}// end nuevaresta

void mpatrimonialview::borraresta() {
   QListViewItem *it;
   it = componentesresta->currentItem();
   componentesresta->takeItem(it);
}// end borrasuma


void mpatrimonialview::buscampatrimonial() {
   mpatrimonialesview * nuevae = new mpatrimonialesview(0,"mpatrimoniales",true);
   nuevae->inicializa(conexionbase);
   nuevae->setmodoselector();
   nuevae->exec();
   mpatrimonial->setText(nuevae->getidmasa());
   delete nuevae;
}// end buscampatrimonial

void mpatrimonialview::accept() {
   QListViewItem *item;
   QString query;

   // Si idmpatrimonial == NULL implica que es una inserción y no una modificacion
   if (idmpatrimonial == "") {
      conexionbase->begin();
      query.sprintf("INSERT INTO mpatrimonial (descmpatrimonial) VALUES ('nueva masa')");
      conexionbase->ejecuta(query.ascii());
      query.sprintf("SELECT MAX(idmpatrimonial) as id FROM mpatrimonial");
      cursor2 *curs = conexionbase->cargacursor(query,"cargaid");
      conexionbase->commit();
      idmpatrimonial = curs->valor("id").ascii();
   }// end if
   
   // Ponemos los datos correctos sobre la masa patrimonial.
   QString text = descmpatrimonial->text();
   query.sprintf("UPDATE mpatrimonial SET descmpatrimonial='%s' WHERE idmpatrimonial =%s", text.ascii(), idmpatrimonial.ascii());
   conexionbase->ejecuta(query);
   
   query.sprintf("DELETE FROM compmasap WHERE masaperteneciente=%s", idmpatrimonial.ascii());
   conexionbase->ejecuta(query);
   item = componentessuma->firstChild();
   while (item) {
     QString id = item->text(0);
     QString tipo = item->text(3);
     if (tipo == "cuenta") {
        query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial,masaperteneciente,signo) VALUES (%s,NULL,%s,true)",
	conexionbase->sanearCadena(id).ascii(), 
	conexionbase->sanearCadena(idmpatrimonial).ascii() );
     } else {
        query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial,masaperteneciente,signo) VALUES (NULL,%s,%s,true)",
	conexionbase->sanearCadena(id).ascii(), 
	conexionbase->sanearCadena(idmpatrimonial).ascii() );
     }// end if        
     conexionbase->ejecuta(query);
     item = item->nextSibling();
   }// end while

   item = componentesresta->firstChild();
   while (item) {
     QString id = item->text(0).ascii();
     QString tipo = item->text(3).ascii();
     if (tipo == "cuenta") {
        query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial,masaperteneciente,signo) VALUES (%s,NULL,%s,false)",
	conexionbase->sanearCadena(id).ascii(), 
	conexionbase->sanearCadena(idmpatrimonial).ascii() );
     } else {
        query.sprintf("INSERT INTO compmasap(idcuenta, idmpatrimonial,masaperteneciente,signo) VALUES (NULL,%s,%s,false)",
	conexionbase->sanearCadena(id).ascii(), 
	conexionbase->sanearCadena(idmpatrimonial).ascii() );
     }// end if   
     conexionbase->ejecuta(query);
     item = item->nextSibling();
   }// end while
   close();
}// end accept

QString mpatrimonialview::getnommasa() {
   return descmpatrimonial->text().ascii();
}

