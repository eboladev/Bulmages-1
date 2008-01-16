/***************************************************************************
 *   Copyright (C) 2007 by Tomeu Borras Riera                              *
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

#include <QFile>
#include <QTextStream>

#include "fichabf.h"
#include "plugins.h"


class Fixed;


///
/**
\param comp
\param parent
\param f
\param modo
**/
FichaBf::FichaBf ( Company *comp, QWidget *parent, Qt::WFlags f, edmode modo )
        : Ficha ( comp, parent, f, modo )
{
    _depura ( "FichaBf::FichaBf", 0 );
    m_listalineas = NULL;
    m_listadescuentos = NULL;
    _depura ( "END FichaBf::FichaBf", 0 );
}


///
/**
\return
**/
FichaBf::~FichaBf()
{
    _depura ( "FichaBf::~FichaBf", 0, this->windowTitle() );
    empresaBase() ->sacaWindow ( this );
    _depura ( "END FichaBf::~FichaBf", 0 );
}


///
/**
\return
**/
Company * FichaBf::empresaBase()
{
    _depura ( "FichaBf::getcompany", 0 );
    _depura ( "END FichaBf::getcompany", 0 );
    return ( Company * ) Ficha::empresaBase();
}


/** Calcula los totales de factura, descuentos e impuestos y invoca al metodo de pintaTotales para
    que se pinten dichos valores en la pantalla.
*/
/**
\return
**/
void FichaBf::calculaypintatotales()
{
    _depura ( "FichaBf::calculaypintatotales", 0 );
    base basesimp;
    base basesimpreqeq;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    Fixed irpf ( "0" );

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = Fixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if

    if ( exists ( "idproveedor" ) && DBvalue ( "idproveedor" ) != "" ) {
        cur = empresaBase() ->cargacursor ( "SELECT irpfproveedor FROM proveedor WHERE idproveedor = " + DBvalue ( "idproveedor" ) );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = Fixed ( cur->valor ( "irpfproveedor" ) );
            } // end if
            delete cur;
        } // end if
    } // end if

    Fixed descuentolinea ( "0.00" );
    for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
        linea = m_listalineas->lineaat ( i );
        Fixed cant ( linea->DBvalue ( "cant" + m_listalineas->tableName() ).toAscii().constData() );
        Fixed pvpund ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        Fixed desc1 ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() );
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->DBvalue ( "iva" + m_listalineas->tableName() ) ] = basesimp[linea->DBvalue ( "iva" + m_listalineas->tableName() ) ] + base;
        basesimpreqeq[linea->DBvalue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->DBvalue ( "reqeq" + m_listalineas->tableName() ) ] + base;
    } // end for

    Fixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    Fixed porcentt ( "0.00" );
    SDBRecord *linea1;
    if ( m_listadescuentos->rowCount() ) {
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
            Fixed propor ( linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
            porcentt = porcentt + propor;
        } // end for
    } // end if

    /// Calculamos el total de base imponible.
    Fixed totbaseimp ( "0.00" );
    Fixed parbaseimp ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > Fixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    Fixed totiva ( "0.00" );
    Fixed pariva ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        Fixed piva ( it.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    Fixed totreqeq ( "0.00" );
    Fixed parreqeq ( "0.00" );
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        Fixed preqeq ( it.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        totreqeq = totreqeq + parreqeq;
    } // end for

    Fixed totirpf = totbaseimp * irpf / 100;
    pintatotales ( totiva, totbaseimp, totiva + totbaseimp + totreqeq - totirpf, ( basei * porcentt / 100 ) + descuentolinea, totirpf, totreqeq );
    _depura ( "END FichaBf::calculaypintatotales", 0 );
}


