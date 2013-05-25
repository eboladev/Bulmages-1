/***************************************************************************
 *   Copyright (C) 2006 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include <QtWidgets/QWidget>

#include "genalbqtoolbutton.h"
#include "blfunctions.h"
/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QtCore/QString>
#include <QtWidgets/QFileDialog>
#include <QtCore/QMap>
#include <QtCore/QList>
#include "facturaview.h"
#include "albaranclienteview.h"
#include "pedidoclienteview.h"
#include "presupuestoview.h"
#include "facturaslist.h"
#include "blfixed.h"
#include "bfcompany.h"
#include "bldb.h"


///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
GenAlbQToolButton::GenAlbQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_object = fac;
    setBoton();
    
}


///
/**
**/
GenAlbQToolButton::~GenAlbQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void GenAlbQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar albaran" );
    setToolTip ( "Generar albaran" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-order-to-note.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    setContentsMargins ( 0, 0, 0, 0 );
    
}


///
/**
**/
void GenAlbQToolButton::click()
{
    BL_FUNC_DEBUG

    if ( m_object->objectName() == "PedidoClienteBase" ) {
        generarFactura1();
    }// end if

    if ( m_object->objectName() == "PresupuestoClienteBase" ) {
        generarFactura2();
    }// end if


    
}


