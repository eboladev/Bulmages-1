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

#ifndef Bcatalogo_H
#define Bcatalogo_H

#include ".ui/uicatalogo.h"

#include "bfempresa.h"

/**
@author Josep Burcion
*/
class Bcatalogo : public UIcatalogo
{
    Q_OBJECT
    
public:
    Bcatalogo(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent = 0, const char * name = 0, WFlags f=0);
    ~Bcatalogo();

private:
    BfEmpresa* EmpresaTrabajo;
    
};

#endif

