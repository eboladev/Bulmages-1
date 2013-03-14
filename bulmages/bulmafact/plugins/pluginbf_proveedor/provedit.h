/***************************************************************************
 *   Copyright (C) 2004 by J. M. Torres Rigo                               *
 *   joanmi@bulma.net                                                      *
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

#ifndef PROVEDIT_H
#define PROVEDIT_H

#include "ui_proveditbase.h"
#include "bldb.h"
#include "bfform.h"
#include "pdefs_pluginbf_proveedor.h"


/// Muestra y administra la ventana con la informaci&oacute;n de un proveedor.
/** */
class PLUGINBF_PROVEEDOR_EXPORT ProveedorView : public BfForm, public Ui_ProveedorBase
{
    Q_OBJECT

public:
    ProveedorView ( BfCompany *comp, QWidget *parent = 0 );
    ~ProveedorView();
    virtual int beforeSave();
    virtual int afterSave();
    virtual void pintarPost();

public slots:
    virtual void on_mui_cifproveedor_lostFocus();

public:
    virtual int cargarPost ( QString );
};

#endif

