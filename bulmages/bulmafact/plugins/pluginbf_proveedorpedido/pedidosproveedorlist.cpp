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

#include <QMessageBox>
#include <QFile>
#include <QTextStream>

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
PedidosProveedorList::PedidosProveedorList ( QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( NULL, parent, flag, editmodo )
{
    blDebug ( "PedidosProveedorList::PedidosProveedorList", 0 );
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
    blDebug ( "END PedidosProveedorList::PedidosProveedorList", 0 );
}


///
/**
\param comp
\param parent
\param flag
**/
PedidosProveedorList::PedidosProveedorList ( BfCompany *comp, QWidget *parent, Qt::WFlags flag, edmode editmodo )
        : BlFormList ( comp, parent, flag, editmodo )
{
    blDebug ( "PedidosProveedorList::PedidosProveedorList", 0 );
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
    if ( modoEdicion() ) {
        mainCompany() ->meteWindow ( windowTitle(), this );
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
    blDebug ( "END PedidosProveedorList::PedidosProveedorList", 0 );
}


///
/**
**/
PedidosProveedorList::~PedidosProveedorList()
{
    blDebug ( "PedidosProveedorList::~PedidosProveedorList", 0 );
    blDebug ( "END PedidosProveedorList::~PedidosProveedorList", 0 );
}


///
/**
\return
**/
void PedidosProveedorList::iniciaForm()
{
    blDebug ( "PedidosProveedorList::iniciaForm" );
    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "PedidosProveedorList_iniciaForm", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _ ( "Todos los pedidos" ) );
    mui_procesada->insertItem ( 1, _ ( "Pedidos procesados" ) );
    mui_procesada->insertItem ( 2, _ ( "Pedidos no procesados" ) );
    blDebug ( "END PedidosProveedorList::iniciaForm" );
}


///
/**
\returns
**/
void PedidosProveedorList::presentar()
{
    blDebug ( "PedidosProveedorList::presentar", 0 );
    mui_list->cargar ( "SELECT *, totalpedidoproveedor AS total, bimppedidoproveedor AS base, imppedidoproveedor AS impuestos FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro() );
    /// Hacemos el calculo del total.
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT SUM(totalpedidoproveedor) AS total FROM pedidoproveedor LEFT JOIN proveedor ON pedidoproveedor.idproveedor=proveedor.idproveedor LEFT JOIN almacen ON pedidoproveedor.idalmacen=almacen.idalmacen WHERE 1=1 " + generarFiltro() );
    /// En caso de que el query haya fallado (por problemas de permisos p.ej.) salimos.
    if ( !cur ) return;
    m_total->setText ( cur->valor ( "total" ) );
    delete cur;
    blDebug ( "END PedidosProveedorList::presentar", 0 );
}


///
/**
\return
**/
QString PedidosProveedorList::generarFiltro()
{
    blDebug ( "PedidosProveedorList::generarFiltro", 0 );
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
    blDebug ( "END PedidosProveedorList::generarFiltro", 0 );
    return ( filtro );
}


///
/**
**/
void PedidosProveedorList::imprimir()
{
    blDebug ( "PedidosProveedorList::imprimir", 0 );
    mui_list->imprimirPDF ( _ ( "Pedidos a proveedores" ) );
    blDebug ( "END PedidosProveedorList::imprimir", 0 );
}


///
/**
\return
**/
void PedidosProveedorList::borrar()
{
    blDebug ( "PedidosProveedorList::borrar", 0 );
    int a = mui_list->currentRow();
    if ( a < 0 ) {
        blMsgInfo ( _ ( "Debe seleccionar una linea" ), this );
        return;
    } // end if
    try {
        mdb_idpedidoproveedor = mui_list->dbValue ( QString ( "idpedidoproveedor" ) );
        if ( modoEdicion() ) {
            PedidoProveedorView * ppv = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( ppv->cargar ( mdb_idpedidoproveedor ) ) {
                throw - 1;
            } // end if
            ppv->on_mui_borrar_clicked();
            ppv->close();
        } // end if
        presentar();
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al borrar el pedido a proveedor" ), this );
    } // end try
    blDebug ( "END PedidosProveedorList::borrar", 0 );
}


///
/**
\param row
**/
void PedidosProveedorList::editar ( int row )
{
    blDebug ( "PedidosProveedorList::editar", 0 );
    try {
        mdb_idpedidoproveedor = mui_list->dbValue ( QString ( "idpedidoproveedor" ), row );
        if ( modoEdicion() ) {
            PedidoProveedorView * prov = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
            if ( prov->cargar ( mdb_idpedidoproveedor ) ) {
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
    blDebug ( "END PedidosProveedorList::editar", 0 );
}


///
/**
**/
void PedidosProveedorList::crear()
{
    blDebug ( "PedidosProveedorList::crear", 0 );
    PedidoProveedorView *ppv = new PedidoProveedorView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( ppv );
    ppv->pintar();
    
    /// Si se crea el nuevo pedido desde la lista de pedidos a un proveedor,
    /// entonces se crea el  pedido a este proveedor.
    if (m_proveedor->id() != "") {
	ppv->mui_idproveedor->setId( m_proveedor->id() );
    } // end if
    
    ppv->show();
    blDebug ( "END PedidosProveedorList::crear", 0 );
}


///
/**
\param comp
**/
void PedidosProveedorList::setMainCompany ( BfCompany *comp )
{
    blDebug ( "PedidosProveedorList::setMainCompany", 0 );
    BlMainCompanyPointer::setMainCompany ( comp );
    m_proveedor->setMainCompany ( comp );
    mui_list->setMainCompany ( comp );
    blDebug ( "END PedidosProveedorList::setMainCompany", 0 );
}


///
/**
\return
**/
QString PedidosProveedorList::idpedidoproveedor()
{
    blDebug ( "PedidosProveedorList::idpedidoproveedor", 0 );
    return mdb_idpedidoproveedor;
    blDebug ( "END PedidosProveedorList::idpedidoproveedor", 0 );
}


///
/**
\param val
**/
void PedidosProveedorList::setidproveedor ( QString val )
{
    blDebug ( "PedidosProveedorList::setidproveedor", 0 );
    m_proveedor->setId ( val );
    blDebug ( "END PedidosProveedorList::setidproveedor", 0 );
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
    blDebug ( "PedidosProveedorListSubform::PedidosProveedorListSubform", 0 );
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
    setInsert ( FALSE );
    setDelete ( FALSE );
    setSortingEnabled ( TRUE );
    blDebug ( "END PedidosProveedorListSubform::PedidosProveedorListSubform", 0 );
}


///
/**
**/
void PedidosProveedorListSubform::cargar()
{
    blDebug ( "PedidosProveedorListSubform::cargar", 0 );
    QString SQLQuery = "SELECT * FROM pedidoproveedor";
    BlSubForm::cargar ( SQLQuery );
    blDebug ( "END PedidosProveedorListSubform::cargar", 0 );
}


///
/**
\param query
**/
void PedidosProveedorListSubform::cargar ( QString query )
{
    blDebug ( "PedidosProveedorListSubform::cargar", 0 );
    BlSubForm::cargar ( query );
    blDebug ( "END PedidosProveedorListSubform::cargar", 0 );
}


///
/**
**/
PedidosProveedorListSubform::~PedidosProveedorListSubform()
{
    blDebug ( "PedidosProveedorListSubform::~PedidosProveedorListSubform", 0 );
    blDebug ( "END PedidosProveedorListSubform::~PedidosProveedorListSubform", 0 );
}
