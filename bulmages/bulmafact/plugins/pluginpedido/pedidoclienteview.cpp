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
#include "company.h"
#include "listlinpedidoclienteview.h"
#include "clientslist.h"
#include "presupuestoview.h"
#include "presupuestolist.h"
#include "informereferencia.h"
#include "funcaux.h"
#include "plugins.h"


///
/**
\param comp
\param parent
\return
**/
PedidoClienteView::PedidoClienteView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "PedidoClienteView::PedidoClienteView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        setupUi ( this );

        setTitleName ( tr ( "Pedido Cliente" ) );
        setDBTableName ( "pedidocliente" );
        setDBCampoId ( "idpedidocliente" );
        addDBCampo ( "idpedidocliente", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "PedidoCliente", "Identificador" ) );
        addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "PedidoCliente", "Cliente" ) );
        addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "PedidoCliente", "Almacen" ) );
        addDBCampo ( "numpedidocliente", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Numero pedido cliente" ) );
        addDBCampo ( "fechapedidocliente", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Identificador presupuesto" ) );
        addDBCampo ( "descpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Fecha" ) );
        addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Forma pago" ) );
        addDBCampo ( "idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Trabajador" ) );
        addDBCampo ( "contactpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Contacto" ) );
        addDBCampo ( "telpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Telefono" ) );
        addDBCampo ( "comentpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Comentarios" ) );
        addDBCampo ( "procesadopedidocliente", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Procesado" ) );
        addDBCampo ( "refpedidocliente", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "PedidoCliente", "Referencia" ) );


        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "PedidoClienteView_PedidoClienteView", this );
        if ( res != 0 )
            return;
        subform3->setEmpresaBase ( comp );
        mui_idcliente->setEmpresaBase ( comp );
        mui_idforma_pago->setEmpresaBase ( comp );
        m_descuentos->setEmpresaBase ( comp );
        mui_idalmacen->setEmpresaBase ( comp );
        mui_idtrabajador->setEmpresaBase ( comp );
        mui_refpedidocliente->setEmpresaBase ( comp );
        setListaLineas ( subform3 );
        setListaDescuentos ( m_descuentos );
        mui_idforma_pago->setidforma_pago ( "" );
        mui_idalmacen->setidalmacen ( "" );
        mui_idtrabajador->setidtrabajador ( "" );
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el pedido cliente" ), this );
    } // end try
    _depura ( "END PedidoClienteView::PedidoClienteView", 0 );
}


///
/**
**/
PedidoClienteView::~PedidoClienteView()
{
    _depura ( "PedidoClienteView::~PedidoClienteView", 0 );
    empresaBase() ->refreshPedidosCliente();
    _depura ( "END PedidoClienteView::~PedidoClienteView", 0 );
}


