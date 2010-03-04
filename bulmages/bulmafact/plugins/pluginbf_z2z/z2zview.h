/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef FACTURAVIEW_H
#define FACTURAVIEW_H

#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>

#include "blfunctions.h"
#include "ui_z2zbase.h"
#include "bldatesearch.h"
#include "bfbuscarformapago.h"
#include "bfbuscarseriefactura.h"
#include "bfbuscaralmacen.h"
#include "bldialogchanges.h"
#include "bfform.h"
#include "bfcompany.h"


/// Muestra y administra la ventana de una factura a cliente.
/** */
class Z2ZView : public BfForm, public Ui_Z2ZBase
{
    Q_OBJECT

public:
    Z2ZView ( BfCompany *, QWidget *parent = 0 );
    ~Z2ZView();
    void calculaTotalTickets();

public slots:
    virtual void on_mui_referencia_returnPressed();
    virtual void on_mui_listarefs_itemDoubleClicked(QListWidgetItem *it);
    virtual void on_mui_traspasar_clicked();
};


#endif

