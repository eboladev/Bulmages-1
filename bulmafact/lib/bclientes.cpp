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
#include "bclientes.h"

BClientes::BClientes(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent, const char * name) : UIClientes(parent, name) {
    EmpresaTrabajo=punteroEmpresaTrabajo;
}


BClientes::~BClientes(){
}

void BClientes::iniciarBusqueda() {
    QString Retorno("resultado");
    QString tabla("clientes");
    QString* arg1 = new QString("campo01");
    QString* arg2 = new QString("campo02");
    QString* arg3 = new QString("campo03");
    QString* arg4 = new QString("campo04");
    (new Bbuscador(this, "buscaCli",WType_TopLevel,EmpresaTrabajo,&Retorno,&tabla, arg1,arg2,arg3,arg4,0))->show();
}

