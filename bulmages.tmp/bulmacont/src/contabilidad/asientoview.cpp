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


asientoview::asientoview(empresa *emp,QWidget *parent, const char *name, bool modal) : asientodlg(parent,name,modal) , dialogChanges(this){
empresaactual=emp;
ordenasiento_mostrado=0;
}

asientoview::~asientoview(){
}

void asientoview::inicializa(postgresiface2 *conn) {
   conexionbase = conn;
}// end inicializa


/**
  * Esta funcion esta a medio construir, falta poder coger 
  * la fecha y la descripcion del asiento.
  */
void asientoview::cargaasiento(int idasiento1) {
   int indiceClaseAsiento=1;
   cursor2 *cursoraux;   
   conexionbase->begin();
   idasiento_mostrado = idasiento1;
   cursoraux = conexionbase->cargaasiento(idasiento1);
   conexionbase->commit();
   if (!cursoraux->eof()) {
      descasiento->setText(cursoraux->valor("descripcion"));
      fechaasiento1->setText(cursoraux->valor("fecha").mid(0,10));
      idasiento->setText(cursoraux->valor("ordenasiento"));
      ordenasiento_mostrado =idasiento->text().toInt();
      notasasiento->setText(cursoraux->valor("comentariosasiento"));
      switch (cursoraux->valor("clase").toInt()) {
          case 0:  indiceClaseAsiento=0; break;
          case 1:  indiceClaseAsiento=1; break;
          case 98: indiceClaseAsiento=2; break;
          case 99: indiceClaseAsiento=3; break;
      }
      claseAsiento->setCurrentItem(indiceClaseAsiento);
   }// end if
   delete cursoraux;
   dialogChanges_cargaInicial();
}// end cargaasiento


/**
  * Esta funcion esta a medio construir, falta contemplar las modificaciones
  */
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


void asientoview::close() {
    /// Si se ha modificado el contenido advertimos y guardamos.
    if (dialogChanges_hayCambios()) {
    	    if ( QMessageBox::warning( this, "Guardar Asiento",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel ) == QMessageBox::Ok)
		acceptar();	
    }// end if
    QDialog::close();
}// end close



/** SLOT que responde a la pulsación del botón de guardar el tipo de iva que se está editando.
  * Lo que hace es que se hace un update de todos los campos
  */
void asientoview::s_saveAsiento() {
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
   dialogChanges_cargaInicial();
}// end s_saveFPago



/**
  * Esta funcion crea un asiento y devuelve su identificador.
  * Si todo va bien la funcion devuelve el identificador
  * Si algo falla devuelve cero.
  */
int asientoview::creaasiento(QString texto, QString fecha, int numasiento, int clase) {
   int val;
   conexionbase->begin();
   val = conexionbase->nuevoasiento(texto, fecha, numasiento,clase);
   conexionbase->commit();
   return(val);
}// end creaasiento


int asientoview::modificaasiento(QString texto, QString fecha, int numasiento, QString notas, QString orden) {
  QString query;
  QString valorClaseAsiento="1";
  QString cadenaAux=""; 
  query.sprintf("UPDATE asiento SET descripcion='%s', fecha='%s', comentariosasiento='%s', ordenasiento=%s, clase=%s WHERE idasiento=%d",texto.ascii(),fecha.ascii(),notas.ascii(),orden.ascii(), valorClaseAsiento.ascii(), numasiento);

  conexionbase->ejecuta(query);
  conexionbase->commit();
  return(numasiento);
}// end modificaasiento


void asientoview::return_fecha() {
  fechaasiento1->setText(normalizafecha(fechaasiento1->text()).toString("dd/MM/yyyy"));
}// end return_fecha

