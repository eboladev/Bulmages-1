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
#include "efqtoolbutton.h"
#include "facturaview.h"
#include "blfunctions.h"
#include "bfcompany.h"
#include "blwidget.h"

/// Para Exportacion de efacturas
#include <QFile>
#include <QTextStream>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#define _PLANTILLA_ g_confpr->value(CONF_PROGDATA) + "efactura/plantilla_efactura.xml"
#define _RESULTADO_ "/tmp/efactura"


///
/**
\param fac
\param parent
**/
EFQToolButton::EFQToolButton ( FacturaView *fac, QWidget *parent ) : QToolButton ( parent ), BlMainCompanyPointer()
{
    BL_FUNC_DEBUG
    setMainCompany ( fac->mainCompany() );
    m_factura = fac;
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    
}


///
/**
**/
EFQToolButton::~EFQToolButton()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param string
\param descuentos_factura
\param bimpfactura
**/
void EFQToolButton::escribe_descuento_factura ( QString &string, BlDbRecordSet *descuentos_factura, BlFixed bimpfactura )
{
    BL_FUNC_DEBUG

    BlFixed descuentoFactura = "0.00";
    descuentoFactura = BlFixed ( descuentos_factura->value( "proporciondfactura" ) );
    descuentoFactura = descuentoFactura * bimpfactura;
    descuentoFactura = descuentoFactura * BlFixed ( "0.01" );

    string += "\t<cac:AllowanceCharge>\n";
    string += "\t\t<cbc:ChargeIndicator>false</cbc:ChargeIndicator>\n";
    string += "\t\t<cbc:MultiplierFactorNumeric>" + descuentos_factura->value( "proporciondfactura" ) + "</cbc:MultiplierFactorNumeric>\n";
    string += "\t\t<cbc:Amount amountCurrencyID=\"EUR\">" + descuentoFactura.toQString() + "</cbc:Amount>\n";
    string += "\t</cac:AllowanceCharge>\n";
    string += "\n";

    
}


/// Funcion que escribe lineas de factura en un QString
/**
\param string
\param lfactura
\param numerolineq
**/
void EFQToolButton::escribe_linea_factura ( QString &string, BlDbRecordSet *lfactura, int numerolinea )
{

    BL_FUNC_DEBUG

    QString numero = QString::number ( numerolinea );

    QString query = "SELECT * FROM articulo WHERE idarticulo = " + lfactura->value( "idarticulo" );
    BlDbRecordSet *articulo = mainCompany() ->loadQuery ( query );

    QString string_iva = lfactura->value( "ivalfactura" );
    QString string_bimp = lfactura->value( "pvplfactura" );
    QString string_descuento = lfactura->value( "descuentolfactura" );

    BlFixed iva ( string_iva ); // es un porcentaje
    BlFixed bimp ( string_bimp );
    BlFixed descuento ( string_descuento );

    BlFixed iva_lfactura = ( iva / 100 ) * bimp;
    BlFixed descuento_lfactura = ( descuento / 100 ) * bimp;

    string += "<cac:InvoiceLine>\n";

    // Numero de linea en el documento
    string += "\t<cac:ID>" + numero + "</cac:ID>\n";

    // Cantidad de elementos en la linea
    string += "\t<cbc:InvoicedQuantity quantityUnitCode=\"UNIT\">" + lfactura->value( "cantlfactura" ) + "</cbc:InvoicedQuantity>\n";

    // PVP de la linea
    string += "\t<cbc:LineExtensionAmount amountCurrencyCodeListVersionID=\"0.3\" amountCurrencyID=\"EUR\">" + lfactura->value( "pvplfactura" ) + "</cbc:LineExtensionAmount>\n";

    // Descuentos o recargos. El false nos dice que es descuento.
    string += "\t<cac:AllowanceCharge>\n";
    string += "\t\t<cbc:ChargeIndicator>false</cbc:ChargeIndicator>\n";
    string += "\t\t<cbc:MultiplierFactorNumeric>" + lfactura->value( "descuentolfactura" ) + "</cbc:MultiplierFactorNumeric>\n";
    string += "\t\t<cbc:Amount amountCurrencyID=\"EUR\">" + descuento_lfactura.toQString() + "</cbc:Amount>\n";
    string += "\t</cac:AllowanceCharge>\n";

    // Total de los impuestos
    string += "\t<cac:TaxTotal>\n";
    string += "\t\t<cbc:TotalTaxAmount amountCurrencyID=\"EUR\">" + iva_lfactura.toQString() + "</cbc:TotalTaxAmount>\n";
    string += "\t</cac:TaxTotal>\n";

    // Descripcion del elemento de la linea
    string += "\t<cac:Item>\n";
    string += "\t\t<cbc:Description>" + lfactura->value( "desclfactura" ) + "</cbc:Description>\n";
    // Codigo de articulo
    string += "\t\t<cac:SellersItemIdentification>\n";
    string += "\t\t\t<cac:ID>" + articulo->value( "codigocompletoarticulo" ) + "</cac:ID>\n";
    string += "\t\t</cac:SellersItemIdentification>\n";
    // Tipo de Impuestos
    string += "\t\t<cac:TaxCategory>\n";
    string += "\t\t\t<cac:ID>" + lfactura->value( "ivalfactura" ) + "</cac:ID>\n";
    string += "\t\t\t<cbc:Percent>" + lfactura->value( "ivalfactura" ) + "</cbc:Percent>\n";
    string += "\t\t\t<cac:TaxScheme>\n";
    string += "\t\t\t\t<cac:TaxTypeCode>IVA</cac:TaxTypeCode>\n";
    string += "\t\t\t</cac:TaxScheme>\n";
    string += "\t\t</cac:TaxCategory>\n";
    // PVP de un articulo
    string += "\t\t<cac:BasePrice>\n";
    string += "\t\t\t<cbc:PriceAmount amountCurrencyCodeListVersionID=\"0.3\" amountCurrencyID=\"EUR\">" + articulo->value( "pvparticulo" ) + "</cbc:PriceAmount>\n";
    string += "\t\t</cac:BasePrice>\n";
    string += "\t</cac:Item>\n";

    string += "</cac:InvoiceLine>\n";
    string += "\n"; // Dejamos sitio para otra linea

    delete articulo;

    
}

