/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
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

#include "bproveedor.h"

Bproveedor::Bproveedor(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent, const char * name, WFlags f) : UIProveedor(parent, name, f) {
    EmpresaTrabajo=punteroEmpresaTrabajo;
}


Bproveedor::~Bproveedor(){
}

void Bproveedor::cerrarVentana() {
    delete this;
}

void Bproveedor::closeEvent(QCloseEvent * e) {
    //int exit=0;
    //if (exit==1) e->accept();
    //else e->ignore();
   cerrarVentana();    
}
