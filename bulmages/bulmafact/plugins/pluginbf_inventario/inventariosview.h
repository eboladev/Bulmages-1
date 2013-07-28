/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#ifndef INVENTARIOSVIEW_H
#define INVENTARIOSVIEW_H

#include "bfcompany.h"
#include "bfsubform.h"
#include "blformlist.h"


/// Muestra y administra las l&iacute;neas de detalle del listado de inventarios.
/** */
class InventariosSubForm : public BfSubForm
{
    Q_OBJECT

public:
    InventariosSubForm ( QWidget *parent = 0 );
    ~InventariosSubForm() {};

public slots:
    virtual void load();
};


#include "ui_inventariosbase.h"


/// Muestra y administra el listado de inventarios.
/** */
class InventariosView : public BlFormList, public Ui_InventariosBase
{
    Q_OBJECT

public:
    InventariosView ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmode = BL_EDIT_MODE );
    ~InventariosView();
    void inicializa();
    void presentar();
    virtual void on_mui_imprimir_clicked();
    virtual void on_mui_crear_clicked();
    virtual void on_mui_editar_clicked();
    virtual void on_mui_listado_itemDoubleClicked ( QTableWidgetItem * );

public slots:
    virtual void on_mui_listado_itemDoubleClicked();
    virtual void on_mui_borrar2_clicked();
};

#endif

