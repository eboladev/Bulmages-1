/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef RUTACOMERCIALVIEW_H
#define RUTACOMERCIALVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_rutacomercialbase.h"
#include "postgresiface2.h"
#include "rutacomercial.h"
#include "busquedacliente.h"
#include "busquedafecha.h"
#include "dialogchanges.h"


class company;


class RutaComercialView : public RutaComercial, public Ui_RutaComercialBase {
    Q_OBJECT

public:
    RutaComercialView(company *, QWidget *parent = NULL);
    RutaComercialView(QWidget *parent = NULL);
    ~RutaComercialView();
    virtual void pintar() {};
    void setEmpresaBase(company *comp);
    virtual int guardar();
    virtual int cargar(QString id);
};

#endif

