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




abreempresaview::abreempresaview(BSelector *parent, const char *name, bool modal) : abreempresadlg(0,name,modal) {
   padre = parent;
   //padre->hide();
   postgresiface2 *apuestatealgo;
   apuestatealgo = new postgresiface2();
   apuestatealgo->inicializa( confpr->valor(CONF_METABASE).c_str() );
   QListViewItem *it;
   cursor2 *a;
   apuestatealgo->begin();
   a=apuestatealgo->cargaempresas();
   apuestatealgo->commit();
   while (!a->eof()) {
         it =new QListViewItem(empresas);
         it->setText(0,a->valor(1));
         it->setText(1,a->valor(2));
         it->setText(2,a->valor(3));
         a->siguienteregistro();
   }// end while
   delete a;
   delete apuestatealgo;
   intentos=0;
}// end abreempresaview


abreempresaview::~abreempresaview(){
//padre->show();
//parentWidget()->show();

}// end ~abreempresaview


/*
void abreempresaview::slotabreempresaview()  {
}// end slotabreempresaview
*/




void abreempresaview::accept() {
   postgresiface2 apuestatealgo;
   QListViewItem *it;
   int num;
   it = empresas->currentItem();
   nombre=login->text();
   contrasena=password->text();
   empresabd= it->text(2);
   QString ejercicio = it->text(1);
   nombreempresa= it->text(0);
   num = apuestatealgo.cargaempresa(empresabd, nombre, contrasena);
   if (num >0) {
       padre->NombreUsuario = nombre;
       padre->PasswordUsuario = contrasena;
       padre->NombreBaseDatos = empresabd;
       padre->nombreempresa->setText(nombreempresa);
       padre->ejercicio->setText(ejercicio.ascii());
       delete this;
   }//end if
   if ((intentos+=1)>3) padre->close();
}// end accept


void abreempresaview::closeEvent(QCloseEvent * e) {
    e->accept();
    padre->close();
}

