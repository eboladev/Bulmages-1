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
#include <QRegExp>

#include "bfform.h"
#include "blplugins.h"

class BlFixed;

///
/**
\param comp
\param parent
\param f
\param modo
**/
BfForm::BfForm ( BfCompany *comp, QWidget *parent, Qt::WFlags f, edmode modo )
        : BlForm ( comp, parent, f, modo )
{
    _depura ( "BfForm::BfForm", 0 );
    
    m_listalineas = NULL;
    m_listadescuentos = NULL;
    
    _depura ( "END BfForm::BfForm", 0 );
}

///
/**
\return
**/
BfForm::~BfForm()
{
    _depura ( "BfForm::~BfForm", 0, this->windowTitle() );
    
    mainCompany() ->sacaWindow ( this );
    
    _depura ( "END BfForm::~BfForm", 0 );
}

///
/**
\return
**/
BfCompany * BfForm::mainCompany()
{
    _depura ( "BfForm::company", 0 );
    _depura ( "END BfForm::company", 0 );
    
    return ( BfCompany * ) BlForm::mainCompany();
}


/** Calcula los totales de los documentos teniendo en cuenta los descuentos e impuestos.
    Invoca al metodo de pintaTotales para que se pinten dichos valores en la pantalla.
*/
/**
\return
**/
void BfForm::calculaypintatotales()
{
    _depura ( "BfForm::calculaypintatotales", 0 );
    
    base basesimp;
    base basesimpreqeq;
    BlDbSubFormRecord *linea;
    
    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = BlFixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if

    if ( exists ( "idproveedor" ) && dbValue ( "idproveedor" ) != "" ) {
        cur = mainCompany() ->loadQuery ( "SELECT irpfproveedor FROM proveedor WHERE idproveedor = " + dbValue ( "idproveedor" ) );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = BlFixed ( cur->valor ( "irpfproveedor" ) );
            } // end if
            delete cur;
        } // end if
    } // end if

    BlFixed descuentolinea ( "0.00" );
    
    for ( int i = 0; i < m_listalineas->rowCount(); ++i ) {
        linea = m_listalineas->lineaat ( i );
        BlFixed cant ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() );
        BlFixed pvpund ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        BlFixed desc1 ( linea->dbValue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() );
        BlFixed cantpvp = cant * pvpund;
        BlFixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->dbValue ( "iva" + m_listalineas->tableName() ) ] = basesimp[linea->dbValue ( "iva" + m_listalineas->tableName() ) ] + base;
        basesimpreqeq[linea->dbValue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->dbValue ( "reqeq" + m_listalineas->tableName() ) ] + base;
    } // end for

    BlFixed basei ( "0.00" );
    base::Iterator it;
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    BlFixed porcentt ( "0.00" );
    BlDbSubFormRecord *linea1;
    
    if ( m_listadescuentos->rowCount() ) {
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
            BlFixed propor ( linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
            porcentt = porcentt + propor;
        } // end for
    } // end if

    /// Calculamos el total de base imponible.
    BlFixed totbaseimp ( "0.00" );
    BlFixed parbaseimp ( "0.00" );
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
    
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        
        totbaseimp = totbaseimp + parbaseimp;
        
    } // end for

    /// Calculamos el total de IVA.
    BlFixed totiva ( "0.00" );
    BlFixed pariva ( "0.00" );
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
    
        BlFixed piva ( it.key().toAscii().constData() );
        
        if ( porcentt > BlFixed ( "0.00" ) ) {
            QString evpariva = "( 1 - " + porcentt.toQString() + " / 100 ) * " + it.value().toQString() + " * " + piva.toQString() + " / 100";
            QString tot = mainCompany()->PGEval ( evpariva );
            pariva = BlFixed ( tot );
//            pariva = ( it.value() - it.value() * porcentt / 100 ) * piva / 100;
        } else {
            QString evpariva = it.value().toQString() + " * " + piva.toQString() + " / 100";
            QString tot = mainCompany()->PGEval ( evpariva );
            pariva = BlFixed ( tot );
        } // end if
        
        totiva = totiva + pariva;
        
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    BlFixed totreqeq ( "0.00" );
    BlFixed parreqeq ( "0.00" );
    
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
    
        BlFixed preqeq ( it.key().toAscii().constData() );
        
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        
        totreqeq = totreqeq + parreqeq;
        
    } // end for

    BlFixed totirpf = totbaseimp * irpf / 100;
    pintatotales ( totiva, totbaseimp, totiva + totbaseimp + totreqeq - totirpf, ( basei * porcentt / 100 ) + descuentolinea, totirpf, totreqeq );
    
    _depura ( "END BfForm::calculaypintatotales", 0 );
}

