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

#ifndef Btipo_iva_H
#define Btipo_iva_H

#include ".ui/uitipo_iva.h"

#include "bfempresa.h"

/**
@author Josep Burcion
*/
class Btipo_iva : public UItipo_iva
{
    Q_OBJECT
    
public:
    Btipo_iva(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent = 0, const char * name = 0, WFlags f=0);
    ~Btipo_iva();

private:
    BfEmpresa* EmpresaTrabajo;
    
};

#endif

