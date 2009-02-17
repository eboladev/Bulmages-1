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
#include <QTextStream>

#include "pedidoproveedorview.h"
#include "bfcompany.h"
#include "listlinpedidoproveedorview.h"
#include "providerslist.h"
#include "blfunctions.h"


///
/**
\param comp
\param parent
**/
PedidoProveedorView::PedidoProveedorView ( BfCompany *comp, QWidget *parent )
        : FichaBf ( comp, parent )
{
    _depura ( "PedidoProveedorView::PedidoProveedorView", 0 );
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _( "Pedido Proveedor" ) );
        setDBTableName ( "pedidoproveedor" );
        setDBCampoId ( "idpedidoproveedor" );
        addDBCampo ( "idpedidoproveedor", BlDbField::DBint, BlDbField::DBPrimaryKey, _( "Id pedido proveedor" ) );
        addDBCampo ( "idproveedor", BlDbField::DBint, BlDbField::DBNotNull, _( "Id proveedor" ) );
        addDBCampo ( "idalmacen", BlDbField::DBint, BlDbField::DBNotNull, _( "Id almacen" ) );
        addDBCampo ( "numpedidoproveedor", BlDbField::DBint, BlDbField::DBNothing, _( "Numero pedido proveedor" ) );
        addDBCampo ( "fechapedidoproveedor", BlDbField::DBdate, BlDbField::DBNothing, _( "Fecha pedido proveedor" ) );
        addDBCampo ( "comentpedidoproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Comentario pedido" ) );
        addDBCampo ( "procesadopedidoproveedor", BlDbField::DBboolean, BlDbField::DBNothing, _( "Pedido procesado" ) );
        addDBCampo ( "descpedidoproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Descripcion pedido" ) );
        addDBCampo ( "refpedidoproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Referencia pedido" ) );
        addDBCampo ( "idforma_pago", BlDbField::DBint, BlDbField::DBNothing, _( "Id forma de pago" ) );
        addDBCampo ( "idtrabajador", BlDbField::DBint, BlDbField::DBNothing, _( "Id trabajador" ) );
        addDBCampo ( "contactpedidoproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Persona de contacto proveedor" ) );
        addDBCampo ( "telpedidoproveedor", BlDbField::DBvarchar, BlDbField::DBNothing, _( "Telefono proveedor" ) );

        /// Disparamos los plugins.
        int res = g_plugins->lanza ( "PedidoProveedorView_PedidoProveedorView", this );
        if ( res != 0 ) {
            return;
        } // end if

        /// Usurpamos la identidad de mlist y ponemos nuestro propio widget con sus cosillas.
        mui_lineasDetalle->setMainCompany ( comp );
        mui_lineasDetalle->inicializar();
        mui_idproveedor->setMainCompany ( comp );
        mui_idforma_pago->setMainCompany ( comp );
        mui_idforma_pago->setId ( "0" );
        mui_descuentos->setMainCompany ( comp );
        mui_descuentos->inicializar();
        mui_idalmacen->setMainCompany ( comp );
        mui_idalmacen->setId ( "0" );
        mui_idtrabajador->setMainCompany ( comp );
        mui_idtrabajador->setId ( "0" );
        mui_refpedidoproveedor->setMainCompany ( comp );

	/// Establecemos los parametros de busqueda del Cliente
	mui_idproveedor->setLabel ( _( "Proveedor:" ) );
	mui_idproveedor->setTableName( "proveedor" );
	mui_idproveedor->m_valores["cifproveedor"] = "";
	mui_idproveedor->m_valores["nomproveedor"] = "";

        setListaLineas ( mui_lineasDetalle );
        setListaDescuentos ( mui_descuentos );

        dialogChanges_cargaInicial();
        meteWindow ( windowTitle(), this, FALSE );
    } catch ( ... ) {
        mensajeInfo ( _( "Error al crear el pedido a proveedor" ), this );
    } // end try
    _depura ( "END PedidoProveedorView::PedidoProveedorView", 0 );
}