/// Busca strings del tipo [xxxx] entro del texto pasado y los sustituye
/// Por valores existentes en la base de datos.
/**
\param buff El texto entero sobre el que se hace el reemplazo de sentencias.
**/
int BfForm::trataTags ( QString &buff, int tipoEscape )
{
    _depura ( "BfForm::trataTags", 0 );
    
    int pos =  0;

    /// Buscamos algo de lineas de detalle
    QRegExp rx ( "<!--\\s*LINEAS\\s*DETALLE\\s*-->(.*)<!--\\s*END\\s*LINEAS\\s*DETALLE\\s*-->" );
    rx.setMinimal ( TRUE );
    
    while ( ( pos = rx.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataLineasDetalle ( rx.cap ( 1 ), tipoEscape );
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
        QString ldetalle = trataLineasDescuento ( rx2.cap ( 1 ), tipoEscape );
        buff.replace ( pos, rx2.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    /// Buscamos lineas de totales de Bases Imponibles e IVAS.
    pos = 0;
    QRegExp rx5 ( "<!--\\s*TOTALES\\s*-->(.*)<!--\\s*END\\s*TOTALES\\s*-->" );
    rx5.setMinimal ( TRUE );
    
    while ( ( pos = rx5.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataTotales ( rx5.cap ( 1 ), 1 );
        buff.replace ( pos, rx5.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    pos = 0;
    QRegExp rx6 ( "<!--\\s*TOTREQ\\s*-->(.*)<!--\\s*END\\s*TOTREQ\\s*-->" );
    rx6.setMinimal ( TRUE );
    
    while ( ( pos = rx6.indexIn ( buff, pos ) ) != -1 ) {
        QString ldetalle = trataTotales ( rx6.cap ( 1 ), 2 );
        buff.replace ( pos, rx6.matchedLength(), ldetalle );
        pos = 0;
    } // end while

    int ret = BlForm::trataTags ( buff, tipoEscape );

    trataTagsBf ( buff, tipoEscape );
    
    _depura ( "END BfForm::trataTags", 0 );
    
    return ret;
}

void BfForm::trataTagsBf ( QString &buff, int tipoEscape )
{
    _depura ( "BfForm::trataTagsBf", 0 );

    QString fitxersortidatxt = "";
    BlDbRecordSet *cur = NULL;
    
    try {
    
        base basesimp;
        base basesimpreqeq;

        BlFixed irpf ( "0" );
        
        cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
        
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = BlFixed ( cur->valor ( "valor" ) );
            } // end if
            delete cur;
        } // end if

        if ( exists ( "id" + m_tablename ) )
            buff.replace ( "[id" + m_tablename + "]", genEscape ( dbValue ( "id" + m_tablename ), tipoEscape ) );
        if ( exists ( "num" + m_tablename ) )
            buff.replace ( "[num" + m_tablename + "]", genEscape ( dbValue ( "num" + m_tablename ), tipoEscape ) );
        if ( exists ( "f" + m_tablename ) )
            buff.replace ( "[f" + m_tablename + "]", genEscape ( dbValue ( "f" + m_tablename ), tipoEscape ) );
        if ( exists ( "venc" + m_tablename ) )
            buff.replace ( "[venc" + m_tablename + "]", genEscape ( dbValue ( "venc" + m_tablename ), tipoEscape ) );
        if ( exists ( "contact" + m_tablename ) )
            buff.replace ( "[contact" + m_tablename + "]", genEscape ( dbValue ( "contact" + m_tablename ), tipoEscape ) );
        if ( exists ( "tel" + m_tablename ) )
            buff.replace ( "[tel" + m_tablename + "]", genEscape ( dbValue ( "tel" + m_tablename ), tipoEscape ) );
        if ( exists ( "coment" + m_tablename ) )
            buff.replace ( "[coment" + m_tablename + "]", genEscape ( dbValue ( "coment" + m_tablename ), tipoEscape ) );
        if ( exists ( "desc" + m_tablename ) )
            buff.replace ( "[desc" + m_tablename + "]", genEscape ( dbValue ( "desc" + m_tablename ), tipoEscape ) );
        if ( exists ( "ref" + m_tablename ) )
            buff.replace ( "[ref" + m_tablename + "]", genEscape ( dbValue ( "ref" + m_tablename ), tipoEscape ) );
        if ( exists ( "codigoserie_" + m_tablename ) )
            buff.replace ( "[codigoserie_" + m_tablename + "]", genEscape ( dbValue ( "codigoserie_" + m_tablename ), tipoEscape ) );
        if ( exists ( "fecha" + m_tablename ) )
            buff.replace ( "[fecha" + m_tablename + "]", genEscape ( dbValue ( "fecha" + m_tablename ), tipoEscape ) );

        /// Impresion de la tabla de contenidos.
        QString l;

        /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
        BlDbSubFormRecord *linea;

        /// Si no hay lista de lineas salimos.
        if (!m_listalineas) {
            _depura ( "END BfForm::trataTagsBf", 0 );
            return;
        } // end if

        /// Expresion regular para saber los parametros de STORY.
        QRegExp rx("\\[story(.*)\\]");
        rx.setMinimal(true);
        rx.setCaseSensitivity(Qt::CaseInsensitive);

        /// Sustituye todas las ocurrencias de STORY.
        int pos = 0;
        
        while (( pos = rx.indexIn ( buff, pos )) != -1 ) {

            /// Extrae los parametros de STORY.
            QStringList parametros_story = rx.cap(1).toLower().split(",");

            /// Si STORY tiene parametros se crean las columnas en el orden indicado.
            /// Si no tiene parametros se escriben unas columnas predeterminadas.
            /// parametros_story siempre tiene, al menos, 1 elemento aunque no se hayan
            /// especificado parametros. Por eso tenemos que hacer unas pocas comprobaciones mas.
            if ((parametros_story.count() >= 1) && (parametros_story[0].trimmed() != "")) {

                /// Recorre las lineas del documento
                for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {

                    linea = m_listalineas->lineaat ( i );
                    BlFixed base = BlFixed ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
                    fitxersortidatxt += "<tr>\n";
                    
                    /// Recorre todos los parametros
                    for (int j = 0; j < parametros_story.count(); ++j ) {
            
                        if ( parametros_story[j].trimmed() == "codigocompletoarticulo" ){
                            ///Impresion del codigo de los articulos
                            fitxersortidatxt += "    <td>" + genEscape ( linea->dbValue ( "codigocompletoarticulo" ), tipoEscape ) + "</td>\n";
                        } // end if
            
                        if ( parametros_story[j].trimmed() == "desc" + m_tablename ){
                            ///Impresion del descripcion de los articulos
                            fitxersortidatxt += "    <td><para style=\"paragrafo\">" + genEscape ( linea->dbValue ( "desc" + m_listalineas->tableName() ), tipoEscape ).replace ( QChar ( '\n' ), "<br/>" ) + "</para></td>\n";
                        } // end if
            
                        if ( parametros_story[j].trimmed() == "cant" + m_tablename ){
                            ///Impresion de las candidad de los articulos
                            fitxersortidatxt += "    <td>" + linea->dbValue ( "cant" + m_listalineas->tableName() ) + "</td>\n";
                        } // end if
            
                        if ( parametros_story[j].trimmed() == "pvp" + m_tablename ){
                            ///Impresion del precio de los articulos
                            fitxersortidatxt += "    <td>" + l.sprintf ( "%s", genEscape ( linea->dbValue ( "pvp" + m_listalineas->tableName() ), tipoEscape ).toAscii().constData() ) + "</td>\n";
                        } // end if
            
                        if ( parametros_story[j].trimmed() == "descuento" + m_tablename ){
                            ///Impresion del subtotal de los articulos
                            fitxersortidatxt += "    <td>" + l.sprintf ( "%s", genEscape ( linea->dbValue ( "descuento" + m_listalineas->tableName() ), tipoEscape ).toAscii().constData() ) + " %</td>\n";
                        } // end if

                        if ( parametros_story[j].trimmed() == "subtotal" + m_tablename ){
                            ///Impresion del subtotal de los articulos
                            fitxersortidatxt += "    <td>" + l.sprintf ( "%s", ( base - base * BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ) ) / 100 ).toQString().toAscii().constData() ) + "</td>\n";
                        } // end if
            
                    } //end for
            
                    fitxersortidatxt += "</tr>\n";

                } // end for

                buff.replace ( pos, rx.matchedLength(), fitxersortidatxt );
                pos += rx.matchedLength();

            } else {

                /// Impresion de las lineas
                for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
                    linea = m_listalineas->lineaat ( i );
                    BlFixed base = BlFixed ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
                    ///Impresion de los contenidos
                    fitxersortidatxt += "<tr>";
                    fitxersortidatxt += "    <td>" + genEscape ( linea->dbValue ( "codigocompletoarticulo" ), tipoEscape ) + "</td>\n";
                    fitxersortidatxt += "    <td><para style=\"paragrafo\">" + genEscape ( linea->dbValue ( "desc" + m_listalineas->tableName() ), tipoEscape ).replace ( QChar ( '\n' ), "<br/>" ) + "</para></td>\n";
                    fitxersortidatxt += "    <td>" + linea->dbValue ( "cant" + m_listalineas->tableName() ) + "</td>\n";
                    fitxersortidatxt += "    <td>" + l.sprintf ( "%s", genEscape ( linea->dbValue ( "pvp" + m_listalineas->tableName() ), tipoEscape ).toAscii().constData() ) + "</td>\n";
                    fitxersortidatxt += "    <td>" + l.sprintf ( "%s", genEscape ( linea->dbValue ( "descuento" + m_listalineas->tableName() ), tipoEscape ).toAscii().constData() ) + " %</td>\n";
                    fitxersortidatxt += "    <td>" + l.sprintf ( "%s", ( base - base * BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ) ) / 100 ).toQString().toAscii().constData() ) + "</td>\n";
                    fitxersortidatxt += "</tr>";
                } // end for
            
                buff.replace ( "[story]", fitxersortidatxt );

            } // end if

        } // end while

        /// calculamos las bases
        for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
            linea = m_listalineas->lineaat ( i );
            BlFixed base = BlFixed ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
            basesimp[linea->dbValue ( "iva"+m_listalineas->tableName() ) ] = basesimp[linea->dbValue ( "iva"+m_listalineas->tableName() ) ] + base - base * BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;
            basesimpreqeq[linea->dbValue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->dbValue ( "reqeq"+m_listalineas->tableName() ) ] + base - base * BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;
        } // end for

        BlFixed basei ( "0.00" );
        base::Iterator it;

        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
            basei = basei + it.value();
        } // end for

        /// Impresion de los descuentos.
        fitxersortidatxt = "";
        BlFixed porcentt ( "0.00" );
        BlDbSubFormRecord *linea1;
        
        if ( m_listadescuentos ) {
            
            if ( m_listadescuentos->rowCount() - 1 ) {
            
                fitxersortidatxt += "<blockTable style=\"tabladescuento\">\n";
                fitxersortidatxt += "<tr>\n";
                fitxersortidatxt += "    <td>" + genEscape ( _ ( "Descuento" ), tipoEscape ) + "</td>\n";
                fitxersortidatxt += "    <td>" + genEscape ( _ ( "Porcentaje" ), tipoEscape ) + "</td>\n";
                fitxersortidatxt += "    <td>" + genEscape ( _ ( "Total Desc." ), tipoEscape ) + "</td>\n";
                fitxersortidatxt += "</tr>\n";
                
                for ( int i = 0; i < ( m_listadescuentos->rowCount() - 1 ); ++i ) {
                    linea1 = m_listadescuentos->lineaat ( i );
                    porcentt = porcentt + BlFixed ( linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
                    fitxersortidatxt += "<tr>\n";
                    fitxersortidatxt += "    <td>" +  genEscape ( linea1->dbValue ( "concept" + m_listadescuentos->tableName() ), tipoEscape ) + "</td>\n";
                    fitxersortidatxt += "    <td>" + genEscape ( l.sprintf ( "%s", linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() ), tipoEscape ) + " %</td>\n";
                    fitxersortidatxt += "    <td>" + genEscape ( l.sprintf ( "-%s", ( BlFixed ( linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ) ) * basei / 100 ).toQString().toAscii().constData() ), tipoEscape ) + "</td>\n";
                    fitxersortidatxt += "</tr>";
                } // end for
                
                fitxersortidatxt += "</blockTable>\n";
                
            } // end if
            
        } // end if
        
        buff.replace ( "[descuentos]", fitxersortidatxt );

        /// Impresion de las bases imponibles.
        fitxersortidatxt = "";
        QString tr1 = ""; /// Rellena el primer tr de titulares.
        QString tr2 = ""; /// Rellena el segundo tr de cantidades.
        fitxersortidatxt += "<blockTable style=\"tabladescuento\" repeatRows=\"1\" >\n";
        BlFixed totbaseimp ( "0.00" );
        BlFixed parbaseimp ( "0.00" );
        BlFixed totdesc ( "0.00" );
        
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        
            if ( porcentt > 0 ) {
                parbaseimp = it.value() - it.value() * porcentt / 100;
                totdesc = totdesc + it.value() * porcentt / 100;
            } else {
                parbaseimp = it.value();
            } // end if
            
            totbaseimp = totbaseimp + parbaseimp;
            tr1 += "    <td>" + genEscape ( _ ( "Base Imponible" ) + " " + genEscape ( it.key() ), tipoEscape ) + " %</td>\n";
            tr2 += "    <td>" + l.sprintf ( " %s ", parbaseimp.toQString().toAscii().constData() ) + "</td>\n";
            
        } // end for

        /// Impresion de los IVAS.
        BlFixed totiva ( "0.0" );
        BlFixed pariva ( "0.0" );
        
        for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        
            if ( porcentt > 0 ) {
                pariva = ( it.value() - it.value() * porcentt / 100 ) * BlFixed ( it.key() ) / 100;
            } else {
                pariva = it.value() * BlFixed ( it.key() ) / 100;
            } // end if
            
            totiva = totiva + pariva;
            tr1 += "    <td>" + genEscape ( _ ( "I.V.A." ), tipoEscape ) + " " + genEscape ( it.key(), tipoEscape ) + " %</td>\n";
            tr2 += "    <td>" + l.sprintf ( " %s ", genEscape ( pariva.toQString(), tipoEscape ).toAscii().constData() ) + "</td>\n";
            
        } // end for

        /// Impresion de los Recargos de Equivalencia.
        BlFixed totreqeq ( "0.0" );
        BlFixed parreqeq ( "0.0" );
        
        for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
        
            if ( porcentt > 0 ) {
                parreqeq = ( it.value() - it.value() * porcentt / 100 ) * BlFixed ( it.key() ) / 100;
            } else {
                parreqeq = it.value() * BlFixed ( it.key() ) / 100;
            } // end if
            
            totreqeq = totreqeq + parreqeq;
            
            if ( parreqeq > 0 ) {
                tr1 += "    <td>" + _ ( "R.E." ) + " " + genEscape ( it.key(), tipoEscape ) + " %</td>\n";
                tr2 += "    <td>" + l.sprintf ( " %s ", parreqeq.toQString().toAscii().constData() ) + "</td>\n";
            } // end if
            
        } // end for

        BlFixed totirpf = totbaseimp * irpf / 100;
        
        if ( totirpf > 0 ) {
            tr1 += "    <td>" + genEscape ( _ ( "I.R.P.F (-" ) + " " +  irpf.toQString() + ") %", tipoEscape ) + "</td>\n";
            tr2 += "    <td>" + genEscape ( l.sprintf ( " %s ", totirpf.toQString().toAscii().constData() ), tipoEscape ) + "</td>\n";
        } // end if

        tr1 += "    <td>" + genEscape ( _ ( "Total" ), tipoEscape ) + "</td>\n";
        tr2 += "    <td>" + genEscape ( l.sprintf ( " %s ", ( totiva + totbaseimp + totreqeq - totirpf ).toQString().toAscii().constData() ), tipoEscape ) + "</td>\n";
        fitxersortidatxt += "<tr>" + tr1 + "</tr><tr>" + tr2 + "</tr></blockTable>\n";
        buff.replace ( "[totales]", fitxersortidatxt );
        buff.replace ( "[irpf]", totirpf.toQString() );
        buff.replace ( "[tirpf]", irpf.toQString() );
        buff.replace ( "[totalre]", totreqeq.toQString() );
        buff.replace ( "[teoricbimp]", basei.toQString() );
        buff.replace ( "[totalbimp]", totbaseimp.toQString() );
        buff.replace ( "[totaldesc]", totdesc.toQString() );
        buff.replace ( "[totaliva]", totiva.toQString() );
        buff.replace ( "[total]", ( totiva + totbaseimp + totreqeq - totirpf ).toQString() );

        /// En la version para windows hay problemas con las imagenes,
        /// por eso de momento lo dejamos asi.
