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

/**************************************************************************/
/* Clase Contratos de compra.                                             */
/**************************************************************************/

#include "bcontrcompra.h"

BContrCompra::BContrCompra(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent, const char * name, WFlags f) : UIContrCompra(parent, name, f) {
    EmpresaTrabajo=punteroEmpresaTrabajo;
}


BContrCompra::~BContrCompra(){
}

