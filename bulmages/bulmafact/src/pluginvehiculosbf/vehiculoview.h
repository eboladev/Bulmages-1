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
#include "ficha.h"

class company;


/// Clase que muestra la ventana de amortizaci&oacute;n.
/** Formulario de introducci&oacute;n y/o visi&oacute;n de amortizaciones. */
class VehiculoView : public Ficha, Ui_VehiculoBase, DBRecord {
    Q_OBJECT

private:
    /// Empresa con la que se trabaja.
    company *m_companyact;

public:
    VehiculoView(company *, QWidget *parent = 0);
    ~VehiculoView();
    virtual int cargar(QString idvehiculo);
    virtual int guardar();
//    virtual int borrar();


public slots:
//    virtual void contextMenuRequested(int, int, const QPoint &);
//    virtual void on_mui_guardar_clicked();
};

#endif

