/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#include "bnuevaempresa.h"


BNuevaEmpresa::BNuevaEmpresa(QWidget * parent, const char * name, WFlags f) : nuevaempresadlg(parent,name,f) {
}// end BNuevaEmpresa


BNuevaEmpresa::~BNuevaEmpresa() {
}

void BNuevaEmpresa::accept() {
// Falta comprobar que tengas permisos para crear nuevas empresas.
  QString nombredb;
  QString nombreEmp;

  nombredb = bdempresa->text().stripWhiteSpace()+ejercicioempresa->text().stripWhiteSpace();
  nombreEmp = nombreempresa->text().stripWhiteSpace();
  QString cadena = confpr->valor(CONF_PROGDATA);
  cadena += "dbmodels/creabulmages --texto "+nombredb+" 1 "+nombreEmp+" "+ ejercicioempresa->text().stripWhiteSpace()+" "+confpr->valor(CONF_METABASE);  
  system(cadena.ascii());
  close();
}// end accept
