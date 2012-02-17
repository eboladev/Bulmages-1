/***************************************************************************
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#include <QCloseEvent>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QObject>
#include <QToolButton>
#include <QWidget>

#include "albaranproveedorview.h"
#include "bfcompany.h"
#include "blconfiguration.h"
#include "blfunctions.h"
#include "bfproveedoralbaransubform.h"
#include "pedidoproveedorview.h"
#include "blplugins.h"


/** Inicializa todos los componentes de la ventana.
    Mete la ventana en el workSpace.
*/
/**
\param comp
\param parent
\return
**/
AlbaranProveedorView::AlbaranProveedorView ( BfCompany *comp, QWidget *parent )
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );
        setTitleName ( _ ( "Albaran de proveedor" ) );
        setDbTableName ( "albaranp" );
        setDbFieldId ( "idalbaranp" );
        addDbField ( "idalbaranp", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id albaran proveedor" ) );
        addDbField ( "numalbaranp", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero albaran proveedor" ) );
        addDbField ( "fechaalbaranp", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha albaran proveedor" ) );
        addDbField ( "comentalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario albaran proveedor" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id almacen" ) );
        addDbField ( "refalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia albaran proveedor" ) );
        addDbField ( "descalbaranp", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion albaran proveedor" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "AlbaranProveedorView_AlbaranProveedorView", this );
        if ( res != 0 ) {
            return;
        } // end if
        subform2->setMainCompany ( comp );
        mui_idalmacen->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idproveedor->setMainCompany ( comp );
        m_descuentos->setMainCompany ( comp );
        mui_refalbaranp->setMainCompany ( comp );

        /// Inicializamos BfForm.
        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        /// Establecemos los parametros de busqueda del Cliente
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

        m_totalBases->setReadOnly ( TRUE );
        m_totalBases->setAlignment ( Qt::AlignRight );
        m_totalTaxes->setReadOnly ( TRUE );
        m_totalTaxes->setAlignment ( Qt::AlignRight );
        m_totalDiscounts->setReadOnly ( TRUE );
        m_totalDiscounts->setAlignment ( Qt::AlignRight );
        m_totalalbaranp->setReadOnly ( TRUE );
        m_totalalbaranp->setAlignment ( Qt::AlignRight );
        mui_idforma_pago->setId ( "0" );
        mui_idalmacen->setId ( "0" );
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el albaran proveedor" ), this );
    } // end try
    
}


/** Hace que el listado de Albaranes se refresque
*/
/**
**/
AlbaranProveedorView::~AlbaranProveedorView()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void AlbaranProveedorView::on_m_descuentos_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


///
/**
**/
void AlbaranProveedorView::on_subform2_editFinish ( int, int )
{
    BL_FUNC_DEBUG
    calculaypintatotales();
    
}


/** Inicializa los elementos de la ventana.
    Este metodo es invocado al crear un nuevo Albaran de Proveedor sin
    tener que hacer una carga del mismo.
*/
/**
**/
void AlbaranProveedorView::inicializar()
{
    BL_FUNC_DEBUG
    subform2->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_readValues();
    
}


/// Pinta los totales indicados en sus correspondientes textEdit.
/**
\param iva
\param base
\param desc
\param irpf
\param reqeq
**/
void AlbaranProveedorView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    BL_FUNC_DEBUG
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    m_totalalbaranp->setText ( QString ( total.toQString() ) );
    
}


/** SLOT que responde a la solicitud de ver todos los pedidos de proveedor
    relacionados con el albaran abierto.
    Realiza una consulta sobre los pedidos de proveedor que tienen la misma
    referencia que este pedido de proveedor y los instancia y muestra.
    NOTA: Este plugin debe ser trasladado al plugin de pedidos de proveedor.
*/
/**
**/
void AlbaranProveedorView::on_mui_verpedidosproveedor_clicked()
{
    BL_FUNC_DEBUG
    QString query = "SELECT * FROM pedidoproveedor WHERE refpedidoproveedor = '" + dbValue ( "refalbaranp" ) + "'";
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );
    while ( !cur->eof() ) {
	/// Como estamos en un plugin buscamos nuevas formas de creacion de objetos.
	int resur = g_plugins->run ( "SNewPedidoProveedorView", ( BfCompany * ) mainCompany() );
	if ( !resur ) {
	      blMsgInfo (_( "no se pudo crear instancia de pedido proveedor" ));
	      return;
	} // end if
	PedidoProveedorView * pedpro = ( PedidoProveedorView * ) g_plugParams;
        pedpro->load ( cur->value( "idpedidoproveedor" ) );
        mainCompany() ->m_pWorkspace->addSubWindow ( pedpro );
        pedpro->show();
        cur->nextRecord();
    } // end while
    delete cur;
    
}


///
/**
\param id
**/
void AlbaranProveedorView::on_mui_idproveedor_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    subform2->setIdProveedor ( id );
    mui_idforma_pago->setIdProveedor ( id );
    
}


/** Se encarga del borrado en la base de datos de una albaran de proveedor.
    Si se produce algun error devuelve una excepcion.
    Primero llama al borrado de las lineas y luego a los descuentos.
    Por ultimo llama al borrado del registro correspondiente en albaranp.
*/
/**
\return
**/
int AlbaranProveedorView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}


/// Esta funcion carga un AlbaranProveedor.
/// Hace el query adecuado, carga el registro a traves de BlDbRecord.
/// Hace la carga de las lineas y de los descuentos.
/// Invoca al pintado.
/**
\param idbudget
\return
**/
int AlbaranProveedorView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->run ( "AlbaranProveedorView_cargarPost_Post", this );

    calculaypintatotales();
    
    return 0;
}


/** Guarda el albaran de proveedor en la base de datos.
    Para ello hace el guardado de BlDbRecord y luego guarda las lineas y los descuentos.
    Una vez hecho el guardado se hace una carga para recuperar posibles datos que haya
    introducido la base de datos como la referencia.

    Si algo falla devuelve una excepcion -1.
*/
/**
\return
**/
int AlbaranProveedorView::afterSave()
{
    BL_FUNC_DEBUG

    m_listalineas->setColumnValue ( "idalbaranp", dbValue ( "idalbaranp" ) );
    m_listadescuentos->setColumnValue ( "idalbaranp", dbValue ( "idalbaranp" ) );

    m_listalineas->save();
    m_listadescuentos->save();

    
    return 0;
}

