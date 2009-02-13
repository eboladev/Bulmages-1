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
#include "funcaux.h"
#include "company.h"
#include "blwidget.h"

/// Para Exportacion de efacturas
#include <QFile>
#include <QTextStream>
#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#define _PLANTILLA_ confpr->valor(CONF_PROGDATA) + "efactura/plantilla_efactura.xml"
#define _RESULTADO_ "/tmp/efactura"


///
/**
\param fac
\param parent
**/
EFQToolButton::EFQToolButton ( FacturaView *fac, QWidget *parent ) : QToolButton ( parent ), PEmpresaBase()
{
    _depura ( "EFQToolButton::EFQToolButton", 0 );
    setEmpresaBase ( fac->empresaBase() );
    m_factura = fac;
    connect ( this, SIGNAL ( clicked() ), this, SLOT ( click() ) );
    _depura ( "END EFQToolButton::EFQToolButton", 0 );
}


///
/**
**/
EFQToolButton::~EFQToolButton()
{
    _depura ( "EFQToolButton::~EFQToolButton", 0 );
    _depura ( "END EFQToolButton::~EFQToolButton", 0 );
}


///
/**
\param string
\param descuentos_factura
\param bimpfactura
**/
void EFQToolButton::escribe_descuento_factura ( QString &string, BlDbRecordSet *descuentos_factura, BlFixed bimpfactura )
{
    _depura ( "EFQToolButton::escribe_descuento_factura", 0 );

    BlFixed descuentoFactura = "0.00";
    descuentoFactura = BlFixed ( descuentos_factura->valor ( "proporciondfactura" ) );
    descuentoFactura = descuentoFactura * bimpfactura;
    descuentoFactura = descuentoFactura * BlFixed ( "0.01" );

    string += "\t<cac:AllowanceCharge>\n";
    string += "\t\t<cbc:ChargeIndicator>false</cbc:ChargeIndicator>\n";
    string += "\t\t<cbc:MultiplierFactorNumeric>" + descuentos_factura->valor ( "proporciondfactura" ) + "</cbc:MultiplierFactorNumeric>\n";
    string += "\t\t<cbc:Amount amountCurrencyID=\"EUR\">" + descuentoFactura.toQString() + "</cbc:Amount>\n";
    string += "\t</cac:AllowanceCharge>\n";
    string += "\n";

    _depura ( "END EFQToolButton::escribe_descuento_factura", 0 );
}


