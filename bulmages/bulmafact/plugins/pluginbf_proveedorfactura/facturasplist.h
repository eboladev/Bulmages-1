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

#ifndef FACTURASPROVEEDORLIST_H
#define FACTURASPROVEEDORLIST_H

#include "bfbuscararticulo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bfsubform.h"
#include "blformlist.h"
#include "pdefs_pluginbf_proveedorfactura.h"


/// Administra el detalle del listado de facturas de proveedor.
/** */
class PLUGINBF_PROVEEDORFACTURA_EXPORT FacturasProveedorListSubform : public BfSubForm
{
    Q_OBJECT

public:
    FacturasProveedorListSubform ( QWidget *parent = 0 );
    ~FacturasProveedorListSubform();

public slots:
    virtual void load();
    virtual void load ( QString query );
};


#include "ui_facturasplistbase.h"


/// Administra el listado de facturas de proveedor.
/** */
class PLUGINBF_PROVEEDORFACTURA_EXPORT FacturasProveedorList : public BlFormList, public Ui_FacturasProveedorListBase
{
    Q_OBJECT

private:
    QString mdb_idfacturap;

public:
    FacturasProveedorList ( QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE  );
    FacturasProveedorList ( BfCompany *, QWidget *parent = 0, Qt::WindowFlags flag = 0, edmode editmodo = BL_EDIT_MODE  );
    ~FacturasProveedorList();
    QString idfacturap();
    const QString generaFiltro();
    void presentar();
    void setMainCompany ( BfCompany *comp );
    void setidproveedor ( QString val );
    void setidarticulo ( QString val );
    void editar ( int );
    void remove();
    void crear();
    void imprimir();
    void iniciaForm();
};

#endif