#ifndef WINDOWS
        //   buff.replace("[detallearticulos]", detalleArticulos());
#endif

// a ver. No se porque pongo esto aqui.
// Como que he sacado codigo de generaRML(arch) para ponerlo aqui (porque me parece quees trataTags
// quien deberia tratar todos los tags y porque asi aprovecho el proceso de escribir el fichero en el
// encoding correcto que ya esta hecho en Ficha) pues he copiado tambien esto, pero yo hubiera dicho
// que C++ ya haria el delete solito y si
// no es asi, porque en generaRML había dos catchs y solo uno hacia delete?
    } catch ( ... ) {
    
        if ( cur ) delete cur;
        throw ( -1 );
        
    }

    _depura ( "END BfForm::trataTagsBf", 0 );
}

/// Trata las lineas de detalle encontradas dentro de los tags <!--LINEAS DETALLE-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString BfForm::trataLineasDetalle ( const QString &det, int tipoEscape )
{
    _depura ( "BfForm::trataLineasDetalle", 0 );
    
    QString result = "";

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    BlDbSubFormRecord *linea;
    
    /// Impresion de las lineas
    for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
    
        QString salidatemp = det;
        linea = m_listalineas->lineaat ( i );
        BlFixed base = BlFixed ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        QString l;

        /// Los saltos de carro se deben tratar de modo especial ya que RML no los contempla bien.
        QString desc = linea->dbValue ( "desc" + m_listalineas->tableName() );
        QStringList descp = desc.split ( "\n" );
        QString desc1 = "";
        
        for ( int i = 0; i < descp.size(); ++i ) {
            if ( descp[i] != "" ) {
                if ( descp[i].startsWith ( " " ) ) {
                    desc1 += "<pre>" + genEscape ( descp[i], tipoEscape ) + "</pre>\n";
                } else {
                    desc1 += "<para>" + genEscape ( descp[i], tipoEscape ) + "</para>\n";
                } // end if
            } else {
                desc1 += "<spacer length=\"0.1cm\"/>\n";
            } // end if
        } // end for

        salidatemp.replace ( "[desc" + m_listalineas->tableName() + "]", desc1 );
        salidatemp.replace ( "[cant" + m_listalineas->tableName() + "]",  linea->dbValue ( "cant" + m_listalineas->tableName() ) );

        salidatemp.replace ( "[pvp" + m_listalineas->tableName() + "]",  genEscape ( linea->dbValue ( "pvp" + m_listalineas->tableName() ), tipoEscape ).toAscii().constData() );
        salidatemp.replace ( "[descuento" + m_listalineas->tableName() + "]" ,  genEscape ( linea->dbValue ( "descuento" + m_listalineas->tableName() ), tipoEscape ).toAscii().constData() );

        BlFixed ftotal = BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ) );
        BlFixed fftotal = base - base * ftotal / 100;
        salidatemp.replace ( "[total" + m_listalineas->tableName() + "]" ,  genEscape ( fftotal.toQString(), tipoEscape ) );

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( linea->exists ( rx.cap ( 1 ) ) ) {
                salidatemp.replace ( pos, rx.matchedLength(), genEscape ( linea->dbValue ( rx.cap ( 1 ) ), tipoEscape ) );
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
        
    } // end for

    _depura ( "END BfForm::trataLineasDetalle", 0 );
    
    return result;
}

