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

#ifndef PAGOSLIST_H
#define PAGOSLIST_H

#include <QtWidgets/QLineEdit>

#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "blformlist.h"


/// Administra el detalle del listado de pagos.
/** */
class PagosListSubForm : public BfSubForm
{
    Q_OBJECT

public:
    PagosListSubForm ( QWidget *parent = 0 );
    ~PagosListSubForm();
};


#include "ui_pagoslistbase.h"


/// Muestra y administra el listado de pagos.
/** */
class PagosList : public BlFormList, private Ui_PagosListBase
{
    Q_OBJECT

private:
    QString mdb_idpago;

public:
    PagosList ( QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    PagosList ( BfCompany *comp = NULL, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ~PagosList();
    void presentar();
    void setMainCompany ( BfCompany *comp );
    QString idpago();
    void imprimir();
    void setidproveedor ( QString val );
    const QString generaFiltro();
    void crear();
    void remove();
    void editar ( int );

};

#endif