/// Funcion que escribe lineas de factura en un QString
/**
\param string
\param lfactura
\param numerolineq
**/
void EFQToolButton::escribe_linea_factura ( QString &string, BlDbRecordSet *lfactura, int numerolinea )
{

    _depura ( "EFQToolButton::escribe_linea_factura", 0 );

    QString numero = QString::number ( numerolinea );

    QString query = "SELECT * FROM articulo WHERE idarticulo = " + lfactura->valor ( "idarticulo" );
    BlDbRecordSet *articulo = empresaBase() ->cargacursor ( query );

    QString string_iva = lfactura->valor ( "ivalfactura" );
    QString string_bimp = lfactura->valor ( "pvplfactura" );
    QString string_descuento = lfactura->valor ( "descuentolfactura" );

    BlFixed iva ( string_iva ); // es un porcentaje
    BlFixed bimp ( string_bimp );
    BlFixed descuento ( string_descuento );

    BlFixed iva_lfactura = ( iva / 100 ) * bimp;
    BlFixed descuento_lfactura = ( descuento / 100 ) * bimp;

    string += "<cac:InvoiceLine>\n";

    // Numero de linea en el documento
    string += "\t<cac:ID>" + numero + "</cac:ID>\n";

    // Cantidad de elementos en la linea
    string += "\t<cbc:InvoicedQuantity quantityUnitCode=\"UNIT\">" + lfactura->valor ( "cantlfactura" ) + "</cbc:InvoicedQuantity>\n";

    // PVP de la linea
    string += "\t<cbc:LineExtensionAmount amountCurrencyCodeListVersionID=\"0.3\" amountCurrencyID=\"EUR\">" + lfactura->valor ( "pvplfactura" ) + "</cbc:LineExtensionAmount>\n";

    // Descuentos o recargos. El false nos dice que es descuento.
    string += "\t<cac:AllowanceCharge>\n";
    string += "\t\t<cbc:ChargeIndicator>false</cbc:ChargeIndicator>\n";
    string += "\t\t<cbc:MultiplierFactorNumeric>" + lfactura->valor ( "descuentolfactura" ) + "</cbc:MultiplierFactorNumeric>\n";
    string += "\t\t<cbc:Amount amountCurrencyID=\"EUR\">" + descuento_lfactura.toQString() + "</cbc:Amount>\n";
    string += "\t</cac:AllowanceCharge>\n";

    // Total de los impuestos
    string += "\t<cac:TaxTotal>\n";
    string += "\t\t<cbc:TotalTaxAmount amountCurrencyID=\"EUR\">" + iva_lfactura.toQString() + "</cbc:TotalTaxAmount>\n";
    string += "\t</cac:TaxTotal>\n";

    // Descripcion del elemento de la linea
    string += "\t<cac:Item>\n";
    string += "\t\t<cbc:Description>" + lfactura->valor ( "desclfactura" ) + "</cbc:Description>\n";
    // Codigo de articulo
    string += "\t\t<cac:SellersItemIdentification>\n";
    string += "\t\t\t<cac:ID>" + articulo->valor ( "codigocompletoarticulo" ) + "</cac:ID>\n";
    string += "\t\t</cac:SellersItemIdentification>\n";
    // Tipo de Impuestos
    string += "\t\t<cac:TaxCategory>\n";
    string += "\t\t\t<cac:ID>" + lfactura->valor ( "ivalfactura" ) + "</cac:ID>\n";
    string += "\t\t\t<cbc:Percent>" + lfactura->valor ( "ivalfactura" ) + "</cbc:Percent>\n";
    string += "\t\t\t<cac:TaxScheme>\n";
    string += "\t\t\t\t<cac:TaxTypeCode>IVA</cac:TaxTypeCode>\n";
    string += "\t\t\t</cac:TaxScheme>\n";
    string += "\t\t</cac:TaxCategory>\n";
    // PVP de un articulo
    string += "\t\t<cac:BasePrice>\n";
    string += "\t\t\t<cbc:PriceAmount amountCurrencyCodeListVersionID=\"0.3\" amountCurrencyID=\"EUR\">" + articulo->valor ( "pvparticulo" ) + "</cbc:PriceAmount>\n";
    string += "\t\t</cac:BasePrice>\n";
    string += "\t</cac:Item>\n";

    string += "</cac:InvoiceLine>\n";
    string += "\n"; // Dejamos sitio para otra linea

    delete articulo;

    _depura ( "END EFQToolButton::escribe_linea_factura", 0 );
}

/// ---------------- Generacion de factura a partir de una plantilla en formato UBL 1.0 ------------------ ///


