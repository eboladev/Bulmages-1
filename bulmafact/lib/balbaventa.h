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
#ifndef BALBAVENTA_H
#define BALBAVENTA_H

#include ".ui/uialbaventa.h"
#include "bfempresa.h"

#include <qlineedit.h>
#include <qlabel.h>
#include <qscrollbar.h>
#include <qgroupbox.h>
#include <qtable.h>
#include <qrect.h>
#include <qlayout.h>

/**
@author Josep Burcion
*/
class BAlbaVenta : public UIAlbaVenta
{
public:
    BAlbaVenta(BfEmpresa* punteroEmpresaTrabajo, QWidget * parent = 0, const char * name = 0);
    ~BAlbaVenta();


private:
    BfEmpresa* EmpresaTrabajo;
    
private slots:    
    virtual void ScrollCabeceraV(int valor=0);
    virtual void ScrollCabeceraH(int valor=0);
    virtual void resizeEvent (QResizeEvent * evento=0);


};

#endif
