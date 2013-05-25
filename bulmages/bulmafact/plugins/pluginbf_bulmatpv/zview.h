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

#ifndef ZVIEW_H
#define ZVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "ui_zbase.h"
#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "pdefs_pluginbf_bulmatpv.h"


class BfCompany;


/** Ventana de Ver Cuadre de Caja.
    Se encarga de la presentacion de la ficha de cobro y del tratamiento de eventos producidos
    en dicha ventana.
    Deriva de Ficha para metodos comunes a todas las ventanas.
    Deriva de Cobro para el manejo de la Base de datos. */
class PLUGINBF_BULMATPV_EXPORT ZView : public BfForm, public Ui_ZBase
{
    Q_OBJECT

public:
    ZView ( BfCompany *, QWidget * );
    ~ZView();
    int cargarPost ( QString idz );

public slots:
     virtual void on_mui_list_cellDoubleClicked(int row, int);
     virtual void on_mui_actualizar_clicked();
};

#endif

