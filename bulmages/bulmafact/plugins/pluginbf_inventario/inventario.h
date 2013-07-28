/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <QtCore/QString>

#include "listcontrolstockview.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfform.h"


/// Administra los datos de inventario.
/** */
class Inventario : public BfForm
{
protected:
    ListControlStockView *listalineas;
    BfCompany *companyact;

public:
    Inventario ( BfCompany *comp, QWidget *parent );
    virtual ~Inventario();
    void setListControlStock ( ListControlStockView *a );
    ListControlStockView *getlistalineas();
    void pintaInventario();
    virtual int save();
    virtual int remove();
    void imprimirInventario();
    virtual void pintaIdInventario ( QString );
    virtual void pintaFechaInventario ( QString );
    virtual void pintaNomInventario ( QString );
    void setIdInventario ( QString val );
    void setFechaInventario ( QString val );
    void setNomInventario ( QString val );
    void vaciaInventario();
};

#endif

