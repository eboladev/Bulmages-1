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

#include <fstream>

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QLayout>
#include <QTextStream>

#include "pedidoproveedorview.h"
#include "bfcompany.h"
#include "listlinpedidoproveedorview.h"
#include "providerslist.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
PedidoProveedorView::PedidoProveedorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Pedido Proveedor" ) );
        setDbTableName ( "pedidoproveedor" );
        setDbFieldId ( "idpedidoproveedor" );
        addDbField ( "idpedidoproveedor", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id pedido proveedor" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id almacen" ) );
        addDbField ( "numpedidoproveedor", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero pedido proveedor" ) );
        addDbField ( "fechapedidoproveedor", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha pedido proveedor" ) );
        addDbField ( "comentpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario pedido" ) );
        addDbField ( "procesadopedidoproveedor", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Pedido procesado" ) );
        addDbField ( "descpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion pedido" ) );
        addDbField ( "refpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia pedido" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id trabajador" ) );
        addDbField ( "contactpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Persona de contacto proveedor" ) );
        addDbField ( "telpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono proveedor" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "PedidoProveedorView_PedidoProveedorView", this );
        if ( res != 0 ) {
            return;
        } // end if

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_lineasDetalle->setMainCompany ( comp );
        mui_lineasDetalle->inicializar();
        mui_idproveedor->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idforma_pago->setId ( "0" );
        mui_descuentos->setMainCompany ( comp );
        mui_descuentos->inicializar();
        mui_idalmacen->setMainCompany ( comp );
        mui_idalmacen->setId ( "0" );
        mui_idtrabajador->setMainCompany ( comp );
        mui_idtrabajador->setId ( "0" );
        mui_refpedidoproveedor->setMainCompany ( comp );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

        setListaLineas ( mui_lineasDetalle );
        setListaDescuentos ( mui_descuentos );

        dialogChanges_readValues();
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el pedido a proveedor" ), this );
    } // end try
    
}


///
/**
**/
PedidoProveedorView::~PedidoProveedorView()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param iva
\param base
\param total
\param desc
\param irpf
\param reqeq
**/
void PedidoProveedorView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    mui_totalBaseImponible->setText ( base.toQString() );
    mui_totalImpuestos->setText ( iva.toQString() );
    mui_totalPedido->setText ( total.toQString() );
    mui_totalDescuentos->setText ( desc.toQString() );
    mui_totalIRPF->setText ( QString ( irpf.toQString() ) );
    mui_totalRecargo->setText ( QString ( reqeq.toQString() ) );
    
}




///
/**
\param id
**/
void PedidoProveedorView::on_mui_idproveedor_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    mui_lineasDetalle->setIdProveedor ( id );
    mui_idforma_pago->setIdProveedor ( id );
}


///
/**
**/
void PedidoProveedorView::on_mui_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
}


///
/**
**/
void PedidoProveedorView::on_mui_lineasDetalle_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
}


///
/**
\return
**/
int PedidoProveedorView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}


/// Esta funcion termina la carga de de un pedido a proveedor.
/**
\param idbudget
\return
**/
int PedidoProveedorView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->run ( "PedidoProveedorView_cargarPost_Post", this );

    calculaypintatotales();

    
    return 0;
}


///
/**
\return
**/
int PedidoProveedorView::afterSave()
{
    BL_FUNC_DEBUG

    m_listalineas->setColumnValue ( "idpedidoproveedor", dbValue ( "idpedidoproveedor" ) );
    m_listadescuentos->setColumnValue ( "idpedidoproveedor", dbValue ( "idpedidoproveedor" ) );

    m_listalineas->save();
    m_listadescuentos->save();
    return 0;
}


