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
 *                                                                         *
 ***************************************************************************/
#ifndef BVisorEmpresas_H
#define BVisorEmpresas_H


#include "uivisorempresas.h"
#include "bfempresa.h"
#include <qlistview.h>

class BfCursor;

/**
@author Josep Burcion
*/

class BVisorEmpresas : public UIvisorEmpresas
{

public:
    BVisorEmpresas(QString * ptrRetorno=0, BfCursor* recordSet=0, QWidget * parent = 0, const char * name = 0, WFlags f = WType_TopLevel);
    ~BVisorEmpresas();


private slots:
    virtual void botonAceptar();
    virtual void botonCancelar();


};

#endif