/// Trata las lineas de descuento encontradas dentro de los tags <!--LINEAS DESCUENTO-->
/**
\param det Texto de entrada para ser tratado por iteracion.
\return
**/
QString BfForm::trataLineasDescuento ( const QString &det, int tipoEscape )
{
    _depura ( "BfForm::trataLineasDescuento", 0 );
    
    QString result = "";

    base basesimp;
    base basesimpreqeq;

    /// Calculamos la base imponible
    BlDbSubFormRecord *linea;
    
    /// Impresion de las lineas
    for ( int i = 0; i < ( m_listalineas->rowCount() - 1 ); ++i ) {
        linea = m_listalineas->lineaat ( i );
        BlFixed base = BlFixed ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() ) * BlFixed ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        basesimp[linea->dbValue ( "iva"+m_listalineas->tableName() ) ] = basesimp[linea->dbValue ( "iva"+m_listalineas->tableName() ) ] + base - base * BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;
        basesimpreqeq[linea->dbValue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->dbValue ( "reqeq"+m_listalineas->tableName() ) ] + base - base * BlFixed ( linea->dbValue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() ) / 100;

    } // end for

    BlFixed basei ( "0.00" );
    base::Iterator it;
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Contador que sirve para poner lineas de mas en caso de que sea preciso.
    BlDbSubFormRecord *linea1;
    
    /// Impresion de las lineas
    for ( int i = 0; i < ( m_listadescuentos->rowCount() - 1 ); ++i ) {
    
        QString salidatemp = det;
        linea1 = m_listadescuentos->lineaat ( i );
        QString l;
        salidatemp.replace ( "[concept" + m_listadescuentos->tableName() + "]", genEscape ( linea1->dbValue ( "concept" + m_listadescuentos->tableName() ), tipoEscape ) );
        salidatemp.replace ( "[proporciondesc" + m_listadescuentos->tableName() + "]", genEscape ( l.sprintf ( "%s", linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() ), tipoEscape ) );
        salidatemp.replace ( "[totaldesc" + m_listadescuentos->tableName() + "]", l.sprintf ( "-%s", ( BlFixed ( linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ) ) * basei / 100 ).toQString().toAscii().constData() ) );

        /// Buscamos cadenas perdidas adicionales que puedan quedar por poner.
        QRegExp rx ( "\\[(\\w*)\\]" );
        int pos =  0;
        
        while ( ( pos = rx.indexIn ( salidatemp, pos ) ) != -1 ) {
            if ( linea1->exists ( rx.cap ( 1 ) ) ) {
                salidatemp.replace ( pos, rx.matchedLength(), genEscape ( linea1->dbValue ( rx.cap ( 1 ) ), tipoEscape ) );
                pos = 0;
            } else {
                pos += rx.matchedLength();
            }
        } // end while

        result += salidatemp;
        
    } // end for
    
    _depura ( "END BfForm::trataLineasDescuento", 0 );
    
    return result;
}

