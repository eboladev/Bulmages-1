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

#include <QtCore/QFile>
#include <QtWidgets/QMessageBox>
#include <QtCore/QTextStream>

#include "bfbuscararticulo.h"
#include "bldatesearch.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "movimientosview.h"
#include "blfunctions.h"
#include "blplugins.h"
#include "blform.h"
#include "blmaincompany.h"


/** Prepara todos los componentes i los inicializa a NULL para que no haya dudas sobre si
    ya han sido inicializados o no.
    Inicializando la clase con este constructor precisa que sea establecido el company con setcompany.
*/
/** Inicializa todos los componentes y prepara la ventana para funcionar.
*/


///
/**
\param comp
\param parent
\param editmodo
**/
MovimientosView::MovimientosView ( BfCompany *comp, QWidget *parent, edmode editmodo )
        : BfForm ( comp, parent, 0 )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    setupUi ( this );
    iniciaForm();
    setDbTableName ( "movimiento" );
    m_cliente->setMainCompany ( mainCompany() );
    m_articulo->setMainCompany ( mainCompany() );
    mui_list->setMainCompany ( mainCompany() );
    presenta();
    m_modo = editmodo;
    mdb_idfactura = "";
    /// Establecemos los parametros de busqueda del Cliente
    m_cliente->setLabel ( _ ( "Cliente:" ) );
    m_cliente->setTableName ( "cliente" );
    m_cliente->m_valores["cifcliente"] = "";
    m_cliente->m_valores["nomcliente"] = "";
    if ( m_modo == BL_EDIT_MODE ) {
        insertWindow ( windowTitle(), this );
    } // end if
    hideBusqueda();
    blScript(this);
    
}


///
/**
\return
**/
void MovimientosView::iniciaForm()
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    int res = g_plugins->run ( "MovimientosView_MovimientosView", this );
    if ( res != 0 )
        return;
    mui_procesada->insertItem ( 0, _ ( "Todas las facturas" ) );
    mui_procesada->insertItem ( 1, _ ( "Facturas procesadas" ) );
    mui_procesada->insertItem ( 2, _ ( "Facturas no procesadas" ) );
    
}


/** No precisa de acciones especiales en el destructor de la clase.
*/
///
/**
**/
MovimientosView::~MovimientosView()
{
    BL_FUNC_DEBUG
    mainCompany() ->removeWindow ( this );
    
}


/** Hace la carga del subformulario y el calculo de los totales.
*/
/**
**/
void MovimientosView::presenta()
{
    BL_FUNC_DEBUG

    mui_list->load ( "SELECT * FROM movimiento LEFT JOIN articulo ON movimiento.idarticulo=articulo.idarticulo LEFT JOIN almacen ON movimiento.idalmacen = almacen.idalmacen ORDER BY idmovimiento" );

    
}


/** Funcion auxiliar que genera la clausula WHERE de la consulta del listado a partir de las opciones de filtrado que el usuario haya especificado.
*/
/**
\return
**/
const QString MovimientosView::generaFiltro()
{
    BL_FUNC_DEBUG
    /// Tratamiento de los filtros.
    QString filtro = "";

    /// Hacemos el filtrado like del campo m_filtro
    filtro += mui_list->likeFilterSQL(m_filtro->text());
        
    return ( filtro );
}




/** SLOT que responde a la pulsacion del boton de imprimir.
    La impresion de listados esta completamente delegada en BfSubForm
*/
///
/**
**/
void MovimientosView::on_mui_imprimir_clicked()
{
    BL_FUNC_DEBUG
    mui_list->printPDF ( _ ( "Facturas a clientes" ) );
    
}





/// =============================================================================
///                    SUBFORMULARIO
/// =============================================================================
/** Prepara el subformulario para trabajar con la tabla factura.
*/
/**
\param parent
\return
**/
MovimientosSubform::MovimientosSubform ( QWidget *parent, const char * ) : BfSubForm ( parent )
{
    BL_FUNC_DEBUG
    /// Disparamos los plugins.
    int res = g_plugins->run ( "MovimientosSubform_MovimientosSubform", this );
    if ( res != 0 )
        return;
    setDbTableName ( "movimiento" );
    setDbFieldId ( "idmovimiento" );
    addSubFormHeader ( "idmovimiento", BlDbField::DbInt, BlDbField::DbPrimaryKey, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "idarticulo" ) );
    addSubFormHeader ( "fechamovimiento", BlDbField::DbDate, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Fecha" ) );
    addSubFormHeader ( "idarticulo", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id articulo" ) );
    addSubFormHeader ( "codigocompletoarticulo", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo articulo" ) );
    addSubFormHeader ( "nomarticulo", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre articulo" ) );
    addSubFormHeader ( "cantidadmovimiento", BlDbField::DbNumeric, BlDbField::DbNotNull, BlSubFormHeader::DbNone , _ ( "Cantidad" ) );
    addSubFormHeader ( "lotemovimiento", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Lote" ) );
    addSubFormHeader ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Id almacen" ) );
    addSubFormHeader ( "codigoalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Codigo almacen" ) );
    addSubFormHeader ( "nomalmacen", BlDbField::DbVarChar, BlDbField::DbNothing, BlSubFormHeader::DbNone | BlSubFormHeader::DbNoWrite, _ ( "Nombre almacen" ) );
    addSubFormHeader ( "idlalbaran", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Id albaran cliente" ) );
    addSubFormHeader ( "idlalbaranp", BlDbField::DbInt, BlDbField::DbNothing, BlSubFormHeader::DbNone, _ ( "Id albaran proveedor" ) );

    setInsert ( false );
    setDelete ( false );
    setSortingEnabled ( false );
    
}