///
/**
**/
void PedidoClienteView::inicializar()
{
    _depura ( "PedidoClienteView::inicializar", 0 );
    subform3->inicializar();
    m_descuentos->inicializar();
    pintar();
    dialogChanges_cargaInicial();
    _depura ( "END PedidoClienteView::inicializar", 0 );
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
void PedidoClienteView::pintatotales ( Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq )
{
    _depura ( "PedidoClienteView::pintatotales", 0 );
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    m_totalpedidocliente->setText ( total.toQString() );
    _depura ( "END PedidoClienteView::pintatotales", 0 );
}


///
/**
**/
void PedidoClienteView::on_mui_verpresupuesto_clicked()
{
    _depura ( "PedidoClienteView::on_mui_verpresupuesto_clicked", 0 );
    PresupuestoView *bud = NULL;
    cursor2 *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.

        QString SQLQuery = "";

        if ( DBvalue ( "refpedidocliente" ).isEmpty() || DBvalue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( mui_idcliente->idcliente().isEmpty() ) {
                mensajeInfo ( tr ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" + mui_refpedidocliente->text() + "' AND idcliente = " + mui_idcliente->idcliente();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM presupuesto WHERE refpresupuesto = '" + DBvalue ( "refpedidocliente" ) + "' AND idcliente = " + DBvalue ( "idcliente" );
        } // end if

        cur = empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            while ( !cur->eof() ) {
                bud = empresaBase() ->nuevoPresupuestoView();
                empresaBase() ->m_pWorkspace->addWindow ( bud );
                if ( bud->cargar ( cur->valor ( "idpresupuesto" ) ) ) {
                    delete bud;
                    return;
                } // end if
                bud->show();
                cur->siguienteregistro();
            } // end while
        } else {
            mensajeInfo ( tr ( "No hay presupuestos con la misma referencia." ), this );
            _depura ( "No hay presupuestos con la misma referencia.", 2 );
        } // end if

        delete cur;

    } catch ( ... ) {
        mensajeInfo ( tr ( "Error inesperado." ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try

    _depura ( "END PedidoClienteView::on_mui_verpresupuesto_clicked", 0 );
}


/** Genera un Albaran a Cliente a partir de los datos
    que tiene el pedido.
*/
/**
\return
**/
void PedidoClienteView::generarAlbaran()
{
    _depura ( "PedidoClienteView::generarAlbaran", 0 );
/*

    AlbaranClienteView *bud = NULL;
    cursor2 *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el albaran con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( DBvalue ( "refpedidocliente" ).isEmpty() || DBvalue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( mui_idcliente->idcliente().isEmpty() ) {
                mensajeInfo ( tr ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + mui_refpedidocliente->text() + "' AND idcliente = " + mui_idcliente->idcliente();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM albaran WHERE refalbaran = '" + DBvalue ( "refpedidocliente" ) + "' AND idcliente = " + DBvalue ( "idcliente" );
        } // end if

        cur = empresaBase() ->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un albaran y que la abriremos.
            /// Si no salimos de la funci&oacute;n.
            if ( QMessageBox::question ( this,
                                         tr ( "Albaran ya existe" ),
                                         tr ( "Existe un albaran a este cliente con la misma referencia que este pedido. Desea abrirlo para verificar?" ),
                                         tr ( "&Si" ), tr ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if
            bud = new AlbaranClienteView ( empresaBase(), NULL );
            empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idalbaran" ) );
            bud->show();
            return;
        } // end if
        delete cur;

        /// Creamos el albaran.
        bud = empresaBase() ->newAlbaranClienteView();
        empresaBase() ->m_pWorkspace->addWindow ( bud );
        bud->cargar ( "0" );

        /// Traspasamos los datos al albaran.
        recogeValores();
        bud->setDBvalue ( "comentalbaran", DBvalue ( "comentpedidocliente" ) );
        bud->setDBvalue ( "descalbaran", DBvalue ( "descpedidocliente" ) );
        bud->setDBvalue ( "idforma_pago", DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "refalbaran", DBvalue ( "refpedidocliente" ) );
        bud->setDBvalue ( "idcliente", DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "idalmacen", DBvalue ( "idalmacen" ) );
        bud->setDBvalue ( "contactalbaran", DBvalue ( "contactpedidocliente" ) );
        bud->setDBvalue ( "telalbaran", DBvalue ( "telpedidocliente" ) );
        bud->setDBvalue ( "idtrabajador", DBvalue ( "idtrabajador" ) );

        /// Traspasamos las lineas al albaran.
        SDBRecord *linea, *linea1;
        for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
            linea = m_listalineas->lineaat ( i );
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
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
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
        mensajeInfo ( tr ( "Error inesperado" ), this );
        if ( cur ) delete cur;
        if ( bud ) delete bud;
    } // end try
*/
    _depura ( "END PedidoClienteView::generarAlbaran", 0 );
}


///
/**
\param id
**/
void PedidoClienteView::on_mui_idcliente_valueChanged ( QString id )
{
    _depura ( "PedidoClienteView::on_m_cliente_valueChanged", 0 );
    subform3->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    _depura ( "END PedidoClienteView::on_m_cliente_valueChanged", 0 );
}


///
/**
\return
**/
int PedidoClienteView::borrarPre()
{
    _depura ( "PedidoClienteView::borrar", 0 );
    m_listalineas->borrar();
    m_listadescuentos->borrar();
    _depura ( "PedidoClienteView::borrar", 0 );
    return 0;
}


/// Esta funcion carga un PedidoCliente.
/**
\param idbudget
\return
**/
int PedidoClienteView::cargarPost ( QString idbudget )
{
    _depura ( "PedidoClienteView::cargarPost", 0 );

    m_listalineas->cargar ( idbudget );
    m_listadescuentos->cargar ( idbudget );

    /// Disparamos los plugins.
    g_plugins->lanza ( "PedidoClienteView_cargarPost_Post", this );

    calculaypintatotales();

    _depura ( "END PedidoClienteView::cargar", 0 );
    return 0;
}


/// Guardamos el pedido cliente.
/**
\return
**/
int PedidoClienteView::guardarPost()
{
    _depura ( "PedidoClienteView::guardar", 0 );
    m_listalineas->setColumnValue ( "idpedidocliente", DBvalue ( "idpedidocliente" ) );
    m_listalineas->guardar();
    m_listadescuentos->setColumnValue ( "idpedidocliente", DBvalue ( "idpedidocliente" ) );
    m_listadescuentos->guardar();
    return 0;
}


///
/**
**/
void PedidoClienteView::s_pintaTotales()
{
    _depura ( "PedidoClienteView::s_pintaTotales", 0 );
    calculaypintatotales();
    _depura ( "END PedidoClienteView::s_pintaTotales", 0 );
}


///
/**
**/
void PedidoClienteView::on_mui_pasaraalbaran_clicked()
{
    _depura ( "PedidoClienteView::on_mui_pasaraalbaran_clicked", 0 );
    generarAlbaran();
    _depura ( "END PedidoClienteView::on_mui_pasaraalbaran_clicked", 0 );
}


///
/**
**/
void PedidoClienteView::on_m_descuentos_editFinish ( int, int )
{
    _depura ( "PedidoClienteView::on_m_descuentos_editFinish", 0 );
    calculaypintatotales();
    _depura ( "END PedidoClienteView::on_m_descuentos_editFinish", 0 );
}


///
/**
**/
void PedidoClienteView::on_subform3_editFinish ( int, int )
{
    _depura ( "PedidoClienteView::on_subform3_editFinish", 0 );
    calculaypintatotales();
    _depura ( "END sPedidoClienteView::on_subform3_editFinish", 0 );
}


///
/**
\param id
**/
void PedidoClienteView::on_mui_idalmacen_valueChanged ( QString id )
{
    _depura ( "PedidoClienteView::on_mui_idalmacen_valueChanged", 0 );
    m_listalineas->setIdAlmacen ( id );
    _depura ( "END PedidoClienteView::on_mui_idalmacen_valueChanged", 0 );
}

