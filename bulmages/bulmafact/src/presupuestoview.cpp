/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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

#include <QMessageBox>
#include <QWidget>
#include <QObject>
#include <QComboBox>
#include <QToolButton>
#include <QFile>
#include <QTextStream>
#include <QLayout>
#include <QMessageBox>
#include <QCloseEvent>

#include "funcaux.h"
#include "listlinpresupuestoview.h"
#include "presupuestoview.h"
#include "company.h"
#include "clientslist.h"
#include "articulolist.h"
#include "configuracion.h"
#include "pedidoclienteview.h"
#include "albaranclienteview.h"
#include "busquedacliente.h"
#include "busquedaformapago.h"
#include "informereferencia.h"
#include "plugins.h"


///
/**
\param comp
\param parent
\return
**/
PresupuestoView::PresupuestoView ( Company *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "Inicializacion de PresupuestoView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( tr ( "Presupuesto" ) );
        setDBTableName ( "presupuesto" );
        setDBCampoId ( "idpresupuesto" );
        addDBCampo ( "idpresupuesto", DBCampo::DBint, DBCampo::DBPrimaryKey, QApplication::translate ( "Presupuesto", "ID presupuesto" ) );
        addDBCampo ( "idcliente", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Presupuesto", "Cliente" ) );
        addDBCampo ( "idalmacen", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Presupuesto", "Almacen" ) );
        addDBCampo ( "numpresupuesto", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Numero de presupuesto" ) );
        addDBCampo ( "fpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Fecha de creacion" ) );
        addDBCampo ( "vencpresupuesto", DBCampo::DBdate, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Fecha de vencimiento" ) );
        addDBCampo ( "contactpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Persona de contacto" ) );
        addDBCampo ( "telpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Numero de telefono" ) );
        addDBCampo ( "comentpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Comentarios" ) );
        addDBCampo ( "procesadopresupuesto", DBCampo::DBboolean, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Procesado" ) );
        addDBCampo ( "descpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "% Descuento" ) );
        addDBCampo ( "refpresupuesto", DBCampo::DBvarchar, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Referencia" ) );
        addDBCampo ( "idforma_pago", DBCampo::DBint, DBCampo::DBNotNull, QApplication::translate ( "Presupuesto", "Forma de pago" ) );
        addDBCampo ( "idtrabajador", DBCampo::DBint, DBCampo::DBNothing, QApplication::translate ( "Presupuesto", "Trabajador" ) );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "PresupuestoView_PresupuestoView", this );
        if ( res != 0 ) {
            return;
        } // end if

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        subform2->setEmpresaBase ( comp );
        m_descuentos->setEmpresaBase ( comp );
        mui_idcliente->setEmpresaBase ( comp );
        mui_idforma_pago->setEmpresaBase ( comp );
        mui_idalmacen->setEmpresaBase ( comp );
        mui_idtrabajador->setEmpresaBase ( comp );
        mui_refpresupuesto->setEmpresaBase ( comp );

        setListaLineas ( subform2 );
        setListaDescuentos ( m_descuentos );

        /// Inicializamos para que no se queden sin ser pintada.
        mui_idforma_pago->setValorCampo ( "0" );
        mui_idalmacen->setValorCampo ( "0" );
        mui_idtrabajador->setValorCampo ( "0" );

        meteWindow ( windowTitle(), this, FALSE );
        /// Disparamos los plugins por flanco descendente.
        g_plugins->lanza ( "PresupuestoView_PresupuestoView_Post", this );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error al crear el presupuesto" ), this );
    } // end try
    _depura ( "Fin de la inicializacion de PresupuestoView", 0 );
}


/// Este metodo es llamado cuando hacemos un nuevo registro, pero no hay carga desde la base de datos.
/**
**/
void PresupuestoView::inicializar()
{
    _depura ( "PresupuestoView::inicializar", 0 );
    subform2->inicializar();
    m_descuentos->inicializar();
    dialogChanges_cargaInicial();
    _depura ( "END PresupuestoView::inicializar", 0 );
}


///
/**
**/
PresupuestoView::~PresupuestoView()
{
    _depura ( "PresupuestoView::~PresupuestoView", 0 );
    ( ( Company * ) empresaBase() ) ->refreshPresupuestos();

    /// Disparamos los plugins.
    g_plugins->lanza ( "PresupuestoView_DesPresupuestoView", this );
    _depura ( "END PresupuestoView::~PresupuestoView", 0 );
}


///
/**
**/
void PresupuestoView::s_pintaTotales()
{
    _depura ( "PresupuestoView::s_pintaTotales", 0 );
    calculaypintatotales();
    _depura ( "END PresupuestoView::s_pintaTotales", 0 );
}


///
/**
**/
void PresupuestoView::on_mui_pasarapedido_clicked()
{
    _depura ( "PresupuestoView::on_mui_pasarapedido_clicked", 0 );
    generarPedidoCliente();
    _depura ( "END PresupuestoView::on_mui_pasarapedido_clicked", 0 );
}


///
/**
**/
void PresupuestoView::on_m_descuentos_editFinish ( int, int )
{
    _depura ( "PresupuestoView::on_m_descuentos_editFinish", 0 );
    calculaypintatotales();
    _depura ( "END PresupuestoView::on_m_descuentos_editFinish", 0 );
}


///
/**
**/
void PresupuestoView::on_subform2_editFinish ( int, int )
{
    _depura ( "PresupuestoView::on_subform2_editFinish", 0 );
    calculaypintatotales();
    _depura ( "END PresupuestoView::on_subform2_editFinish", 0 );
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
void PresupuestoView::pintatotales ( Fixed iva, Fixed base, Fixed total, Fixed desc, Fixed irpf, Fixed reqeq )
{
    _depura ( "PresupuestoView::pintatotales", 0 );
    m_totalBases->setText ( QString ( base.toQString() ) );
    m_totalTaxes->setText ( QString ( iva.toQString() ) );
    m_totalPresupuestoCliente->setText ( QString ( total.toQString() ) );
    m_totalDiscounts->setText ( QString ( desc.toQString() ) );
    m_totalIRPF->setText ( QString ( irpf.toQString() ) );
    m_totalReqEq->setText ( QString ( reqeq.toQString() ) );
    _depura ( "END PresupuestoView::pintatotales", 0 );
}


/// Se encarga de generar un pedido a partir del presupuesto.
/**
\return
**/
void PresupuestoView::generarPedidoCliente()
{
    _depura ( "PresupuestoView::generarPedidoCliente", 0 );
    PedidoClienteView *bud = NULL;
    cursor2 *cur = NULL;

    try {
        /// Comprueba si disponemos de los datos m&iacute;nimos. Si no se hace esta
        /// comprobaci&oacute;n la consulta a la base de datos ser&aacute; erronea y al hacer
        /// el siguiente cur->eof() el programa fallar&aacute;.
        /// Comprobamos que existe el pedido con esos datos, y en caso afirmativo lo mostramos.

        QString SQLQuery = "";

        if ( DBvalue ( "refpresupuesto" ).isEmpty() || DBvalue ( "idcliente" ).isEmpty() ) {
            /// El presupuesto no se ha guardado y no se dispone en la base de datos
            /// de estos datos. Se utilizan en su lugar los del formulario.
            /// Verifica que exista, por lo menos, un cliente seleccionado.
            if ( mui_idcliente->idcliente().isEmpty() ) {
                mensajeInfo ( tr ( "Tiene que seleccionar un cliente" ), this );
                return;
            } else {
                SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + mui_refpresupuesto->text() + "' AND idcliente = " + mui_idcliente->idcliente();
            } // end if
        } else {
            SQLQuery = "SELECT * FROM pedidocliente WHERE refpedidocliente = '" + DBvalue ( "refpresupuesto" ) + "' AND idcliente = " + DBvalue ( "idcliente" );
        } // end if

        cur = empresaBase()->cargacursor ( SQLQuery );

        if ( !cur->eof() ) {
            /// Informamos que ya hay un pedido y lo abrimos.
            /// Si no existe pedido salimos de la funci&oacute;n y creamos el pedido.
            if ( QMessageBox::question ( this,
                                         tr ( "Pedido existente" ),
                                         tr ( "Existe un pedido a este cliente con la misma referencia que este presupuesto. Desea abrirla para verificar?" ),
                                         tr ( "&Si" ), tr ( "&No" ), QString::null, 0, 1 ) ) {
                return;
            } // end if

            bud = empresaBase() ->newPedidoClienteView();
            empresaBase() ->m_pWorkspace->addWindow ( bud );
            bud->cargar ( cur->valor ( "idpedidocliente" ) );
            bud->show();
            delete cur;
            return;
        } // end if

        delete cur;

        /// Creamos el pedido.
        bud = empresaBase() ->newPedidoClienteView();
        bud->cargar ( "0" );
        empresaBase() ->m_pWorkspace->addWindow ( bud );

        /// Traspasamos toda la informacion del presupuesto al pedido.
        recogeValores();
        bud->setDBvalue ( "idcliente", DBvalue ( "idcliente" ) );
        bud->setDBvalue ( "comentpedidocliente", DBvalue ( "comentpresupuesto" ) );
        bud->setDBvalue ( "descpedidocliente", DBvalue ( "descpresupuesto" ) );
        bud->setDBvalue ( "fechapedidocliente", DBvalue ( "fpresupuesto" ) );
        bud->setDBvalue ( "idforma_pago", DBvalue ( "idforma_pago" ) );
        bud->setDBvalue ( "refpedidocliente", DBvalue ( "refpresupuesto" ) );
        bud->setDBvalue ( "procesadopedidocliente", DBvalue ( "procesadopresupuesto" ) );
        bud->setDBvalue ( "idalmacen", DBvalue ( "idalmacen" ) );
        bud->setDBvalue ( "contactpedidocliente", DBvalue ( "contactpresupuesto" ) );
        bud->setDBvalue ( "telpedidocliente", DBvalue ( "telpresupuesto" ) );

        /// Traspasamos las lineas del presupuesto a lineas del pedido.
        SDBRecord *linea;
        SDBRecord *linea2;

        for ( int i = 0; i < m_listalineas->rowCount(); i++ ) {
            linea = m_listalineas->lineaat ( i );
            if ( linea->DBvalue ( "idarticulo" ) != "" ) {
                linea2 = bud->getlistalineas() ->lineaat ( bud->getlistalineas() ->rowCount() - 1 );
                bud->getlistalineas() ->nuevoRegistro();
                bud->getlistalineas() ->setProcesarCambios ( FALSE );
                linea2->setDBvalue ( "desclpedidocliente", linea->DBvalue ( "desclpresupuesto" ) );
                linea2->setDBvalue ( "cantlpedidocliente", linea->DBvalue ( "cantlpresupuesto" ) );
                linea2->setDBvalue ( "pvplpedidocliente", linea->DBvalue ( "pvplpresupuesto" ) );
                linea2->setDBvalue ( "ivalpedidocliente", linea->DBvalue ( "ivalpresupuesto" ) );
                linea2->setDBvalue ( "descuentolpedidocliente", linea->DBvalue ( "descuentolpresupuesto" ) );
                linea2->setDBvalue ( "idarticulo", linea->DBvalue ( "idarticulo" ) );
                linea2->setDBvalue ( "codigocompletoarticulo", linea->DBvalue ( "codigocompletoarticulo" ) );
                linea2->setDBvalue ( "nomarticulo", linea->DBvalue ( "nomarticulo" ) );
                bud->getlistalineas() ->setProcesarCambios ( TRUE );
            } // end if
        } // end for

        /// Traspasamos los descuentos del presupuesto a descuentos del pedido.
        SDBRecord *linea1;
        SDBRecord *linea3;
        for ( int i = 0; i < m_listadescuentos->rowCount(); i++ ) {
            linea1 = m_listadescuentos->lineaat ( i );
            if ( linea1->DBvalue ( "proporciondpresupuesto" ) != "" ) {
                linea3 = bud->getlistadescuentos() ->lineaat ( bud->getlistadescuentos() ->rowCount() - 1 );
                bud->getlistadescuentos() ->setProcesarCambios ( FALSE );
                linea3->setDBvalue ( "conceptdpedidocliente", linea1->DBvalue ( "conceptdpresupuesto" ) );
                linea3->setDBvalue ( "proporciondpedidocliente", linea1->DBvalue ( "proporciondpresupuesto" ) );
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

    _depura ( "END PresupuestoView::generarPedidoCliente", 0 );
}


///
/**
\param id
**/
void PresupuestoView::on_mui_idcliente_valueChanged ( QString id )
{
    _depura ( "PresupuestoView::on_mui_idcliente_valueChanged", 0 );
    subform2->setIdCliente ( id );
    mui_idforma_pago->setIdCliente ( id );
    _depura ( "END PresupuestoView::on_mui_idcliente_valueChanged", 0 );
}


///
/**
\param id
**/
void PresupuestoView::on_mui_idalmacen_valueChanged ( QString id )
{
    _depura ( "PresupuestoView::on_mui_idalmacen_valueChanged", 0 );
    m_listalineas->setIdAlmacen ( id );
    _depura ( "END PresupuestoView::on_mui_idalmacen_valueChanged", 0 );
}


///
/**
\return
**/
int PresupuestoView::borrarPre()
{
    _depura ( "PresupuestoView::borrar", 0 );
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->lanza ( "Presupuesto_borrarPre", this );
    m_listalineas->borrar();
    m_listadescuentos->borrar();

    _depura ( "END PresupuestoView::borrar", 0 );
    return 0;
}


/// Esta funcion carga un Presupuesto.
/**
\param idbudget
\return
**/
int PresupuestoView::cargarPost ( QString idbudget )
{
    _depura ( "PresupuestoView::cargarPost", 0 );

    m_listalineas->cargar ( idbudget );
    m_listadescuentos->cargar ( idbudget );

    calculaypintatotales();

    _depura ( "END PresupuestoView::cargar", 0 );
    return 0;
}


///
/**
\return
**/
int PresupuestoView::guardarPost()
{
    _depura ( "PresupuestoView::guardarPost", 0 );

    m_listalineas->setColumnValue ( "idpresupuesto", DBvalue ( "idpresupuesto" ) );
    m_listadescuentos->setColumnValue ( "idpresupuesto", DBvalue ( "idpresupuesto" ) );

    m_listalineas->guardar();
    m_listadescuentos->guardar();
    /// Disparamos los plugins con presupuesto_imprimirPresupuesto.
    g_plugins->lanza ( "Presupuesto_guardarPost_Post", this );

    _depura ( "END PresupuestoView::guardar", 0 );
    return 0;
}


///
/**
\return
**/
QString PresupuestoView::detalleArticulos()
{
    _depura ( "PresupuestoView::detalleArticulos", 0 );
    QString texto = "";
    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE presentablearticulo AND idpresupuesto=" + DBvalue ( "idpresupuesto" ) );
    int i = 0;
    while ( !cur->eof() ) {
        i = !i;
        if ( i ) {
            texto += "<blockTable style=\"tabladetalle1\" colWidths=\"5cm, 8cm\" rowHeights=\"5.5cm\">\n";
        } else {
            texto += "<blockTable style=\"tabladetalle2\" colWidths=\"8cm, 5cm\" rowHeights=\"5.5cm\">\n";
        } // end if
        texto += "<tr>\n";
        if ( i ) {
            texto += "<td><h1>" + cur->valor ( "nomarticulo" ) + "</h1>";
            texto += "<para><pre>" + cur->valor ( "obserarticulo" ) + "</pre></para></td>\n";
        } // end if
        QString file = confpr->valor ( CONF_DIR_IMG_ARTICLES ) + cur->valor ( "codigocompletoarticulo" ) + ".jpg";
        QFile f ( file );
        if ( f.exists() ) {
            texto += "    <td><illustration x=\"0\" y=\"0\" height=\"5cm\">\n"
                     "        <image file=\"" + confpr->valor ( CONF_DIR_IMG_ARTICLES ) + cur->valor ( "codigocompletoarticulo" ) + ".jpg\" x=\"0\" y=\"0\" height=\"5cm\"/>\n"
                     "        </illustration></td>\n";
        } else {
            texto += "<td></td>\n";
        } // end if
        if ( !i ) {
            texto += "<td><h1>" + cur->valor ( "nomarticulo" ) + "</h1>";
            texto += "<para><pre>" + cur->valor ( "obserarticulo" ) + "</pre></para></td>\n";
        } // end if
        texto += "</tr>\n";
        texto += "</blockTable>";
        cur->siguienteregistro();
    } // end while
    delete cur;
    _depura ( "END PresupuestoView::detalleArticulos", 0 );
    return texto;
}