///
/**
**/
void EFQToolButton::exporta_factura_ubl()
{

    _depura ( "EFQToolButton::exporta_factura_ubl", 0 );

    QString query = "";

    QFile *file_in  = new QFile ( _PLANTILLA_ );

    if ( !file_in->open ( QIODevice::ReadOnly | QIODevice::Text ) ) {
        _depura ( "Problemas al abir la plantilla de factura", 2 );
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

    if ( m_factura->DBvalue ( "idfactura" ).isEmpty() ) {
        _depura ( "ERROR: El campo idfactura del BlDbRecord esta vacio.", 2 );
        exit ( -1 );
    }

    if ( m_factura->DBvalue ( "idcliente" ).isEmpty() ) {
        _depura ( "ERROR: El campo idcliente del BlDbRecord esta vacio.", 2 );
        exit ( -1 );
    }

    if ( m_factura->DBvalue ( "idtrabajador" ).isEmpty() ) {
        // Esto no es un error grave. Este campo falta si la factura no viene desde un albaran.
        // Marcamos el error y evitamos hacer el query despues
        error_idtrabajador = true;

    }

    if ( m_factura->DBvalue ( "idforma_pago" ).isEmpty() ) {
        _depura ( "ERROR: El campo idforma_pago del BlDbRecord esta vacio.", 2 );
        exit ( -1 );
    }

    // El fichero de salida

    QString nombrearchivo = "";
    nombrearchivo += QString ( _RESULTADO_ );
    nombrearchivo += QString ( m_factura->DBvalue ( "idfactura" ) );
    nombrearchivo += ".xml";

    QFile *file_out = new QFile ( nombrearchivo );

    if ( !file_out->open ( QIODevice::WriteOnly | QIODevice::Text ) ) {
        _depura ( "Problemas al crear el archivo de salida", 2 );
        exit ( -1 );
    }

    // Datos de la factura que no estan en el BlDbRecord
    query = "SELECT totalfactura, bimpfactura, impfactura FROM factura WHERE idfactura = " + m_factura->DBvalue ( "idfactura" );
    BlDbRecordSet *factura_totales = empresaBase() ->cargacursor ( query );

    // Datos del cliente
    query = "SELECT * FROM cliente WHERE idcliente = " + m_factura->DBvalue ( "idcliente" );
    BlDbRecordSet *cliente = empresaBase() ->cargacursor ( query );

    // Datos del trabajador que emitio la factura

    BlDbRecordSet *trabajador = NULL;

    if ( !error_idtrabajador ) {
        query = "SELECT * FROM trabajador WHERE idtrabajador = " + m_factura->DBvalue ( "idtrabajador" );
        trabajador = empresaBase() ->cargacursor ( query );
    }

    // Datos de la forma de pago convenida
    query = "SELECT * FROM forma_pago WHERE idforma_pago = " + m_factura->DBvalue ( "idforma_pago" );
    BlDbRecordSet *forma_pago = empresaBase() ->cargacursor ( query );

    // Datos de la tabla configuracion

    bool error_configuracion = false;

    query = "SELECT * FROM configuracion WHERE nombre = 'NombreEmpresa'";
    BlDbRecordSet *nombre_empresa = empresaBase() ->cargacursor ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'CIF'";
    BlDbRecordSet *cif_empresa = empresaBase() ->cargacursor ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'DireccionCompleta'";
    BlDbRecordSet *dir_empresa = empresaBase() ->cargacursor ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'Ciudad'";
    BlDbRecordSet *ciudad_empresa = empresaBase() ->cargacursor ( query );

    query = "SELECT * FROM configuracion WHERE nombre = 'CodPostal'";
    BlDbRecordSet *cp_empresa = empresaBase() ->cargacursor ( query );

    if ( nombre_empresa->valor ( "valor" ).isEmpty() ) {
        _depura ( "El campo valor con nombre nombre_empresa de la tabla de configuracion esta vacio", 2 );
        error_configuracion = true;
    }

    if ( cif_empresa->valor ( "valor" ).isEmpty() ) {
        _depura ( "El campo valor con nombre cif_empresa de la tabla de configuracion esta vacio", 2 );
        error_configuracion = true;
    }

    if ( dir_empresa->valor ( "valor" ).isEmpty() ) {
        _depura ( "El campo valor con nombre dir_empresa de la tabla de configuracion esta vacio", 2 );
        error_configuracion = true;
    }

    if ( ciudad_empresa->valor ( "valor" ).isEmpty() ) {
        _depura ( "El campo valor con nombre ciudad_empresa de la tabla de configuracion esta vacio", 2 );
        error_configuracion = true;
    }

    if ( cp_empresa->valor ( "valor" ).isEmpty() ) {
        _depura ( "El campo valor con nombre cp_empresa de la tabla de configuracion esta vacio", 2 );
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

    FacturaXml.replace ( "[numfactura]", m_factura->DBvalue ( "numfactura" ) );
    FacturaXml.replace ( "[ffactura]", m_factura->DBvalue ( "ffactura" ) );
    FacturaXml.replace ( "[descfactura]", m_factura->DBvalue ( "descfactura" ) );
    FacturaXml.replace ( "[impfactura]", factura_totales->valor ( "impfactura" ) );
    FacturaXml.replace ( "[bimpfactura]", factura_totales->valor ( "bimpfactura" ) );
    FacturaXml.replace ( "[totalfactura]", factura_totales->valor ( "totalfactura" ) );

    FacturaXml.replace ( "[nomcliente]", cliente->valor ( "nomcliente" ) );
    FacturaXml.replace ( "[cifcliente]", cliente->valor ( "cifcliente" ) );
    FacturaXml.replace ( "[departamento]", cliente->valor ( "departamento" ) );
    FacturaXml.replace ( "[dircliente]", cliente->valor ( "dircliente" ) );
    FacturaXml.replace ( "[pobcliente]", cliente->valor ( "poblcliente" ) );
    FacturaXml.replace ( "[cpcliente]", cliente->valor ( "cpcliente" ) );

    FacturaXml.replace ( "[nombre_empresa]", nombre_empresa->valor ( "valor" ) );
    FacturaXml.replace ( "[cif_empresa]", cif_empresa->valor ( "valor" ) );
    FacturaXml.replace ( "[dir_empresa]", dir_empresa->valor ( "valor" ) );
    FacturaXml.replace ( "[ciudad_empresa]", ciudad_empresa->valor ( "valor" ) );
    FacturaXml.replace ( "[cp_empresa]", cp_empresa->valor ( "valor" ) );

    if ( trabajador != NULL )
        FacturaXml.replace ( "[trabajador]", trabajador->valor ( "nomtrabajador" ) + " " + trabajador->valor ( "apellidostrabajador" ) );
    else
        FacturaXml.replace ( "[trabajador]", "--" );

    FacturaXml.replace ( "[forma_de_pago]", forma_pago->valor ( "descforma_pago" ) );

    /// Obtenemos las lineas de factura y las escribimos en el buffer

    query = "SELECT * FROM lfactura WHERE idfactura = " + m_factura->DBvalue ( "idfactura" );
    BlDbRecordSet *lfacturas = empresaBase() ->cargacursor ( query );

    // Por si las moscas...
    lfacturas->primerregistro();

    QString LineasFactura = "\n";
    int numerolinea = 1;

    BlFixed totalFactura = "0.00";

    /// Escribimos la informacion sobre cada linea y de paso ya obtenemos el total
    /// de la factura sumando todos los PVPs de las lineas
    while ( !lfacturas->eof() ) {
        escribe_linea_factura ( LineasFactura, lfacturas, numerolinea );

        totalFactura = totalFactura + ( BlFixed ( lfacturas->valor ( "cantlfactura" ) ) * BlFixed ( lfacturas->valor ( "pvplfactura" ) ) );

        lfacturas->siguienteregistro();
        numerolinea++;
    }

    FacturaXml.replace ( "[lineas_factura]", LineasFactura );

    /// Descuento al PVP de la factura (cogidos de la tabla dfactura)
    query = "SELECT * FROM dfactura WHERE idfactura = " + m_factura->DBvalue ( "idfactura" );
    BlDbRecordSet *descuentos_factura = empresaBase() ->cargacursor ( query );

    QString DescuentosFactura = "\n";

    descuentos_factura->primerregistro();

    while ( !descuentos_factura->eof() ) {
        escribe_descuento_factura ( DescuentosFactura, descuentos_factura, totalFactura );
        descuentos_factura->siguienteregistro();
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
        _depura ( "ERROR: no se pudo abrir el archivo " + nombrearchivo + " para su parseo.", 2 );
        exit ( -1 );
    }

    QXmlSimpleReader *xmlReader = new QXmlSimpleReader();
    QXmlInputSource *source = new QXmlInputSource ( file );

    bool ok = xmlReader->parse ( source );

    if ( !ok )
        _depura ( "Problemas en el parseo del archivo", 2 );

    file->close();

    delete source;
    delete xmlReader;
    delete file;

    _depura ( "Exportacion completada. Su efactura se encuentra en " + nombrearchivo, 2 );

    _depura ( "END EFQToolButton::exporta_factura_ubl", 0 );
}


///
/**
**/
void EFQToolButton::click()
{

    _depura ( "EFQToolButton::click", 0 );

    if ( ( !m_factura->dialogChanges_hayCambios() ) && ( m_factura->DBvalue ( "idfactura" ) != "" ) ) {
        exporta_factura_ubl();
    } else {
        _depura ( "Es necesario Guardar la factura antes de exportarla a UBL", 2 );
    }

    _depura ( "END EFQToolButton::click", 0 );
}

