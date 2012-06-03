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

/// Necesarios para importacion de efactura
#include <QtXml/QDomDocument>
#include <QtXml/QDomNodeList>
#include <QtXml/QDomNode>
#include <QString>
#include <QFileDialog>
#include <QMap>
#include <QList>
#include <QWidget>

#include "genfactqtoolbutton.h"
#include "blfunctions.h"
#include "facturapview.h"
#include "albaranproveedorview.h"
#include "pedidoproveedorview.h"
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
GenFacProQToolButton::GenFacProQToolButton ( QWidget *fac , QWidget *parent ) : QToolButton ( parent )
{
    BL_FUNC_DEBUG
    m_object = fac;
    setBoton();
    
}


///
/**
**/
GenFacProQToolButton::~GenFacProQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void GenFacProQToolButton::setBoton()
{
    BL_FUNC_DEBUG
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    setObjectName ( QString::fromUtf8 ( "exporta" ) );
    setStatusTip ( "Generar factura proveedor" );
    setToolTip ( "Generar factura proveedor" );
    setMinimumSize ( QSize ( 32, 32 ) );
    setMaximumSize ( QSize ( 32, 32 ) );
    setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-invoice.png" ) ) );
    setIconSize ( QSize ( 22, 22 ) );
    setContentsMargins ( 0, 0, 0, 0 );
    
}


///
/**
**/
void GenFacProQToolButton::click()
{
    BL_FUNC_DEBUG

    if ( m_object->objectName() == "AlbaranProveedorBase" ) {
        generarFacturaProveedor();
    }// end if

    if ( m_object->objectName() == "PedidoProveedorBase" ) {
//        PedidoProveedorView *fpv = ( PedidoProveedorView * ) m_object;
        generarFacturaProveedor1();
    }// end if

    
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
void GenFacProQToolButton::generarFacturaProveedor()
{
    BL_FUNC_DEBUG
    AlbaranProveedorView *fpv = ( AlbaranProveedorView * ) m_object;


    FacturaProveedorView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->dbValue ( "refalbaranp" ).isEmpty() || fpv->dbValue ( "idproveedor" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idproveedor->id().isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un proveedor" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->mui_refalbaranp->text() + "' AND idproveedor = " + fpv->mui_idproveedor->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->dbValue ( "refalbaranp" ) + "' AND idproveedor = " + fpv->dbValue ( "idproveedor" );
        } // end if

        cur = fpv->mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _ ( "Factura de proveedor existente" ),
                                         _ ( "Existe una factura de este proveedor con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                         _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) fpv->mainCompany(), 0 );
            fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->load ( cur->value( "idfacturap" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos la factura.
        FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->inicializar();

        bud->setDbValue ( "comentfacturap", fpv->dbValue ( "comentalbaranp" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "reffacturap", fpv->dbValue ( "refalbaranp" ) );
        bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
        bud->pintar();
        bud->show();

        /// Traspasamos las lineas de factura
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDbValue ( "desclfacturap", linea->dbValue ( "desclalbaranp" ) );
                linea1->setDbValue ( "cantlfacturap", linea->dbValue ( "cantlalbaranp" ) );
                linea1->setDbValue ( "pvplfacturap", linea->dbValue ( "pvplalbaranp" ) );
                linea1->setDbValue ( "descuentolfacturap", linea->dbValue ( "descuentolalbaranp" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalfacturap", linea->dbValue ( "ivalalbaranp" ) );
                linea1->setDbValue ( "reqeqlfacturap", linea->dbValue ( "reqeqlalbaranp" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    
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
void GenFacProQToolButton::generarFacturaProveedor1()
{
    BL_FUNC_DEBUG
    PedidoProveedorView *fpv = ( PedidoProveedorView * ) m_object;


    FacturaProveedorView *bud = NULL;
    BlDbRecordSet *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe una factura con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( fpv->dbValue ( "refpedidoproveedor" ).isEmpty() || fpv->dbValue ( "idproveedor" ).isEmpty() ) {
            /// El albaran no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( fpv->mui_idproveedor->id().isEmpty() ) {
                blMsgInfo ( _ ( "Tiene que seleccionar un proveedor" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->mui_refpedidoproveedor->text() + "' AND idproveedor = " + fpv->mui_idproveedor->id();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM facturap WHERE reffacturap = '" + fpv->dbValue ( "refpedidoproveedor" ) + "' AND idproveedor = " + fpv->dbValue ( "idproveedor" );
        } // end if

        cur = fpv->mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay una factura y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         _ ( "Factura de proveedor existente" ),
                                         _ ( "Existe una factura de este proveedor con la misma referencia que este albaran. Desea abrirla para verificar?" ),
                                         _ ( "&Si" ), _ ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) fpv->mainCompany(), 0 );
            fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );
            bud->load ( cur->value( "idfacturap" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos la factura.
        FacturaProveedorView *bud = new FacturaProveedorView ( ( BfCompany * ) fpv->mainCompany(), 0 );
        fpv->mainCompany() ->m_pWorkspace->addSubWindow ( bud );

        /// Cargamos un elemento que no existe para inicializar bien la clase.
        bud->inicializar();

        bud->setDbValue ( "comentfacturap", fpv->dbValue ( "comentpedidoproveedor" ) );
        bud->setDbValue ( "idforma_pago", fpv->dbValue ( "idforma_pago" ) );
        bud->setDbValue ( "reffacturap", fpv->dbValue ( "refpedidoproveedor" ) );
        bud->setDbValue ( "idproveedor", fpv->dbValue ( "idproveedor" ) );
        bud->pintar();
        bud->show();

        /// Traspasamos las lineas de factura
        QString l;
        BlDbSubFormRecord *linea, *linea1;
        for ( int i = 0; i < fpv->m_listalineas->rowCount(); ++i ) {
            linea = fpv->m_listalineas->lineaat ( i );
            if ( linea->dbValue ( "idarticulo" ) != "" ) {
                linea1 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->newRecord();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea1->setDbValue ( "desclfacturap", linea->dbValue ( "desclpedidoproveedor" ) );
                linea1->setDbValue ( "cantlfacturap", linea->dbValue ( "cantlpedidoproveedor" ) );
                linea1->setDbValue ( "pvplfacturap", linea->dbValue ( "pvplpedidoproveedor" ) );
                linea1->setDbValue ( "descuentolfacturap", linea->dbValue ( "descuentolpedidoproveedor" ) );
                linea1->setDbValue ( "idarticulo", linea->dbValue ( "idarticulo" ) );
                linea1->setDbValue ( "codigocompletoarticulo", linea->dbValue ( "codigocompletoarticulo" ) );
                linea1->setDbValue ( "nomarticulo", linea->dbValue ( "nomarticulo" ) );
                linea1->setDbValue ( "ivalfacturap", linea->dbValue ( "ivalpedidoproveedor" ) );
                linea1->setDbValue ( "reqeqlfacturap", linea->dbValue ( "reqeqlpedidoproveedor" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for
        bud->calculaypintatotales();

    } catch ( ... ) {
        blMsgInfo ( _ ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    
}


