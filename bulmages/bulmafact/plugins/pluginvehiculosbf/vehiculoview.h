/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef VEHICULOVIEW_H
#define VEHICULOVIEW_H

#include "postgresiface2.h"
#include "dialogchanges.h"
#include "ui_vehiculobase.h"
#include "dbrecord.h"
#include "fichabf.h"



/// Clase que muestra la ventana de amortizaci&oacute;n.
/** Formulario de introducci&oacute;n y/o visi&oacute;n de amortizaciones. */
class VehiculoView : public FichaBf, Ui_VehiculoBase
{
    Q_OBJECT

public:
    VehiculoView ( Company *, QWidget *parent = 0 );
    ~VehiculoView();
    virtual int cargar ( QString idvehiculo );
    virtual int guardar();

};

#endif

