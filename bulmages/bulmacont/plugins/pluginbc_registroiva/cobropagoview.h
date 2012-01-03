/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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

#ifndef COBROPAGOVIEW_H
#define COBROPAGOVIEW_H

#include "bcform.h"
#include "ui_cobropagobase.h"
#include "bccompany.h"


/// Listado de gestion de cobros y pagos.
/// TODO: Esta clase debe derivar de BcForm
class cobropagoview : public BcForm, public Ui_CobroPagoBase
{
    Q_OBJECT

private:
    BcCompany *m_companyact;

public:
    cobropagoview ( BcCompany *, QWidget *parent = 0 );
    ~cobropagoview();

public slots:
    virtual void on_mui_actualizar_clicked();
    virtual void s_guardar();
    virtual void s_recalculaSaldo();
};

#endif