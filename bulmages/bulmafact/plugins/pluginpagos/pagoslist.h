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


#include <QLineEdit>

#include "company.h"
#include "busquedaproveedor.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "listado.h"

/// Administra el detalle del listado de pagos.
/** */
class PagosListSubForm : public SubForm2Bf
{
    Q_OBJECT

public:
    PagosListSubForm ( QWidget *parent = 0 );
    ~PagosListSubForm();
};


#include "ui_pagoslistbase.h"


/// Muestra y administra el listado de pagos.
/** */
class PagosList : public Listado, private Ui_PagosListBase
{
    Q_OBJECT

private:
    QString mdb_idpago;

public:
    PagosList ( QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    PagosList ( Company *comp = NULL, QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode );
    ~PagosList();
    void presentar();
    void setEmpresaBase ( Company *comp );
    QString idpago();
    void imprimir();
    void setidproveedor ( QString val );
    QString generaFiltro();
    void crear();
    void borrar();
    void editar ( int );

};

#endif

