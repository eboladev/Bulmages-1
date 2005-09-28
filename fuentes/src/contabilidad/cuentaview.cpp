/***************************************************************************
                          cuentaview.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 2002
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

#include "cuentaview.h"

cuentaview::cuentaview(QWidget *parent, const char *name, int fl): cuentadlg(parent,name,fl) {
   idcuenta=0;
}// end cuentaview

cuentaview::~cuentaview(){
}// end ~cuentaview


/*********************************************************************
 * Esta función se activa cada vez quese pulsa una tecla sobre la cuenta
 *********************************************************************/
void cuentaview::cambiapadre(const QString &cadena)  {
  QString cad;
   cursor2 *cursoraux;
   cad = codigopadre->text();
   unsigned int i = cad.find(".",0);
   if (i+1 < cad.length()) {
      if (i != 0) {
         cad.replace(i,1,"%");
      }// end if
      conexionbase->begin();
      cursoraux = conexionbase->cargacuenta(0, cad );
      conexionbase->commit();
      if (!cursoraux->eof()) {
         codigopadre->setText(cursoraux->valor(1));
      }// end if
      delete cursoraux;
   }// end if
   
   // PAra quitar el warning
   cadena.isNull();
}// end cambiapadre


/***************************************************************
 * Este es el slot que se activa al pulsar el boton ok del formulario
 * Lo que hace es recoger los datos del formulario y hacer una insercion
 * o una modificacion de la tabla de cuentas.
 ***************************************************************/
void cuentaview::aceptar() {
   QString codigocuenta;
   int idpadre=0;
   cursor2 *cursoraux;   
   // Comprobamos que el campo codigopadre tiene un numero de cuenta y
   // calculamos el id correspondiente consultando en la base de datos.
   codigocuenta = codigopadre->text();
   if (codigocuenta!="") {
      conexionbase->begin();
      cursoraux = conexionbase->cargacuenta(0,codigocuenta);
      conexionbase->commit();
      idpadre = atoi(cursoraux->valor(0).ascii());
      delete cursoraux;
   }// end if
   // Recogemos el valor de tipocuenta
   int tipocuenta=0;
   if (cuentaactivo->isChecked())
      tipocuenta= 1;
   else if (cuentapasivo->isChecked())
      tipocuenta = 2;
   else if (cuentaneto->isChecked())
      tipocuenta = 3;
   else if (cuentaingreso->isChecked())
      tipocuenta = 4;
   else if (cuentagasto->isChecked())
      tipocuenta = 5;
   else
      tipocuenta = 0;
   // Si se trata de una modificacion modificamos y si se trata de
   // una insercion insertamos.
   if (idcuenta != 0) {
      conexionbase->begin();
      conexionbase->modificacuenta(idcuenta,descripcion->text() ,codigo->text(), imputacion->isChecked(), bloqueada->isChecked(),idgrupos[combogrupos->currentItem()],TRUE, nombreent->text(), cif->text(), direccion->text(), telf->text(), coments->text(), banco->text(),  email->text(),  web->text(), tipocuenta , nodebe->isChecked(), nohaber->isChecked());
      conexionbase->commit();
   } else {
      //QMessageBox::information( 0, "Se va a dar de alta una nueva cuenta", "Unable to find the user preferences file.\n""The factory default will be used instead." );
      conexionbase->begin();
      conexionbase->nuevacuenta( descripcion->text(), codigo->text(), idpadre, idgrupos[combogrupos->currentItem()],nombreent->text(),  cif->text(), direccion->text(),  telf->text(),coments->text(), banco->text(), email->text(), web->text(), tipocuenta, nodebe->isChecked(), nohaber->isChecked() );
      QString query = "SELECT max(idcuenta) from cuenta";
      cursoraux = conexionbase->cargacursor(query, "maxidcuenta");
      idcuenta = atoi(cursoraux->valor(0).ascii());
      conexionbase->commit();
      delete cursoraux;
   }// end if
   // Liberamos memoria y terminamos.
   done(1);
}// end accept

int cuentaview::inicializa(postgresiface2 *conn) {
    cursor2 *cursorgrupos, *cursoraux1;
    conexionbase = conn;
    for (int i=0;i<100;i++) {
       idgrupos[i]=-1;
    }// end for
    conexionbase->begin();
    cursorgrupos = conexionbase->cargagrupos();
    conexionbase->commit();
    while (!cursorgrupos->eof()) {
       combogrupos->insertItem(cursorgrupos->valor(1),-1);
       idgrupos[combogrupos->count()-1]=atoi(cursorgrupos->valor(0));
       cursorgrupos->siguienteregistro();
    }// end while
   delete cursorgrupos;

   // Vamos a cargar el número de digitos de cuenta para poder hacer una introduccion de numeros de cuenta mas practica.
   conexionbase->begin();
   QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursoraux1 = conexionbase->cargacursor(query,"codcuenta");
   numdigitos=cursoraux1->valor(2).length();
   conexionbase->commit();
   delete cursoraux1;   
   return(0);
}// end inicializa


