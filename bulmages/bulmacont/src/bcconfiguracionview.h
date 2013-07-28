/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org Asociación Iglues -- Contabilidad Linux         *
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

#ifndef BCCONFIGURACIONVIEW_H
#define BCCONFIGURACIONVIEW_H

#include <stdlib.h>

#include <QtWidgets/QLineEdit>

#include "blpostgresqlclient.h"
#include "bldialogchanges.h"
#include "bcform.h"
#include "bcsubform.h"


class BcCompany;


/// Muestra y administra las l&iacute;neas de detalle del listado de configuraci&oacute;nes.
/** */
class BC_EXPORT BcConfiguracionSubForm : public BcSubForm
{
    Q_OBJECT

public:
    BcConfiguracionSubForm ( QWidget *parent = 0 );
    ~BcConfiguracionSubForm();

public slots:
    virtual void load();
};


#include "ui_bcconfiguracionbase.h"


/** Gestion la configuracion de la empresa abierta. Basicamente hace gestiones
sobre la tabla configuracion en dicha empresa
 @TODO: Como estan duplicadas estas tablas tanto en bulmafact conmo en bulmacont se podria hacer una abstraccion en bulmalib.
*/
class BC_EXPORT BcConfiguracionView : public BcForm, public Ui_BcConfiguracionBase
{
    Q_OBJECT

public:
    BcConfiguracionView ( BcCompany *emp, QWidget *parent = 0 );
    ~BcConfiguracionView();
    int inicializa();
    virtual void on_mui_guardar_clicked();

public slots:
    virtual bool close();
    virtual void on_mui_modificarplan_clicked();
};

#endif
