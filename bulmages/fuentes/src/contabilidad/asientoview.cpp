/***************************************************************************
                          asientoview.cpp  -  description
                             -------------------
    begin                : Thu Dec 19 2002
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

 
#include "asientoview.h"
#include "funcaux.h"

asientoview::asientoview(QWidget *parent, const char *name, bool modal) : asientodlg(parent,name,modal) {
}

asientoview::~asientoview(){
}

void asientoview::inicializa(postgresiface2 *conn) {
   conexionbase = conn;
}// end inicializa


// **********************************************************************
// Esta funcion esta a medio construir, falta poder coger la fecha y la
// descripcion del asiento.
// **********************************************************************
void asientoview::cargaasiento(int idasiento1) {
   cursor2 *cursoraux;   
   conexionbase->begin();
   idasiento_mostrado = idasiento1;
   cursoraux = conexionbase->cargaasiento(idasiento1);
   conexionbase->commit();
   if (!cursoraux->eof()) {
      descasiento->setText(cursoraux->valor("descripcion"));
      fechaasiento1->setText(cursoraux->valor("fecha").mid(0,10));
      idasiento->setText(cursoraux->valor("ordenasiento"));
      notasasiento->setText(cursoraux->valor("comentariosasiento"));
   }// end if
   delete cursoraux;
}// end cargaasiento


// ****************************************************************************
// * Esta funcion esta a medio construir, falta contemplar las modificaciones *
// ****************************************************************************
void asientoview::acceptar() {
   QString fecha;
   int val=0;
   int numasiento = idasiento_mostrado;
   QString text = descasiento->text();
   fecha = fechaasiento1->text();
   QString ordenasiento = idasiento->text();
   
   if (numasiento==0) {
      val = creaasiento(text, fecha);
   } else {
      val = modificaasiento(text, fecha, numasiento, notasasiento->text(), ordenasiento);
   }// end if
   done(val);
}// end accept



/**********************************************************
 * Esta funcion crea un asiento y devuelve su identificador.
 * Si todo va bien la funcion devuelve el identificador
 * Si algo falla devuelve cero.
 ***********************************************************/
int asientoview::creaasiento(QString texto, QString fecha, int numasiento) {
   int val;
   conexionbase->begin();
   val = conexionbase->nuevoasiento(texto, fecha, numasiento,1);
   conexionbase->commit();
   return(val);
}// end creaasiento

int asientoview::modificaasiento(QString texto, QString fecha, int numasiento, QString notas, QString orden) {
  QString query;
  int val;
  // Si ya existe un asiento con el orden que intentamos asignar debemos desplazar los asientos posteriores.
  query.sprintf("SELECT * FROM asiento WHERE ordenasiento=%s",orden.ascii());
  conexionbase->begin();
  cursor2 *cur = conexionbase->cargacursor(query,"micursor");
  conexionbase->commit();
  if (!cur->eof()) {
     query.sprintf("UPDATE asiento set ordenasiento = ordenasiento+1 WHERE ordenasiento >= %s", orden.ascii());
     conexionbase->ejecuta(query);
  }// end if
  
  query.sprintf("UPDATE asiento SET descripcion='%s', fecha='%s', comentariosasiento='%s', ordenasiento=%s WHERE idasiento=%d",texto.ascii(),fecha.ascii(),notas.ascii(),orden.ascii(), numasiento);
  conexionbase->begin();
  val = conexionbase->ejecuta(query);
  conexionbase->commit();
  return(numasiento);
}// end modificaasiento

void asientoview::return_fecha() {
  fechaasiento1->setText(normalizafecha(fechaasiento1->text()).toString("dd/MM/yyyy"));
}// end return_fecha