/// ---------------- Generacion de factura a partir de una plantilla en formato UBL 1.0 ------------------ ///


///
/**
**/
void EFQToolButton::exporta_factura_ubl()
{

    BL_FUNC_DEBUG

    QString query = "";

    QFile *file_in  = new QFile ( _PLANTILLA_ );

    if ( !file_in->open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al abir la plantilla de factura.") );
	blMsgError (_("Error al abir la plantilla de factura."));
        exit ( -1 );
    }

    /// Inicializamos a vacio, por si acaso
    QString FacturaXml = "";

    /// Creamos un stream en el fichero de entrada
    QTextStream in ( file_in );

    /// Cargamos el fichero plantilla en FacturaXML
    while ( !in.atEnd() ) {
        FacturaXml += in.readLine();
        FacturaXml += "\n";
    }

    /// Aqui empieza la sustitucion de cadenas

    // Comprobamos que los campos necesarios para ejecutar las consultas a la BD existen

    bool error_idtrabajador = false;

    if ( m_factura->dbValue ( "idfactura" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error. El campo 'idfactura' esta vacio.") );
	blMsgError(_("Error. El campo 'idfactura' esta vacio."));
        exit ( -1 );
    }

    if ( m_factura->dbValue ( "idcliente" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error. El campo 'idcliente' esta vacio.") );
	blMsgError(_("Error. El campo 'idcliente' esta vacio."));
        exit ( -1 );
    }

    if ( m_factura->dbValue ( "idtrabajador" ).isEmpty() ) {
        // Esto no es un error grave. Este campo falta si la factura no viene desde un albaran.
        // Marcamos el error y evitamos hacer el query despues
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("El campo 'idtrabajador' esta vacio.") );
        error_idtrabajador = true;
    }

    if ( m_factura->dbValue ( "idforma_pago" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error. El campo 'idforma_pago' esta vacio.") );
	blMsgError(_("Error. El campo 'idforma_pago' esta vacio."));
        exit ( -1 );
    }

    // El fichero de salida
    QString nombrearchivo = "";
    nombrearchivo += QString ( _RESULTADO_ );
    nombrearchivo += QString ( m_factura->dbValue ( "idfactura" ) );
    nombrearchivo += ".xml";

    QFile *file_out = new QFile ( nombrearchivo );

    if ( !file_out->open ( QIODevice::WriteOnly | QIODevice::Text ) ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al crear el archivo de salida.") );
	blMsgError(_("Error al crear el archivo de salida."));
        exit ( -1 );
    }

    // Datos de la factura que no estan en el BlDbRecord
    query = "SELECT totalfactura, bimpfactura, impfactura FROM factura WHERE idfactura = " + m_factura->dbValue ( "idfactura" );
    BlDbRecordSet *factura_totales = mainCompany() ->loadQuery ( query );

    // Datos del cliente
    query = "SELECT * FROM cliente WHERE idcliente = " + m_factura->dbValue ( "idcliente" );
    BlDbRecordSet *cliente = mainCompany() ->loadQuery ( query );

    // Datos del trabajador que emitio la factura

    BlDbRecordSet *trabajador = NULL;

    if ( !error_idtrabajador ) {
        query = "SELECT * FROM trabajador WHERE idtrabajador = " + m_factura->dbValue ( "idtrabajador" );
        trabajador = mainCompany() ->loadQuery ( query );
    }

    // Datos de la forma de pago convenida
    query = "SELECT * FROM forma_pago WHERE idforma_pago = " + m_factura->dbValue ( "idforma_pago" );
    BlDbRecordSet *forma_pago = mainCompany() ->loadQuery ( query );

    // Datos de la tabla configuracion

    bool error_configuracion = false;

    query = "SELECT * FROM configuracion WHERE nombre = 'NombreEmpresa'";
    BlDbRecordSet *nombre_empresa = mainCompany() ->loadQuery ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'CIF'";
    BlDbRecordSet *cif_empresa = mainCompany() ->loadQuery ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'DireccionCompleta'";
    BlDbRecordSet *dir_empresa = mainCompany() ->loadQuery ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'Ciudad'";
    BlDbRecordSet *ciudad_empresa = mainCompany() ->loadQuery ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'CodPostal'";
    BlDbRecordSet *cp_empresa = mainCompany() ->loadQuery ( query );

    if ( nombre_empresa->value( "valor" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("El nombre de la empresa de la tabla de configuracion esta vacio.") );
	blMsgError(_("El nombre de la empresa de la tabla de configuracion esta vacio."));
        error_configuracion = true;
    }

    if ( cif_empresa->value( "valor" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("El CIF de la empresa de la tabla de configuracion esta vacio.") );
	blMsgError(_("El CIF de la empresa de la tabla de configuracion esta vacio."));
        error_configuracion = true;
    }

    if ( dir_empresa->value( "valor" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("La direccion de la empresa de la tabla de configuracion esta vacio.") );
	blMsgError(_("La direccion de la empresa de la tabla de configuracion esta vacio."));
        error_configuracion = true;
    }

    if ( ciudad_empresa->value( "valor" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("La ciudad de la empresa de la tabla de configuracion esta vacio.") );
	blMsgError(_("La ciudad de la empresa de la tabla de configuracion esta vacio."));
        error_configuracion = true;
    }

    if ( cp_empresa->value( "valor" ).isEmpty() ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("El codigo postal de la empresa de la tabla de configuracion esta vacio.") );
	blMsgError(_("El codigo postal de la empresa de la tabla de configuracion esta vacio."));
        error_configuracion = true;
    }

    if ( error_configuracion ) {
        delete factura_totales;
        delete cliente;

        if ( trabajador != NULL )
            delete trabajador;

        delete forma_pago;
        delete nombre_empresa;
        delete cif_empresa;
        delete dir_empresa;
        delete ciudad_empresa;
        delete cp_empresa;

        exit ( -1 );;
    }

    // Sustituimos...

    FacturaXml.replace ( "[numfactura]", m_factura->dbValue ( "numfactura" ) );
    FacturaXml.replace ( "[ffactura]", m_factura->dbValue ( "ffactura" ) );
    FacturaXml.replace ( "[descfactura]", m_factura->dbValue ( "descfactura" ) );
    FacturaXml.replace ( "[impfactura]", factura_totales->value( "impfactura" ) );
    FacturaXml.replace ( "[bimpfactura]", factura_totales->value( "bimpfactura" ) );
    FacturaXml.replace ( "[totalfactura]", factura_totales->value( "totalfactura" ) );

    FacturaXml.replace ( "[nomcliente]", cliente->value( "nomcliente" ) );
    FacturaXml.replace ( "[cifcliente]", cliente->value( "cifcliente" ) );
    FacturaXml.replace ( "[departamento]", cliente->value( "departamento" ) );
    FacturaXml.replace ( "[dircliente]", cliente->value( "dircliente" ) );
    FacturaXml.replace ( "[pobcliente]", cliente->value( "poblcliente" ) );
    FacturaXml.replace ( "[cpcliente]", cliente->value( "cpcliente" ) );

    FacturaXml.replace ( "[nombre_empresa]", nombre_empresa->value( "valor" ) );
    FacturaXml.replace ( "[cif_empresa]", cif_empresa->value( "valor" ) );
    FacturaXml.replace ( "[dir_empresa]", dir_empresa->value( "valor" ) );
    FacturaXml.replace ( "[ciudad_empresa]", ciudad_empresa->value( "valor" ) );
    FacturaXml.replace ( "[cp_empresa]", cp_empresa->value( "valor" ) );

    if ( trabajador != NULL )
        FacturaXml.replace ( "[trabajador]", trabajador->value( "nomtrabajador" ) + " " + trabajador->value( "apellidostrabajador" ) );
    else
        FacturaXml.replace ( "[trabajador]", "--" );

    FacturaXml.replace ( "[forma_de_pago]", forma_pago->value( "descforma_pago" ) );

    /// Obtenemos las lineas de factura y las escribimos en el buffer

    query = "SELECT * FROM lfactura WHERE idfactura = " + m_factura->dbValue ( "idfactura" );
    BlDbRecordSet *lfacturas = mainCompany() ->loadQuery ( query );

    // Por si las moscas...
    lfacturas->firstRecord();

    QString LineasFactura = "\n";
    int numerolinea = 1;

    BlFixed totalFactura = "0.00";

    /// Escribimos la informacion sobre cada linea y de paso ya obtenemos el total
    /// de la factura sumando todos los PVPs de las lineas
    while ( !lfacturas->eof() ) {
        escribe_linea_factura ( LineasFactura, lfacturas, numerolinea );

        totalFactura = totalFactura + ( BlFixed ( lfacturas->value( "cantlfactura" ) ) * BlFixed ( lfacturas->value( "pvplfactura" ) ) );

        lfacturas->nextRecord();
        numerolinea++;
    }

    FacturaXml.replace ( "[lineas_factura]", LineasFactura );

    /// Descuento al PVP de la factura (cogidos de la tabla dfactura)
    query = "SELECT * FROM dfactura WHERE idfactura = " + m_factura->dbValue ( "idfactura" );
    BlDbRecordSet *descuentos_factura = mainCompany() ->loadQuery ( query );

    QString DescuentosFactura = "\n";

    descuentos_factura->firstRecord();

    while ( !descuentos_factura->eof() ) {
        escribe_descuento_factura ( DescuentosFactura, descuentos_factura, totalFactura );
        descuentos_factura->nextRecord();
    }

    FacturaXml.replace ( "[descuentos]", DescuentosFactura );

    delete descuentos_factura;
    delete lfacturas;
    delete factura_totales;
    delete cliente;

    if ( trabajador != NULL )
        delete trabajador;

    delete forma_pago;
    delete nombre_empresa;
    delete cif_empresa;
    delete dir_empresa;
    delete ciudad_empresa;
    delete cp_empresa;

    /// Escribimos los datos en el fichero
    QTextStream out ( file_out );

    out << FacturaXml;

    file_in->close();
    file_out->close();

    delete file_in;
    delete file_out;

    /// Parseamos fichero XML ---------------------------

    QFile *file = new QFile ( nombrearchivo );

    if ( !file->open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Error. No se pudo abrir el archivo '%1' para su parseo.")).arg(nombrearchivo) );
	blMsgError(QString(_("Error. No se pudo abrir el archivo '%1' para su parseo.")).arg(nombrearchivo));
        exit ( -1 );
    } // end if

    QXmlSimpleReader *xmlReader = new QXmlSimpleReader();
    QXmlInputSource *source = new QXmlInputSource ( file );

    bool ok = xmlReader->parse ( source );

    if ( !ok ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error al parsear el archivo.") );
	blMsgError(_("Error al parsear el archivo."));
    } // end if

    file->close();

    delete source;
    delete xmlReader;
    delete file;

    blMsgInfo(QString(_("Exportacion completada. Su efactura se encuentra en '$1'.")).arg(nombrearchivo));

    
}


///
/**
**/
void EFQToolButton::click()
{

    BL_FUNC_DEBUG

    if ( ( !m_factura->dialogChanges_isChanged() ) && ( m_factura->dbValue ( "idfactura" ) != "" ) ) {
        exporta_factura_ubl();
    } else {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Es necesario guardar la factura antes de exportarla a UBL.") );
	blMsgError(_("Es necesario guardar la factura antes de exportarla a UBL."));
    }

    
}

