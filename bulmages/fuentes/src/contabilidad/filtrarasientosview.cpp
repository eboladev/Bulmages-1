/***************************************************************************
                          filtrarasientosview.cpp  -  description
                             -------------------
    begin                : Sat Jan 4 2003
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

#include "filtrarasientosview.h"
#include "selectcanalview.h"
#include "selectccosteview.h"
#include "empresa.h"

filtrarasientosview::filtrarasientosview(empresa * emp,QWidget *parent, const char *name ) : filtrarasientosdlg(parent,name) {
   fprintf(stderr,"constructor de filtrarasientosview\n");
   empresaactual = emp;
   conexionbase = emp->bdempresa();
   numdigitos = emp->numdigitosempresa();
}

filtrarasientosview::~filtrarasientosview(){
}

void filtrarasientosview::boton_reset() {
  saldoasiento->setText("");
  nombreasiento->setText("");
  cantidadapunte->setText("");
  done(1);
}// end boton_reset


void filtrarasientosview::boton_canales() {
   fprintf(stderr,"Boton canales\n");
   selectcanalview *selcanales = empresaactual->getselcanales();
   selcanales->exec();
}// end boton_canales

void filtrarasientosview::boton_ccostes() {
   fprintf(stderr,"Boton ccostes\n");
   selectccosteview *selccostes = empresaactual->getselccostes();
   selccostes->exec();
}// end boton_ccostes