int cuentaview::cargacuenta(int idcuenta1){
    QString cadena;
    int cpadre;
    cursor2 *cursorcuenta, *cursorpadre;
    idcuenta = idcuenta1;
    conexionbase->begin();
    cursorcuenta = conexionbase->cargacuenta(idcuenta1);
    conexionbase->commit();
    codigo->setText(cursorcuenta->valor(1));
    descripcion->setText(cursorcuenta->valor(2));
    debe->setText(cursorcuenta->valor(8));
    haber->setText(cursorcuenta->valor(9));
    if (cursorcuenta->valor(5) != "f") {
       bloqueada->setChecked(true);
    } else {
       bloqueada->setChecked(false);
    }// end if
    if (cursorcuenta->valor(3)!= "f") {
       imputacion->setChecked(true);
    } else {
       imputacion->setChecked(false);
    }// end if
    if (cursorcuenta->valor(10) != "f") {
   nodebe->setChecked(true);
  } else {
    nodebe->setChecked(false);
  }// end if
  if (cursorcuenta->valor(11) != "f") {
    nohaber->setChecked(true);
  } else {
    nohaber->setChecked(false);
  }// end if
  if (cursorcuenta->valor(12) != "f") {
     regularizacion->setChecked(true);
  } else {
     regularizacion->setChecked(false);
  }// end if
  // Vamos a hacer la carga del tipocuenta
  int tipocuenta = atoi(cursorcuenta->valor("tipocuenta").ascii());
  switch(tipocuenta) {
    case 0:
      cuentasintipo->setChecked(TRUE);
	 break;
    case 1:
      cuentaactivo->setChecked(TRUE);
    break;
    case 2:
      cuentapasivo->setChecked(TRUE);
    break;
    case 3:
      cuentaneto->setChecked(TRUE);
    break;
    case 4:
      cuentaingreso->setChecked(TRUE);
    break;
    case 5:
      cuentagasto->setChecked(TRUE);
    break;
  }// end switch
  // Vamos a hacer la carga del grupo
  int idgrupo = atoi(cursorcuenta->valor(6).ascii());
  int i=0;
  while (idgrupos[i]!=idgrupo && i<100) i++;
  combogrupos->setCurrentItem(i);
  // Vamos a coger el código del padre de la cuenta para que también
  // aparezca en el formulario.
  cpadre = atoi( cursorcuenta->valor(4).ascii());
  if (cpadre != 0) {
     conexionbase->begin();
     cursorpadre = conexionbase->cargacuenta(cpadre);
     conexionbase->commit();
     cadena = cursorpadre->valor(1);
     codigopadre->setText(cadena);
     codigopadre->setReadOnly(TRUE);
     delete cursorpadre;
  }// end if
  // Vamos a hacer las inserciones pertinentes para los campos de la entidad
  nombreent->setText(cursorcuenta->valor("nombreent_cuenta"));
  cif->setText(cursorcuenta->valor("cifent_cuenta"));
  direccion->setText(cursorcuenta->valor("dirent_cuenta"));
  telf->setText(cursorcuenta->valor("telent_cuenta"));
  coments->setText(cursorcuenta->valor("coment_cuenta"));
  banco->setText(cursorcuenta->valor("bancoent_cuenta"));
  email->setText(cursorcuenta->valor("emailent_cuenta"));
  web->setText(cursorcuenta->valor("webent_cuenta"));
  delete cursorcuenta;
  return(0);
}// end cargacuenta


int cuentaview::nuevacuenta(QString codpadre, int idgrupo) {

	// Suponiendo que las cuentas son numericas, al crear una nueva cuenta
	// Buscamos entre las que seran sus hermanas y le asignamos el numero siguiente
	// que le corresponda.
	 QString cpadreaux;
	 QString query;
	 query.sprintf("SELECT max(codigo) as maximo FROM cuenta WHERE padre = id_cuenta('%s')",codpadre.ascii());
	 conexionbase->begin();
	 cursor2 *cur = conexionbase->cargacursor(query,"uncursor");
	 conexionbase->commit();
	 long int valor = atol(cur->valor("maximo").ascii());
	 valor ++;
	 cpadreaux.setNum(valor);
	 codigo->setText(cpadreaux);
	 
	 // Establecemos el valor del padre y del grupo.
    codigopadre->setText(codpadre);
    int i=0;
    while (idgrupos[i]!=idgrupo && i<100) i++;
    if (idgrupos[i] == idgrupo) combogrupos->setCurrentItem(i);    
    return(0);
}// end inicializa

/***************************************************
 * Esta funcion se dispara cuando se ha pulsado
 * return sobre el boton del codigo                *
 ***************************************************/
void cuentaview::codigo_return() {
  QString cod=codigo->text();
  cod = extiendecodigo(cod,numdigitos);
  codigo->setText(cod);
}// end codigo_return

void cuentaview::accept() {
}// end accept
