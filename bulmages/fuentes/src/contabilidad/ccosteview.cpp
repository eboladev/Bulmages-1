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
 /**************************************************************************
  * Esta clase se encarga de presentar los centros de coste, la ventana, y *
  * de controlar la inserción de nuevos centros de coste, borrarlos, etc   *
  **************************************************************************/
#include <qlistview.h>
#include "ccosteview.h"
#include "empresa.h"
#include "selectccosteview.h"

// El inicializador de la clase.
ccosteview::ccosteview(empresa *emp, QWidget *parent, const char *name, bool modal) : ccostedlg(parent,name, modal) {
   fprintf(stderr,"inicializador de ccosteview\n");
   empresaactual = emp;
   conexionbase=empresaactual->bdempresa();
   idc_coste=0;        
   col_nom_coste = listcoste->addColumn("nom_coste",-1);
   col_desc_coste = listcoste->addColumn("desc_coste",-1);
   col_idc_coste=listcoste->addColumn("idc_coste",0);
   pintar();
}// end ccosteview

ccosteview::~ccosteview(){
}// end ~ccosteview


void ccosteview::pintar() {  
    QListViewItem * it;
    QMap <int,QListViewItem *>Lista;
    int padre;
    int idc_coste=0;
    cursor2 *cursoraux1, *cursoraux2;

    listcoste->clear();    
    conexionbase->begin();
    cursoraux1 = conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre ISNULL ORDER BY idc_coste","centroscoste");
    conexionbase->commit();
    while (!cursoraux1->eof()) {
        padre = atoi( cursoraux1->valor("padre").ascii());
        idc_coste = atoi( cursoraux1->valor("idc_coste").ascii());
        it =new QListViewItem(listcoste);
        Lista[idc_coste]=it;
        it->setText(col_idc_coste, cursoraux1->valor("idc_coste"));
        it->setText(col_desc_coste,cursoraux1->valor("descripcion"));
        it->setText(col_nom_coste, cursoraux1->valor("nombre"));
        it->setOpen(true);
        cursoraux1->siguienteregistro ();
    }// end while
    delete cursoraux1;

    conexionbase->begin();
    cursoraux2= conexionbase->cargacursor("SELECT * FROM c_coste WHERE padre IS NOT NULL ORDER BY idc_coste","mascostes");
//    cursoraux2=conexionbase->cargacuentas(-2);
    conexionbase->commit();
    //   cursoraux1->ultimoregistro();
    while (!cursoraux2->eof()) {
        padre = atoi(cursoraux2->valor("padre").ascii());
        idc_coste = atoi(cursoraux2->valor("idc_coste").ascii());
        fprintf(stderr,"Cuentas de subnivel:%d",padre);
 //       if (padre != 0) {
            it = new QListViewItem(Lista[padre]);
            Lista[idc_coste]=it;
            it->setText(col_idc_coste,cursoraux2->valor("idc_coste"));
            it->setText(col_desc_coste,cursoraux1->valor("descripcion"));
            it->setText(col_nom_coste,cursoraux1->valor("nombre"));
            it->setOpen(true);
//        }// end if
        cursoraux2->siguienteregistro();
    }// end while
    delete cursoraux2;
    
    
  if ( idc_coste!= 0) {
    mostrarplantilla();
  }// end if
  
  // Ya que se han producido cambios en los centros de coste
  // Se inicializa el selector de centros de coste.
  selectccosteview *scoste = empresaactual->getselccostes();
  scoste->cargacostes();
}// end pintar

/*****************************************************
  Esta funcion sirve para hacer el cambio sobre un
  centro de coste .
  ****************************************************/
void ccosteview::cambiacombo(int numcombo) {
  idc_coste = ccostes[numcombo];
  mostrarplantilla();
}// end cambiacombo              

void ccosteview::seleccionado(QListViewItem *it) {
   idc_coste = atoi(it->text(col_idc_coste).ascii());
   mostrarplantilla();
}// end seleccionado


void ccosteview::mostrarplantilla() {
  fprintf(stderr,"mostramos la plantilla");
  QString query;
  query.sprintf("SELECT * from c_coste WHERE idc_coste=%d",idc_coste);
  conexionbase->begin();
  cursor2 *cursorcoste = conexionbase->cargacursor(query,"costes1");
  conexionbase->commit();
  if (!cursorcoste->eof()) {
    nomcentro->setText(cursorcoste->valor("nombre"));
    desccoste->setText(cursorcoste->valor("descripcion"));
  }// end if
  
  delete cursorcoste;
//  i=0;
//  while (ccostes[i] != idc_coste)
//    i++;
//  combocoste->setCurrentItem(i);
   botondefault->setOn(TRUE);
}// end mostrarplantilla


void ccosteview::boton_guardar() {
  QString nom = nomcentro->text();
  QString desc = desccoste->text();
  QString query;
  query.sprintf ("UPDATE c_coste SET nombre='%s', descripcion='%s' WHERE idc_coste=%d",nom.ascii(), desc.ascii(), idc_coste);
  conexionbase->begin();
  conexionbase->ejecuta(query);
  conexionbase->commit();
  fprintf(stderr,"Se ha guardado el centro de coste");
  pintar();
}// end boton_guardar


void ccosteview::boton_nuevo() {
  QString query;
  QListViewItem *it;
  it=listcoste->selectedItem();
  if (it) {
      idc_coste=atoi(it->text(col_idc_coste).ascii());
      query.sprintf("INSERT INTO c_coste (padre, nombre, descripcion) VALUES (%d, 'Nuevo Centro de Coste', 'Escriba su descripcion')", idc_coste);
      conexionbase->begin();
      conexionbase->ejecuta(query);
  } else {
      query.sprintf("INSERT INTO c_coste (nombre, descripcion) VALUES ('Nuevo Centro de Coste','Escriba su descripcion')");
      conexionbase->begin();
      conexionbase->ejecuta(query);
  }// end if
  query.sprintf("SELECT MAX(idc_coste) AS id_coste FROM c_coste");
  cursor2 *cur = conexionbase->cargacursor(query,"queryy");
  idc_coste= atoi(cur->valor("id_coste").ascii());
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


