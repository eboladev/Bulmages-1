/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

#ifndef BFBUSCARFAMILIA_H
#define BFBUSCARFAMILIA_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>

#include "blwidget.h"
#include "blsearchwidget.h"


class BfCompany;


/// Permite buscar y seleccionar una familia de art&iacute;culos.
/** Este Widget permite buscar y seleccionar una familia de art&iacute;culos.
    Para usar en forma de selector de articulos. */
class BF_EXPORT BfBuscarFamilia : public BlSearchWidget
{
    Q_OBJECT
public:
    BfBuscarFamilia ( QWidget *parent = 0 );
    ~BfBuscarFamilia();
    virtual QString codigocompletofamilia();
    virtual QString idfamilia();
    virtual QString nombrefamilia();
    virtual void setidfamilia ( QString val );
    virtual void setcodigocompletofamilia ( QString val );
};

#endif

