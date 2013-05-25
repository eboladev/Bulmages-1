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

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "ui_rutacomercialbase.h"
#include "blpostgresqlclient.h"
#include "bfform.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"


class BfCompany;


class RutaComercialView : public BfForm, public Ui_RutaComercialBase
{
    Q_OBJECT

public:
    RutaComercialView ( BfCompany *, QWidget *parent = NULL );
    RutaComercialView ( QWidget *parent = NULL );
    ~RutaComercialView();
/*
    virtual void pintar() {};
    void setMainCompany ( BfCompany *comp );
    virtual int save();
    virtual int load ( QString id );
*/
};

#endif

