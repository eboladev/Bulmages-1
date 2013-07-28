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

#ifndef PEDIDOSPROVEEDORLIST_H
#define PEDIDOSPROVEEDORLIST_H

#include "bfbuscararticulo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "pdefs_pluginbf_proveedorpedido.h"


/// Administra las l&iacute;neas de detalle de pedidos a proveedor.
/** */
class PLUGINBF_PROVEEDORPEDIDO_EXPORT PedidosProveedorListSubform : public BfSubForm
{
    Q_OBJECT

public:
    PedidosProveedorListSubform ( QWidget *parent = 0 );
    ~PedidosProveedorListSubform();
    virtual void load();
    virtual void load ( QString query );
};


#include "ui_pedidosproveedorlistbase.h"


/// Muestra y administra la ventana con la informaci&oacute;n de los pedidos a proveedor.
/** */
class PLUGINBF_PROVEEDORPEDIDO_EXPORT PedidosProveedorList : public BlFormList, public Ui_PedidosProveedorListBase
{
    Q_OBJECT

private:
    QString mdb_idpedidoproveedor;

public:
    PedidosProveedorList ( QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    PedidosProveedorList ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE );
    ~PedidosProveedorList();
    void presentar();
    void imprimir();
    void setMainCompany ( BfCompany *comp );
    QString idpedidoproveedor();
    void setidproveedor ( QString val );
    QString generarFiltro();
    void editar ( int );
    void remove();
    void crear();
    void iniciaForm();

};

#endif

