/***************************************************************************
                          ccosteview.cpp  -  description
                             -------------------
    begin                : sáb mar 22 2003
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
#include "ccosteview.h"
#include "empresa.h"

// El inicializador de la clase.
ccosteview::ccosteview(empresa *emp, QWidget *parent, const char *name, bool modal) : ccostedlg(parent,name, modal) {
	fprintf(stderr,"inicializador de ccosteview\n");
	empresaactual = emp;
	conexionbase=empresaactual->bdempresa();
	idc_coste=0;
	pintar();
}// end ccosteview

ccosteview::~ccosteview(){
}// end ~ccosteview


void ccosteview::pintar() {
  // Vamos a inicializar el combo de los centros de coste
  QString query = "SELECT * from c_coste ORDER BY nombre";
  conexionbase->begin();
  cursor2 *cursorcoste = conexionbase->cargacursor(query,"ccostes");
  conexionbase->commit();
  int i = 0;
  combocoste->clear();
  while (!cursorcoste->eof()) {
    combocoste->insertItem(cursorcoste->valor(2), -1);
    ccostes[i++] = atoi (cursorcoste->valor(0).latin1());
    cursorcoste->siguienteregistro();
  }// end while
  delete cursorcoste;

  // Si el combocoste no está vacio se muestra el registro que
  // contiene
  if ( idc_coste!= 0) {
    mostrarplantilla();
  } else if (combocoste->count() > 0) {
    idc_coste = ccostes[combocoste->currentItem()];
    mostrarplantilla();
  }// end if
}// end pintar

/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void ccosteview::cambiacombo(int numcombo) {
  idc_coste = ccostes[numcombo];
  mostrarplantilla();
}// end cambiacombo              

void ccosteview::mostrarplantilla() {
  fprintf(stderr,"mostramos la plantilla");
  int i;
  char cadena[300];
  sprintf(cadena,"%d", idc_coste);
  QString query;
  query.sprintf("SELECT * from c_coste WHERE idc_coste=%d",idc_coste);
  conexionbase->begin();
  cursor2 *cursorcoste = conexionbase->cargacursor(query,"costes1");
  conexionbase->commit();
  if (!cursorcoste->eof()) {
    nomcentro->setText(cursorcoste->valor(2));
    desccoste->setText(cursorcoste->valor(1));
  }// end if
  i=0;
  while (ccostes[i] != idc_coste)
    i++;
  combocoste->setCurrentItem(i);
}// end mostrarplantilla


void ccosteview::boton_guardar() {
  QString nom = nomcentro->text();
  QString desc = desccoste->text();
  QString query;
  query.sprintf ("UPDATE c_coste SET nombre='%s', descripcion='%s' WHERE idc_coste=%d",nom.latin1(), desc.latin1(), idc_coste);
  conexionbase->begin();
  conexionbase->ejecuta(query);
  conexionbase->commit();
  fprintf(stderr,"Se ha guardado el centro de coste");
  pintar();
}// end boton_guardar


void ccosteview::boton_nuevo() {
  QString query;
  query.sprintf("INSERT INTO c_coste (nombre, descripcion) VALUES ('Nuevo Centro de Coste','Escriba su descripcion')");
  conexionbase->begin();
  conexionbase->ejecuta(query);
  query.sprintf("SELECT MAX(idc_coste) AS id_coste FROM c_coste");
  cursor2 *cur = conexionbase->cargacursor(query,"queryy");
  idc_coste= atoi(cur->valor("id_coste").latin1());
  delete cur;
  conexionbase->commit();
  pintar();
}// end boton_nuevo


void ccosteview::boton_borrar() {
  QString query;
  query.sprintf("DELETE FROM c_coste WHERE idc_coste=%d", idc_coste);
  conexionbase->begin();
  conexionbase->ejecuta(query);
  conexionbase->commit();
  idc_coste=0;
  pintar();
}// end boton_borrar
