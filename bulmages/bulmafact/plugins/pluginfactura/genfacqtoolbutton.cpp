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
#include "genfacqtoolbutton.h"
#include "blfunctions.h"

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
GenFacQToolButton::GenFacQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "GenFacQToolButton::GenFacQToolButton", 0 );
    m_object = fac;
    setBoton();
    _depura ( "END GenFacQToolButton::GenFacQToolButton", 0 );
}


///
/**
**/
GenFacQToolButton::~GenFacQToolButton()
{
    _depura ( "GenFacQToolButton::~GenFacQToolButton", 0 );
    _depura ( "END GenFacQToolButton::~GenFacQToolButton", 0 );
}


///
/**
**/
void GenFacQToolButton::setBoton()
{
    _depura ( "GenFacQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar Factura" );
    setToolTip ( "Generar Factura" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize (32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note-to-invoice.svg" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    setContentsMargins (0, 0, 0, 0);
    _depura ( "END GenFacQToolButton::setBoton", 0 );
}


///
/**
**/
void GenFacQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );

    if (m_object->objectName() == "AlbaranClienteBase" ) {
		generarFactura();
    }// end if


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

/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenFacQToolButton::generarFactura()
{
    _depura ( "GenFacQToolButton::generarFactura", 0 );
	AlbaranClienteView *fpv = (AlbaranClienteView *) m_object;

    /// Disparamos los plugins.
    int res = g_plugins->lanza ( "AlbaranClienteView_generarFactura", this );
    if ( res != 0 )
        return;

    FacturaView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->DBvalue ( "refalbaran" ).isEmpty() || fpv->DBvalue ( "idcliente" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idcliente->id().isEmpty() ) {
                mensajeInfo ( _( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + fpv->mui_refalbaran->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + fpv->DBvalue ( "refalbaran" ) + "' AND idcliente = " + fpv->DBvalue ( "idcliente" );
        } // end if

        cur = fpv->empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _( "Factura existente" ),
                                         _( "Existe una factura a este cliente con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                         _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                return;
            }
            bud = new FacturaView( (Company *) fpv->empresaBase(), 0);
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idfactura" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos la factura.
        bud = new FacturaView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->cargar ( "0" );

        /// Traspasamos los datos a la factura.
        fpv->recogeValores();
        bud->setDBvalue ( "comentfactura", fpv->DBvalue ( "comentalbaran" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "reffactura", fpv->DBvalue ( "refalbaran" ) );
        bud->setDBvalue ( "idcliente", fpv->DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "idalmacen", fpv->DBvalue ( "idalmacen" ) );

        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                /// Haciendo el nuevo registro antes nos evitamos problemas de foco.
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                linea1->setDBvalue ( "desclfactura", linea->DBvalue ( "desclalbaran" ) );
                linea1->setDBvalue ( "cantlfactura", linea->DBvalue ( "cantlalbaran" ) );
                linea1->setDBvalue ( "pvplfactura", linea->DBvalue ( "pvplalbaran" ) );
                linea1->setDBvalue ( "ivalfactura", linea->DBvalue ( "ivalalbaran" ) );
                linea1->setDBvalue ( "descuentolfactura", linea->DBvalue ( "descuentolalbaran" ) );
                linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
                linea1->refresh();
            } // end if
        } // end for

        /// Traspasamos los descuentos.
        for ( int i = 0; i < fpv->m_listadescuentos->rowCount(); ++i ) {
            linea1 = fpv->m_listadescuentos->lineaat ( i );
            if ( linea1->DBvalue ( "proporciondalbaran" ) != "" ) {
                linea = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                bud->getlistadescuentos() ->setProcesarCambios ( FALSE );
                linea->setDBvalue ( "conceptdfactura", linea1->DBvalue ( "conceptdalbaran" ) );
                linea->setDBvalue ( "proporciondfactura", linea1->DBvalue ( "proporciondalbaran" ) );
                bud->getlistadescuentos() ->setProcesarCambios ( TRUE );
                bud->getlistadescuentos() ->nuevoRegistro();
            } // end if
        } // end for

        bud->pintar();
        bud->calculaypintatotales();
        bud->show();

        fpv->mui_procesadoalbaran->setChecked ( TRUE );

    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try


    _depura ( "END GenFacQToolButton::generarFactura", 0 );
}



/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenFacQToolButton::generarFactura1()
{
    _depura ( "GenFacQToolButton::generarFacturaProveedor1", 0 );

	PedidoClienteView *fpv = (PedidoClienteView *) m_object;


    FacturaView *bud = NULL;
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
                SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + fpv->mui_refpedidocliente->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + fpv->DBvalue ( "refpedidocliente" ) + "' AND idcliente = " + fpv->DBvalue ( "idcliente" );
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
            bud = new FacturaView ( fpv->empresaBase(), NULL );
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idfactura" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = new FacturaView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( "0" );

        /// Traspasamos los datos al albaran.
        fpv->recogeValores();
        bud->setDBvalue ( "comentfactura", fpv->DBvalue ( "comentpedidocliente" ) );
        bud->setDBvalue ( "descfactura", fpv->DBvalue ( "descpedidocliente" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "reffactura", fpv->DBvalue ( "refpedidocliente" ) );
        bud->setDBvalue ( "idcliente", fpv->DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "idalmacen", fpv->DBvalue ( "idalmacen" ) );
        bud->setDBvalue ( "contactfactura", fpv->DBvalue ( "contactpedidocliente" ) );
        bud->setDBvalue ( "telfactura", fpv->DBvalue ( "telpedidocliente" ) );
        bud->setDBvalue ( "idtrabajador", fpv->DBvalue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "desclfactura", linea->DBvalue ( "desclpedidocliente" ) );
                linea1->setDBvalue ( "cantlfactura", linea->DBvalue ( "cantlpedidocliente" ) );
                linea1->setDBvalue ( "pvplfactura", linea->DBvalue ( "pvplpedidocliente" ) );
                linea1->setDBvalue ( "ivalfactura", linea->DBvalue ( "ivalpedidocliente" ) );
                linea1->setDBvalue ( "descuentolfactura", linea->DBvalue ( "descuentolpedidocliente" ) );
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
                linea->setDBvalue ( "conceptdfactura", linea1->DBvalue ( "conceptdpedidocliente" ) );
                linea->setDBvalue ( "proporciondfactura", linea1->DBvalue ( "proporciondpedidocliente" ) );
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


    _depura ( "END GenFacQToolButton::generarFactura", 0 );
}






/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void GenFacQToolButton::generarFactura2()
{
    _depura ( "GenFacQToolButton::generarFactura2", 0 );

	PresupuestoView *fpv = (PresupuestoView *) m_object;


    FacturaView *bud = NULL;
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
                SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + fpv->mui_refpresupuesto->text() + "' AND idcliente = " + fpv->mui_idcliente->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM factura WHERE reffactura = '" + fpv->DBvalue ( "refpresupuesto" ) + "' AND idcliente = " + fpv->DBvalue ( "idcliente" );
        } // end if

        cur = fpv->empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un albaran y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _( "Factura ya existe" ),
                                         _( "Existe una factura a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                         _( "&Si" ), _( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            bud = new FacturaView ( fpv->empresaBase(), NULL );
            fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idfactura" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = new FacturaView((Company *) fpv->empresaBase(), 0);
        fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( "0" );

        /// Traspasamos los datos al albaran.
        fpv->recogeValores();
        bud->setDBvalue ( "comentfactura", fpv->DBvalue ( "comentpresupuesto" ) );
        bud->setDBvalue ( "descfactura", fpv->DBvalue ( "descpresupuesto" ) );
        bud->setDBvalue ( "idforma_pago", fpv->DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "reffactura", fpv->DBvalue ( "refpresupuesto" ) );
        bud->setDBvalue ( "idcliente", fpv->DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "idalmacen", fpv->DBvalue ( "idalmacen" ) );
        bud->setDBvalue ( "contactfactura", fpv->DBvalue ( "contactpresupuesto" ) );
        bud->setDBvalue ( "telfactura", fpv->DBvalue ( "telpresupuesto" ) );
        bud->setDBvalue ( "idtrabajador", fpv->DBvalue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDBvalue ( "desclfactura", linea->DBvalue ( "desclpresupuesto" ) );
                linea1->setDBvalue ( "cantlfactura", linea->DBvalue ( "cantlpresupuesto" ) );
                linea1->setDBvalue ( "pvplfactura", linea->DBvalue ( "pvplpresupuesto" ) );
                linea1->setDBvalue ( "ivalfactura", linea->DBvalue ( "ivalpresupuesto" ) );
                linea1->setDBvalue ( "descuentolfactura", linea->DBvalue ( "descuentolpresupuesto" ) );
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
                linea->setDBvalue ( "conceptdfactura", linea1->DBvalue ( "conceptdpresupuesto" ) );
                linea->setDBvalue ( "proporciondfactura", linea1->DBvalue ( "proporciondpresupuesto" ) );
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


    _depura ( "END GenFacQToolButton::generarFactura", 0 );
}




// ====================



///
/**
\param pres
\param ped
\param alb
\param fac
\param parent
**/
AgFacQToolButton::AgFacQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    _depura ( "AgFacQToolButton::AgFacQToolButton", 0 );
    m_object = fac;
    setBoton();
    _depura ( "END AgFacQToolButton::AgFacQToolButton", 0 );
}


///
/**
**/
AgFacQToolButton::~AgFacQToolButton()
{
    _depura ( "AgFacQToolButton::~AgFacQToolButton", 0 );
    _depura ( "END AgFacQToolButton::~AgFacQToolButton", 0 );
}


///
/**
**/
void AgFacQToolButton::setBoton()
{
    _depura ( "AgFacQToolButton::setBoton", 0 );
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Agregar a Factura" );
    setToolTip ( "Agregar a Factura" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize (32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/client-delivery-note-add-to-invoice.svg" ) ) );
    setIconSize ( QSize ( 32, 32 ) );
    setContentsMargins (0, 0, 0, 0);
    _depura ( "END AgFacQToolButton::setBoton", 0 );
}


///
/**
**/
void AgFacQToolButton::click()
{
    _depura ( "ImpQToolButton::click", 0 );

    if (m_object->objectName() == "AlbaranClienteBase" ) {
		generarFactura();
    }// end if

    _depura ( "END ImpQToolButton::click", 0 );
}

/// Se encarga de generar una factura a partir de un albar&aacute;n.
/** Primero de todo busca una factura por referencia que tenga este albaran.
    Si dicha factura existe entonces la cargamos y terminamos.
    Si no existe dicha factura el sistema avisa y permite crear una poniendo
    Todos los datos del albaran automaticamente en ella.
*/
/**
\return
**/
void AgFacQToolButton::generarFactura()
{
    _depura ( "AgFacQToolButton::generarFactura", 0 );
	AlbaranClienteView *fpv = (AlbaranClienteView *) m_object;

    QDialog *diag = new QDialog ( 0 );
    diag->setWindowTitle ( _( "Seleccione la factura a la que agregar el albaran." ) );
    diag->setModal ( true );

    /// \TODO: Debe pasar por company la creacion del listado
    FacturasList *fac = new FacturasList ( fpv->empresaBase(), diag, 0, FacturasList::SelectMode );
    connect ( fac, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

    /// Hacemos que las opciones de filtrado del listado ya est&eacute;n bien.
    fac->m_cliente->setId ( fpv->DBvalue ( "idcliente" ) );
    fac->on_mui_actualizar_clicked();

    /// Lanzamos el di&aacute;logo.
    diag->exec();
    QString idfactura = fac->idfactura();
    delete diag;

    /// Si no hay idfactura es que hemos abortado y por tanto cancelamos la operaci&oacute;n.
    if ( idfactura == "" )
        return;

    /// Creamos la factura.
    FacturaView *bud = new FacturaView( (Company*) fpv->empresaBase());
    bud->cargar ( idfactura );

    /// Agregamos en los comentarios que se ha a&ntilde;adido este albar&aacute;n.
    bud->setDBvalue ( "comentfactura", bud->DBvalue ( "comentfactura" ) + _( "Num. albaran" ) + fpv->DBvalue ( "numalbaran" ) + "\n" );

    fpv->empresaBase() ->m_pWorkspace->addWindow ( bud );
    /// \TODO EN TEORIA SE DEBERIA COMPROBAR QUE LA FACTURA ES DEL MISMO CLIENTE,
    /// PERO POR AHORA PASAMOS DE HACERLO.
    QString l;
    BlDbSubFormRecord *linea, *linea1;
    for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
        linea = fpv->m_listalineas->lineaat ( i );
        if ( linea->DBvalue ( "idarticulo" ) != "" ) {
            linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
            bud->getlistalineas() ->setProcesarCambios ( FALSE );
            linea1->setDBvalue ( "desclfactura", linea->DBvalue ( "desclalbaran" ) );
            linea1->setDBvalue ( "cantlfactura", linea->DBvalue ( "cantlalbaran" ) );
            linea1->setDBvalue ( "pvplfactura", linea->DBvalue ( "pvplalbaran" ) );
            linea1->setDBvalue ( "descuentolfactura", linea->DBvalue ( "descuentolalbaran" ) );
            linea1->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
            linea1->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
            linea1->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
            linea1->setDBvalue ( "ivalfactura", linea->DBvalue ( "ivalalbaran" ) );
            bud->getlistalineas() ->setProcesarCambios ( TRUE );
            bud->getlistalineas() ->nuevoRegistro();
        } // end if
    } // end for
    bud->calculaypintatotales();
    bud->show();
    fpv->mui_procesadoalbaran->setChecked ( TRUE );

    _depura ( "END AgFacQToolButton::generarFactura", 0 );
}




