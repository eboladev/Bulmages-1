/***************************************************************************
                          filtrarextractosview.cpp  -  description
                             -------------------
    begin                : dom jul 27 2003
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

#include "filtrarextractosview.h"
#include "empresa.h"

#include "busquedacuenta.h"

   QString filtrarextractosview::codigocontrapartida() {
	return m_codigocontrapartida->text();
}

filtrarextractosview::filtrarextractosview(empresa *emp,QWidget *parent, const char *name ) : filtrarextractosdlg(parent,name) {
   fprintf(stderr,"Constructor de filtrarextractosview\n");
   empresaactual = emp;
m_codigocontrapartida->setempresa(emp);
   conexionbase = empresaactual->bdempresa();
   numdigitos = empresaactual->numdigitosempresa();
   fprintf(stderr,"Fin del constructor de fitrarextractosview\n");
}// end filtrarextractosview


filtrarextractosview::~filtrarextractosview(){
}

void filtrarextractosview::boton_ccostes() {
   fprintf(stderr,"Boton ccostes\n");
   selectccosteview *selccostes = empresaactual->getselccostes();
   selccostes->exec();
}// end boton_ccostes


void filtrarextractosview::boton_canales() {
   fprintf(stderr,"Boton canales\n");
   selectcanalview *selcanales = empresaactual->getselcanales();
   selcanales->exec();
}// end boton_ccostes





