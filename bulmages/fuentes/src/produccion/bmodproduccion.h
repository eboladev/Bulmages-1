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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BMODPRODUCCION_H
#define BMODPRODUCCION_H

#include "uiproduccion.h"

#include "bselector.h"
#include <qworkspace.h>
#include <qvbox.h>
#include <qframe.h>

/**
@author Josep Burcion
*/
class BSelector;

class BModProduccion : public UIproduccion
{

public:
    BModProduccion(BSelector * ref=0, QWidget * parent = 0, const char * name = 0, WFlags f = WType_TopLevel);
    ~BModProduccion();


public slots:
    virtual void mostrar_selector();

private:
        QWorkspace * zona0;
        BSelector * PunteroAlSelector;
};

#endif
