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

#include <stdio.h>

#include <QtWidgets/QMessageBox>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "pedidosproveedorlist.h"
#include "blfunctions.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "pedidoproveedorview.h"


///
/**
\param parent
\param flag
**/
PedidosProveedorList::PedidosProveedorList ( QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    mdb_idpedidoproveedor = "";
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";
    setSubForm ( mui_list );
    hideBusqueda();
    iniciaForm();
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
\param comp
\param parent
\param flag
**/
PedidosProveedorList::PedidosProveedorList ( BfCompany *comp, QWidget *parent, Qt::WindowFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    BL_FUNC_DEBUG
    setupUi ( this );
    m_proveedor->setMainCompany ( comp );
    m_articulo->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    /// Establecemos los parametros de busqueda del Cliente
    m_proveedor->setLabel ( _ ( "Proveedor:" ) );
    m_proveedor->setTableName ( "proveedor" );
    m_proveedor->m_valores["cifproveedor"] = "";
    m_proveedor->m_valores["nomproveedor"] = "";

    setSubForm ( mui_list );
    mdb_idpedidoproveedor = "";
    if ( editMode() ) {
        mainCompany() ->insertWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    iniciaForm();
    
    /// Cargamos los filtros guardados.
    cargaFiltrosXML();

    presentar();
    
    /// Hacemos el tratamiento de los permisos que desabilita botones en caso de no haber suficientes permisos.
    trataPermisos ( "pedidoproveedor" );
    /// Llamamos a los scripts
    blScript(this);
    
}


///
/**
**/
PedidosProveedorList::~PedidosProveedorList()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
void PedidosProveedorList::iniciaForm()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    int res = g_plugins->run ( "PedidosProveedorList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _ ( "Todos los pedidos" ) );
    mui_procesada->insertItem ( 1, _ ( "Pedidos procesados" ) );
    mui_procesada->insertItem ( 2, _ ( "Pedidos no procesados" ) );
    
}


///
/**
\returns
**/
void PedidosProveedorList::presentar()
{
    BL_FUNC_DEBUG
    mui_list->load ( "SELECT *, totalpedidoproveedor AS total, bimppedidoproveedor AS base, imppedidoproveedor AS impuestos FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro() );
    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalpedidoproveedor) AS total FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro() );
    /// En caso de que el query haya fallado (por problemas de permisos p.ej.) salimos.
    if ( !cur ) return;
    m_total->setText ( cur->value( "total" ) );
    delete cur;
    
}


///
/**
\return
**/
QString PedidosProveedorList::generarFiltro()
{
    BL_FUNC_DEBUG
    /// Tratamiento de los filtros.
    QString filtro = "";

    if ( m_filtro->text() != "" ) {
        filtro = " AND ( lower(descpedidoproveedor) LIKE lower('%" + m_filtro->text() + "%') ";
        filtro += " OR refpedidoproveedor LIKE '" + m_filtro->text() + "%' ";
        filtro += " OR lower(nomproveedor) LIKE lower('%" + m_filtro->text() + "%')) ";
    } // end if
    if ( m_proveedor->id() != "" ) {
        filtro += " AND pedidoproveedor.idproveedor = " + m_proveedor->id();
    } // end if


    /// Tratamos los elementos procesados y no procesados.
    if ( mui_procesada->currentIndex() == 1 ) {
        /// Muestra solo las procesadas.
        filtro += " AND procesadopedidoproveedor";
    } else if ( mui_procesada->currentIndex() == 2 ) {
        /// Muestra solo las NO procesadas.
        filtro += " AND NOT procesadopedidoproveedor ";
    } // end if


    if ( m_articulo->idarticulo() != "" ) {
        filtro += " AND idpedidoproveedor IN (SELECT DISTINCT idpedidoproveedor FROM lpedidoproveedor WHERE idarticulo = '" + m_articulo->idarticulo() + "')";
    } // end if
    if ( m_fechain->text() != "" ) {
        filtro += " AND fechapedidoproveedor >= '" + m_fechain->text() + "' ";
    } // end if
    if ( m_fechafin->text() != "" ) {
        filtro += " AND fechapedidoproveedor <= '" + m_fechafin->text() + "' ";
        //filtro += " ORDER BY "+orden[m_orden->currentItem()];
    } // end if
    
    return ( filtro );
}


///
/**
**/
void PedidosProveedorList::imprimir()
{
    BL_FUNC_DEBUG
    printPDF ( _ ( "Pedidos a proveedores" ) );
    
}


///
/**
\return
**/
void PedidosProveedorList::remove()
{
    BL_FUNC_DEBUG
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idpedidoproveedor = mui_list->dbValue ( QString ( "idpedidoproveedor" ) );
        if ( editMode() ) {
            PedidoProveedorView * ppv = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( ppv->load ( mdb_idpedidoproveedor ) ) {
                throw - 1;
            } // end if
            ppv->on_mui_borrar_clicked();
            ppv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el pedido a proveedor" ), this );
    } // end try
    
}


///
/**
\param row
**/
void PedidosProveedorList::editar ( int row )
{
    BL_FUNC_DEBUG
    try {
        mdb_idpedidoproveedor = mui_list->dbValue ( QString ( "idpedidoproveedor" ), row );
        if ( editMode() ) {
            PedidoProveedorView * prov = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( prov->load ( mdb_idpedidoproveedor ) ) {
                delete prov;
                return;
            } // end if
            mainCompany() ->m_pWorkspace->addSubWindow ( prov );
            prov->show();
        } else {
            emit ( selected ( mdb_idpedidoproveedor ) );
        } // end if
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al cargar el pedido proveedor" ), this );
    } // end try
    
}


///
/**
**/
void PedidosProveedorList::crear()
{
    BL_FUNC_DEBUG
    PedidoProveedorView *ppv = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( ppv );
    ppv->pintar();
    
    /// Si se crea el nuevo pedido desde la lista de pedidos a un proveedor,
    /// entonces se crea el  pedido a este proveedor.
    if (m_proveedor->id() != "") {
	ppv->mui_idproveedor->setId( m_proveedor->id() );
    } // end if
    
    ppv->show();
    
}


///
/**
\param comp
**/
void PedidosProveedorList::setMainCompany ( BfCompany *comp )
{
    BL_FUNC_DEBUG
    BlMainCompanyPointer::setMainCompany ( comp );
    m_proveedor->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    
}


///
/**
\return
**/
QString PedidosProveedorList::idpedidoproveedor()
{
    BL_FUNC_DEBUG
    return mdb_idpedidoproveedor;
    
}


///
/**
\param val
**/
void PedidosProveedorList::setidproveedor ( QString val )
{
    BL_FUNC_DEBUG
    m_proveedor->setId ( val );
    
}


/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
///
/**
\param parent
**/
PedidosProveedorListSubform::PedidosProveedorListSubform ( QWidget *parent ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    setDbTableName ( "pedidoproveedor" );
    setDbFieldId ( "idpedidoproveedor" );
    addSubFormHeader ( "numpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Numero de pedido" ) );
    addSubFormHeader ( "nomproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre de proveedor" ) );
    addSubFormHeader ( "fechapedidoproveedor", BlDbField::DbDate, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Fecha pedido proveedor" ) );
    addSubFormHeader ( "refpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Referencia pedido" ) );
    addSubFormHeader ( "base", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Base Imponible" ) );
    addSubFormHeader ( "impuestos", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Impuestos" ) );
    addSubFormHeader ( "total", BlDbField::DbNumeric, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Total pedido" ) );
    addSubFormHeader ( "descpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Descripcion pedido proveedor" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo de almacen" ) );
    addSubFormHeader ( "contactpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Persona de contacto proveedor" ) );
    addSubFormHeader ( "telpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Telefono proveedor" ) );
    addSubFormHeader ( "comentpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Comentario pedido proveedor" ) );
    addSubFormHeader ( "idpedidoproveedor", BlDbField::DbInt, BlDbField::DbNotNull | BlDbField::DbPrimaryKey, BlSubFormHeader::DbHideView | BlSubFormHeader::DbNoWrite, _ ( "Id pedido" ) );
    addSubFormHeader ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id trabajador" ) );
    addSubFormHeader ( "idproveedor", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id proveedor" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNoSave, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id almacen" ) );
    setInsert ( false );
    setDelete ( false );
    setSortingEnabled ( true );
    
}


///
/**
**/
void PedidosProveedorListSubform::load()
{
    BL_FUNC_DEBUG
    QString SQLQuery = "SELECT * FROM pedidoproveedor";
    BlSubForm::load ( SQLQuery );
    
}


///
/**
\param query
**/
void PedidosProveedorListSubform::load ( QString query )
{
    BL_FUNC_DEBUG
    BlSubForm::load ( query );
    
}


///
/**
**/
PedidosProveedorListSubform::~PedidosProveedorListSubform()
{
    BL_FUNC_DEBUG
    
}
