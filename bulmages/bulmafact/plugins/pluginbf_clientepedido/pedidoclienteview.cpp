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
#include <QCloseEvent>


#include "pedidoclienteview.h"
#include "bfcompany.h"
#include "listlinpedidoclienteview.h"
#include "clientslist.h"
#include "bfinformereferencia.h"
#include "blfunctions.h"
#include "blplugins.h"


///
/**
\param comp
\param parent
\return
**/
PedidoClienteView::PedidoClienteView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi ( this );

        setTitleName ( _ ( "Pedido Cliente" ) );
        setDbTableName ( "pedidocliente" );
        setDbFieldId ( "idpedidocliente" );
        addDbField ( "idpedidocliente", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador" ) );
        addDbField ( "idcliente", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Cliente" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Almacen" ) );
        addDbField ( "numpedidocliente", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero pedido cliente" ) );
        addDbField ( "fechapedidocliente", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Identificador presupuesto" ) );
        addDbField ( "descpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Fecha" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Trabajador" ) );
        addDbField ( "contactpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Contacto" ) );
        addDbField ( "telpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono" ) );
        addDbField ( "comentpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios" ) );
        addDbField ( "procesadopedidocliente", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Procesado" ) );
        addDbField ( "refpedidocliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia" ) );


        /// Disparamos los plugins.
        int res = g_plugins->run ( "PedidoClienteView_PedidoClienteView", this );
        if ( res != 0 )
            return;
        subform3->setMainCompany ( comp );
        mui_idcliente->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idtrabajador->setMainCompany ( comp );
        mui_refpedidocliente->setMainCompany ( comp );
        setListaLineas ( subform3 );
        setListaDescuentos ( m_descuentos );
        mui_idforma_pago->setId ( "" );
        mui_idalmacen->setId ( "" );
        mui_idtrabajador->setId ( "0" );
        /// Establecemos los parametros de busqueda del Cliente
        mui_idcliente->setLabel ( _ ( "Cliente:" ) );
        mui_idcliente->setTableName ( "cliente" );
        mui_idcliente->m_valores["cifcliente"] = "";
        mui_idcliente->m_valores["nomcliente"] = "";
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el pedido cliente" ), this );
    } // end try
    
}


///
/**
**/
PedidoClienteView::~PedidoClienteView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void PedidoClienteView::inicializar()
{
    BL_FUNC_DEBUG
    subform3->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    
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
void PedidoClienteView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    m_totalpedidocliente->setText ( total.toQString() );
    
}


///
/**
**/
void PedidoClienteView::on_mui_verpresupuesto_clicked()
{
    BL_FUNC_DEBUG

    
}


/** Genera un Albaran a Cliente a partir de los datos
    que tiene el pedido.
*/
/**
\return
**/
void PedidoClienteView::generarAlbaran()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param id
**/
void PedidoClienteView::on_mui_idcliente_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform3->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    
}


///
/**
\return
**/
int PedidoClienteView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}


/// Esta funcion carga un PedidoCliente.
/**
\param idbudget
\return
**/
int PedidoClienteView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->run ( "PedidoClienteView_cargarPost_Post", this );

    calculaypintatotales();

    
    return 0;
}


/// Guardamos el pedido cliente.
/**
\return
**/
int PedidoClienteView::afterSave()
{
    BL_FUNC_DEBUG
    m_listalineas->setColumnValue ( "idpedidocliente", dbValue ( "idpedidocliente" ) );
    m_listalineas->save();
    m_listadescuentos->setColumnValue ( "idpedidocliente", dbValue ( "idpedidocliente" ) );
    m_listadescuentos->save();
    return 0;
}


///
/**
**/
void PedidoClienteView::s_pintaTotales()
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void PedidoClienteView::on_mui_pasaraalbaran_clicked()
{
    BL_FUNC_DEBUG
    generarAlbaran();
    
}


///
/**
**/
void PedidoClienteView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void PedidoClienteView::on_subform3_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
\param id
**/
void PedidoClienteView::on_mui_idalmacen_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    m_listalineas->setIdAlmacen ( id );
    
}