/** Calcula los totales de factura, descuentos e impuestos y invoca al metodo de pintaTotales para
    que se pinten dichos valores en la impresion.
*/
/**
\return
**/
QString BfForm::trataTotales ( const QString &det, int bimporeq )
{
    _depura ( "BfForm::trataTotales", 0 );
    
    QString result = "";
    QString salidatemp = "";
    base basesimp;
    base basesimpreqeq;
    BlDbSubFormRecord *linea;
    
    /// Impresion de los contenidos.
    QString l;
    BlFixed irpf ( "0" );

    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT * FROM configuracion WHERE nombre = 'IRPF'" );
    if ( cur ) {
        if ( !cur->eof() ) {
            irpf = BlFixed ( cur->valor ( "valor" ) );
        } // end if
        delete cur;
    } // end if

    if ( exists ( "idproveedor" ) && dbValue ( "idproveedor" ) != "" ) {
        cur = mainCompany() ->loadQuery ( "SELECT irpfproveedor FROM proveedor WHERE idproveedor = " + dbValue ( "idproveedor" ) );
        if ( cur ) {
            if ( !cur->eof() ) {
                irpf = BlFixed ( cur->valor ( "irpfproveedor" ) );
            } // end if
            delete cur;
        } // end if
    } // end if

    BlFixed descuentolinea ( "0.00" );
    
    /// Iteramos para cada linea para saber totales.
    for ( int i = 0; i < m_listalineas->rowCount() - 1; ++i ) {
        linea = m_listalineas->lineaat ( i );
        BlFixed cant ( linea->dbValue ( "cant" + m_listalineas->tableName() ).toAscii().constData() );
        BlFixed pvpund ( linea->dbValue ( "pvp" + m_listalineas->tableName() ).toAscii().constData() );
        BlFixed desc1 ( linea->dbValue ( "descuento" + m_listalineas->tableName() ).toAscii().constData() );
        BlFixed cantpvp = cant * pvpund;
        BlFixed base = cantpvp - cantpvp * desc1 / 100;
        descuentolinea = descuentolinea + ( cantpvp * desc1 / 100 );
        basesimp[linea->dbValue ( "iva" + m_listalineas->tableName() ) ] = basesimp[linea->dbValue ( "iva" + m_listalineas->tableName() ) ] + base;
        basesimpreqeq[linea->dbValue ( "reqeq" + m_listalineas->tableName() ) ] = basesimpreqeq[linea->dbValue ( "reqeq" + m_listalineas->tableName() ) ] + base;
    } // end for

    BlFixed basei ( "0.00" );
    base::Iterator it;
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
        basei = basei + it.value();
    } // end for

    /// Calculamos el total de los descuentos.
    BlFixed porcentt ( "0.00" );
    BlDbSubFormRecord *linea1;
    
    if ( m_listadescuentos->rowCount() ) {
        for ( int i = 0; i < m_listadescuentos->rowCount(); ++i ) {
            linea1 = m_listadescuentos->lineaat ( i );
            BlFixed propor ( linea1->dbValue ( "proporcion" + m_listadescuentos->tableName() ).toAscii().constData() );
            porcentt = porcentt + propor;
        } // end for
    } // end if

    /// Calculamos el total de base imponible.
    BlFixed totbaseimp ( "0.00" );
    BlFixed parbaseimp ( "0.00" );
    BlFixed totdesc ( "0.00" );
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
    
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parbaseimp = it.value() - it.value() * porcentt / 100;
            totdesc = totdesc + it.value() * porcentt / 100;
        } else {
            parbaseimp = it.value();
        } // end if
        
        totbaseimp = totbaseimp + parbaseimp;
        
    } // end for

    /// Calculamos el total de IVA.
    BlFixed totiva ( "0.00" );
    BlFixed pariva ( "0.00" );
    
    for ( it = basesimp.begin(); it != basesimp.end(); ++it ) {
    
        BlFixed piva ( it.key().toAscii().constData() );
        
        if ( porcentt > BlFixed ( "0.00" ) ) {
            QString evpariva = "( 1 - " + porcentt.toQString() + " / 100 ) * " + it.value().toQString() + " * " + piva.toQString() + " / 100";
            QString tot = mainCompany()->PGEval ( evpariva );
            pariva = BlFixed ( tot );
        } else {
            pariva = it.value() * piva / 100;
        } // end if
        
        totiva = totiva + pariva;
        
    } // end for

    /// Calculamos el total de recargo de equivalencia.
    BlFixed totreqeq ( "0.00" );
    BlFixed parreqeq ( "0.00" );
    
    for ( it = basesimpreqeq.begin(); it != basesimpreqeq.end(); ++it ) {
    
        BlFixed preqeq ( it.key().toAscii().constData() );
        
        if ( porcentt > BlFixed ( "0.00" ) ) {
            parreqeq = ( it.value() - it.value() * porcentt / 100 ) * preqeq / 100;
        } else {
            parreqeq = it.value() * preqeq / 100;
        } // end if
        
        totreqeq = totreqeq + parreqeq;
        
    } // end for

    BlFixed totirpf = totbaseimp * irpf / 100;

    QString cero = BlFixed ( "0.00" ).toQString().toAscii();

    base::Iterator ot;
    base::Iterator at;

    switch ( bimporeq ) {
        case 1:
            /// Sustituye Bases Imponibles de cada tipo de IVA.

            /// Comprueba que haya elementos. Si no sustituye con ceros.
            if ( basesimp.isEmpty() ) {
            
                salidatemp = det;

                salidatemp.replace ( "[bimp]", cero );
                salidatemp.replace ( "[tbimp]", cero );
                salidatemp.replace ( "[iva]", cero );
                salidatemp.replace ( "[tiva]", cero );
                salidatemp.replace ( "[irpf]", cero );
                salidatemp.replace ( "[tirpf]", cero );
                salidatemp.replace ( "[totalre]", cero );
                salidatemp.replace ( "[teoricbimp]", cero );
                salidatemp.replace ( "[totalbimp]", cero );
                salidatemp.replace ( "[totaldesc]", cero );
                salidatemp.replace ( "[totaliva]", cero );
                salidatemp.replace ( "[total]", cero );

                result += salidatemp;

            } else {
            
                for ( ot = basesimp.begin(); ot != basesimp.end(); ++ot ) {

                    salidatemp = det;
                    if ( porcentt > BlFixed ( "0.00" ) ) {
                        parbaseimp = ot.value() - ot.value() * porcentt / 100;
                    } else {
                        parbaseimp = ot.value();
                    } // end if
                    salidatemp.replace ( "[bimp]", parbaseimp.toQString() );
                    salidatemp.replace ( "[tbimp]", ot.key() );

                    BlFixed piva ( ot.key().toAscii().constData() );
                    if ( porcentt > BlFixed ( "0.00" ) ) {
                        pariva = ( ot.value() - ot.value() * porcentt / 100 ) * piva / 100;
                    } else {
                        pariva = ot.value() * piva / 100;
                    } // end if

                    salidatemp.replace ( "[iva]", pariva.toQString() );
                    salidatemp.replace ( "[tiva]", ot.key() );
                    salidatemp.replace ( "[irpf]", totirpf.toQString() );
                    salidatemp.replace ( "[tirpf]", irpf.toQString() );
                    salidatemp.replace ( "[totalre]", totreqeq.toQString() );
                    salidatemp.replace ( "[teoricbimp]", basei.toQString() );
                    salidatemp.replace ( "[totalbimp]", totbaseimp.toQString() );
                    salidatemp.replace ( "[totaldesc]", totdesc.toQString() );
                    salidatemp.replace ( "[totaliva]", totiva.toQString() );
                    salidatemp.replace ( "[total]", ( totiva + totbaseimp + totreqeq - totirpf ).toQString().toAscii().constData() );

                    result += salidatemp;
                } // end for
                
            } // end if

            break;

        case 2:
            /// Sustituye Recargos de equivalencia

            /// Comprueba que haya elementos. Si no sustituye con ceros.
            if ( basesimpreqeq.isEmpty() ) {
            
                salidatemp = det;
                salidatemp.replace ( "[re]", cero );
                salidatemp.replace ( "[tre]", cero );

                result += salidatemp;

            } else {
            
                for ( at = basesimpreqeq.begin(); at != basesimpreqeq.end(); ++at ) {

                    salidatemp = det;
                    BlFixed preqeq ( at.key().toAscii().constData() );

                    if ( porcentt > BlFixed ( "0.00" ) ) {
                        parreqeq = ( at.value() - at.value() * porcentt / 100 ) * preqeq / 100;
                    } else {
                        parreqeq = at.value() * preqeq / 100;
                    } // end if

                    salidatemp.replace ( "[re]", parreqeq.toQString() );
                    salidatemp.replace ( "[tre]", at.key() );

                    result += salidatemp;
                } // end for
                
            } // end if
            
            break;

    } // end switch

    _depura ( "END BfForm::trataTotales", 0 );
    
    return result;
}

