/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C. (Porting to QT4)              *
 *   fcojavmc@todo-redes.com                                               *
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

#ifndef LISTSERIEVIEW_H
#define LISTSERIEVIEW_H

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
class BF_EXPORT ListSerieFacturaSubForm : public BfSubForm
{
    Q_OBJECT

public:
    ListSerieFacturaSubForm ( QWidget *parent = 0 );
    ~ListSerieFacturaSubForm() {}

public slots:
    virtual void cargar() {
        _depura ( "ListSerieFacturaSubForm::cargar\n", 0 );
        BlSubForm::cargar ( "SELECT *, codigoserie_factura AS codigoserie_facturaorig FROM serie_factura" );
    }
};


#include "ui_listseriefacturabase.h"


/// Muestra y administra la ventana con la lista de series de facturas.
/** */
class BF_EXPORT ListSerieFacturaView : public BfForm, public Ui_ListSerieFacturaBase
{
    Q_OBJECT

public:
    ListSerieFacturaView ( BfCompany *, QWidget * );
    virtual ~ListSerieFacturaView();

public slots:
    virtual void on_mui_aceptar_clicked() {
        mui_listado->guardar();
        close();
    }

};

#endif