/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
void FichaBf::trataTags ( QString &buff )
{
    _depura ( "FichaBf::trataTags", 0 );
    int pos =  0;

    /// Buscamos algo de lineas de detalle
    QRegExp rx ( "<!--\\s*LINEAS\\s*DETALLE\\s*-->(.*)<!--\\s*END\\s*LINEAS\\s*DETALLE\\s*-->" );
    rx.setMinimal ( TRUE );
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataLineasDetalle ( rx.cap ( 1 ) );
        buff.replace ( pos, rx.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos Si hay descuentos en condicional
    pos = 0;
    QRegExp rx3 ( "<!--\\s*IF\\s*DESCUENTOS\\s*-->(.*)<!--\\s*END\\s*IF\\s*DESCUENTOS\\s*-->" );
    rx3.setMinimal ( TRUE );
    while ( ( pos = rx3.indexIn ( buff, pos ) ) != -1 ) {
        if ( m_listadescuentos->rowCount() - 1 <= 0 ) {
            buff.replace ( pos, rx3.matchedLength(), "" );
            pos = 0;
        } else {
            pos += rx3.matchedLength();
        } // end if
    } // end while

    /// Buscamos lineas de descuento
    pos = 0;
    QRegExp rx2 ( "<!--\\s*LINEAS\\s*DESCUENTO\\s*-->(.*)<!--\\s*END\\s*LINEAS\\s*DESCUENTO\\s*-->" );
    rx2.setMinimal ( TRUE );
    while ( ( pos = rx2.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataLineasDescuento ( rx2.cap ( 1 ) );
        buff.replace ( pos, rx2.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos lineas de totales
    pos = 0;
    QRegExp rx5 ( "<!--\\s*TOTALES\\s*-->(.*)<!--\\s*END\\s*TOTALES\\s*-->" );
    rx5.setMinimal ( TRUE );
    while ( ( pos = rx5.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataTotales ( rx5.cap ( 1 ) );
        buff.replace ( pos, rx5.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    Ficha::trataTags ( buff );

    _depura ( "END FichaBf::trataTags", 0 );
}





/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString FichaBf::trataLineasDetalle ( const QString &det )
{
    _depura ( "FichaBf::trataLineasDetalle", 0 );
    QString result = "";

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    SDBRecord *linea;
    /// Impresion de las lineas
    for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
        QString salidatemp = det;
        linea = m_listalineas->lineaat ( i );
        Fixed base = Fixed ( linea->DBvalue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * Fixed ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        QString l;

        /// Los saltos de carro se deben tratar de modo especial ya que RML no los contempla bien.
        QString desc = linea->DBvalue ( "desc" + m_listalineas->tableName() );
        QStringList descp = desc.split ( "\n" );
        QString desc1 = "";
        for ( int i = 0; i < descp.size(); ++i ) {
            if ( descp[i] != "" ) {
                if ( descp[i].startsWith ( " " ) ) {
                    desc1 += "<pre>" + XMLProtect ( descp[i] ) + "</pre>\n";
                } else {
                    desc1 += "<para>" + XMLProtect ( descp[i] ) + "</para>\n";
                } // end if
            } else {
                desc1 += "<spacer length=\"0.1cm\"/>\n";
            } // end if
        } // end for


        salidatemp.replace ( "[desc" + m_listalineas->tableName() + "]", desc1 );

        salidatemp.replace ( "[cant" + m_listalineas->tableName() + "]", l.sprintf ( "%s", linea->DBvalue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) );
        salidatemp.replace ( "[pvp" + m_listalineas->tableName() + "]", l.sprintf ( "%s", XMLProtect ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ) ).toAscii().constData() ) );
        salidatemp.replace ( "[descuento" + m_listalineas->tableName() + "]" , l.sprintf ( "%s", XMLProtect ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ) ).toAscii().constData() ) );
        salidatemp.replace ( "[total" + m_listalineas->tableName() + "]" , l.sprintf ( "%s", ( base - base * Fixed ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ) ) / 100 ).toQString().toAscii().constData() ) );

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( linea->exists ( rx.cap ( 1 ) ) ) {
                salidatemp.replace ( pos, rx.matchedLength(), linea->DBvalue ( rx.cap ( 1 ) ) );
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
    } // end for
    _depura ( "END FichaBf::trataLineasDetalle", 0 );
    return result;
}


/// Trata las lineas de descuento encontradas dentro de los tags <!--LINEAS DESCUENTO-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString FichaBf::trataLineasDescuento ( const QString &det )
{
    _depura ( "FichaBf::trataLineasDescuento", 0 );
    QString result = "";

    base basesimp;
    base basesimpreqeq;

    /// Calculamos la base imponible
    SDBRecord *linea;
    /// Impresion de las lineas
    for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
        linea = m_listalineas->lineaat ( i );
        Fixed base = Fixed ( linea->DBvalue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * Fixed ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        basesimp[linea->DBvalue ( "iva"+m_listalineas->tableName() ) ] = basesimp[linea->DBvalue ( "iva"+m_listalineas->tableName() ) ] + base - base * Fixed ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;
        basesimpreqeq[linea->DBvalue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->DBvalue ( "reqeq"+m_listalineas->tableName() ) ] + base - base * Fixed ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;

    } // end for

    Fixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    SDBRecord *linea1;
    /// Impresion de las lineas
    for ( int i = 0; i < ( m_listadescuentos->rowCount() - 1 ); ++i ) {
        QString salidatemp = det;
        linea1 = m_listadescuentos->lineaat ( i );
        QString l;
        salidatemp.replace ( "[concept" + m_listadescuentos->tableName() + "]", XMLProtect ( linea1->DBvalue ( "concept" + m_listadescuentos->tableName() ) ) );
        salidatemp.replace ( "[proporciondesc" + m_listadescuentos->tableName() + "]", l.sprintf ( "%s", linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() ) );
        salidatemp.replace ( "[totaldesc" + m_listadescuentos->tableName() + "]", l.sprintf ( "-%s", ( Fixed ( linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ) ) * basei / 100 ).toQString().toAscii().constData() ) );

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( linea1->exists ( rx.cap ( 1 ) ) ) {
                salidatemp.replace ( pos, rx.matchedLength(), linea1->DBvalue ( rx.cap ( 1 ) ) );
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
    } // end for
    _depura ( "END FichaBf::trataLineasDescuento", 0 );
    return result;
}


/** Calcula los totales de factura, descuentos e impuestos y invoca al metodo de pintaTotales para
    que se pinten dichos valores en la impresion.
*/
/**
\return
**/
QString FichaBf::trataTotales ( const QString &det )
{
    _depura ( "FichaBf::trataTotales", 0 );
    QString result = "";
    QString salidatemp = "";
    base basesimp;
    base basesimpreqeq;
    SDBRecord *linea;
    /// Impresion de los contenidos.
    QString l;
    Fixed irpf ( "0" );

    cursor2 *cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = Fixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if

    if ( exists ( "idproveedor" ) && DBvalue ( "idproveedor" ) != "" ) {
        cur = empresaBase() ->cargacursor ( "SELECT irpfproveedor FROM proveedor WHERE idproveedor = " + DBvalue ( "idproveedor" ) );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = Fixed ( cur->valor ( "irpfproveedor" ) );
            } // end if
            delete cur;
        } // end if
    } // end if

    Fixed descuentolinea ( "0.00" );
    for ( int i = 0; i < m_listalineas->rowCount() - 1; ++i ) {
        linea = m_listalineas->lineaat ( i );
        Fixed cant ( linea->DBvalue ( "cant" + m_listalineas->tableName() ).toAscii().constData() );
        Fixed pvpund ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        Fixed desc1 ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() );
        Fixed cantpvp = cant * pvpund;
        Fixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->DBvalue ( "iva" + m_listalineas->tableName() ) ] = basesimp[linea->DBvalue ( "iva" + m_listalineas->tableName() ) ] + base;
        basesimpreqeq[linea->DBvalue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->DBvalue ( "reqeq" + m_listalineas->tableName() ) ] + base;
    } // end for

    Fixed basei ( "0.00" );
    base::Iterator it;
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    Fixed porcentt ( "0.00" );
    SDBRecord *linea1;
    if ( m_listadescuentos->rowCount() ) {
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
            Fixed propor ( linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
            porcentt = porcentt + propor;
        } // end for
    } // end if

    /// Calculamos el total de base imponible.
    Fixed totbaseimp ( "0.00" );
    Fixed parbaseimp ( "0.00" );
    Fixed totdesc ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        if ( porcentt > Fixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
            totdesc = totdesc + it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        totbaseimp = totbaseimp + parbaseimp;
    } // end for

    /// Calculamos el total de IVA.
    Fixed totiva ( "0.00" );
    Fixed pariva ( "0.00" );
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        Fixed piva ( it.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        totiva = totiva + pariva;
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    Fixed totreqeq ( "0.00" );
    Fixed parreqeq ( "0.00" );
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        Fixed preqeq ( it.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        totreqeq = totreqeq + parreqeq;
    } // end for

    Fixed totirpf = totbaseimp * irpf / 100;

    base::Iterator ot;
    base::Iterator at;
    at = basesimpreqeq.begin();
    for ( ot = basesimp.begin(); ot != basesimp.end(); ++ot ) {
        salidatemp = det;
        if ( porcentt > Fixed ( "0.00" ) ) {
            parbaseimp = ot.value() - ot.value() * porcentt / 100;
        } else {
            parbaseimp = ot.value();
        } // end if
        salidatemp.replace ( "[bimp]", parbaseimp.toQString() );
        salidatemp.replace ( "[tbimp]", ot.key() );

        Fixed piva ( ot.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            pariva = ( ot.value() - ot.value() * porcentt / 100 ) * piva / 100;
        } else {
            pariva = ot.value() * piva / 100;
        } // end if
        salidatemp.replace ( "[iva]", pariva.toQString() );
        salidatemp.replace ( "[tiva]", ot.key() );

        // Recargos de equivalencia
        Fixed preqeq ( at.key().toAscii().constData() );
        if ( porcentt > Fixed ( "0.00" ) ) {
            parreqeq = ( at.value() - at.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = at.value() * preqeq / 100;
        } // end if
        salidatemp.replace ( "[re]", parreqeq.toQString() );
        salidatemp.replace ( "[tre]", at.key() );

        salidatemp.replace ( "[irpf]", totirpf.toQString() );
        salidatemp.replace ( "[tirpf]", irpf.toQString() );

        salidatemp.replace ( "[totalre]", totreqeq.toQString() );
        salidatemp.replace ( "[teoricbimp]", basei.toQString() );
        salidatemp.replace ( "[totalbimp]", totbaseimp.toQString() );
        salidatemp.replace ( "[totaldesc]", totdesc.toQString() );
        salidatemp.replace ( "[totaliva]", totiva.toQString() );
        salidatemp.replace ( "[total]", ( totiva + totbaseimp + totreqeq - totirpf ).toQString().toAscii().constData() );

        result += salidatemp;
        ++at;
    } // end for

    _depura ( "END FichaBf::trataTotales", 0 );
    return result;
}



///
/**
**/
void FichaBf::generaRML()
{
    _depura ( "FichaBf::generaRML", 0 );
    generaRML ( m_tablename + ".rml" );
    _depura ( "END FichaBf::generaRML", 0 );

}

///
/**
\param arch archivo a generar
**/
void FichaBf::generaRML ( const QString &arch )
{
    _depura ( "FichaBf::generaRML", 0, arch );
    cursor2 *cur = NULL;
    try {
        /// Disparamos los plugins
        int res = g_plugins->lanza ( "FichaBf_generaRML", this );
        if ( res != 0 ) {
            return;
        } // end if
        base basesimp;
        base basesimpreqeq;
        QString archivo = confpr->valor ( CONF_DIR_OPENREPORTS ) + arch;
        QString archivod = confpr->valor ( CONF_DIR_USER ) + arch;
        QString archivologo = confpr->valor ( CONF_DIR_OPENREPORTS ) + "logo.jpg";

        Fixed irpf ( "0" );

        cur = empresaBase() ->cargacursor ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = Fixed ( cur->valor ( "valor" ) );
            } // end if
            delete cur;
        } // end if

        /// Copiamos el archivo.
#ifdef WINDOWS

        archivo = "copy " + archivo + " " + archivod;
#else

        archivo = "cp " + archivo + " " + archivod;
#endif

        system ( archivo.toAscii().constData() );
        /// Copiamos el logo
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

        /// Hacemos el tratamiento avanzado de TAGS
        trataTags ( buff );

        QString fitxersortidatxt = "";



        /// Impresion del cliente
        QString SQLQuery = "SELECT * FROM cliente WHERE idcliente = " + DBvalue ( "idcliente" );
        cur = empresaBase() ->cargacursor ( SQLQuery );
        if ( !cur ) throw - 1;
        if ( !cur->eof() ) {
            buff.replace ( "[dircliente]", cur->valor ( "dircliente" ) );
            buff.replace ( "[poblcliente]", cur->valor ( "poblcliente" ) );
            buff.replace ( "[telcliente]", cur->valor ( "telcliente" ) );
            buff.replace ( "[nomcliente]", cur->valor ( "nomcliente" ) );
            buff.replace ( "[cifcliente]", cur->valor ( "cifcliente" ) );
            buff.replace ( "[idcliente]", cur->valor ( "idcliente" ) );
            buff.replace ( "[cpcliente]", cur->valor ( "cpcliente" ) );
            buff.replace ( "[codcliente]", cur->valor ( "codcliente" ) );
        } // end if
        delete cur;

        if ( exists ( "id" + m_tablename ) )
            buff.replace ( "[id" + m_tablename + "]", DBvalue ( "id" + m_tablename ) );
        if ( exists ( "num" + m_tablename ) )
            buff.replace ( "[num" + m_tablename + "]", DBvalue ( "num" + m_tablename ) );
        if ( exists ( "f" + m_tablename ) )
            buff.replace ( "[f" + m_tablename + "]", DBvalue ( "f" + m_tablename ) );
        if ( exists ( "venc" + m_tablename ) )
            buff.replace ( "[venc" + m_tablename + "]", DBvalue ( "venc" + m_tablename ) );
        if ( exists ( "contact" + m_tablename ) )
            buff.replace ( "[contact" + m_tablename + "]", DBvalue ( "contact" + m_tablename ) );
        if ( exists ( "tel" + m_tablename ) )
            buff.replace ( "[tel" + m_tablename + "]", DBvalue ( "tel" + m_tablename ) );
        if ( exists ( "coment" + m_tablename ) )
            buff.replace ( "[coment" + m_tablename + "]", DBvalue ( "coment" + m_tablename ) );
        if ( exists ( "desc" + m_tablename ) )
            buff.replace ( "[desc" + m_tablename + "]", DBvalue ( "desc" + m_tablename ) );
        if ( exists ( "ref" + m_tablename ) )
            buff.replace ( "[ref" + m_tablename + "]", DBvalue ( "ref" + m_tablename ) );
        if ( exists ( "codigoserie_" + m_tablename ) )
            buff.replace ( "[codigoserie_" + m_tablename + "]", DBvalue ( "codigoserie_" + m_tablename ) );
        if ( exists ( "fecha" + m_tablename ) )
            buff.replace ( "[fecha" + m_tablename + "]", DBvalue ( "fecha" + m_tablename ) );
        /// Impresion de la tabla de contenidos.
        QString l;

        /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
        SDBRecord *linea;
        /// Impresion de las lineas
        for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
            linea = m_listalineas->lineaat ( i );
            Fixed base = Fixed ( linea->DBvalue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * Fixed ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
            basesimp[linea->DBvalue ( "iva"+m_listalineas->tableName() ) ] = basesimp[linea->DBvalue ( "iva"+m_listalineas->tableName() ) ] + base - base * Fixed ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;
            basesimpreqeq[linea->DBvalue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->DBvalue ( "reqeq"+m_listalineas->tableName() ) ] + base - base * Fixed ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;

            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "    <td>" + XMLProtect ( linea->DBvalue ( "codigocompletoarticulo" ) ) + "</td>\n";
            fitxersortidatxt += "    <td><para>" + XMLProtect ( linea->DBvalue ( "desc" + m_listalineas->tableName() ) ).replace ( QChar ( '\n' ), "</para><para>" ) + "</para></td>\n";
            fitxersortidatxt += "    <td>" + linea->DBvalue ( "cant" + m_listalineas->tableName() ) + "</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf ( "%s", XMLProtect ( linea->DBvalue ( "pvp" + m_listalineas->tableName() ) ).toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf ( "%s", XMLProtect ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ) ).toAscii().constData() ) + " %</td>\n";
            fitxersortidatxt += "    <td>" + l.sprintf ( "%s", ( base - base * Fixed ( linea->DBvalue ( "descuento" + m_listalineas->tableName() ) ) / 100 ).toQString().toAscii().constData() ) + "</td>\n";
            fitxersortidatxt += "</tr>";
        } // end for

        buff.replace ( "[story]", fitxersortidatxt );

        Fixed basei ( "0.00" );
        base::Iterator it;
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            basei = basei + it.value();
        } // end for

        /// Impresion de los descuentos.
        fitxersortidatxt = "";
        Fixed porcentt ( "0.00" );
        SDBRecord *linea1;
        if ( m_listadescuentos->rowCount() - 1 ) {
            fitxersortidatxt += "<blockTable style=\"tabladescuento\">\n";
            fitxersortidatxt += "<tr>\n";
            fitxersortidatxt += "    <td>" + QApplication::translate ( "Presupuesto", "Descuento" ) + "</td>\n";
            fitxersortidatxt += "    <td>" + QApplication::translate ( "Presupuesto", "Porcentaje" ) + "</td>\n";
            fitxersortidatxt += "    <td>" + QApplication::translate ( "Presupuesto", "Total Desc." ) + "</td>\n";
            fitxersortidatxt += "</tr>\n";
            for ( int i = 0; i < ( m_listadescuentos->rowCount() - 1 ); ++i ) {
                linea1 = m_listadescuentos->lineaat ( i );
                porcentt = porcentt + Fixed ( linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
                fitxersortidatxt += "<tr>\n";
                fitxersortidatxt += "    <td>" + XMLProtect ( linea1->DBvalue ( "concept" + m_listadescuentos->tableName() ) ) + "</td>\n";
                fitxersortidatxt += "    <td>" + l.sprintf ( "%s", linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() ) + " %</td>\n";
                fitxersortidatxt += "    <td>" + l.sprintf ( "-%s", ( Fixed ( linea1->DBvalue ( "proporcion" + m_listadescuentos->tableName() ) ) * basei / 100 ).toQString().toAscii().constData() ) + "</td>\n";
                fitxersortidatxt += "</tr>";
            } // end for
            fitxersortidatxt += "</blockTable>\n";
        } // end if
        buff.replace ( "[descuentos]", fitxersortidatxt );

        /// Impresion de las bases imponibles.
        fitxersortidatxt = "";
        QString tr1 = ""; /// Rellena el primer tr de titulares.
        QString tr2 = ""; /// Rellena el segundo tr de cantidades.
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" >\n";
        Fixed totbaseimp ( "0.00" );
        Fixed parbaseimp ( "0.00" );
        Fixed totdesc ( "0.00" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > 0 ) {
                parbaseimp = it.value() - it.value() * porcentt / 100;
                totdesc = totdesc + it.value() * porcentt / 100;
            } else {
                parbaseimp = it.value();
            } // end if
            totbaseimp = totbaseimp + parbaseimp;
            tr1 += "    <td>" + QApplication::translate ( "Presupuesto", "Base Imponible" ) + " " + XMLProtect ( it.key() ) + " %</td>\n";
            tr2 += "    <td>" + l.sprintf ( " %s ", parbaseimp.toQString().toAscii().constData() ) + "</td>\n";
        } // end for

        /// Impresion de los IVAS.
        Fixed totiva ( "0.0" );
        Fixed pariva ( "0.0" );
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            if ( porcentt > 0 ) {
                pariva = ( it.value() - it.value() * porcentt / 100 ) * Fixed ( it.key() ) / 100;
            } else {
                pariva = it.value() * Fixed ( it.key() ) / 100;
            } // end if
            totiva = totiva + pariva;
            tr1 += "    <td>" + QApplication::translate ( "Presupuesto", "I.V.A." ) + " " + XMLProtect ( it.key() ) + " %</td>\n";
            tr2 += "    <td>" + l.sprintf ( " %s ", pariva.toQString().toAscii().constData() ) + "</td>\n";
        } // end for

        /// Impresion de los Recargos de Equivalencia.
        Fixed totreqeq ( "0.0" );
        Fixed parreqeq ( "0.0" );
        for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
            if ( porcentt > 0 ) {
                parreqeq = ( it.value() - it.value() * porcentt / 100 ) * Fixed ( it.key() ) / 100;
            } else {
                parreqeq = it.value() * Fixed ( it.key() ) / 100;
            } // end if
            totreqeq = totreqeq + parreqeq;
            if ( parreqeq > 0 ) {
                tr1 += "    <td>" + QApplication::translate ( "Presupuesto", "R.E." ) + " " + XMLProtect ( it.key() ) + " %</td>\n";
                tr2 += "    <td>" + l.sprintf ( " %s ", parreqeq.toQString().toAscii().constData() ) + "</td>\n";
            } // end if
        } // end for

        Fixed totirpf = totbaseimp * irpf / 100;
        if ( totirpf > 0 ) {
            tr1 += "    <td>" + QApplication::translate ( "Presupuesto", "I.R.P.F (-" ) + " " + XMLProtect ( irpf.toQString() ) + ") %</td>\n";
            tr2 += "    <td>" + l.sprintf ( " %s ", totirpf.toQString().toAscii().constData() ) + "</td>\n";
        } // end if

        tr1 += "    <td>" + QApplication::translate ( "Presupuesto", "Total" ) + "</td>\n";
        tr2 += "    <td>" + l.sprintf ( " %s ", ( totiva + totbaseimp + totreqeq - totirpf ).toQString().toAscii().constData() ) + "</td>\n";
        fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
        buff.replace ( "[totales]", fitxersortidatxt );



        buff.replace ( "[irpf]", totirpf.toQString() );
        buff.replace ( "[tirpf]", irpf.toQString() );
        buff.replace ( "[totalre]", totreqeq.toQString() );
        buff.replace ( "[teoricbimp]", basei.toQString() );
        buff.replace ( "[totalbimp]", totbaseimp.toQString() );
        buff.replace ( "[totaldesc]", totdesc.toQString() );
        buff.replace ( "[totaliva]", totiva.toQString() );
        buff.replace ( "[total]", ( totiva + totbaseimp + totreqeq - totirpf ).toQString().toAscii().constData() );


        /// En la version para windows hay problemas con las imagenes,
        /// por eso de momento lo dejamos asi.