int BfForm::generaRML ( void )
{
    _depura ( "BfForm::generaRML(void)", 0 );
    
    BlForm::generaRML();
    
    _depura ( "END BfForm::generaRML(void)", 0 );
   return 0;
}


QString BfForm::nombrePlantilla ( void )
{
    _depura ( "BfForm::nombrePlantilla", 0 );
    _depura ( "END BfForm::nombrePlantilla", 0 );
    
    return m_tablename;
}

///
/**
\param arch archivo a generar
\return 0 si ha ocurrido un error, != 0 si todo ha ido bien.
**/
int BfForm::generaRML ( const QString &arch )
{
    _depura ( "BfForm::generaRML, con argumento const QString &arch", 0, arch );
    
    try {

        if ( dbValue ( "id" + m_tablename ).isEmpty() && dbValue ( "num" + m_tablename ).isEmpty() ) {
            throw 100;
        } // end if

        /// Disparamos los plugins
        int res = g_plugins->lanza ( "BfForm_generaRML", this );
        if ( res != 0 ) {
            return 1;
        } // end if

        BlForm::generaRML ( arch );

        _depura ( "END BfForm::generaRML, con argumento const QString &arch", 0 );
        
        return 1;

    } catch ( int e ) {
        _depura ( "Error en el procesado del archivo RML", 2 );
        throw e;

    } catch ( ... ) {
        _depura ( "Error en el procesado del archivo RML", 2 );
        throw ( -1 );
    } // end try
}

