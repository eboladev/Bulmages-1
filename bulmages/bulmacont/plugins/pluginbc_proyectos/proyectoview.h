/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#ifndef PROYECTOVIEW_H
#define PROYECTOVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "ui_proyectobase.h"
#include "blpostgresqlclient.h"
#include "bldialogchanges.h"
#include "bcform.h"


class ProyectoView : public BcForm, public Ui_ProyectoBase
{
    Q_OBJECT

public:
    ProyectoView ( BcCompany *, QWidget * );
    ~ProyectoView();
    int cargarPost ( QString );
    int afterSave();

public slots:
    void on_mui_geninforme_clicked();

signals:
    void saved();
};

#endif

