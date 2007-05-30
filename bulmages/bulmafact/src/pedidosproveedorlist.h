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

#ifndef PEDIDOSPROVEEDORLIST_H
#define PEDIDOSPROVEEDORLIST_H

#include "busquedaproveedor.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "listado.h"


/// Administra las l&iacute;neas de detalle de pedidos a proveedor.
/** */
class PedidosProveedorListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    PedidosProveedorListSubform(QWidget *parent = 0);
    ~PedidosProveedorListSubform();
    virtual void cargar();
    virtual void cargar(QString query);
};


#include "ui_pedidosproveedorlistbase.h"


/// Muestra y administra la ventana con la informaci&oacute;n de los pedidos a proveedor.
/** */
class PedidosProveedorList : public Listado, public Ui_PedidosProveedorListBase {
    Q_OBJECT

private:
    QString mdb_idpedidoproveedor;

public:
    PedidosProveedorList(QWidget *parent = 0, Qt::WFlags flag = 0);
    PedidosProveedorList(company *, QWidget *parent = 0, Qt::WFlags flag = 0);
    ~PedidosProveedorList();
    void presentar();
    void imprimir();
    void setEmpresaBase(company *comp);
    QString idpedidoproveedor();
    void setidproveedor(QString val);
    QString generarFiltro();
    void editar(int);
    void borrar();
    void crear();
    void iniciaForm();

};

#endif