///
/**
\return
**/
void BfForm::imprimir()
{
    _depura ( "BfForm::imprimir", 0 );
    
    try {
        /// Disparamos los plugins
        int res = g_plugins->lanza ( "BfForm_imprimir", this );
        if ( res != 0 ) {
            return;
        } // end if

        BlForm::imprimir();

        _depura ( "END BfForm::imprimir", 0 );

    } catch ( int e ) {
        if ( e == 100 ) {
            /// El documento no se ha guardado y no se dispone en la base de datos de estos datos.
            mensajeInfo ( _ ( "Tiene que guardar el documento antes de poder procesarlo." ) );
        } // end if
    } catch ( ... ) {
        mensajeInfo ( _ ( "Error inesperado en la impresion" ) );
    } // end try
}

///
/**
\param form
**/
void BfForm::setListaLineas ( BfSubForm *form )
{
    _depura ( "BfForm::setListaLineas", 0 );
    
    m_listalineas = form;
    
    _depura ( "END BfForm::setListaLineas", 0 );
}

///
/**
\param form
**/
void BfForm::setListaDescuentos ( BfSubForm *form )
{
    _depura ( "BfForm::setListaDescuentos", 0 );
    
    m_listadescuentos = form;
    
    _depura ( "END BfForm::setListaDescuentos", 0 );
}

///
/**
\return
**/
BfSubForm* BfForm::getlistalineas()
{
    _depura ( "BfForm::getlistalineas", 0 );
    _depura ( "END BfForm::getlistalineas", 0 );
    
    return m_listalineas;
}

///
/**
\return
**/
BfSubForm* BfForm::getlistadescuentos()
{
    _depura ( "BfForm::getlistadescuentos", 0 );
    _depura ( "END BfForm::getlistadescuentos", 0 );
    
    return m_listadescuentos;
}
