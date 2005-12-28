/***************************************************************************
                          filtrardiarioview.cpp  -  description
                             -------------------
    begin                : mar jul 29 2003
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

#include "listcuentasview1.h"
#include "filtrardiarioview.h"
#include "empresa.h"

#include "busquedacuenta.h"
#include "selectcanalview.h"
#include "selectccosteview.h"

filtrardiarioview::filtrardiarioview(empresa *emp, QWidget *parent, const char *name ) : filtrardiariodlg(parent,name) {
	fprintf(stderr,"CONSTRUCTOR de filtrardiarioview\n");
	empresaactual = emp;
	m_contrapartida->setempresa(emp);
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
   
   fprintf(stderr,"Fin del CONSTRUCTOR de filtrardiarioview\n");

}// end filtrardiarioview

filtrardiarioview::~filtrardiarioview(){
}



void filtrardiarioview::boton_canales() {
   fprintf(stderr,"Boton canales\n");
   selectcanalview *selcanales = empresaactual->getselcanales();
   selcanales->exec();
   selcanales->firstcanal();
   while (selcanales->nextcanal());
//   selccostes->show();
}// end boton_canales

void filtrardiarioview::boton_ccostes() {
   fprintf(stderr,"Boton ccostes\n");
   selectccosteview *selccostes = empresaactual->getselccostes();
   selccostes->exec();
   fprintf(stderr,"---------------INICIO---------------------\n");
   selccostes->firstccoste();
   while (selccostes->nextccoste());
   fprintf(stderr,"-----------------FIN---------------------\n");
//   selccostes->show();
}// end boton_ccostes
