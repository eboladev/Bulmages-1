/***************************************************************************
 *   Copyright (C) 2011 by Tomeu Borras Riera                              *
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

#ifndef LISTCUOTASPORACTIVIDADVIEW_H
#define LISTCUOTASPORACTIVIDADVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bfsubform.h"
#include "bfform.h"
#include "pdefs_pluginbf_alumno.h"


class BfCompany;


/// Muestra y administra las l&iacute;neas de detalle del listado.
/** */
class PLUGINBF_ALUMNO_EXPORT ListCuotasPorActividadSubForm : public BfSubForm
{
    Q_OBJECT

public:
    ListCuotasPorActividadSubForm ( QWidget *parent = 0 );
    virtual ~ListCuotasPorActividadSubForm() {};

public slots:
    virtual void load();
};


#include "ui_listcuotasporactividadbase.h"


/// Muestra y administra el listado.
/** */
class PLUGINBF_ALUMNO_EXPORT ListCuotasPorActividadView : public BfForm, public Ui_ListCuotasPorActividadBase
{
    Q_OBJECT

public:
    ListCuotasPorActividadView ( BfCompany *comp, QWidget *parent );
    virtual ~ListCuotasPorActividadView();

public slots:
    virtual void on_mui_aceptar_clicked();
};

#endif

