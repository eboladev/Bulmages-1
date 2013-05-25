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

#ifndef LISTALMACENVIEW_H
#define LISTALMACENVIEW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QLabel>
#include <QtWidgets/QCheckBox>

#include "blpostgresqlclient.h"
#include "bldatesearch.h"
#include "bfsubform.h"
#include "bfform.h"
#include "pdefs_pluginbf_almacen.h"


class BfCompany;


/// Muestra y administra las l&iacute;neas de detalle del listado de almacenes.
/** */
class PLUGINBF_ALMACEN_EXPORT ListAlmacenSubForm : public BfSubForm
{
    Q_OBJECT

public:
    ListAlmacenSubForm ( QWidget *parent = 0 );
    virtual ~ListAlmacenSubForm() {};

public slots:
    virtual void load();
};


#include "ui_listalmacenbase.h"


/// Muestra y administra el listado de almacenes.
/** */
class PLUGINBF_ALMACEN_EXPORT ListAlmacenView : public BfForm, public Ui_ListAlmacenBase
{
    Q_OBJECT

public:
    ListAlmacenView ( BfCompany *comp, QWidget *parent );
    virtual ~ListAlmacenView();

public slots:
    virtual void on_mui_aceptar_clicked();
};

#endif
