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

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtCore/QObject>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QLayout>
#include <QtGui/QCloseEvent>


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
        insertWindow ( windowTitle(), this, false );
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



/** Pintar un pedido.
*/
/**
\param idbudget
\return
**/
void PedidoClienteView::pintarPost ( )
{
    BL_FUNC_DEBUG

    /// Escribimos como descripcion el nombre del cliente para que aparezca en el titulo y en el dockwidget
    setDescripcion( mui_idcliente->fieldValue("nomcliente") + "\n" + mui_fechapedidocliente->text());
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



/// Se encarga de generar una pedidocliente a partir de un albar&aacute;n.
/** Primero de todo busca una pedidocliente por referencia que tenga este pedidocliente.
    Si dicha pedidocliente existe entonces la cargamos y terminamos.
    Si no existe dicha pedidocliente el sistema avisa y permite crear una poniendo
    Todos los datos del pedidocliente automaticamente en ella.
*/
/**
\return
**/
void PedidoClienteView::on_mui_duplicar_released()
{
    BL_FUNC_DEBUG
    PedidoClienteView *fpv = this ;


    PedidoClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una pedidocliente con esos datos, y en caso afirmativo lo mostramos.

        /// Creamos la pedidocliente.
        PedidoClienteView *bud = new PedidoClienteView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
	bud->inicializar();

        bud->setDbValue ( "comentpedidocliente", fpv->dbValue ( "comentpedidocliente" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "refpedidocliente", fpv->dbValue ( "refpedidocliente" ) );
        bud->setDbValue ( "idcliente", fpv->dbValue ( "idcliente" ) );
        bud->pintar();
        bud->show();

        /// Traspasamos las lineas de pedidocliente
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "desclpedidocliente", linea->dbValue ( "desclpedidocliente" ) );
                linea1->setDbValue ( "cantlpedidocliente", linea->dbValue ( "cantlpedidocliente" ) );
                linea1->setDbValue ( "pvplpedidocliente", linea->dbValue ( "pvplpedidocliente" ) );
                linea1->setDbValue ( "descuentolpedidocliente", linea->dbValue ( "descuentolpedidocliente" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalpedidocliente", linea->dbValue ( "ivalpedidocliente" ) );
                linea1->setDbValue ( "reqeqlpedidocliente", linea->dbValue ( "reqeqlpedidocliente" ) );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

}








