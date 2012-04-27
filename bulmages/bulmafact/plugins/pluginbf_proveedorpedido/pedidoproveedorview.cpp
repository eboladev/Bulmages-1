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
        : BfForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setAttribute ( Qt::WA_DeleteOnClose );
    try {
        setupUi ( this );

        setTitleName ( _ ( "Pedido Proveedor" ) );
        setDbTableName ( "pedidoproveedor" );
        setDbFieldId ( "idpedidoproveedor" );
        addDbField ( "idpedidoproveedor", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Id pedido proveedor" ) );
        addDbField ( "idproveedor", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id proveedor" ) );
        addDbField ( "idalmacen", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Id almacen" ) );
        addDbField ( "numpedidoproveedor", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Numero pedido proveedor" ) );
        addDbField ( "fechapedidoproveedor", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha pedido proveedor" ) );
        addDbField ( "comentpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentario pedido" ) );
        addDbField ( "procesadopedidoproveedor", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Pedido procesado" ) );
        addDbField ( "descpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Descripcion pedido" ) );
        addDbField ( "refpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Referencia pedido" ) );
        addDbField ( "idforma_pago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id forma de pago" ) );
        addDbField ( "idtrabajador", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Id trabajador" ) );
        addDbField ( "contactpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Persona de contacto proveedor" ) );
        addDbField ( "telpedidoproveedor", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Telefono proveedor" ) );

        /// Disparamos los plugins.
        int res = g_plugins->run ( "PedidoProveedorView_PedidoProveedorView", this );
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
        mui_idproveedor->setLabel ( _ ( "Proveedor:" ) );
        mui_idproveedor->setTableName ( "proveedor" );
        mui_idproveedor->m_valores["cifproveedor"] = "";
        mui_idproveedor->m_valores["nomproveedor"] = "";

        setListaLineas ( mui_lineasDetalle );
        setListaDescuentos ( mui_descuentos );

        dialogChanges_readValues();
        insertWindow ( windowTitle(), this, FALSE );
	blScript(this);
    } catch ( ... ) {
        blMsgInfo ( _ ( "Error al crear el pedido a proveedor" ), this );
    } // end try
    
}


///
/**
**/
PedidoProveedorView::~PedidoProveedorView()
{
    BL_FUNC_DEBUG
    
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
    BL_FUNC_DEBUG
    mui_totalBaseImponible->setText ( base.toQString() );
    mui_totalImpuestos->setText ( iva.toQString() );
    mui_totalPedido->setText ( total.toQString() );
    mui_totalDescuentos->setText ( desc.toQString() );
    mui_totalIRPF->setText ( QString ( irpf.toQString() ) );
    mui_totalRecargo->setText ( QString ( reqeq.toQString() ) );
    
}




///
/**
\param id
**/
void PedidoProveedorView::on_mui_proveedor_valueChanged ( QString id )
{
    BL_FUNC_DEBUG
    mui_lineasDetalle->setColumnValue ( "idproveedor", id );
    mui_idforma_pago->setIdProveedor ( id );
    
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
int PedidoProveedorView::beforeDelete()
{
    BL_FUNC_DEBUG
    m_listalineas->remove();
    m_listadescuentos->remove();
    
    return 0;
}


/// Esta funcion termina la carga de de un pedido a proveedor.
/**
\param idbudget
\return
**/
int PedidoProveedorView::cargarPost ( QString idbudget )
{
    BL_FUNC_DEBUG

    m_listalineas->load ( idbudget );
    m_listadescuentos->load ( idbudget );

    /// Disparamos los plugins.
    g_plugins->run ( "PedidoProveedorView_cargarPost_Post", this );

    calculaypintatotales();

    
    return 0;
}


///
/**
\return
**/
int PedidoProveedorView::afterSave()
{
    BL_FUNC_DEBUG

    m_listalineas->setColumnValue ( "idpedidoproveedor", dbValue ( "idpedidoproveedor" ) );
    m_listadescuentos->setColumnValue ( "idpedidoproveedor", dbValue ( "idpedidoproveedor" ) );

    m_listalineas->save();
    m_listadescuentos->save();
    return 0;
}



/// Impresion de un pedido a proveedor
/** Usa la plantilla pedidoproveedor.rml */
/**
**/

/*
void PedidoProveedorView::imprimir()
{
    BL_FUNC_DEBUG
    BlDbRecordSet *cur = NULL;

    try {

        QString SQLQuery = "";

        if ( dbValue ( "idproveedor" ).isEmpty() ) {
            /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
            blMsgInfo ( _ ( "Tiene que guardar el documento antes de poder imprimirlo." ), this );
            return;
        } else {
            SQLQuery = "SELECT * FROM proveedor WHERE idproveedor = " + dbValue ( "idproveedor" );
        } // end if

        base basesimp;

        /// Copiamos el archivo.
        QString archivo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "pedidoproveedor.rml";
        QString archivod = g_confpr->value( CONF_DIR_USER ) + "pedidoproveedor.rml";
        blCopyFile(archivo, archivod);

        /// Copiamos el logo.
        QString archivologo = g_confpr->value( CONF_DIR_OPENREPORTS ) + "logo.jpg";
        QString logousuario = g_confpr->value( CONF_DIR_USER ) + "logo.jpg";
        blCopyFile(archivologo, logousuario);

        QFile file;
        file.setFileName ( archivod );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();
        QString fitxersortidatxt = "";

        substrConf ( buff );

        /// Linea de totales del pedido.
        cur = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur->eof() ) {
            buff.replace ( "[dirproveedor]", cur->value( "dirproveedor" ) );
            buff.replace ( "[poblproveedor]", cur->value( "poblproveedor" ) );
            buff.replace ( "[telproveedor]", cur->value( "telproveedor" ) );
            buff.replace ( "[nomproveedor]", cur->value( "nomproveedor" ) );
            buff.replace ( "[cifproveedor]", cur->value( "cifproveedor" ) );
            buff.replace ( "[idproveedor]", cur->value( "idproveedor" ) );
            buff.replace ( "[cpproveedor]", cur->value( "cpproveedor" ) );
        } // end if
        delete cur;

        buff.replace ( "[numpedidoproveedor]", dbValue ( "numpedidoproveedor" ) );
        buff.replace ( "[fechapedidoproveedor]", dbValue ( "fechapedidoproveedor" ) );
        buff.replace ( "[contactpedidoproveedor]", dbValue ( "contactpedidoproveedor" ) );
        buff.replace ( "[telpedidoproveedor]", dbValue ( "telpedidoproveedor" ) );
        buff.replace ( "[comentpedidoproveedor]", dbValue ( "comentpedidoproveedor" ) );
        buff.replace ( "[descpedidoproveedor]", dbValue ( "descpedidoproveedor" ) );
        buff.replace ( "[refpedidoproveedor]", dbValue ( "refpedidoproveedor" ) );

        /// Impresion de la tabla de contenidos.
        QString l;

        BlDbSubFormRecord *linea;
        for ( int i = 0; i < m_listalineas->rowCount() - 1; ++i ) {
            linea = m_listalineas->lineaat ( i );
            BlFixed base = BlFixed ( linea->dbValue ( "cantlpedidoproveedor" ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvplpedidoproveedor" ).toAscii().constData() );
            basesimp[linea->dbValue ( "ivalpedidoproveedor" ) ] = basesimp[linea->dbValue ( "ivalpedidoproveedor" ) ] + base - base * BlFixed ( linea->dbValue ( "descuentolpedidoproveedor" ).toAscii().constData() ) / 100;
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "   <td>" + linea->dbValue ( "codigocompletoarticulo" ) + "</td>\n";
            fitxersortidatxt += "   <td><para>" + blXMLEncode ( linea->dbValue ( "desclpedidoproveedor" ) ) + "</para></td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", linea->dbValue ( "cantlpedidoproveedor" ).toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", linea->dbValue ( "pvplpedidoproveedor" ).toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", linea->dbValue ( "descuentolpedidoproveedor" ).toAscii().constData() ) + " %</td>\n";
            fitxersortidatxt += "   <td>" + l.sprintf ( "%s", ( base - base * BlFixed ( linea->dbValue ( "descuentolpedidoproveedor" ) ) / 100 ).toQString().toAscii().constData() ) + "</td>\n";
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
            fitxersortidatxt += "        <td>" + _ ( "Descuento" ) + "</td>\n";
            fitxersortidatxt += "        <td>" + _ ( "Porcentaje" ) + "</td>\n";
            fitxersortidatxt += "        <td>" + _ ( "Total" ) + "</td>\n";
            fitxersortidatxt += "</tr>\n";
            for ( int i = 0; i < m_listadescuentos->rowCount() - 1; ++i ) {
                linea1 = m_listadescuentos->lineaat ( i );
                porcentt = porcentt + BlFixed ( linea1->dbValue ( "proporciondpedidoproveedor" ).toAscii().constData() );
                fitxersortidatxt += "<tr>\n";
                fitxersortidatxt += "        <td>" + linea1->dbValue ( "conceptdpedidoproveedor" ) + "</td>\n";
                fitxersortidatxt += "        <td>" + l.sprintf ( "%s", linea1->dbValue ( "proporciondpedidoproveedor" ).toAscii().constData() ) + " %</td>\n";
                fitxersortidatxt += "        <td>" + l.sprintf ( "-%s", ( BlFixed ( linea1->dbValue ( "proporciondpedidoproveedor" ) ) * basei / 100 ).toQString().toAscii().constData() ) + "</td>\n";
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
            tr1 += "        <td>" + _ ( "Base " ) + it.key() + " %</td>\n";
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
            tr1 += "        <td>" + _ ( "Iva " ) + it.key() + " %</td>\n";
            tr2 += "        <td>" + l.sprintf ( "%s", pariva.toQString().toAscii().constData() ) + "</td>\n";
        } // end for
        tr1 += "        <td>" + _ ( "Total " ) + " </td>\n";
        tr2 += "        <td>" + l.sprintf ( "%s", ( totiva + totbaseimp ).toQString().toAscii().constData() ) + "</td>\n";
        fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
        buff.replace ( "[totales]", fitxersortidatxt );

        if ( file.open ( QIODevice::WriteOnly ) ) {
            QTextStream stream ( &file );
            stream << buff;
            file.close();
        }

        blCreateAndLoadPDF ( "pedidoproveedor" );
        
        return;

    } catch ( ... ) {
        if ( cur ) delete cur;
        BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error.") );
        throw - 1;
    } // end try
}

*/
