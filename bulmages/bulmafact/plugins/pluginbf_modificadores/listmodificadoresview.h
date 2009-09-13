/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#ifndef LISTMODIFICADORESVIEW_H
#define LISTMODIFICADORESVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bfsubform.h"
#include "bfform.h"


class BfCompany;


/// Administra el detalle del listado de series de factura.
/** */
class ListModificadoresSubForm : public BfSubForm
{
    Q_OBJECT

public:
    ListModificadoresSubForm ( QWidget *parent = 0 );
    ~ListModificadoresSubForm() {}

public slots:
    virtual void cargar() {
        _depura ( "ListModificadoresSubForm::cargar\n", 0 );
        BlSubForm::cargar ( "SELECT * FROM modificador" );
    }
};


#include "ui_listmodificadoresbase.h"


/// Muestra y administra la ventana con la lista de series de facturas.
/** */
class ListModificadoresView : public BfForm, public Ui_ListModificadoresBase
{
    Q_OBJECT

public:
    ListModificadoresView ( BfCompany *, QWidget * );
    virtual ~ListModificadoresView();

public slots:
    virtual void on_mui_aceptar_released() {
        mui_listado->guardar();
        close();
    }

};

#endif