///
/**
**/
PedidoProveedorView::~PedidoProveedorView()
{
    _depura ( "PedidoProveedorView::~PedidoProveedorView", 0 );
    _depura ( "END PedidoProveedorView::~PedidoProveedorView", 0 );
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
void PedidoProveedorView::pintatotales ( BlFixed iva, BlFixed base, BlFixed total, BlFixed desc, BlFixed irpf, BlFixed reqeq )
{
    _depura ( "PedidoProveedorView::pintatotales", 0 );
    mui_totalBaseImponible->setText ( base.toQString() );
    mui_totalImpuestos->setText ( iva.toQString() );
    mui_totalPedido->setText ( total.toQString() );
    mui_totalDescuentos->setText ( desc.toQString() );
    mui_totalIRPF->setText ( QString ( irpf.toQString() ) );
    mui_totalRecargo->setText ( QString ( reqeq.toQString() ) );
    _depura ( "END PedidoProveedorView::pintatotales", 0 );
}




///
/**
\param id
**/
void PedidoProveedorView::on_mui_proveedor_valueChanged ( QString id )
{
    _depura ( "PedidoProveedorView::on_m_proveedor_valueChanged", 0 );
    mui_lineasDetalle->setColumnValue ( "idproveedor", id );
    mui_idforma_pago->setIdProveedor ( id );
    _depura ( "END PedidoProveedorView::on_m_proveedor_valueChanged", 0 );
}


///
/**
**/
void PedidoProveedorView::on_mui_descuentos_editFinish ( int, int )
{
    calculaypintatotales();
}


///
/**
**/
void PedidoProveedorView::on_mui_lineasDetalle_editFinish ( int, int )
{
    calculaypintatotales();
}


///
/**
\return
**/
int PedidoProveedorView::borrarPre()
{
    _depura ( "PedidoProveedor::borrarPre", 0 );
    m_listalineas->borrar();
    m_listadescuentos->borrar();
    _depura ( "END PedidoProveedor::borrarPre", 0 );
    return 0;
}


/// Esta funcion termina la carga de de un pedido a proveedor.
/**
\param idbudget
\return
**/
int PedidoProveedorView::cargarPost ( QString idbudget )
{
    _depura ( "PedidoProveedor::cargarPost", 0 );

    m_listalineas->cargar ( idbudget );
    m_listadescuentos->cargar ( idbudget );

    /// Disparamos los plugins.
    g_plugins->lanza ( "PedidoProveedorView_cargarPost_Post", this );

    calculaypintatotales();

    _depura ( "END PedidoProveedor::cargar", 0 );
    return 0;
}


///
/**
\return
**/
int PedidoProveedorView::guardarPost()
{
    _depura ( "PedidoProveedor::guardar", 0 );

    m_listalineas->setColumnValue ( "idpedidoproveedor", DBvalue ( "idpedidoproveedor" ) );
    m_listadescuentos->setColumnValue ( "idpedidoproveedor", DBvalue ( "idpedidoproveedor" ) );

    m_listalineas->guardar();
    m_listadescuentos->guardar();
    return 0;
}


/// Impresion de un pedido a proveedor
/** Usa la plantilla pedidoproveedor.rml */
/**
**/
void PedidoProveedorView::imprimir()
{
    _depura ( "PedidoProveedor::imprimirPedidoProveedor", 0 );
    BlDbRecordSet *cur = NULL;

    try {

        QString SQLQuery = "";

        if ( DBvalue ( "idproveedor" ).isEmpty() ) {
            /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
            mensajeInfo ( _( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
            return;
        } else {
            SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + DBvalue ( "idproveedor" );
        } // end if

        base basesimp;

        QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "pedidoproveedor.rml";
        QString archivod = confpr->valor ( CONF_DIR_USER ) + "pedidoproveedor.rml";
        QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

        /// Copiamos el archivo.
#ifdef WINDOWS

        archivo = "copy " + archivo + " " + archivod;
#else

        archivo = "cp " + archivo + " " + archivod;
#endif

        system ( archivo.toAscii().constData() );

        /// Copiamos el logo.
#ifdef WINDOWS

        archivologo = "copy " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#else

        archivologo = "cp " + archivologo + " " + confpr->valor ( CONF_DIR_USER ) + "logo.jpg";
#endif

        system ( archivologo.toAscii().constData() );

        QFile file;
        file.setFileName ( archivod );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();
        QString fitxersortidatxt = "";

        substrConf(buff);

        /// Linea de totales del pedido.
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur->eof() ) {
            buff.replace ( "[dirproveedor]", cur->valor ( "dirproveedor" ) );
            buff.replace ( "[poblproveedor]", cur->valor ( "poblproveedor" ) );
            buff.replace ( "[telproveedor]", cur->valor ( "telproveedor" ) );
            buff.replace ( "[nomproveedor]", cur->valor ( "nomproveedor" ) );
            buff.replace ( "[cifproveedor]", cur->valor ( "cifproveedor" ) );
            buff.replace ( "[idproveedor]", cur->valor ( "idproveedor" ) );
            buff.replace ( "[cpproveedor]", cur->valor ( "cpproveedor" ) );
        } // end if
        delete cur;

        buff.replace ( "[numpedidoproveedor]", DBvalue ( "numpedidoproveedor" ) );
        buff.replace ( "[fechapedidoproveedor]", DBvalue ( "fechapedidoproveedor" ) );
        buff.replace ( "[contactpedidoproveedor]", DBvalue ( "contactpedidoproveedor" ) );
        buff.replace ( "[telpedidoproveedor]", DBvalue ( "telpedidoproveedor" ) );
        buff.replace ( "[comentpedidoproveedor]", DBvalue ( "comentpedidoproveedor" ) );
        buff.replace ( "[descpedidoproveedor]", DBvalue ( "descpedidoproveedor" ) );
        buff.replace ( "[refpedidoproveedor]", DBvalue ( "refpedidoproveedor" ) );

        /// Impresion de la tabla de contenidos.
        QString l;

        BlDbSubFormRecord *linea;
        for ( int i = 0; i < m_listalineas->rowCount() - 1; ++i ) {
            linea = m_listalineas->lineaat ( i );
            BlFixed base = BlFixed ( linea->DBvalue ( "cantlpedidoproveedor" ).toAscii().constData() ) * BlFixed ( linea->DBvalue ( "pvplpedidoproveedor" ).toAscii().constData() );
            basesimp[linea->DBvalue ( "ivalpedidoproveedor" ) ] = basesimp[linea->DBvalue ( "ivalpedidoproveedor" ) ] + base - base * BlFixed ( linea->DBvalue ( "descuentolpedidoproveedor" ).toAscii().constData() ) / 100;
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "   <td>" + linea->DBvalue ( "codigocompletoarticulo" ) + "</td>\n";
            fitxersortidatxt += "   <td><para>" + XMLProtect ( linea->DBvalue ( "desclpedidoproveedor" ) ) + "</para></td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", linea->DBvalue ( "cantlpedidoproveedor" ).toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", linea->DBvalue ( "pvplpedidoproveedor" ).toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", linea->DBvalue ( "descuentolpedidoproveedor" ).toAscii().constData() ) + " %</td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", ( base - base * BlFixed ( linea->DBvalue ( "descuentolpedidoproveedor" ) ) / 100 ).toQString().toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for


        buff.replace ( "[story]", fitxersortidatxt );

        BlFixed basei ( "0.00" );
        base::Iterator it;
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            basei = basei + it.value();
        } // end for

        /// Impresion de los descuentos.
        fitxersortidatxt = "";
        BlFixed porcentt ( "0.00" );
        BlDbSubFormRecord *linea1;
        if ( m_listadescuentos->rowCount() - 1 ) {
            fitxersortidatxt += "<blockTable style=\"tabladescuento\" colWidths=\"12cm, 2cm, 3cm\" repeatRows=\"1\">\n";
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "        <td>" + _( "Descuento" ) + "</td>\n";
            fitxersortidatxt += "        <td>" + _( "Porcentaje" ) + "</td>\n";
            fitxersortidatxt += "        <td>" + _( "Total" ) + "</td>\n";
            fitxersortidatxt += "</tr>\n";
            for ( int i = 0; i < m_listadescuentos->rowCount() - 1; ++i ) {
                linea1 = m_listadescuentos->lineaat ( i );
                porcentt = porcentt + BlFixed ( linea1->DBvalue ( "proporciondpedidoproveedor" ).toAscii().constData() );
                fitxersortidatxt += "<tr>\n";
                fitxersortidatxt += "        <td>" + linea1->DBvalue ( "conceptdpedidoproveedor" ) + "</td>\n";
                fitxersortidatxt += "        <td>" + l.sprintf ( "%s", linea1->DBvalue ( "proporciondpedidoproveedor" ).toAscii().constData() ) + " %</td>\n";
                fitxersortidatxt += "        <td>" + l.sprintf ( "-%s", ( BlFixed ( linea1->DBvalue ( "proporciondpedidoproveedor" ) ) * basei / 100 ).toQString().toAscii().constData() ) + "</td>\n";
                fitxersortidatxt += "</tr>";
            } // end for
            fitxersortidatxt += "</blockTable>\n";
        } // end if
        buff.replace ( "[descuentos]", fitxersortidatxt );

        /// Impresion de los totales.
        fitxersortidatxt = "";
        QString tr1 = ""; /// Rellena el primer tr de titulares.
        QString tr2 = ""; /// Rellena el segundo tr de cantidades.
        fitxersortidatxt += "<blockTable style=\"tablatotales\">\n";

        BlFixed totbaseimp ( "0.00" );
        BlFixed parbaseimp ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > 0 ) {
                parbaseimp = it.value() - it.value() * porcentt / 100;
            } else {
                parbaseimp = it.value();
            } // end if
            totbaseimp = totbaseimp + parbaseimp;
            tr1 += "        <td>" + _( "Base " ) + it.key() + " %</td>\n";
            tr2 += "        <td>" + l.sprintf ( "%s", parbaseimp.toQString().toAscii().constData() ) + "</td>\n";
        } // end for

        BlFixed totiva ( "0.0" );
        BlFixed pariva ( "0.0" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > 0 ) {
                pariva = ( it.value() - it.value() * porcentt / 100 ) * BlFixed ( it.key() ) / 100;
            } else {
                pariva = it.value() * BlFixed ( it.key() ) / 100;
            } // end if
            totiva = totiva + pariva;
            tr1 += "        <td>" + _( "Iva " ) + it.key() + " %</td>\n";
            tr2 += "        <td>" + l.sprintf ( "%s", pariva.toQString().toAscii().constData() ) + "</td>\n";
        } // end for
        tr1 += "        <td>" + _( "Total " ) + " </td>\n";
        tr2 += "        <td>" + l.sprintf ( "%s", ( totiva + totbaseimp ).toQString().toAscii().constData() ) + "</td>\n";
        fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
        buff.replace ( "[totales]", fitxersortidatxt );

        if ( file.open ( QIODevice::WriteOnly ) ) {
            QTextStream stream ( &file );
            stream << buff;
            file.close();
        }

        invocaPDF ( "pedidoproveedor" );
        _depura ( "PedidoProveedor::imprimirPedidoProveedor", 0 );
        return;

    } catch ( ... ) {
        if ( cur ) delete cur;
        throw - 1;
    } // end try
}