/// Se encarga de generar una albaran a partir de un albar&aacute;n.
/** Primero de todo busca una albaran por referencia que tenga este albaran.
    Si dicha albaran existe entonces la cargamos y terminamos.
    Si no existe dicha albaran el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenAlbQToolButton::generarFactura1()
{
    BL_FUNC_DEBUG

    PedidoClienteView *fpv = ( PedidoClienteView * ) m_object;


    AlbaranClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el albaran con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->dbValue ( "refpedidocliente" ).isEmpty() || fpv->dbValue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idcliente->id().isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->mui_refpedidocliente->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->dbValue ( "refpedidocliente" ) + "' AND idcliente = " + fpv->dbValue ( "idcliente" );
        } // end if

        cur = fpv->mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un albaran y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _ ( "Albaran ya existe" ),
                                         _ ( "Existe un albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                         _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            bud = new AlbaranClienteView ( fpv->mainCompany(), NULL );
            fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->load ( cur->value( "idalbaran" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = new AlbaranClienteView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( "0" );

        /// Traspasamos los datos al albaran.
        fpv->recogeValores();
        bud->setDbValue ( "comentalbaran", fpv->dbValue ( "comentpedidocliente" ) );
        bud->setDbValue ( "descalbaran", fpv->dbValue ( "descpedidocliente" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "refalbaran", fpv->dbValue ( "refpedidocliente" ) );
        bud->setDbValue ( "idcliente", fpv->dbValue ( "idcliente" ) );
        bud->setDbValue ( "idalmacen", fpv->dbValue ( "idalmacen" ) );
        bud->setDbValue ( "contactalbaran", fpv->dbValue ( "contactpedidocliente" ) );
        bud->setDbValue ( "telalbaran", fpv->dbValue ( "telpedidocliente" ) );
        bud->setDbValue ( "idtrabajador", fpv->dbValue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "desclalbaran", linea->dbValue ( "desclpedidocliente" ) );
                linea1->setDbValue ( "cantlalbaran", linea->dbValue ( "cantlpedidocliente" ) );
                linea1->setDbValue ( "pvplalbaran", linea->dbValue ( "pvplpedidocliente" ) );
                linea1->setDbValue ( "ivalalbaran", linea->dbValue ( "ivalpedidocliente" ) );
                linea1->setDbValue ( "descuentolalbaran", linea->dbValue ( "descuentolpedidocliente" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                bud->getlistalineas() ->setProcesarCambios ( true );
            } // end if
        } // end for

        /// Traspasamos los descuentos.
        for ( int i = 0; i < fpv->m_listadescuentos->rowCount(); ++i ) {
            linea1 = fpv->m_listadescuentos->lineaat ( i );
            if ( linea1->dbValue ( "proporciondpedidocliente" ) != "" ) {
                linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                bud->getlistadescuentos() ->setProcesarCambios ( false );
                linea->setDbValue ( "conceptdalbaran", linea1->dbValue ( "conceptdpedidocliente" ) );
                linea->setDbValue ( "proporciondalbaran", linea1->dbValue ( "proporciondpedidocliente" ) );
                bud->getlistadescuentos() ->setProcesarCambios ( true );
                bud->getlistadescuentos() ->newRecord();
            } // end if
        } // end for
        /// Pintamos el pedido y lo presentamos.
        bud->pintar();
        bud->calculaypintatotales();
        bud->show();

	/// Procesamos el pedido
	fpv->mui_procesadopedidocliente->setChecked(true);

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try


    
}






/// Se encarga de generar una albaran a partir de un albar&aacute;n.
/** Primero de todo busca una albaran por referencia que tenga este albaran.
    Si dicha albaran existe entonces la cargamos y terminamos.
    Si no existe dicha albaran el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenAlbQToolButton::generarFactura2()
{
    BL_FUNC_DEBUG

    PresupuestoView *fpv = ( PresupuestoView * ) m_object;


    AlbaranClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el albaran con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->dbValue ( "refpresupuesto" ).isEmpty() || fpv->dbValue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idcliente->id().isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->mui_refpresupuesto->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->dbValue ( "refpresupuesto" ) + "' AND idcliente = " + fpv->dbValue ( "idcliente" );
        } // end if

        cur = fpv->mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un albaran y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _ ( "Factura ya existe" ),
                                         _ ( "Existe una albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                         _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            bud = new AlbaranClienteView ( fpv->mainCompany(), NULL );
            fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->load ( cur->value( "idalbaran" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = new AlbaranClienteView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
        bud->load ( "0" );

        /// Traspasamos los datos al albaran.
        fpv->recogeValores();
        bud->setDbValue ( "comentalbaran", fpv->dbValue ( "comentpresupuesto" ) );
        bud->setDbValue ( "descalbaran", fpv->dbValue ( "descpresupuesto" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "refalbaran", fpv->dbValue ( "refpresupuesto" ) );
        bud->setDbValue ( "idcliente", fpv->dbValue ( "idcliente" ) );
        bud->setDbValue ( "idalmacen", fpv->dbValue ( "idalmacen" ) );
        bud->setDbValue ( "contactalbaran", fpv->dbValue ( "contactpresupuesto" ) );
        bud->setDbValue ( "telalbaran", fpv->dbValue ( "telpresupuesto" ) );
        bud->setDbValue ( "idtrabajador", fpv->dbValue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( false );
                linea1->setDbValue ( "desclalbaran", linea->dbValue ( "desclpresupuesto" ) );
                linea1->setDbValue ( "cantlalbaran", linea->dbValue ( "cantlpresupuesto" ) );
                linea1->setDbValue ( "pvplalbaran", linea->dbValue ( "pvplpresupuesto" ) );
                linea1->setDbValue ( "ivalalbaran", linea->dbValue ( "ivalpresupuesto" ) );
                linea1->setDbValue ( "descuentolalbaran", linea->dbValue ( "descuentolpresupuesto" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                bud->getlistalineas() ->setProcesarCambios ( true );
            } // end if
        } // end for

        /// Traspasamos los descuentos.
        for ( int i = 0; i < fpv->m_listadescuentos->rowCount(); ++i ) {
            linea1 = fpv->m_listadescuentos->lineaat ( i );
            if ( linea1->dbValue ( "proporciondpresupuesto" ) != "" ) {
                linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                bud->getlistadescuentos() ->setProcesarCambios ( false );
                linea->setDbValue ( "conceptdalbaran", linea1->dbValue ( "conceptdpresupuesto" ) );
                linea->setDbValue ( "proporciondalbaran", linea1->dbValue ( "proporciondpresupuesto" ) );
                bud->getlistadescuentos() ->setProcesarCambios ( true );
                bud->getlistadescuentos() ->newRecord();
            } // end if
        } // end for
        /// Pintamos el pedido y lo presentamos.
        bud->pintar();
        bud->calculaypintatotales();
        bud->show();

	/// Procesamos el presupuesto
	fpv->mui_procesadopresupuesto->setChecked(true);

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try


    
}


