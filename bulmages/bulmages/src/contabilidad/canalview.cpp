/***************************************************************************
                          canalview.cpp  -  description
                             -------------------
    begin                : lun mar 24 2003
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

#include "canalview.h"

canalview::canalview(QWidget *parent, const char *name, bool modal ) : canaldlg(parent,name, modal) {
  idcanal=0;
}// end canalview

canalview::~canalview(){
}// end ~canalview

void canalview::inicializa(postgresiface2 *conn) {
  conexionbase= conn;

  // Vamos a inicializar el combo de los canales
  QString query = "SELECT * from canal ORDER BY nombre";
  conexionbase->begin();
  cursor2 *cursorcanal = conexionbase->cargacursor(query,"canales");
  conexionbase->commit();
  int i = 0;
  combocanal->clear();
  while (!cursorcanal->eof()) {
    combocanal->insertItem(cursorcanal->valor(2), -1);
    canales[i++] = atoi (cursorcanal->valor(0).latin1());
    cursorcanal->siguienteregistro();
  }// end while
  delete cursorcanal;

  // Si el combocoste no está vacio se muestra el registro que
  // contiene
  if (idcanal != 0) {
    mostrarplantilla();
  } else if (combocanal->count() > 0) {
    idcanal = canales[combocanal->currentItem()];
    mostrarplantilla();
  }// end if
}// end inicializa

/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void canalview::cambiacombo(int numcombo) {
//  fprintf(stderr,"idasientointeligente %d\n",listasientos[num]);
  idcanal = canales[numcombo];
  mostrarplantilla();
}// end cambiacombo


/****************************************************************
  Esta funcion muestra el canal en la ventana.
  ***************************************************************/
void canalview::mostrarplantilla() {
//  char cadena[300];
  int i;
  QString query;
  query.sprintf("SELECT * from canal WHERE idcanal=%d",idcanal);
  conexionbase->begin();
  cursor2 *cursorcanal = conexionbase->cargacursor(query,"canales1");
  conexionbase->commit();
  if (!cursorcanal->eof()) {
    nomcanal->setText(cursorcanal->valor(2));
    desccanal->setText(cursorcanal->valor(1));
  }// end if
  i=0;
  while (canales[i] != idcanal)
    i++;
  combocanal->setCurrentItem(i);  
}// end mostrarplantilla


void canalview::boton_guardar() {
  QString nom = nomcanal->text();
  QString desc = desccanal->text();
  QString query;
  query.sprintf ("UPDATE canal SET nombre='%s', descripcion='%s' WHERE idcanal=%d",nom.latin1(), desc.latin1(), idcanal);
  conexionbase->begin();
  conexionbase->ejecuta(query);
  conexionbase->commit();
  inicializa(conexionbase);
}// end boton_guardar


void canalview::boton_nuevo() {
  QString query;
  query.sprintf("INSERT INTO canal (nombre, descripcion) VALUES ('Nuevo Canal','Escriba su descripcion')");
  conexionbase->begin();
  conexionbase->ejecuta(query);
  query.sprintf("SELECT MAX(idcanal) AS id FROM canal");
  cursor2 *cur = conexionbase->cargacursor(query,"queryy");
  idcanal= atoi(cur->valor("id").latin1());
  delete cur;
  conexionbase->commit();
  inicializa(conexionbase);
}// end boton_nuevo


void canalview::boton_borrar() {
  QString query;
  query.sprintf("DELETE FROM canal WHERE idcanal=%d", idcanal);
  conexionbase->begin();
  conexionbase->ejecuta(query);
  conexionbase->commit();
  idcanal=0;
  inicializa(conexionbase);
}// end boton_borrar
