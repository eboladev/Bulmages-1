/***************************************************************************
                          abreempresaview.cpp  -  description
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
#include "abreempresaview.h"
#include "postgresiface2.h"



abreempresaview::abreempresaview(BSelector *parent, const char *name, bool modal) : abreempresadlg(0,name,modal) {
   padre = parent;
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa( confpr->valor(CONF_METABASE).c_str() );
   QListViewItem *it;
   
   // Cargamos las contabilidades.
   cursor2 *a;
   apuestatealgo->begin();
   a=apuestatealgo->cargaempresas();
   apuestatealgo->commit();
   while (!a->eof()) {
         it =new QListViewItem(empresas);
         it->setText(0,a->valor(1));
         it->setText(1,a->valor(2));
         it->setText(2,a->valor(3));
         it->setText(3,"BulmaGés");
         a->siguienteregistro();
   }// end while
   delete a;
   
   // Cargamos las facturaciones.
   apuestatealgo->begin();
   a=apuestatealgo->cargacursor("SELECT * FROM empresafact","otroquery");
   apuestatealgo->commit();
   while (!a->eof()) {
         it =new QListViewItem(empresas);
         it->setText(0,a->valor(1));
         it->setText(1,a->valor(2));
         it->setText(2,a->valor(3));
         it->setText(3,"BulmaFact");
         a->siguienteregistro();
   }// end while
   delete a;
   
   delete apuestatealgo;
   intentos=0;
}// end abreempresaview


abreempresaview::~abreempresaview(){
}// end ~abreempresaview




void abreempresaview::accept() {
   QListViewItem *it;
   it = empresas->currentItem();
   empresabd= it->text(2);
   nombreempresa= it->text(0);
   tipo = it->text(3);
   close();
}// end accept


void abreempresaview::closeEvent(QCloseEvent * e) {
    e->accept();
}

