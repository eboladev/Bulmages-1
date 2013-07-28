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

#ifndef INCIDENCIACOMERCIALVIEW_H
#define INCIDENCIACOMERCIALVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "ui_incidenciacomercialbase.h"
#include "blpostgresqlclient.h"
#include "incidenciacomercial.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"


class IncidenciaComercialView : public IncidenciaComercial, private Ui_IncidenciaComercialBase
{
    Q_OBJECT

public:
    IncidenciaComercialView ( QWidget *parent = NULL );
    ~IncidenciaComercialView();
    IncidenciaComercialView ( BfCompany *, QWidget *parent = NULL );
    void closeEvent ( QCloseEvent * );
    virtual void pintar() {}
    ;
    void setMainCompany ( BfCompany *comp );
    virtual int save();
    virtual int load ( QString id );
};

#endif

