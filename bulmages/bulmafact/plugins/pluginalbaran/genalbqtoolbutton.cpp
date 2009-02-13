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

#include <QWidget>

#include "genalbqtoolbutton.h"
#include "funcaux.h"
/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include "facturaview.h"
#include "albaranclienteview.h"
#include "pedidoclienteview.h"
#include "presupuestoview.h"
#include "facturaslist.h"
#include "blfixed.h"
#include "company.h"
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
    _depura ( "GenAlbQToolButton::GenAlbQToolButton", 0 );
    m_object = fac;
    setBoton();
    _depura ( "END GenAlbQToolButton::GenAlbQToolButton", 0 );
}


///
/**
**/
GenAlbQToolButton::~GenAlbQToolButton()
{
    _depura ( "GenAlbQToolButton::~GenAlbQToolButton", 0 );
    _depura ( "END GenAlbQToolButton::~GenAlbQToolButton", 0 );
}


///
/**
**/
void GenAlbQToolButton::setBoton()
{
    _depura ( "GenAlbQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Albaran" );
    setToolTip ( "Generar Albaran" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize (32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-order-to-note.svg" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    setContentsMargins (0, 0, 0, 0);
    _depura ( "END GenAlbQToolButton::setBoton", 0 );
}


///
/**
**/
void GenAlbQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );

    if (m_object->objectName() == "PedidoClienteBase" ) {
	PedidoClienteView *fpv = (PedidoClienteView *) m_object;
		generarFactura1();
    }// end if

    if (m_object->objectName() == "PresupuestoClienteBase" ) {
	PresupuestoView *fpv = (PresupuestoView *) m_object;
		generarFactura2();
    }// end if


    _depura ( "END ImpQToolButton::click", 0 );
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
    _depura ( "GenAlbQToolButton::generarFacturaProveedor1", 0 );

	PedidoClienteView *fpv = (PedidoClienteView *) m_object;


    AlbaranClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el albaran con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->DBvalue ( "refpedidocliente" ).isEmpty() || fpv->DBvalue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idcliente->id().isEmpty() ) {
                mensajeInfo ( _( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->mui_refpedidocliente->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->DBvalue ( "refpedidocliente" ) + "' AND idcliente = " + fpv->DBvalue ( "idcliente" );
        } // end if

        cur = fpv->empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un albaran y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _( "Albaran ya existe" ),
                                         _( "Existe un albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                         _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            bud = new AlbaranClienteView ( fpv->empresaBase(), NULL );
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idalbaran" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = new AlbaranClienteView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( "0" );

        /// Traspasamos los datos al albaran.
        fpv->recogeValores();
        bud->setDBvalue ( "comentalbaran", fpv->DBvalue ( "comentpedidocliente" ) );
        bud->setDBvalue ( "descalbaran", fpv->DBvalue ( "descpedidocliente" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "refalbaran", fpv->DBvalue ( "refpedidocliente" ) );
        bud->setDBvalue ( "idcliente", fpv->DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "idalmacen", fpv->DBvalue ( "idalmacen" ) );
        bud->setDBvalue ( "contactalbaran", fpv->DBvalue ( "contactpedidocliente" ) );
        bud->setDBvalue ( "telalbaran", fpv->DBvalue ( "telpedidocliente" ) );
        bud->setDBvalue ( "idtrabajador", fpv->DBvalue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        SDBRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "desclalbaran", linea->DBvalue ( "desclpedidocliente" ) );
                linea1->setDBvalue ( "cantlalbaran", linea->DBvalue ( "cantlpedidocliente" ) );
                linea1->setDBvalue ( "pvplalbaran", linea->DBvalue ( "pvplpedidocliente" ) );
                linea1->setDBvalue ( "ivalalbaran", linea->DBvalue ( "ivalpedidocliente" ) );
                linea1->setDBvalue ( "descuentolalbaran", linea->DBvalue ( "descuentolpedidocliente" ) );
                linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for

        /// Traspasamos los descuentos.
        for ( int i = 0; i < fpv->m_listadescuentos->rowCount(); ++i ) {
            linea1 = fpv->m_listadescuentos->lineaat ( i );
            if ( linea1->DBvalue ( "proporciondpedidocliente" ) != "" ) {
                linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                bud->getlistadescuentos() ->setProcesarCambios ( FALSE );
                linea->setDBvalue ( "conceptdalbaran", linea1->DBvalue ( "conceptdpedidocliente" ) );
                linea->setDBvalue ( "proporciondalbaran", linea1->DBvalue ( "proporciondpedidocliente" ) );
                bud->getlistadescuentos() ->setProcesarCambios ( TRUE );
                bud->getlistadescuentos() ->nuevoRegistro();
            } // end if
        } // end for
        /// Pintamos el pedido y lo presentamos.
        bud->pintar();
        bud->calculaypintatotales();
        bud->show();

    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try


    _depura ( "END GenAlbQToolButton::generarFactura", 0 );
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
    _depura ( "GenAlbQToolButton::generarFactura2", 0 );

	PresupuestoView *fpv = (PresupuestoView *) m_object;


    AlbaranClienteView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el albaran con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->DBvalue ( "refpresupuesto" ).isEmpty() || fpv->DBvalue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idcliente->id().isEmpty() ) {
                mensajeInfo ( _( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->mui_refpresupuesto->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + fpv->DBvalue ( "refpresupuesto" ) + "' AND idcliente = " + fpv->DBvalue ( "idcliente" );
        } // end if

        cur = fpv->empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un albaran y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _( "Factura ya existe" ),
                                         _( "Existe una albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                         _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            bud = new AlbaranClienteView ( fpv->empresaBase(), NULL );
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idalbaran" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = new AlbaranClienteView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( "0" );

        /// Traspasamos los datos al albaran.
        fpv->recogeValores();
        bud->setDBvalue ( "comentalbaran", fpv->DBvalue ( "comentpresupuesto" ) );
        bud->setDBvalue ( "descalbaran", fpv->DBvalue ( "descpresupuesto" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "refalbaran", fpv->DBvalue ( "refpresupuesto" ) );
        bud->setDBvalue ( "idcliente", fpv->DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "idalmacen", fpv->DBvalue ( "idalmacen" ) );
        bud->setDBvalue ( "contactalbaran", fpv->DBvalue ( "contactpresupuesto" ) );
        bud->setDBvalue ( "telalbaran", fpv->DBvalue ( "telpresupuesto" ) );
        bud->setDBvalue ( "idtrabajador", fpv->DBvalue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        SDBRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "desclalbaran", linea->DBvalue ( "desclpresupuesto" ) );
                linea1->setDBvalue ( "cantlalbaran", linea->DBvalue ( "cantlpresupuesto" ) );
                linea1->setDBvalue ( "pvplalbaran", linea->DBvalue ( "pvplpresupuesto" ) );
                linea1->setDBvalue ( "ivalalbaran", linea->DBvalue ( "ivalpresupuesto" ) );
                linea1->setDBvalue ( "descuentolalbaran", linea->DBvalue ( "descuentolpresupuesto" ) );
                linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for

        /// Traspasamos los descuentos.
        for ( int i = 0; i < fpv->m_listadescuentos->rowCount(); ++i ) {
            linea1 = fpv->m_listadescuentos->lineaat ( i );
            if ( linea1->DBvalue ( "proporciondpresupuesto" ) != "" ) {
                linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                bud->getlistadescuentos() ->setProcesarCambios ( FALSE );
                linea->setDBvalue ( "conceptdalbaran", linea1->DBvalue ( "conceptdpresupuesto" ) );
                linea->setDBvalue ( "proporciondalbaran", linea1->DBvalue ( "proporciondpresupuesto" ) );
                bud->getlistadescuentos() ->setProcesarCambios ( TRUE );
                bud->getlistadescuentos() ->nuevoRegistro();
            } // end if
        } // end for
        /// Pintamos el pedido y lo presentamos.
        bud->pintar();
        bud->calculaypintatotales();
        bud->show();

    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try


    _depura ( "END GenAlbQToolButton::generarFactura", 0 );
}