#ifndef WINDOWS
        //   buff.replace("[detallearticulos]", detalleArticulos());
#endif

        if ( file.open ( QIODevice::WriteOnly ) ) {
            QTextStream stream ( &file );
            stream << buff;
            file.close();
        } // end if

        _depura ( "END FichaBf::generaRML", 0 );
    } catch ( ... ) {
        if ( cur ) delete cur;
        throw - 1;
    } // end try
}


///
/**
\return
**/
void FichaBf::imprimir()
{
    _depura ( "FichaBf::imprimir", 0 );
    try {
        /// Disparamos los plugins
        int res = g_plugins->lanza ( "FichaBf_imprimir", this );
        if ( res != 0 ) {
            return;
        } // end if
        generaRML();
        invocaPDF ( m_tablename );
        _depura ( "END FichaBf::imprimir", 0 );
    } catch ( ... ) {
        mensajeInfo ( tr ( "Error inesperado en la impresion" ) );
    } // end try
}


///
/**
\param form
**/
void FichaBf::setListaLineas ( SubForm2Bf *form )
{
    _depura ( "FichaBf::setListaLineas", 0 );
    m_listalineas = form;
    _depura ( "END FichaBf::setListaLineas", 0 );
}

///
/**
\param form
**/
void FichaBf::setListaDescuentos ( SubForm2Bf *form )
{
    _depura ( "FichaBf::setListaDescuentos", 0 );
    m_listadescuentos = form;
    _depura ( "END FichaBf::setListaDescuentos", 0 );
}


///
/**
\return
**/
SubForm2Bf* FichaBf::getlistalineas()
{
    _depura ( "FichaBf::getlistalineas", 0 );
    _depura ( "END FichaBf::getlistalineas", 0 );
    return m_listalineas;
}


///
/**
\return
**/
SubForm2Bf* FichaBf::getlistadescuentos()
{
    _depura ( "FichaBf::getlistadescuentos", 0 );
    _depura ( "END FichaBf::getlistadescuentos", 0 );
    return m_listadescuentos;
}


