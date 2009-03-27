/***************************************************************************
 *   Copyright (C) 2003 by Santiago Capel                                  *
 *   Santiago Capel Torres (GestiONG)                                      *
 *   Tomeu Borras Riera                                                    *
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
 ***************************************************************************/

#include <stdio.h>

#include <QDateTime>
#include <QTextStream>
#include <QObject>
#include <QString>
#include <QMessageBox>
#include <QXmlDefaultHandler>

#include "blapplication.h"
#include "blimportexport.h"

#define EURO                166.386
#define S_EURO             "166.386"

#define LEN_CODIGO_CUENTA    12
#define LEN_TITULO           40
#define LEN_NIF              15
#define LEN_DOMICILIO        35
#define LEN_POBLACION        25
#define LEN_PROVINCIA        20
#define LEN_CODPOSTAL         5
#define LEN_DIVISA            1
#define LEN_CTA_CODDIVISA     5
#define LEN_CTA_DOCUMENTO     1
#define LEN_AJUSTAME          1
#define LEN_TIPOIVA           1

#define LEN_ASIEN             6
#define LEN_FECHA             8
#define LEN_SUBCTA           12
#define LEN_CONTRA           12
#define LEN_PTADEBE          16
#define LEN_CONCEPTO         25
#define LEN_PTAHABER         16
#define LEN_FACTURA           8
#define LEN_BASEIMPO         16
#define LEN_IVA               5
#define LEN_RECEQUIV          5
#define LEN_DOCUMENTO        10
#define LEN_DEPARTA           3
#define LEN_CLAVE             6
#define LEN_ESTADO            1
#define LEN_NCASADO           6
#define LEN_TCASADO           1
#define LEN_TRANS             6
#define LEN_CAMBIO           16
#define LEN_DEBEME           16
#define LEN_HABERME          16
#define LEN_AUXILIAR          1
#define LEN_SERIE             1
#define LEN_SUCURSAL          4
#define LEN_CODDIVISA         5
#define LEN_IMPAUXME         16
#define LEN_MONEDAUSO         1
#define LEN_EURODEBE         16
#define LEN_EUROHABER        16
#define LEN_BASEEURO         16
#define LEN_NOCONV            1
#define LEN_NUMEROINV        10


extern BlApplication *g_theApp;


/// ---------------------------------------------

/// Clase para leer archivos de XML y hacer la importacion de datos.
class StructureParser : public QXmlDefaultHandler
{
private:
    BlPostgreSqlClient *dbConnection;
    QString cadintermedia; /// Esta variable va almacenando los valores que van saliendo en la clase.
    /// Variables usadas para almacenar los datos de un asiento.
    QString idasiento;
    QString ordenasiento;
    QString fechaasiento;
    /// Variables usadas para almacenar los datos de un apunte.
    QString idapunte;
    QString idborrador;
    QString fechaapunte;
    QString codigocuentaapunte;
    QString debeapunte;
    QString haberapunte;
    QString conceptocontableapunte;
    int m_ordenapunte;
    /// Variables usadas para almacenar los datos de una cuenta.
    QString idcuenta;
    QString descripcioncuenta;
    QString codigocuenta;
    QString codigopadre;
    QString m_bloqueadaCuenta;
    QString m_nodebeCuenta;
    QString m_nohaberCuenta;
    QString m_tipoCuenta;
    /// Variables usadas para almacenar los datos del registro de IVA.
    QString m_idRegistroIva;
    QString m_rIvaContrapartida;
    QString m_rIvaBaseImp;
    QString m_rIvaIva;
    QString m_rIvaFFactura;
    QString m_rIvaFactura;
    QString m_rIvaCIF;
    QString m_rIvaIdFPago;
    QString m_rIvRecRegIva;
    /// Variables usadas para almacenar los datos de la tabla de IVA's.
    QString m_idTipoIva;
    QString m_baseIva;
    QString m_nombreTipoIva;
    /// El tagpadre indica en que posicion estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc.
    QString tagpadre;

public:
    StructureParser ( BlPostgreSqlClient *, unsigned int tip = IMPORT_TODO );
    ~StructureParser();
    bool startDocument();
    bool startElement ( const QString&, const QString&, const QString&, const QXmlAttributes& );
    bool endElement ( const QString&, const QString&, const QString& );
    bool characters ( const QString& );

private:
    QString indent;
    unsigned int m_tipo;
};


/// Clase para leer archivos de XML y hacer la importacion de datos.
/// Usamos este tipo para almacenar todos los valores que va recogiendo la clase.
typedef QMap<QString, QString> tvalores;


class ImportBulmaFact : public QXmlDefaultHandler
{
private:
    BlPostgreSqlClient *dbConnection;
    QString cadintermedia; /// Esta variable va almacenando los valores que van saliendo en la clase.
    /// Variables usadas para almacenar los datos de un asiento.
    tvalores valores;
    /// El tagpadre indica en que posicion estamos. Si estamos en un asiento, un apunte, una cuenta, etc etc etc.
    QString tagpadre;
    BlImportExport *pgimport;
    /// Estas estructuras sirven para guardar datos intermedios.
    QList<tvalores> listalpresupuesto;
    QList<tvalores> listadpresupuesto;
    QList<tvalores> listalfactura;
    QList<tvalores> listadfactura;
    QList<tvalores> listalalbaran;
    QList<tvalores> listadalbaran;
    QList<tvalores> listalpedidocliente;
    QList<tvalores> listadpedidocliente;

public:
    ImportBulmaFact ( BlImportExport *, BlPostgreSqlClient *, unsigned long long int tip = IMPORT_TODO );
    ~ImportBulmaFact();
    bool startDocument();
    bool startElement ( const QString&, const QString&, const QString&, const QXmlAttributes& );
    bool endElement ( const QString&, const QString&, const QString& );
    bool characters ( const QString& );

private:
    QString indent;
    unsigned int m_tipo;
    void printcontents();
    int trataCliente();
    int trataProveedor();
    int trataFormaPago();
    int trataAlmacen();
    int trataArticulo();
    int trataFactura();
    int trataLFactura();
    int trataDFactura();
    int trataPresupuesto();
    int trataLPresupuesto();
    int trataDPresupuesto();
    int trataFamilia();
    int trataAlbaran();
    int trataLAlbaran();
    int trataDAlbaran();
    int trataPedidoCliente();
    int trataLPedidoCliente();
    int trataDPedidoCliente();
};


/// ---------------------------------------------


///
/**
**/
void BlImportExport::alerta ( int, int )
{
    _depura ( "BlImportExport::alerta", 0 );
    _depura ( "END BlImportExport::alerta", 0 );
}


///
/**
**/
void BlImportExport::mensajeria ( QString )
{
    _depura ( "Funcion no implementada en esta clase\n", 2 );
}


///
/**
\param f
**/
void BlImportExport::setFInicial ( QString f )
{
    _depura ( "BlImportExport::setFInicial", 0 );
    m_fInicial = f;
    _depura ( "END BlImportExport::setFInicial", 0 );
}


///
/**
\param f
**/
void BlImportExport::setFFinal ( QString f )
{
    _depura ( "BlImportExport::setFFinal", 0 );
    m_fFinal = f;
    _depura ( "END BlImportExport::setFFinal", 0 );
}


///
/**
**/
void BlImportExport::setModoTest()
{
    _depura ( "BlImportExport::setModoTest", 0 );
    m_modoTest = TRUE;
    _depura ( "END BlImportExport::setModoTest", 0 );
}


///
/**
**/
void BlImportExport::setModoNormal()
{
    _depura ( "BlImportExport::setModoNormal", 0 );
    m_modoTest = FALSE;
    _depura ( "END BlImportExport::setModoNormal", 0 );
}


///
/**
\return
**/
bool BlImportExport::modoTest()
{
    _depura ( "BlImportExport::modoTest", 0 );
    _depura ( "END BlImportExport::modoTest", 0 );
    return ( m_modoTest == TRUE );
}

///
/**
**/
BlImportExport::~BlImportExport()
{
    _depura ( "BlImportExport::~BlImportExport", 0 );
    _depura ( "END BlImportExport::~BlImportExport", 0 );
}

///
/**
\param con
**/
BlImportExport::BlImportExport ( BlPostgreSqlClient *con )
{
    _depura ( "BlImportExport::BlImportExport", 0 );
    dbConnection = con;
    m_fInicial = "";
    m_fFinal = "";
    setModoNormal();
    _depura ( "END BlImportExport::BlImportExport", 0 );
}

/// Esta funcion se encarga de pasar los datos de BulmaGes a Contaplus.
/**
\param subcuentas
\param asientos
\return
**/
int BlImportExport::bulmages2Contaplus ( QFile &subcuentas, QFile &asientos )
{
    _depura ( "BlImportExport::bulmages2Contaplus", 0 );
    QString codigo, descripcion;
    QString strblancomax;
    QTextStream stream ( &subcuentas );
    QTextStream streamas ( &asientos );
    /// Se supone que ho hay campos mayores de 100 caracteres para que el algoritmo funcione.
    strblancomax.fill ( ' ', 100 );
    /// Solo se van a exportar las cuentas utilizadas, Ya que contaplus no hace
    /// ordenacion en arbol.
    QString query = "SELECT * FROM cuenta WHERE idcuenta IN (SELECT DISTINCT idcuenta FROM apunte)";
    dbConnection->begin();
    BlDbRecordSet *curcta = dbConnection->loadQuery ( query, "elquery" );
    dbConnection->commit();
    while ( !curcta->eof() ) {
        QString linea = "";
        linea += ( curcta->valor ( "codigo" ) + strblancomax ).left ( LEN_CODIGO_CUENTA );
        linea += ( curcta->valor ( "descripcion" ) + strblancomax ).left ( LEN_TITULO );
        linea += ( curcta->valor ( "cifent_cuenta" ) + strblancomax ).left ( LEN_NIF );
        linea += ( curcta->valor ( "dirent_cuenta" ) + strblancomax ).left ( LEN_DOMICILIO );
        linea += ( strblancomax ).left ( LEN_POBLACION );
        linea += ( strblancomax ).left ( LEN_CODPOSTAL );
        linea += ( strblancomax ).left ( LEN_DIVISA );
        linea += ( strblancomax ).left ( LEN_CTA_CODDIVISA );
        linea += ( strblancomax ).left ( LEN_CTA_DOCUMENTO );
        linea += ( strblancomax ).left ( LEN_AJUSTAME );
        linea += ( strblancomax ).left ( LEN_TIPOIVA );
        linea += "\n";
        stream << linea;
        curcta->nextRecord();
    } // end while
    delete curcta;
    query = "SELECT * FROM asiento, apunte, cuenta WHERE asiento.idasiento = apunte.idasiento AND cuenta.idcuenta = apunte.idcuenta ";
    if ( m_fInicial != "" )
        query += " AND asiento.fecha >= '" + m_fInicial + "'";
    if ( m_fFinal != "" )
        query += " AND asiento.fecha <= '" + m_fFinal + "'";
    query += " ORDER BY asiento.idasiento ";
    dbConnection->begin();
    BlDbRecordSet *curas = dbConnection->loadQuery ( query, "masquery" );
    dbConnection->commit();
    int i = 0;
    int numreg = curas->numregistros() + 1;
    while ( !curas->eof() ) {
        alerta ( i++, numreg );
        QString linea = "";
        linea += ( strblancomax + curas->valor ( "ordenasiento" ) ).right ( LEN_ASIEN );
        QString fecha = curas->valor ( "fecha" );
        fecha = fecha.mid ( 6, 4 ) + fecha.mid ( 3, 2 ) + fecha.mid ( 0, 2 );
        linea += ( fecha + strblancomax ).left ( LEN_FECHA );
        linea += ( curas->valor ( "codigo" ) + strblancomax ).left ( LEN_SUBCTA );
        linea += ( strblancomax ).left ( LEN_CONTRA );
        QString cadaux;
        cadaux.sprintf ( "%2.2f", curas->valor ( "debe" ).toFloat() );
        linea += ( strblancomax + cadaux ).right ( LEN_PTADEBE );
        linea += ( curas->valor ( "conceptocontable" ) + strblancomax ).left ( LEN_CONCEPTO );
        cadaux.sprintf ( "%2.2f", curas->valor ( "haber" ).toFloat() );
        linea += ( strblancomax + cadaux ).right ( LEN_PTAHABER );
        linea += ( strblancomax ).left ( LEN_FACTURA );
        linea += ( strblancomax ).left ( LEN_BASEIMPO );
        linea += ( strblancomax ).left ( LEN_IVA );
        linea += ( strblancomax ).left ( LEN_RECEQUIV );
        linea += ( strblancomax ).left ( LEN_DOCUMENTO );
        linea += ( strblancomax ).left ( LEN_DEPARTA );
        linea += ( strblancomax ).left ( LEN_CLAVE );
        linea += ( strblancomax ).left ( LEN_ESTADO );
        linea += ( strblancomax ).left ( LEN_NCASADO );
        linea += ( strblancomax ).left ( LEN_TCASADO );
        linea += ( strblancomax ).left ( LEN_TRANS );
        linea += ( strblancomax ).left ( LEN_CAMBIO );
        linea += ( strblancomax + "0.00" ).right ( LEN_DEBEME );
        linea += ( strblancomax + "0.00" ).right ( LEN_HABERME );
        linea += ( strblancomax ).left ( LEN_AUXILIAR );
        linea += ( strblancomax ).left ( LEN_SERIE );
        linea += ( strblancomax ).left ( LEN_SUCURSAL );
        linea += ( strblancomax ).left ( LEN_CODDIVISA );
        linea += ( strblancomax ).left ( LEN_IMPAUXME );
        linea += ( "2" + strblancomax ).left ( LEN_MONEDAUSO );
        /// Para evitar redondeos usamos el valor devuelto en forma de texto por la
        /// base de datos que ya opera ella en punto fijo.
        cadaux.sprintf ( "%2.2f", curas->valor ( "debe" ).toFloat() );
        cadaux = curas->valor ( "debe" );
        linea += ( strblancomax + cadaux ).right ( LEN_EURODEBE );
        cadaux.sprintf ( "%2.2f", curas->valor ( "haber" ).toFloat() );
        cadaux = curas->valor ( "haber" );
        linea += ( strblancomax + cadaux ).right ( LEN_EUROHABER );
        linea += ( strblancomax + "0.00" ).right ( LEN_BASEEURO );
        linea += ( "F" + strblancomax ).left ( LEN_NOCONV );
        linea += ( strblancomax ).left ( LEN_NUMEROINV );
        linea += "\n";
        mensajeria ( _( "Exportando :" ) + curas->valor ( "codigo" ) + "--" + fecha + "\n" );
        streamas << linea;
        curas->nextRecord();
    } // end while
    delete curas;
    alerta ( 100, 100 );
    _depura ( "END BlImportExport::bulmages2Contaplus", 0 );
    return 0;
} // end if


/// Pasa archivos de Contaplus a BulmaGes
/**
\param subcuentas
\param asientos
\return
**/
int BlImportExport::contaplus2Bulmages ( QFile &subcuentas, QFile &asientos )
{
    _depura ( "BlImportExport::contaplus2Bulmages", 0 );
    QString idasiento;
    QString lopd_str, cuenta_str;
    QString debe, haber;
    QString query;
    QDate fecha1;
    QDate fechain ( 1, 1, 1 );
    QDate fechafi ( 2999, 12, 31 );
    int orden = 0;
    if ( m_fInicial != "" )
        fechain.setDate ( m_fInicial.mid ( 6, 4 ).toInt(), m_fInicial.mid ( 3, 2 ).toInt(), m_fInicial.mid ( 0, 2 ).toInt() );
    if ( m_fFinal != "" )
        fechafi.setDate ( m_fFinal.mid ( 6, 4 ).toInt(), m_fFinal.mid ( 3, 2 ).toInt(), m_fFinal.mid ( 0, 2 ).toInt() );
    /// Subcuentas.
    QTextStream stream ( &subcuentas );
    while ( !subcuentas.atEnd() ) {
        alerta ( subcuentas.pos() + asientos.pos(), subcuentas.size() + asientos.size() );
        QString line = stream.readLine();
        if ( line.length() < 2 )
            break;
        int pos = 0;
        QString cod = line.mid ( pos, LEN_CODIGO_CUENTA ).trimmed();
        pos += LEN_CODIGO_CUENTA;
        QString titulo = line.mid ( pos, LEN_TITULO ).trimmed();
        pos += LEN_TITULO;
        QString nif = line.mid ( pos, LEN_NIF ).trimmed();
        pos += LEN_NIF;
        QString domicilio = line.mid ( pos, LEN_DOMICILIO ).trimmed();
        pos += LEN_DOMICILIO;
        QString poblacion = line.mid ( pos, LEN_POBLACION ).trimmed();
        pos += LEN_POBLACION;
        QString provincia = line.mid ( pos, LEN_PROVINCIA ).trimmed();
        pos += LEN_PROVINCIA;
        QString codpostal = line.mid ( pos, LEN_CODPOSTAL ).trimmed();
        pos += LEN_CODPOSTAL;
        QString divisa = line.mid ( pos, LEN_DIVISA ).trimmed();
        pos += LEN_DIVISA;
        QString cta_coddivisa = line.mid ( pos, LEN_CTA_CODDIVISA ).trimmed();
        pos += LEN_CTA_CODDIVISA;
        QString cta_documento = line.mid ( pos, LEN_CTA_DOCUMENTO ).trimmed();
        pos += LEN_CTA_DOCUMENTO;
        QString ajustame = line.mid ( pos, LEN_AJUSTAME ).trimmed();
        pos += LEN_AJUSTAME;
        QString tipoiva = line.mid ( pos, LEN_TIPOIVA ).trimmed();
        pos += LEN_TIPOIVA;
        /// Antes de hacer una insercion comprobamos que la cuenta no exista ya en el sistema.
        QString query = "SELECT * FROM cuenta WHERE codigo = '" + cod + "'";
        dbConnection->begin();
        BlDbRecordSet *cursaux = dbConnection->loadQuery ( query, "hol" );
        dbConnection->commit();
        if ( cursaux->eof() ) {
            if ( !cod.isEmpty() ) {
                QString padre = searchParent ( cod );
                QString idgrupo = cod.left ( 1 );
                query = "INSERT INTO cuenta (imputacion, activo, tipocuenta, codigo, descripcion, cifent_cuenta, padre, nombreent_cuenta, dirent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta) VALUES  (TRUE, TRUE, 1,'" + cod + "', '" + titulo + "', '" + nif + "', " + padre + ", 'importada de ContaPlus','" + domicilio + poblacion + provincia + codpostal + "','','','','','')";
                dbConnection->begin();
                int error = dbConnection->runQuery ( query );
                if ( error ) {
                    dbConnection->rollback();
                    return 1;
                } // end if
                dbConnection->commit();
                mensajeria ( _( "<LI>Se ha insertado la cuenta " ) + cod + "</LI>\n" );
            } // end if
        } else {
            mensajeria ( _( "<LI>Ya hay una cuenta con el codigo " ) + cod + "</LI>\n" );
        } // end if
        delete cursaux;
    } // end while

    QTextStream stream2 ( &asientos );
    QString lastasiento = "0";
    int napunte = 0;
    while ( !asientos.atEnd() ) {
        alerta ( subcuentas.pos() + asientos.pos(), subcuentas.size() + asientos.size() );
        QString line = stream2.readLine();
        if ( line.length() < 2 )
            break;
        int pos = 0;
        QString asiento = line.mid ( pos, LEN_ASIEN ).trimmed();
        pos += LEN_ASIEN;
        QString fecha = line.mid ( pos, LEN_FECHA ).trimmed();
        fecha1.setDate ( fecha.mid ( 0, 4 ).toInt(), fecha.mid ( 4, 2 ).toInt(), fecha.mid ( 6, 2 ).toInt() );
        fecha = fecha.mid ( 0, 4 ) + "-" + fecha.mid ( 4, 2 ) + "-" + fecha.mid ( 6, 2 );
        pos += LEN_FECHA;
        QString subcta = line.mid ( pos, LEN_SUBCTA ).trimmed();
        pos += LEN_SUBCTA;
        QString contra = line.mid ( pos, LEN_CONTRA ).trimmed();
        pos += LEN_CONTRA;
        QString ptadebe = line.mid ( pos, LEN_PTADEBE ).trimmed();
        pos += LEN_PTADEBE;
        QString concepto = line.mid ( pos, LEN_CONCEPTO ).trimmed();
        pos += LEN_CONCEPTO;
        QString ptahaber = line.mid ( pos, LEN_PTAHABER ).trimmed();
        pos += LEN_PTAHABER;
        QString factura = line.mid ( pos, LEN_FACTURA ).trimmed();
        pos += LEN_FACTURA;
        QString baseimpo = line.mid ( pos, LEN_BASEIMPO ).trimmed();
        pos += LEN_BASEIMPO;
        QString iva = line.mid ( pos, LEN_IVA ).trimmed();
        pos += LEN_IVA;
        QString recequiv = line.mid ( pos, LEN_RECEQUIV ).trimmed();
        pos += LEN_RECEQUIV;
        QString documento = line.mid ( pos, LEN_DOCUMENTO ).trimmed();
        pos += LEN_DOCUMENTO;
        QString departa = line.mid ( pos, LEN_DEPARTA ).trimmed();
        pos += LEN_DEPARTA;
        QString clave = line.mid ( pos, LEN_CLAVE ).trimmed();
        pos += LEN_CLAVE;
        QString estado = line.mid ( pos, LEN_ESTADO ).trimmed();
        pos += LEN_ESTADO;
        QString ncasado = line.mid ( pos, LEN_NCASADO ).trimmed();
        pos += LEN_NCASADO;
        QString tcasado = line.mid ( pos, LEN_TCASADO ).trimmed();
        pos += LEN_TCASADO;
        QString trans = line.mid ( pos, LEN_TRANS ).trimmed();
        pos += LEN_TRANS;
        QString cambio = line.mid ( pos, LEN_CAMBIO ).trimmed();
        pos += LEN_CAMBIO;
        QString debeme = line.mid ( pos, LEN_DEBEME ).trimmed();
        pos += LEN_DEBEME;
        QString haberme = line.mid ( pos, LEN_HABERME ).trimmed();
        pos += LEN_HABERME;
        QString auxiliar = line.mid ( pos, LEN_AUXILIAR ).trimmed();
        pos += LEN_AUXILIAR;
        QString serie = line.mid ( pos, LEN_SERIE ).trimmed();
        pos += LEN_SERIE;
        QString sucursal = line.mid ( pos, LEN_SUCURSAL ).trimmed();
        pos += LEN_SUCURSAL;
        QString coddivisa = line.mid ( pos, LEN_CODDIVISA ).trimmed();
        pos += LEN_CODDIVISA;
        QString impauxme = line.mid ( pos, LEN_IMPAUXME ).trimmed();
        pos += LEN_IMPAUXME;
        QString monedauso = line.mid ( pos, LEN_MONEDAUSO ).trimmed();
        pos += LEN_MONEDAUSO;
        QString eurodebe = line.mid ( pos, LEN_EURODEBE ).trimmed();
        pos += LEN_EURODEBE;
        QString eurohaber = line.mid ( pos, LEN_EUROHABER ).trimmed();
        pos += LEN_EUROHABER;
        QString baseeuro = line.mid ( pos, LEN_BASEEURO ).trimmed();
        pos += LEN_BASEEURO;
        QString noconv = line.mid ( pos, LEN_NOCONV ).trimmed();
        pos += LEN_NOCONV;
        QString numeroinv = line.mid ( pos, LEN_NUMEROINV ).trimmed();
        pos += LEN_NUMEROINV;

        if ( asiento != lastasiento ) {
            if ( lastasiento != "0" ) {
                query = "SELECT cierraasiento(" + idasiento + ")";
                if ( !modoTest() ) {
                    dbConnection->begin();
                    BlDbRecordSet * cur = dbConnection->loadQuery ( query, "hola" );
                    dbConnection->commit();
                    delete cur;
                } // end if
            } // end if
            if ( fecha1 >= fechain && fecha1 <= fechafi ) {
                query = "INSERT INTO asiento (fecha, comentariosasiento, clase) VALUES ('" + fecha + "','Importado de Contaplus', 1 )";
                dbConnection->begin();
                if ( !modoTest() ) {
                    int error = dbConnection->runQuery ( query );
                    if ( error ) {
                        dbConnection->rollback();
                        return 1;
                    } // end if
                } // end if
                query = "SELECT max(idasiento) as idasiento FROM asiento";
                BlDbRecordSet *cur = dbConnection->loadQuery ( query );
                idasiento = cur->valor ( "idasiento" );
                dbConnection->commit();
                delete cur;
                napunte = 0;
                lastasiento = asiento;
                orden = 0;
                mensajeria ( _( "<LI>Insercion de asiento" ) + idasiento + "</LI>\n" );
            } // end if
        } // end if
        napunte++;
        if ( monedauso == "1" ) { /// Ptas.
            /// Aqui esta el peor error cometido, usar punto flotante
            debe = ptahaber + "/" + S_EURO;
            haber = ptadebe + "/" + S_EURO;
        } else {
            debe = eurodebe;
            haber = eurohaber;
        } // end if
        query = "SELECT * FROM cuenta WHERE codigo = '" + subcta + "'";
        dbConnection->begin();
        BlDbRecordSet *cur = dbConnection->loadQuery ( query, "elquery" );
        dbConnection->commit();
        if ( !cur->eof() ) {
            if ( fecha1 >= fechain && fecha1 <= fechafi ) {
                if ( !modoTest() ) {
                    query = "INSERT INTO borrador (idasiento, idcuenta, fecha, conceptocontable, debe, haber, orden) VALUES (" + idasiento + ", id_cuenta('" + subcta + "'), '" + fecha + "','" + concepto + "'," + debe + "," + haber + "," + QString::number ( orden++ ) + " )";
                    dbConnection->begin();
                    int error = dbConnection->runQuery ( query );
                    if ( error ) {
                        dbConnection->rollback();
                        return 1;
                    } // end if
                    dbConnection->commit();
                } // end if
                mensajeria ( _( "<LI>Insercion de apunte" ) + subcta + "," + concepto + "</LI>\n" );
            } else {
                mensajeria ( _( "<LI>Apunte fuera de fecha</LI>\n" ) );
            } // end if
        } // end if
    } // end while
    if ( lastasiento != "0" ) {
        if ( !modoTest() ) {
            query = "SELECT cierraasiento(" + idasiento + ")";
            BlDbRecordSet * cur = dbConnection->loadQuery ( query );
            delete cur;
        } // end if
    } // end if
    mensajeria ( _( "<LI>Terminado</LI>\n" ) );
    alerta ( subcuentas.size() + asientos.size(), subcuentas.size() + asientos.size() );
    _depura ( "END BlImportExport::contaplus2Bulmages", 0 );
    return 0;
}


/// Esta funcion busca en la base de datos la cuenta padre de la cuenta seleccionada.
/// Si no existe la cuenta padre devuelve NULL.

/**
\param cod
\return
**/
QString BlImportExport::searchParent ( QString cod )
{
    _depura ( "BlImportExport::searchParent", 0 );
    QString padre = "NULL"; /// Almacena el padre de la cuenta.
    QString query;
    int i = 2;
    int fin = 0;
    while ( !fin ) {
        query = "SELECT * FROM cuenta WHERE codigo = '" + cod.left ( i ) + "'";
        dbConnection->begin();
        BlDbRecordSet *cur = dbConnection->loadQuery ( query, "unquery" );
        dbConnection->commit();
        if ( !cur->eof() ) {
            padre = cur->valor ( "idcuenta" );
        } else {
            fin = 1;
        } // end if
        delete cur;
        i++;
    } // end while
    _depura ( "END BlImportExport::searchParent", 0 );
    return padre;
}


/// Esta funcion pasa de bulmafact a XML.
/**
\param xmlfile
\param tipo
\return
**/
int BlImportExport::bulmafact2XML ( QFile &xmlfile, unsigned long long int tipo )
{
    _depura ( "BlImportExport::bulmafact2XML", 0 );
    QTextStream stream ( &xmlfile );
    stream << "<?xml version=\"1.0\" encoding = \"iso-8859-1\"?>\n"
    "<!DOCTYPE FUGIT>\n"
    "<FUGIT version='0.3.1' origen='BulmaGes'"
    " date='" << QDate().toString ( Qt::ISODate ) << "'>\n";

    if ( tipo & IMPORT_FORMAS_PAGO ) {
        QString query = "SELECT * FROM forma_pago";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<FORMA_PAGO>\n";
            stream << "\t<IDFORMA_PAGO>" << XMLProtect ( curc->valor ( "idforma_pago" ) ) << "</IDFORMA_PAGO>\n";
            stream << "\t<DESCFORMA_PAGO>" << XMLProtect ( curc->valor ( "descforma_pago" ) ) << "</DESCFORMA_PAGO>\n";
            stream << "\t<DIAS1TFORMA_PAGO>" << XMLProtect ( curc->valor ( "dias1tforma_pago" ) ) << "</DIAS1TFORMA_PAGO>\n";
            stream << "\t<DESCUENTOFORMA_PAGO>" << XMLProtect ( curc->valor ( "descuentoforma_pago" ) ) << "</DESCUENTOFORMA_PAGO>\n";
            stream << "</FORMA_PAGO>\n";
            mensajeria ( _( "<LI>Exportando Nueva Forma de Pago</LI>\n" ) );
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    if ( tipo & IMPORT_ALMACENES ) {
        QString query = "SELECT * FROM almacen";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<ALMACEN>\n";
            stream << "\t<IDALMACEN>" << XMLProtect ( curc->valor ( "idalmacen" ) ) << "</IDALMACEN>\n";
            stream << "\t<CODIGOALMACEN>" << XMLProtect ( curc->valor ( "codigoalmacen" ) ) << "</CODIGOALMACEN>\n";
            stream << "\t<NOMALMACEN>" << XMLProtect ( curc->valor ( "nomalmacen" ) ) << "</NOMALMACEN>\n";
            stream << "\t<DIRALMACEN>" << XMLProtect ( curc->valor ( "diralmacen" ) ) << "</DIRALMACEN>\n";
            stream << "\t<POBLALMACEN>" << XMLProtect ( curc->valor ( "poblalmacen" ) ) << "</POBLALMACEN>\n";
            stream << "\t<CPALMACEN>" << XMLProtect ( curc->valor ( "cpalmacen" ) ) << "</CPALMACEN>\n";
            stream << "\t<TELALMACEN>" << XMLProtect ( curc->valor ( "telalmacen" ) ) << "</TELALMACEN>\n";
            stream << "\t<FAXALMACEN>" << XMLProtect ( curc->valor ( "faxalmacen" ) ) << "</FAXALMACEN>\n";
            stream << "\t<EMAILALMACEN>" << XMLProtect ( curc->valor ( "emailalmacen" ) ) << "</EMAILALMACEN>\n";
            stream << "\t<INACTIVOALMACEN>" << XMLProtect ( curc->valor ( "inactivoalmacen" ) ) << "</INACTIVOALMACEN>\n";
            stream << "</ALMACEN>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    if ( tipo & IMPORT_TRABAJADORES ) {
        QString query = "SELECT * FROM trabajador";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<TRABAJADOR>\n";
            stream << "\t<IDTRABAJADOR>" << XMLProtect ( curc->valor ( "idtrabajador" ) ) << "</IDTRABAJADOR>\n";
            stream << "\t<NOMTRABAJADOR>" << XMLProtect ( curc->valor ( "nomtrabajador" ) ) << "</NOMTRABAJADOR>\n";
            stream << "\t<APELLIDOSTRABAJADOR>" << XMLProtect ( curc->valor ( "apellidostrabajador" ) ) << "</APELLIDOSTRABAJADOR>\n";
            stream << "\t<DIRTRABAJADOR>" << XMLProtect ( curc->valor ( "dirtrabajador" ) ) << "</DIRTRABAJADOR>\n";
            stream << "\t<NSSTRABAJADOR>" << XMLProtect ( curc->valor ( "nsstrabajador" ) ) << "</NSSTRABAJADOR>\n";
            stream << "\t<TELTRABAJADOR>" << XMLProtect ( curc->valor ( "teltrabajador" ) ) << "</TELTRABAJADOR>\n";
            stream << "\t<MOVILTRABAJADOR>" << XMLProtect ( curc->valor ( "moviltrabajador" ) ) << "</MOVILTRABAJADOR>\n";
            stream << "\t<EMAILTRABAJADOR>" << XMLProtect ( curc->valor ( "emailtrabajador" ) ) << "</EMAILTRABAJADOR>\n";
            stream << "\t<FOTOTRABAJADOR>" << XMLProtect ( curc->valor ( "fototrabajador" ) ) << "</FOTOTRABAJADOR>\n";
            stream << "\t<ACTIVOTRABAJADOR>" << XMLProtect ( curc->valor ( "activotrabajador" ) ) << "</ACTIVOTRABAJADOR>\n";
            stream << "</TRABAJADOR>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    if ( tipo & IMPORT_CLIENTES ) {
        QString query = "SELECT * FROM cliente ORDER BY cifcliente";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<CLIENTE>\n";
            stream << "\t<IDCLIENTE>" << XMLProtect ( curc->valor ( "idcliente" ) ) << "</IDCLIENTE>\n";
            stream << "\t<NOMCLIENTE>" << XMLProtect ( curc->valor ( "nomcliente" ) ) << "</NOMCLIENTE>\n";
            stream << "\t<NOMALTCLIENTE>" << XMLProtect ( curc->valor ( "nomaltcliente" ) ) << "</NOMALTCLIENTE>\n";
            stream << "\t<CIFCLIENTE>" << XMLProtect ( curc->valor ( "cifcliente" ) ) << "</CIFCLIENTE>\n";
            stream << "\t<BANCOCLIENTE>" << XMLProtect ( curc->valor ( "bancocliente" ) ) << "</BANCOCLIENTE>\n";
            stream << "\t<DIRCLIENTE>" << XMLProtect ( curc->valor ( "dircliente" ) ) << "</DIRCLIENTE>\n";
            stream << "\t<POBLCLIENTE>" << XMLProtect ( curc->valor ( "poblcliente" ) ) << "</POBLCLIENTE>\n";
            stream << "\t<CPCLIENTE>" << XMLProtect ( curc->valor ( "cpcliente" ) ) << "</CPCLIENTE>\n";
            stream << "\t<TELCLIENTE>" << XMLProtect ( curc->valor ( "telcliente" ) ) << "</TELCLIENTE>\n";
            stream << "\t<FAXCLIENTE>" << XMLProtect ( curc->valor ( "faxcliente" ) ) << "</FAXCLIENTE>\n";
            stream << "\t<MAILCLIENTE>" << XMLProtect ( curc->valor ( "mailcliente" ) ) << "</MAILCLIENTE>\n";
            stream << "\t<URLCLIENTE>" << XMLProtect ( curc->valor ( "urlcliente" ) ) << "</URLCLIENTE>\n";
            stream << "\t<FALTACLIENTE>" << XMLProtect ( curc->valor ( "faltacliente" ) ) << "</FALTACLIENTE>\n";
            stream << "\t<FBAJACLIENTE>" << XMLProtect ( curc->valor ( "fbajacliente" ) ) << "</FBAJACLIENTE>\n";
            stream << "\t<COMENTCLIENTE>" << XMLProtect ( curc->valor ( "comentcliente" ) ) << "</COMENTCLIENTE>\n";
            stream << "\t<INACTIVOCLIENTE>" << XMLProtect ( curc->valor ( "inactivocliente" ) ) << "</INACTIVOCLIENTE>\n";
            stream << "\t<PROVCLIENTE>" << XMLProtect ( curc->valor ( "provcliente" ) ) << "</PROVCLIENTE>\n";
            stream << "</CLIENTE>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    if ( tipo & IMPORT_PROVEEDORES ) {
        QString query = "SELECT * FROM proveedor ORDER BY cifproveedor";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<PROVEEDOR>\n";
            stream << "\t<IDPROVEEDOR>" << XMLProtect ( curc->valor ( "idproveedor" ) ) << "</IDPROVEEDOR>\n";
            stream << "\t<NOMPROVEEDOR>" << XMLProtect ( curc->valor ( "nomproveedor" ) ) << "</NOMPROVEEDOR>\n";
            stream << "\t<NOMALTPROVEEDOR>" << XMLProtect ( curc->valor ( "nomaltproveedor" ) ) << "</NOMALTPROVEEDOR>\n";
            stream << "\t<CIFPROVEEDOR>" << XMLProtect ( curc->valor ( "cifproveedor" ) ) << "</CIFPROVEEDOR>\n";
            stream << "\t<CODICLIPROVEEDOR>" << XMLProtect ( curc->valor ( "codicliproveedor" ) ) << "</CODICLIPROVEEDOR>\n";
            stream << "\t<CBANCPROVEEDOR>" << XMLProtect ( curc->valor ( "cbancproveedor" ) ) << "</CBANCPROVEEDOR>\n";
            stream << "\t<COMENTPROVEEDOR>" << XMLProtect ( curc->valor ( "comentproveedor" ) ) << "</COMENTPROVEEDOR>\n";
            stream << "\t<DIRPROVEEDOR>" << XMLProtect ( curc->valor ( "dirproveedor" ) ) << "</DIRPROVEEDOR>\n";
            stream << "\t<POBLPROVEEDOR>" << XMLProtect ( curc->valor ( "poblproveedor" ) ) << "</POBLPROVEEDOR>\n";
            stream << "\t<CPPROVEEDOR>" << XMLProtect ( curc->valor ( "cpproveedor" ) ) << "</CPPROVEEDOR>\n";
            stream << "\t<TELPROVEEDOR>" << XMLProtect ( curc->valor ( "telproveedor" ) ) << "</TELPROVEEDOR>\n";
            stream << "\t<FAXPROVEEDOR>" << XMLProtect ( curc->valor ( "faxproveedor" ) ) << "</FAXPROVEEDOR>\n";
            stream << "\t<EMAILPROVEEDOR>" << XMLProtect ( curc->valor ( "emailproveedor" ) ) << "</EMAILPROVEEDOR>\n";
            stream << "\t<URLPROVEEDOR>" << XMLProtect ( curc->valor ( "urlproveedor" ) ) << "</URLPROVEEDOR>\n";
            stream << "\t<CLAVEPROVEEDOR>" << XMLProtect ( curc->valor ( "clavewebproveedor" ) ) << "</CLAVEPROVEEDOR>\n";
            stream << "\t<INACTIVOPROVEEDOR>" << XMLProtect ( curc->valor ( "inactivoproveedor" ) ) << "</INACTIVOPROVEEDOR>\n";
            stream << "</PROVEEDOR>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    if ( tipo & IMPORT_FAMILIAS ) {
        QString query = "SELECT * FROM familia ORDER BY codigocompletofamilia ";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<FAMILIA>\n";
            stream << "\t<IDFAMILIA>" << XMLProtect ( curc->valor ( "idfamilia" ) ) << "</IDFAMILIA>\n";
            stream << "\t<CODIGOFAMILIA>" << XMLProtect ( curc->valor ( "codigofamilia" ) ) << "</CODIGOFAMILIA>\n";
            stream << "\t<NOMBREFAMILIA>" << XMLProtect ( curc->valor ( "nombrefamilia" ) ) << "</NOMBREFAMILIA>\n";
            stream << "\t<DESCFAMILIA>" << XMLProtect ( curc->valor ( "descfamilia" ) ) << "</DESCFAMILIA>\n";
            stream << "\t<PADREFAMILIA>" << XMLProtect ( curc->valor ( "padrefamilia" ) ) << "</PADREFAMILIA>\n";
            stream << "\t<CODIGOCOMPLETOFAMILIA>" << XMLProtect ( curc->valor ( "codigocompletofamilia" ) ) << "</CODIGOCOMPLETOFAMILIA>\n";
            stream << "</FAMILIA>\n";
            curc->nextRecord();
        }// end while
        delete curc;
    } // end if

    if ( tipo & IMPORT_ARTICULOS ) {
        QString query = "SELECT * FROM articulo LEFT JOIN familia ON familia.idfamilia = articulo.idfamilia ";
        query += " LEFT JOIN tipo_articulo ON articulo.idtipo_articulo = tipo_articulo.idtipo_articulo ";
        query += " LEFT JOIN tipo_iva ON articulo.idtipo_iva = tipo_iva.idtipo_iva ";
        query += " ORDER BY codigocompletoarticulo ";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<ARTICULO>\n";
            stream << "\t<IDARTICULO>" << XMLProtect ( curc->valor ( "idarticulo" ) ) << "</IDARTICULO>\n";
            stream << "\t<CODARTICULO>" << XMLProtect ( curc->valor ( "codarticulo" ) ) << "</CODARTICULO>\n";
            stream << "\t<NOMARTICULO>" << XMLProtect ( curc->valor ( "nomarticulo" ) ) << "</NOMARTICULO>\n";
            stream << "\t<ABREVARTICULO>" << XMLProtect ( curc->valor ( "abrevarticulo" ) ) << "</ABREVARTICULO>\n";
            stream << "\t<OBSERARTICULO>" << XMLProtect ( curc->valor ( "obserarticulo" ) ) << "</OBSERARTICULO>\n";
            stream << "\t<PRESENTABLEARTICULO>" << XMLProtect ( curc->valor ( "presentablearticulo" ) ) << "</PRESENTABLEARTICULO>\n";
            stream << "\t<CONTROLSTOCKARTICULO>" << XMLProtect ( curc->valor ( "controlstockarticulo" ) ) << "</CONTROLSTOCKARTICULO>\n";
            stream << "\t<IDTIPO_ARTICULO>" << XMLProtect ( curc->valor ( "idtipo_articulo" ) ) << "</IDTIPO_ARTICULO>\n";
            stream << "\t<IDTIPO_IVA>" << XMLProtect ( curc->valor ( "idtipo_iva" ) ) << "</IDTIPO_IVA>\n";
            stream << "\t<CODIGOCOMPLETOARTICULO>" << XMLProtect ( curc->valor ( "codigocompletoarticulo" ) ) << "</CODIGOCOMPLETOARTICULO>\n";
            stream << "\t<IDFAMILIA>" << XMLProtect ( curc->valor ( "idfamilia" ) ) << "</IDFAMILIA>\n";
            stream << "\t<STOCKARTICULO>" << XMLProtect ( curc->valor ( "stockarticulo" ) ) << "</STOCKARTICULO>\n";
            stream << "\t<INACTIVOARTICULO>" << XMLProtect ( curc->valor ( "inactivoarticulo" ) ) << "</INACTIVOARTICULO>\n";
            stream << "\t<PVPARTICULO>" << XMLProtect ( curc->valor ( "pvparticulo" ) ) << "</PVPARTICULO>\n";
            /// Campos adicionales a los articulos.
            stream << "\t<CODIGOCOMPLETOFAMILIA>" << XMLProtect ( curc->valor ( "codigocompletofamilia" ) )   << "</CODIGOCOMPLETOFAMILIA>\n";
            stream << "\t<NOMBREFAMILIA>" << XMLProtect ( curc->valor ( "nombrefamilia" ) ) << "</NOMBREFAMILIA>\n";
            stream << "\t<CODTIPO_ARTICULO>" << XMLProtect ( curc->valor ( "codtipo_articulo" ) ) << "</CODTIPO_ARTICULO>\n";
            stream << "\t<DESCTIPO_ARTICULO>" << XMLProtect ( curc->valor ( "desctipo_articulo" ) ) << "</DESCTIPO_ARTICULO>\n";
            stream << "\t<DESCTIPO_IVA>" << XMLProtect ( curc->valor ( "desctipo_iva" ) ) << "</DESCTIPO_IVA>\n";
            stream << "</ARTICULO>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    /// Hacemos la exportacion de facturas de clientes.
    if ( tipo & IMPORT_FACTURASCLIENTE ) {
        QString query = "SELECT * FROM factura ";
        query += " LEFT JOIN almacen ON factura.idalmacen = almacen.idalmacen ";
        query += " LEFT JOIN cliente ON cliente.idcliente = factura.idcliente ";
        query += " LEFT JOIN trabajador ON trabajador.idtrabajador = factura.idtrabajador ";
        query += " LEFT JOIN forma_pago ON forma_pago.idforma_pago = factura.idforma_pago ";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<FACTURA>\n";
            stream << "\t<IDFACTURA>" << XMLProtect ( curc->valor ( "idfactura" ) ) << "</IDFACTURA>\n";
            stream << "\t<CODIGOSERIE_FACTURA>" << XMLProtect ( curc->valor ( "codigoserie_factura" ) ) << "</CODIGOSERIE_FACTURA>\n";
            stream << "\t<NUMFACTURA>" << XMLProtect ( curc->valor ( "numfactura" ) ) << "</NUMFACTURA>\n";
            stream << "\t<REFFACTURA>" << XMLProtect ( curc->valor ( "reffactura" ) ) << "</REFFACTURA>\n";
            stream << "\t<FFACTURA>" << XMLProtect ( curc->valor ( "ffactura" ) ) << "</FFACTURA>\n";
            stream << "\t<DESCFACTURA>" << XMLProtect ( curc->valor ( "descfactura" ) ) << "</DESCFACTURA>\n";
            stream << "\t<IDALMACEN>" << XMLProtect ( curc->valor ( "idalmacen" ) ) << "</IDALMACEN>\n";
            stream << "\t<CONTACTFACTURA>" << XMLProtect ( curc->valor ( "contactfactura" ) ) << "</CONTACTFACTURA>\n";
            stream << "\t<TELFACTURA>" << XMLProtect ( curc->valor ( "telfactura" ) ) << "</TELFACTURA>\n";
            stream << "\t<COMENTFACTURA>" << XMLProtect ( curc->valor ( "comentfactura" ) ) << "</COMENTFACTURA>\n";
            stream << "\t<PROCESADAFACTURA>" << XMLProtect ( curc->valor ( "procesadafactura" ) ) << "</PROCESADAFACTURA>\n";
            stream << "\t<IDUSUARI>" << XMLProtect ( curc->valor ( "idusuari" ) ) << "</IDUSUARI>\n";
            stream << "\t<IDCLIENTE>" << XMLProtect ( curc->valor ( "idcliente" ) ) << "</IDCLIENTE>\n";
            stream << "\t<IDFORMA_PAGO>" << XMLProtect ( curc->valor ( "idforma_pago" ) ) << "</IDFORMA_PAGO>\n";
            stream << "\t<IDTRABAJADOR>" << XMLProtect ( curc->valor ( "idtrabajador" ) ) << "</IDTRABAJADOR>\n";
            /// Datos iniciales para la factura que pueden ser de utilidad.
            stream << "\t<CODIGOALMACEN>" << XMLProtect ( curc->valor ( "codigoalmacen" ) ) << "</CODIGOALMACEN>\n";
            stream << "\t<NOMALMACEN>" << XMLProtect ( curc->valor ( "nomalmacen" ) ) << "</NOMALMACEN>\n";
            stream << "\t<DIRALMACEN>" << XMLProtect ( curc->valor ( "diralmacen" ) ) << "</DIRALMACEN>\n";
            stream << "\t<POBLALMACEN>" << XMLProtect ( curc->valor ( "poblalmacen" ) ) << "</POBLALMACEN>\n";
            stream << "\t<CPALMACEN>" << XMLProtect ( curc->valor ( "cpalmacen" ) ) << "</CPALMACEN>\n";
            stream << "\t<TELALMACEN>" << XMLProtect ( curc->valor ( "telalmacen" ) ) << "</TELALMACEN>\n";
            /// Datos referentes al cliente.
            stream << "\t<NOMCLIENTE>" << XMLProtect ( curc->valor ( "nomcliente" ) ) << "</NOMCLIENTE>\n";
            stream << "\t<NOMALTCLIENTE>" << XMLProtect ( curc->valor ( "nomaltcliente" ) ) << "</NOMALTCLIENTE>\n";
            stream << "\t<CIFCLIENTE>" << XMLProtect ( curc->valor ( "cifcliente" ) ) << "</CIFCLIENTE>\n";
            stream << "\t<BANCOCLIENTE>" << XMLProtect ( curc->valor ( "bancocliente" ) ) << "</BANCOCLIENTE>\n";
            stream << "\t<DIRCLIENTE>" << XMLProtect ( curc->valor ( "dircliente" ) ) << "</DIRCLIENTE>\n";
            stream << "\t<POBLCLIENTE>" << XMLProtect ( curc->valor ( "poblcliente" ) ) << "</POBLCLIENTE>\n";
            stream << "\t<CPCLIENTE>" << XMLProtect ( curc->valor ( "cpcliente" ) ) << "</CPCLIENTE>\n";
            stream << "\t<TELCLIENTE>" << XMLProtect ( curc->valor ( "telcliente" ) ) << "</TELCLIENTE>\n";
            stream << "\t<FAXCLIENTE>" << XMLProtect ( curc->valor ( "faxcliente" ) ) << "</FAXCLIENTE>\n";
            stream << "\t<MAILCLIENTE>" << XMLProtect ( curc->valor ( "mailcliente" ) ) << "</MAILCLIENTE>\n";
            stream << "\t<URLCLIENTE>" << XMLProtect ( curc->valor ( "urlcliente" ) ) << "</URLCLIENTE>\n";
            stream << "\t<PROVCLIENTE>" << XMLProtect ( curc->valor ( "provcliente" ) ) << "</PROVCLIENTE>\n";
            /// Datos sobre el trabajador
            stream << "\t<NOMTRABAJADOR>" << XMLProtect ( curc->valor ( "nomtrabajador" ) ) << "</NOMTRABAJADOR>\n";
            stream << "\t<APELLIDOSTRABAJADOR>" << XMLProtect ( curc->valor ( "apellidostrabajador" ) ) << "</APELLIDOSTRABAJADOR>\n";
            stream << "\t<DIRTRABAJADOR>" << XMLProtect ( curc->valor ( "dirtrabajador" ) ) << "</DIRTRABAJADOR>\n";
            stream << "\t<NSSTRABAJADOR>" << XMLProtect ( curc->valor ( "nsstrabajador" ) ) << "</NSSTRABAJADOR>\n";
            stream << "\t<TELTRABAJADOR>" << XMLProtect ( curc->valor ( "teltrabajador" ) ) << "</TELTRABAJADOR>\n";
            stream << "\t<MOVILTRABAJADOR>" << XMLProtect ( curc->valor ( "moviltrabajador" ) ) << "</MOVILTRABAJADOR>\n";
            stream << "\t<EMAILTRABAJADOR>" << XMLProtect ( curc->valor ( "emailtrabajador" ) ) << "</EMAILTRABAJADOR>\n";
            /// Datos sobre la forma de pago
            stream << "\t<DESCFORMA_PAGO>" << XMLProtect ( curc->valor ( "descforma_pago" ) ) << "</DESCFORMA_PAGO>\n";
            stream << "\t<DIAS1TFORMA_PAGO>" << XMLProtect ( curc->valor ( "dias1tforma_pago" ) ) << "</DIAS1TFORMA_PAGO>\n";
            stream << "\t<DESCUENTOFORMA_PAGO>" << XMLProtect ( curc->valor ( "descuentoforma_pago" ) ) << "</DESCUENTOFORMA_PAGO>\n";

            /// Incorporamos las lineas de detalles de la factura.
            BlDbRecordSet *curlc = dbConnection->loadQuery ( "SELECT * FROM lfactura LEFT JOIN articulo ON lfactura.idarticulo = articulo.idarticulo WHERE idfactura = " + curc->valor ( "idfactura" ) );
            while ( !curlc->eof() ) {
                stream << "\t\t<LFACTURA>\n";
                stream << "\t\t\t<IDLFACTURA>" << XMLProtect ( curlc->valor ( "idlfactura" ) ) << "</IDLFACTURA>\n";
                stream << "\t\t\t<DESCLFACTURA>" << XMLProtect ( curlc->valor ( "desclfactura" ) ) << "</DESCLFACTURA>\n";
                stream << "\t\t\t<CANTLFACTURA>" << XMLProtect ( curlc->valor ( "cantlfactura" ) ) << "</CANTLFACTURA>\n";
                stream << "\t\t\t<PVPLFACTURA>" << XMLProtect ( curlc->valor ( "pvplfactura" ) ) << "</PVPLFACTURA>\n";
                stream << "\t\t\t<IVALFACTURA>" << XMLProtect ( curlc->valor ( "ivalfactura" ) ) << "</IVALFACTURA>\n";
                stream << "\t\t\t<DESCUENTOLFACTURA>" << XMLProtect ( curlc->valor ( "descuentolfactura" ) ) << "</DESCUENTOLFACTURA>\n";
                stream << "\t\t\t<IDFACTURA>" << XMLProtect ( curlc->valor ( "idfactura" ) ) << "</IDFACTURA>\n";
                stream << "\t\t\t<IDARTICULO>" << XMLProtect ( curlc->valor ( "idarticulo" ) ) << "</IDARTICULO>\n";
                /// Los datos relacionados con el articulo
                stream << "\t\t\t<CODARTICULO>" << XMLProtect ( curlc->valor ( "codarticulo" ) ) << "</CODARTICULO>\n";
                stream << "\t\t\t<NOMARTICULO>" << XMLProtect ( curlc->valor ( "nomarticulo" ) ) << "</NOMARTICULO>\n";
                stream << "\t\t\t<ABREVARTICULO>" << XMLProtect ( curlc->valor ( "abrevarticulo" ) ) << "</ABREVARTICULO>\n";
                stream << "\t<CODIGOCOMPLETOARTICULO>" << XMLProtect ( curlc->valor ( "codigocompletoarticulo" ) ) << "</CODIGOCOMPLETOARTICULO>\n";
                stream << "\t\t</LFACTURA>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            /// Incorporamos los descuentos de la factura.
            curlc = dbConnection->loadQuery ( "SELECT * FROM dfactura WHERE idfactura = " + curc->valor ( "idfactura" ) );
            while ( !curlc->eof() ) {
                stream << "\t\t<DFACTURA>\n";
                stream << "\t\t\t<IDDFACTURA>" << XMLProtect ( curlc->valor ( "iddfactura" ) ) << "</IDDFACTURA>\n";
                stream << "\t\t\t<CONCEPTDFACTURA>" << XMLProtect ( curlc->valor ( "conceptdfactura" ) ) << "</CONCEPTDFACTURA>\n";
                stream << "\t\t\t<PROPORCIONDFACTURA>" << XMLProtect ( curlc->valor ( "proporciondfactura" ) ) << "</PROPORCIONDFACTURA>\n";
                stream << "\t\t\t<IDFACTURA>" << XMLProtect ( curlc->valor ( "idfactura" ) ) << "</IDFACTURA>\n";
                stream << "\t\t</DFACTURA>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            stream << "</FACTURA>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    /// Hacemos la exportacion de facturas de clientes.
    if ( tipo & IMPORT_PRESUPUESTOSCLIENTE ) {
        QString query = "SELECT * FROM presupuesto ";
        query += " LEFT JOIN almacen ON presupuesto.idalmacen = almacen.idalmacen ";
        query += " LEFT JOIN cliente ON cliente.idcliente = presupuesto.idcliente ";
        query += " LEFT JOIN trabajador ON trabajador.idtrabajador = presupuesto.idtrabajador ";
        query += " LEFT JOIN forma_pago ON forma_pago.idforma_pago = presupuesto.idforma_pago ";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<PRESUPUESTO>\n";
            stream << "\t<IDPRESUPUESTO>" << XMLProtect ( curc->valor ( "idpresupuesto" ) ) << "</IDPRESUPUESTO>\n";
            stream << "\t<NUMPRESUPUESTO>" << XMLProtect ( curc->valor ( "numpresupuesto" ) ) << "</NUMPRESUPUESTO>\n";
            stream << "\t<REFPRESUPUESTO>" << XMLProtect ( curc->valor ( "refpresupuesto" ) ) << "</REFPRESUPUESTO>\n";
            stream << "\t<FPRESUPUESTO>" << XMLProtect ( curc->valor ( "fpresupuesto" ) ) << "</FPRESUPUESTO>\n";
            stream << "\t<DESCPRESUPUESTO>" << XMLProtect ( curc->valor ( "descpresupuesto" ) ) << "</DESCPRESUPUESTO>\n";
            stream << "\t<CONTACTPRESUPUESTO>" << XMLProtect ( curc->valor ( "contactpresupuesto" ) ) << "</CONTACTPRESUPUESTO>\n";
            stream << "\t<TELPRESUPUESTO>" << XMLProtect ( curc->valor ( "telpresupuesto" ) ) << "</TELPRESUPUESTO>\n";
            stream << "\t<VENCPRESUPUESTO>" << XMLProtect ( curc->valor ( "vencpresupuesto" ) ) << "</VENCPRESUPUESTO>\n";
            stream << "\t<COMENTPRESUPUESTO>" << XMLProtect ( curc->valor ( "comentpresupuesto" ) ) << "</COMENTPRESUPUESTO>\n";
            stream << "\t<IDUSUARI>" << XMLProtect ( curc->valor ( "idusuari" ) ) << "</IDUSUARI>\n";
            stream << "\t<PROCESADOPRESUPUESTO>" << XMLProtect ( curc->valor ( "procesadopresupuesto" ) ) << "</PROCESADOPRESUPUESTO>\n";
            stream << "\t<IDCLIENTE>" << XMLProtect ( curc->valor ( "idcliente" ) ) << "</IDCLIENTE>\n";
            stream << "\t<IDALMACEN>" << XMLProtect ( curc->valor ( "idalmacen" ) ) << "</IDALMACEN>\n";
            stream << "\t<IDFORMA_PAGO>" << XMLProtect ( curc->valor ( "idforma_pago" ) ) << "</IDFORMA_PAGO>\n";
            stream << "\t<IDTRABAJADOR>" << XMLProtect ( curc->valor ( "idtrabajador" ) ) << "</IDTRABAJADOR>\n";
            /// Datos iniciales para el presupuesto  que pueden ser de utilidad.
            stream << "\t<CODIGOALMACEN>" << XMLProtect ( curc->valor ( "codigoalmacen" ) ) << "</CODIGOALMACEN>\n";
            stream << "\t<NOMALMACEN>" << XMLProtect ( curc->valor ( "nomalmacen" ) ) << "</NOMALMACEN>\n";
            stream << "\t<DIRALMACEN>" << XMLProtect ( curc->valor ( "diralmacen" ) ) << "</DIRALMACEN>\n";
            stream << "\t<POBLALMACEN>" << XMLProtect ( curc->valor ( "poblalmacen" ) ) << "</POBLALMACEN>\n";
            stream << "\t<CPALMACEN>" << XMLProtect ( curc->valor ( "cpalmacen" ) ) << "</CPALMACEN>\n";
            stream << "\t<TELALMACEN>" << XMLProtect ( curc->valor ( "telalmacen" ) ) << "</TELALMACEN>\n";
            /// Datos referentes al cliente.
            stream << "\t<NOMCLIENTE>" << XMLProtect ( curc->valor ( "nomcliente" ) ) << "</NOMCLIENTE>\n";
            stream << "\t<NOMALTCLIENTE>" << XMLProtect ( curc->valor ( "nomaltcliente" ) ) << "</NOMALTCLIENTE>\n";
            stream << "\t<CIFCLIENTE>" << XMLProtect ( curc->valor ( "cifcliente" ) ) << "</CIFCLIENTE>\n";
            stream << "\t<BANCOCLIENTE>" << XMLProtect ( curc->valor ( "bancocliente" ) ) << "</BANCOCLIENTE>\n";
            stream << "\t<DIRCLIENTE>" << XMLProtect ( curc->valor ( "dircliente" ) ) << "</DIRCLIENTE>\n";
            stream << "\t<POBLCLIENTE>" << XMLProtect ( curc->valor ( "poblcliente" ) ) << "</POBLCLIENTE>\n";
            stream << "\t<CPCLIENTE>" << XMLProtect ( curc->valor ( "cpcliente" ) ) << "</CPCLIENTE>\n";
            stream << "\t<TELCLIENTE>" << XMLProtect ( curc->valor ( "telcliente" ) ) << "</TELCLIENTE>\n";
            stream << "\t<FAXCLIENTE>" << XMLProtect ( curc->valor ( "faxcliente" ) ) << "</FAXCLIENTE>\n";
            stream << "\t<MAILCLIENTE>" << XMLProtect ( curc->valor ( "mailcliente" ) ) << "</MAILCLIENTE>\n";
            stream << "\t<URLCLIENTE>" << XMLProtect ( curc->valor ( "urlcliente" ) ) << "</URLCLIENTE>\n";
            stream << "\t<PROVCLIENTE>" << XMLProtect ( curc->valor ( "provcliente" ) ) << "</PROVCLIENTE>\n";
            /// Datos sobre el trabajador.
            stream << "\t<NOMTRABAJADOR>" << XMLProtect ( curc->valor ( "nomtrabajador" ) ) << "</NOMTRABAJADOR>\n";
            stream << "\t<APELLIDOSTRABAJADOR>" << XMLProtect ( curc->valor ( "apellidostrabajador" ) ) << "</APELLIDOSTRABAJADOR>\n";
            stream << "\t<DIRTRABAJADOR>" << XMLProtect ( curc->valor ( "dirtrabajador" ) ) << "</DIRTRABAJADOR>\n";
            stream << "\t<NSSTRABAJADOR>" << XMLProtect ( curc->valor ( "nsstrabajador" ) ) << "</NSSTRABAJADOR>\n";
            stream << "\t<TELTRABAJADOR>" << XMLProtect ( curc->valor ( "teltrabajador" ) ) << "</TELTRABAJADOR>\n";
            stream << "\t<MOVILTRABAJADOR>" << XMLProtect ( curc->valor ( "moviltrabajador" ) ) << "</MOVILTRABAJADOR>\n";
            stream << "\t<EMAILTRABAJADOR>" << XMLProtect ( curc->valor ( "emailtrabajador" ) ) << "</EMAILTRABAJADOR>\n";
            /// Datos sobre la forma de pago.
            stream << "\t<DESCFORMA_PAGO>" << XMLProtect ( curc->valor ( "descforma_pago" ) ) << "</DESCFORMA_PAGO>\n";
            stream << "\t<DIAS1TFORMA_PAGO>" << XMLProtect ( curc->valor ( "dias1tforma_pago" ) ) << "</DIAS1TFORMA_PAGO>\n";
            stream << "\t<DESCUENTOFORMA_PAGO>" << XMLProtect ( curc->valor ( "descuentoforma_pago" ) ) << "</DESCUENTOFORMA_PAGO>\n";
            /// Incorporamos las lineas de detalles del presupuesto.
            BlDbRecordSet *curlc = dbConnection->loadQuery ( "SELECT * FROM lpresupuesto LEFT JOIN articulo ON lpresupuesto.idarticulo = articulo.idarticulo WHERE idpresupuesto = " + curc->valor ( "idpresupuesto" ) );
            while ( !curlc->eof() ) {
                stream << "\t<LPRESUPUESTO>\n";
                stream << "\t\t<IDLPRESUPUESTO>" << XMLProtect ( curlc->valor ( "idlpresupuesto" ) ) << "</IDLPRESUPUESTO>\n";
                stream << "\t\t\t<DESCLPRESUPUESTO>" << XMLProtect ( curlc->valor ( "desclpresupuesto" ) ) << "</DESCLPRESUPUESTO>\n";
                stream << "\t\t<CANTLPRESUPUESTO>" << XMLProtect ( curlc->valor ( "cantlpresupuesto" ) ) << "</CANTLPRESUPUESTO>\n";
                stream << "\t\t<PVPLPRESUPUESTO>" << XMLProtect ( curlc->valor ( "pvplpresupuesto" ) ) << "</PVPLPRESUPUESTO>\n";
                stream << "\t\t<IVALPRESUPUESTO>" << XMLProtect ( curlc->valor ( "ivalpresupuesto" ) ) << "</IVALPRESUPUESTO>\n";
                stream << "\t\t<DESCUENTOLPRESUPUESTO>" << XMLProtect ( curlc->valor ( "descuentolpresupuesto" ) ) << "</DESCUENTOLPRESUPUESTO>\n";
                stream << "\t\t<IDPRESUPUESTO>" << XMLProtect ( curlc->valor ( "idpresupuesto" ) ) << "</IDPRESUPUESTO>\n";
                stream << "\t\t<IDARTICULO>" << XMLProtect ( curlc->valor ( "idarticulo" ) ) << "</IDARTICULO>\n";
                /// Los datos relacionados con el articulo
                stream << "\t\t<CODARTICULO>" << XMLProtect ( curlc->valor ( "codarticulo" ) ) << "</CODARTICULO>\n";
                stream << "\t\t<NOMARTICULO>" << XMLProtect ( curlc->valor ( "nomarticulo" ) ) << "</NOMARTICULO>\n";
                stream << "\t\t<ABREVARTICULO>" << XMLProtect ( curlc->valor ( "abrevarticulo" ) ) << "</ABREVARTICULO>\n";
                stream << "\t\t<CODIGOCOMPLETOARTICULO>" << XMLProtect ( curlc->valor ( "codigocompletoarticulo" ) ) << "</CODIGOCOMPLETOARTICULO>\n";
                stream << "\t</LPRESUPUESTO>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            /// Incorporamos los descuentos del presupuesto.
            curlc = dbConnection->loadQuery ( "SELECT * FROM dpresupuesto WHERE idpresupuesto = " + curc->valor ( "idpresupuesto" ) );
            while ( !curlc->eof() ) {
                stream << "\t<DPRESUPUESTO>\n";
                stream << "\t\t<IDDPRESUPUESTO>" << XMLProtect ( curlc->valor ( "iddpresupuesto" ) ) << "</IDDPRESUPUESTO>\n";
                stream << "\t\t<CONCEPTDPRESUPUESTO>" << XMLProtect ( curlc->valor ( "conceptdpresupuesto" ) ) << "</CONCEPTDPRESUPUESTO>\n";
                stream << "\t\t<PROPORCIONDPRESUPUESTO>" << XMLProtect ( curlc->valor ( "proporciondpresupuesto" ) ) << "</PROPORCIONDPRESUPUESTO>\n";
                stream << "\t\t<IDPRESUPUESTO>" << XMLProtect ( curlc->valor ( "idpresupuesto" ) ) << "</IDPRESUPUESTO>\n";
                stream << "\t</DPRESUPUESTO>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            stream << "</PRESUPUESTO>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    /// Hacemos la exportacion de facturas de clientes.
    if ( tipo & IMPORT_PEDIDOSCLIENTE ) {
        QString query = "SELECT * FROM pedidocliente ";
        query += " LEFT JOIN almacen ON pedidocliente.idalmacen = almacen.idalmacen ";
        query += " LEFT JOIN cliente ON cliente.idcliente = pedidocliente.idcliente ";
        query += " LEFT JOIN trabajador ON trabajador.idtrabajador = pedidocliente.idtrabajador ";
        query += " LEFT JOIN forma_pago ON forma_pago.idforma_pago = pedidocliente.idforma_pago ";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<PEDIDOCLIENTE>\n";
            stream << "\t<IDPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "idpedidocliente" ) ) << "</IDPEDIDOCLIENTE>\n";
            stream << "\t<NUMPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "numpedidocliente" ) ) << "</NUMPEDIDOCLIENTE>\n";
            stream << "\t<FECHAPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "fechapedidocliente" ) ) << "</FECHAPEDIDOCLIENTE>\n";
            stream << "\t<REFPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "refpedidocliente" ) ) << "</REFPEDIDOCLIENTE>\n";
            stream << "\t<DESCPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "descpedidocliente" ) ) << "</DESCPEDIDOCLIENTE>\n";
            stream << "\t<CONTACTPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "contactpedidocliente" ) ) << "</CONTACTPEDIDOCLIENTE>\n";
            stream << "\t<TELPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "telpedidocliente" ) ) << "</TELPEDIDOCLIENTE>\n";
            stream << "\t<IDPRESUPUESTO>" << XMLProtect ( curc->valor ( "idpresupuesto" ) ) << "</IDPRESUPUESTO>\n";
            stream << "\t<COMENTPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "comentpedidocliente" ) ) << "</COMENTPEDIDOCLIENTE>\n";
            stream << "\t<IDUSUARI>" << XMLProtect ( curc->valor ( "idusuari" ) ) << "</IDUSUARI>\n";
            stream << "\t<PROCESADOPEDIDOCLIENTE>" << XMLProtect ( curc->valor ( "procesadopedidocliente" ) ) << "</PROCESADOPEDIDOCLIENTE>\n";
            stream << "\t<IDCLIENTE>" << XMLProtect ( curc->valor ( "idcliente" ) ) << "</IDCLIENTE>\n";
            stream << "\t<IDALMACEN>" << XMLProtect ( curc->valor ( "idalmacen" ) ) << "</IDALMACEN>\n";
            stream << "\t<IDFORMA_PAGO>" << XMLProtect ( curc->valor ( "idforma_pago" ) ) << "</IDFORMA_PAGO>\n";
            stream << "\t<IDTRABAJADOR>" << XMLProtect ( curc->valor ( "idtrabajador" ) ) << "</IDTRABAJADOR>\n";
            /// Datos iniciales para el presupuesto  que pueden ser de utilidad.
            stream << "\t<CODIGOALMACEN>" << XMLProtect ( curc->valor ( "codigoalmacen" ) ) << "</CODIGOALMACEN>\n";
            stream << "\t<NOMALMACEN>" << XMLProtect ( curc->valor ( "nomalmacen" ) ) << "</NOMALMACEN>\n";
            stream << "\t<DIRALMACEN>" << XMLProtect ( curc->valor ( "diralmacen" ) ) << "</DIRALMACEN>\n";
            stream << "\t<POBLALMACEN>" << XMLProtect ( curc->valor ( "poblalmacen" ) ) << "</POBLALMACEN>\n";
            stream << "\t<CPALMACEN>" << XMLProtect ( curc->valor ( "cpalmacen" ) ) << "</CPALMACEN>\n";
            stream << "\t<TELALMACEN>" << XMLProtect ( curc->valor ( "telalmacen" ) ) << "</TELALMACEN>\n";
            /// Datos referentes al cliente.
            stream << "\t<NOMCLIENTE>" << XMLProtect ( curc->valor ( "nomcliente" ) ) << "</NOMCLIENTE>\n";
            stream << "\t<NOMALTCLIENTE>" << XMLProtect ( curc->valor ( "nomaltcliente" ) ) << "</NOMALTCLIENTE>\n";
            stream << "\t<CIFCLIENTE>" << XMLProtect ( curc->valor ( "cifcliente" ) ) << "</CIFCLIENTE>\n";
            stream << "\t<BANCOCLIENTE>" << XMLProtect ( curc->valor ( "bancocliente" ) ) << "</BANCOCLIENTE>\n";
            stream << "\t<DIRCLIENTE>" << XMLProtect ( curc->valor ( "dircliente" ) ) << "</DIRCLIENTE>\n";
            stream << "\t<POBLCLIENTE>" << XMLProtect ( curc->valor ( "poblcliente" ) ) << "</POBLCLIENTE>\n";
            stream << "\t<CPCLIENTE>" << XMLProtect ( curc->valor ( "cpcliente" ) ) << "</CPCLIENTE>\n";
            stream << "\t<TELCLIENTE>" << XMLProtect ( curc->valor ( "telcliente" ) ) << "</TELCLIENTE>\n";
            stream << "\t<FAXCLIENTE>" << XMLProtect ( curc->valor ( "faxcliente" ) ) << "</FAXCLIENTE>\n";
            stream << "\t<MAILCLIENTE>" << XMLProtect ( curc->valor ( "mailcliente" ) ) << "</MAILCLIENTE>\n";
            stream << "\t<URLCLIENTE>" << XMLProtect ( curc->valor ( "urlcliente" ) ) << "</URLCLIENTE>\n";
            stream << "\t<PROVCLIENTE>" << XMLProtect ( curc->valor ( "provcliente" ) ) << "</PROVCLIENTE>\n";
            /// Datos sobre el trabajador
            stream << "\t<NOMTRABAJADOR>" << XMLProtect ( curc->valor ( "nomtrabajador" ) ) << "</NOMTRABAJADOR>\n";
            stream << "\t<APELLIDOSTRABAJADOR>" << XMLProtect ( curc->valor ( "apellidostrabajador" ) ) << "</APELLIDOSTRABAJADOR>\n";
            stream << "\t<DIRTRABAJADOR>" << XMLProtect ( curc->valor ( "dirtrabajador" ) ) << "</DIRTRABAJADOR>\n";
            stream << "\t<NSSTRABAJADOR>" << XMLProtect ( curc->valor ( "nsstrabajador" ) ) << "</NSSTRABAJADOR>\n";
            stream << "\t<TELTRABAJADOR>" << XMLProtect ( curc->valor ( "teltrabajador" ) ) << "</TELTRABAJADOR>\n";
            stream << "\t<MOVILTRABAJADOR>" << XMLProtect ( curc->valor ( "moviltrabajador" ) ) << "</MOVILTRABAJADOR>\n";
            stream << "\t<EMAILTRABAJADOR>" << XMLProtect ( curc->valor ( "emailtrabajador" ) ) << "</EMAILTRABAJADOR>\n";
            /// Datos sobre la forma de pago
            stream << "\t<DESCFORMA_PAGO>" << XMLProtect ( curc->valor ( "descforma_pago" ) ) << "</DESCFORMA_PAGO>\n";
            stream << "\t<DIAS1TFORMA_PAGO>" << XMLProtect ( curc->valor ( "dias1tforma_pago" ) ) << "</DIAS1TFORMA_PAGO>\n";
            stream << "\t<DESCUENTOFORMA_PAGO>" << XMLProtect ( curc->valor ( "descuentoforma_pago" ) ) << "</DESCUENTOFORMA_PAGO>\n";
            /// Incorporamos las lineas de detalles del presupuesto.
            BlDbRecordSet *curlc = dbConnection->loadQuery ( "SELECT * FROM lpedidocliente LEFT JOIN articulo ON lpedidocliente.idarticulo = articulo.idarticulo WHERE idpedidocliente = " + curc->valor ( "idpedidocliente" ) );
            while ( !curlc->eof() ) {
                stream << "\t\t<LPEDIDOCLIENTE>\n";
                stream << "\t\t\t<IDLPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "idlpedidocliente" ) ) << "</IDLPEDIDOCLIENTE>\n";
                stream << "\t\t\t<DESCLPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "desclpedidocliente" ) ) << "</DESCLPEDIDOCLIENTE>\n";
                stream << "\t\t\t<CANTLPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "cantlpedidocliente" ) ) << "</CANTLPEDIDOCLIENTE>\n";
                stream << "\t\t\t<PVPLPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "pvplpedidocliente" ) ) << "</PVPLPEDIDOCLIENTE>\n";
                stream << "\t\t\t<IVALPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "ivalpedidocliente" ) ) << "</IVALPEDIDOCLIENTE>\n";
                stream << "\t\t\t<DESCUENTOLPEDIDOCLIENTEO>" << XMLProtect ( curlc->valor ( "descuentolpedidocliente" ) ) << "</DESCUENTOLPEDIDOCLIENTEO>\n";
                stream << "\t\t\t<IDPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "idpresupuesto" ) ) << "</IDPEDIDOCLIENTE>\n";
                stream << "\t\t\t<IDARTICULO>" << XMLProtect ( curlc->valor ( "idarticulo" ) ) << "</IDARTICULO>\n";
                /// Los datos relacionados con el articulo.
                stream << "\t\t\t<CODARTICULO>" << XMLProtect ( curlc->valor ( "codarticulo" ) ) << "</CODARTICULO>\n";
                stream << "\t\t\t<NOMARTICULO>" << XMLProtect ( curlc->valor ( "nomarticulo" ) ) << "</NOMARTICULO>\n";
                stream << "\t\t\t<ABREVARTICULO>" << XMLProtect ( curlc->valor ( "abrevarticulo" ) ) << "</ABREVARTICULO>\n";
                stream << "\t<CODIGOCOMPLETOARTICULO>" << XMLProtect ( curlc->valor ( "codigocompletoarticulo" ) ) << "</CODIGOCOMPLETOARTICULO>\n";
                stream << "\t\t</LPRESUPUESTO>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            /// Incorporamos los descuentos del presupuesto.
            curlc = dbConnection->loadQuery ( "SELECT * FROM dpedidocliente WHERE idpedidocliente = " + curc->valor ( "idpedidocliente" ) );
            while ( !curlc->eof() ) {
                stream << "\t\t<DPEDIDOCLIENTE>\n";
                stream << "\t\t\t<IDDPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "iddpedidocliente" ) ) << "</IDDPEDIDOCLIENTE>\n";
                stream << "\t\t\t<CONCEPTDPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "conceptdpedidocliente" ) ) << "</CONCEPTDPEDIDOCLIENTE>\n";
                stream << "\t\t\t<PROPORCIONDPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "proporciondpedidocliente" ) ) << "</PROPORCIONDPEDIDOCLIENTE>\n";
                stream << "\t\t\t<IDPEDIDOCLIENTE>" << XMLProtect ( curlc->valor ( "idpedidocliente" ) ) << "</IDPEDIDOCLIENTE>\n";
                stream << "\t\t</DPEDIDOCLIENTE>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            stream << "</PEDIDOCLIENTE>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if

    /// Hacemos la exportacion de facturas de clientes.
    if ( tipo & IMPORT_ALBARANESCLIENTE ) {
        QString query = "SELECT * FROM albaran ";
        query += " LEFT JOIN almacen ON albaran.idalmacen = almacen.idalmacen ";
        query += " LEFT JOIN cliente ON cliente.idcliente = albaran.idcliente ";
        query += " LEFT JOIN trabajador ON trabajador.idtrabajador = albaran.idtrabajador ";
        query += " LEFT JOIN forma_pago ON forma_pago.idforma_pago = albaran.idforma_pago ";
        BlDbRecordSet *curc = dbConnection->loadQuery ( query );
        while ( !curc->eof() ) {
            stream << "<ALBARAN>\n";
            stream << "\t<IDALBARAN>" << XMLProtect ( curc->valor ( "idalbaran" ) ) << "</IDALBARAN>\n";
            stream << "\t<NUMALBARAN>" << XMLProtect ( curc->valor ( "numalbaran" ) ) << "</NUMALBARAN>\n";
            stream << "\t<DESCALBARAN>" << XMLProtect ( curc->valor ( "descalbaran" ) ) << "</DESCALBARAN>\n";
            stream << "\t<REFALBARAN>" << XMLProtect ( curc->valor ( "refalbaran" ) ) << "</REFALBARAN>\n";
            stream << "\t<FECHAALBARAN>" << XMLProtect ( curc->valor ( "fechaalbaran" ) ) << "</FECHAALBARAN>\n";
            stream << "\t<COMENTALBARAN>" << XMLProtect ( curc->valor ( "comentalbaran" ) ) << "</COMENTALBARAN>\n";
            stream << "\t<PROCESADOALBARAN>" << XMLProtect ( curc->valor ( "procesadoalbaran" ) ) << "</PROCESADOALBARAN>\n";
            stream << "\t<CONTACTALBARAN>" << XMLProtect ( curc->valor ( "contactalbaran" ) ) << "</CONTACTALBARAN>\n";
            stream << "\t<TELALBARAN>" << XMLProtect ( curc->valor ( "telalbaran" ) ) << "</TELALBARAN>\n";
            stream << "\t<IDUSUARI>" << XMLProtect ( curc->valor ( "idusuari" ) ) << "</IDUSUARI>\n";
            stream << "\t<IDCLIENTE>" << XMLProtect ( curc->valor ( "idcliente" ) ) << "</IDCLIENTE>\n";
            stream << "\t<IDALMACEN>" << XMLProtect ( curc->valor ( "idalmacen" ) ) << "</IDALMACEN>\n";
            stream << "\t<IDFORMA_PAGO>" << XMLProtect ( curc->valor ( "idforma_pago" ) ) << "</IDFORMA_PAGO>\n";
            stream << "\t<IDTRABAJADOR>" << XMLProtect ( curc->valor ( "idtrabajador" ) ) << "</IDTRABAJADOR>\n";
            /// Datos iniciales para el albaran  que pueden ser de utilidad.
            stream << "\t<CODIGOALMACEN>" << XMLProtect ( curc->valor ( "codigoalmacen" ) ) << "</CODIGOALMACEN>\n";
            stream << "\t<NOMALMACEN>" << XMLProtect ( curc->valor ( "nomalmacen" ) ) << "</NOMALMACEN>\n";
            stream << "\t<DIRALMACEN>" << XMLProtect ( curc->valor ( "diralmacen" ) ) << "</DIRALMACEN>\n";
            stream << "\t<POBLALMACEN>" << XMLProtect ( curc->valor ( "poblalmacen" ) ) << "</POBLALMACEN>\n";
            stream << "\t<CPALMACEN>" << XMLProtect ( curc->valor ( "cpalmacen" ) ) << "</CPALMACEN>\n";
            stream << "\t<TELALMACEN>" << XMLProtect ( curc->valor ( "telalmacen" ) ) << "</TELALMACEN>\n";
            /// Datos referentes al cliente.
            stream << "\t<NOMCLIENTE>" << XMLProtect ( curc->valor ( "nomcliente" ) ) << "</NOMCLIENTE>\n";
            stream << "\t<NOMALTCLIENTE>" << XMLProtect ( curc->valor ( "nomaltcliente" ) ) << "</NOMALTCLIENTE>\n";
            stream << "\t<CIFCLIENTE>" << XMLProtect ( curc->valor ( "cifcliente" ) ) << "</CIFCLIENTE>\n";
            stream << "\t<BANCOCLIENTE>" << XMLProtect ( curc->valor ( "bancocliente" ) ) << "</BANCOCLIENTE>\n";
            stream << "\t<DIRCLIENTE>" << XMLProtect ( curc->valor ( "dircliente" ) ) << "</DIRCLIENTE>\n";
            stream << "\t<POBLCLIENTE>" << XMLProtect ( curc->valor ( "poblcliente" ) ) << "</POBLCLIENTE>\n";
            stream << "\t<CPCLIENTE>" << XMLProtect ( curc->valor ( "cpcliente" ) ) << "</CPCLIENTE>\n";
            stream << "\t<TELCLIENTE>" << XMLProtect ( curc->valor ( "telcliente" ) ) << "</TELCLIENTE>\n";
            stream << "\t<FAXCLIENTE>" << XMLProtect ( curc->valor ( "faxcliente" ) ) << "</FAXCLIENTE>\n";
            stream << "\t<MAILCLIENTE>" << XMLProtect ( curc->valor ( "mailcliente" ) ) << "</MAILCLIENTE>\n";
            stream << "\t<URLCLIENTE>" << XMLProtect ( curc->valor ( "urlcliente" ) ) << "</URLCLIENTE>\n";
            stream << "\t<PROVCLIENTE>" << XMLProtect ( curc->valor ( "provcliente" ) ) << "</PROVCLIENTE>\n";
            /// Datos sobre el trabajador.
            stream << "\t<NOMTRABAJADOR>" << XMLProtect ( curc->valor ( "nomtrabajador" ) ) << "</NOMTRABAJADOR>\n";
            stream << "\t<APELLIDOSTRABAJADOR>" << XMLProtect ( curc->valor ( "apellidostrabajador" ) ) << "</APELLIDOSTRABAJADOR>\n";
            stream << "\t<DIRTRABAJADOR>" << XMLProtect ( curc->valor ( "dirtrabajador" ) ) << "</DIRTRABAJADOR>\n";
            stream << "\t<NSSTRABAJADOR>" << XMLProtect ( curc->valor ( "nsstrabajador" ) ) << "</NSSTRABAJADOR>\n";
            stream << "\t<TELTRABAJADOR>" << XMLProtect ( curc->valor ( "teltrabajador" ) ) << "</TELTRABAJADOR>\n";
            stream << "\t<MOVILTRABAJADOR>" << XMLProtect ( curc->valor ( "moviltrabajador" ) ) << "</MOVILTRABAJADOR>\n";
            stream << "\t<EMAILTRABAJADOR>" << XMLProtect ( curc->valor ( "emailtrabajador" ) ) << "</EMAILTRABAJADOR>\n";
            /// Datos sobre la forma de pago.
            stream << "\t<DESCFORMA_PAGO>" << XMLProtect ( curc->valor ( "descforma_pago" ) ) << "</DESCFORMA_PAGO>\n";
            stream << "\t<DIAS1TFORMA_PAGO>" << XMLProtect ( curc->valor ( "dias1tforma_pago" ) ) << "</DIAS1TFORMA_PAGO>\n";
            stream << "\t<DESCUENTOFORMA_PAGO>" << XMLProtect ( curc->valor ( "descuentoforma_pago" ) ) << "</DESCUENTOFORMA_PAGO>\n";
            /// Incorporamos las lineas de detalles del presupuesto.
            BlDbRecordSet *curlc = dbConnection->loadQuery ( "SELECT * FROM lalbaran LEFT JOIN articulo ON lalbaran.idarticulo = articulo.idarticulo WHERE idalbaran = " + curc->valor ( "idalbaran" ) );
            while ( !curlc->eof() ) {
                stream << "\t\t<LALBARAN>\n";
                stream << "\t\t\t<IDLALBARAN>" << XMLProtect ( curlc->valor ( "idlalbaran" ) ) << "</IDLALBARAN>\n";
                stream << "\t\t\t<DESCLALBARAN>" << XMLProtect ( curlc->valor ( "desclalbaran" ) ) << "</DESCLPRESUPUESTO>\n";
                stream << "\t\t\t<CANTLALBARANE>" << XMLProtect ( curlc->valor ( "cantlalbaran" ) ) << "</CANTLALBARANE>\n";
                stream << "\t\t\t<PVPLALBARAN>" << XMLProtect ( curlc->valor ( "pvplalbaran" ) ) << "</PVPLALBARAN>\n";
                stream << "\t\t\t<IVALALBARAN>" << XMLProtect ( curlc->valor ( "ivalalbaran" ) ) << "</IVALALBARAN>\n";
                stream << "\t\t\t<DESCUENTOLALBARAN>" << XMLProtect ( curlc->valor ( "descuentolalbaran" ) ) << "</DESCUENTOLALBARAN>\n";
                stream << "\t\t\t<IDALBARAN>" << XMLProtect ( curlc->valor ( "idalbaran" ) ) << "</IDALBARAN>\n";
                stream << "\t\t\t<IDARTICULO>" << XMLProtect ( curlc->valor ( "idarticulo" ) ) << "</IDARTICULO>\n";
                /// Los datos relacionados con el articulo.
                stream << "\t\t\t<CODARTICULO>" << XMLProtect ( curlc->valor ( "codarticulo" ) ) << "</CODARTICULO>\n";
                stream << "\t\t\t<NOMARTICULO>" << XMLProtect ( curlc->valor ( "nomarticulo" ) ) << "</NOMARTICULO>\n";
                stream << "\t\t\t<ABREVARTICULO>" << XMLProtect ( curlc->valor ( "abrevarticulo" ) ) << "</ABREVARTICULO>\n";
                stream << "\t<CODIGOCOMPLETOARTICULO>" << XMLProtect ( curlc->valor ( "codigocompletoarticulo" ) ) << "</CODIGOCOMPLETOARTICULO>\n";
                stream << "\t\t</LALBARAN>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            /// Incorporamos los descuentos del presupuesto.
            curlc = dbConnection->loadQuery ( "SELECT * FROM dalbaran WHERE idalbaran = " + curc->valor ( "idalbaran" ) );
            while ( !curlc->eof() ) {
                stream << "\t\t<DALBARAN>\n";
                stream << "\t\t\t<IDDALBARAN>" << XMLProtect ( curlc->valor ( "iddalbaran" ) ) << "</IDDALBARAN>\n";
                stream << "\t\t\t<CONCEPTDALBARAN>" << XMLProtect ( curlc->valor ( "conceptdalbaran" ) ) << "</CONCEPTDALBARAN>\n";
                stream << "\t\t\t<PROPORCIONDALBARAN>" << XMLProtect ( curlc->valor ( "proporciondalbaran" ) ) << "</PROPORCIONDALBARAN>\n";
                stream << "\t\t\t<IDALBARAN>" << XMLProtect ( curlc->valor ( "idalbaran" ) ) << "</IDALBARAN>\n";
                stream << "\t\t</DALBARAN>\n";
                curlc->nextRecord();
            } // end while
            delete curlc;
            stream << "</ALBARAN>\n";
            curc->nextRecord();
        } // end while
        delete curc;
    } // end if
    stream << "</FUGIT>\n";
    alerta ( 100, 100 );
    _depura ( "END BlImportExport::bulmafact2XML", 0 );
    return 0;
}


/// Esta funcion se encarga de pasar los datos de BulmaGes a XML.
/// Los datos pasados de esta forma son mucho mas sencillos de pasar.
/**
\param xmlfile
\param tipo
\return
**/
int BlImportExport::bulmages2XML ( QFile &xmlfile, unsigned long long int tipo )
{
    _depura ( "BlImportExport::bulmages2XML", 0 );
    QString codigo, descripcion;
    QString strblancomax;
    QString query;
    int numreg = 0;
    QTextStream stream ( &xmlfile );
    stream << "<?xml version=\"1.0\" encoding = \"iso-8859-1\"?>\n"
    "<!DOCTYPE FUGIT>\n"
    "<FUGIT version='0.3.1' origen='BulmaGes'"
    " date='" << QDate().toString ( Qt::ISODate ) << "'>\n";

    /// Comprobamos que tenemos que importar cuentas o no
    if ( tipo & IMPORT_CUENTAS ) {
        /// Se exporta todo el plan contable
        query = "SELECT * FROM cuenta WHERE padre ISNULL ORDER BY codigo";
        BlDbRecordSet *curcta = dbConnection->loadQuery ( query );
        while ( !curcta->eof() ) {
            stream << "<CUENTA>\n";
            stream << "\t<IDCUENTA>" << XMLProtect ( curcta->valor ( "idcuenta" ) ) << "</IDCUENTA>\n";
            stream << "\t<CODIGO>" << XMLProtect ( curcta->valor ( "codigo" ) ) << "</CODIGO>\n";
            stream << "\t<DESCRIPCION>" << XMLProtect ( curcta->valor ( "descripcion" ) ) << "</DESCRIPCION>\n";
            stream << "\t<CIFENT_CUENTA>" << XMLProtect ( curcta->valor ( "cifent_cuenta" ) ) << "</CIFENT_CUENTA>\n";
            stream << "\t<DIRENT_CUENTA>" << XMLProtect ( curcta->valor ( "dirent_cuenta" ) ) << "</DIRENT_CUENTA>\n";
            stream << "\t<BLOQUEADA>" << XMLProtect ( curcta->valor ( "bloqueada" ) ) << "</BLOQUEADA>\n";
            stream << "\t<NODEBE>" << XMLProtect ( curcta->valor ( "nodebe" ) ) << "</NODEBE>\n";
            stream << "\t<NOHABER>" << XMLProtect ( curcta->valor ( "nohaber" ) ) << "</NOHABER>\n";
            stream << "\t<NOMBREENT_CUENTA>" << XMLProtect ( curcta->valor ( "nombreent_cuenta" ) ) << "</NOMBREENT_CUENTA>\n";
            stream << "\t<TIPOCUENTA>" << XMLProtect ( curcta->valor ( "tipocuenta" ) ) << "</TIPOCUENTA>\n";
            stream << "\t<WEBENT_CUENTA>" << XMLProtect ( curcta->valor ( "webent_cuenta" ) ) << "</WEBENT_CUENTA>\n";
            stream << "\t<EMAILENT_CUENTA>" << XMLProtect ( curcta->valor ( "emailent_cuenta" ) ) << "</EMAILENT_CUENTA>\n";
            stream << "\t<BANCOENT_CUENTA>" << XMLProtect ( curcta->valor ( "bancoent_cuenta" ) ) << "</BANCOENT_CUENTA>\n";
            stream << "\t<COMENT_CUENTA>" << XMLProtect ( curcta->valor ( "coment_cuenta" ) ) << "</COMENT_CUENTA>\n";
            stream << "\t<TELENT_CUENTA>" << XMLProtect ( curcta->valor ( "telent_cuenta" ) ) << "</TELENT_CUENTA>\n";
            stream << "\t<CPENT_CUENTA>" << XMLProtect ( curcta->valor ( "cpent_cuenta" ) ) << "</CPENT_CUENTA>\n";
            stream << "\t<REGULARIZACION>" << XMLProtect ( curcta->valor ( "regularizacion" ) ) << "</REGULARIZACION>\n";
            stream << "\t<IMPUTACION>" << XMLProtect ( curcta->valor ( "imputacion" ) ) << "</IMPUTACION>\n";
            stream << "</CUENTA>\n";
            curcta->nextRecord();
        } // end while
        delete curcta;
        query = "SELECT * FROM cuenta LEFT JOIN (SELECT codigo AS codpadre, idcuenta as idpadre FROM cuenta) AS t1 ON cuenta.padre = t1.idpadre WHERE padre IS NOT NULL ORDER BY idpadre";
        curcta = dbConnection->loadQuery ( query );
        while ( !curcta->eof() ) {
            stream << "<CUENTA>\n";
            stream << "\t<IDCUENTA>" << XMLProtect ( curcta->valor ( "idcuenta" ) ) << "</IDCUENTA>\n";
            stream << "\t<CODIGO>" << XMLProtect ( curcta->valor ( "codigo" ) ) << "</CODIGO>\n";
            stream << "\t<DESCRIPCION>" << XMLProtect ( curcta->valor ( "descripcion" ) ) << "</DESCRIPCION>\n";
            stream << "\t<CIFENT_CUENTA>" << XMLProtect ( curcta->valor ( "cifent_cuenta" ) ) << "</CIFENT_CUENTA>\n";
            stream << "\t<DIRENT_CUENTA>" << XMLProtect ( curcta->valor ( "dirent_cuenta" ) ) << "</DIRENT_CUENTA>\n";
            stream << "\t<CODPADRE>" << XMLProtect ( curcta->valor ( "codpadre" ) ) << "</CODPADRE>\n";
            stream << "\t<BLOQUEADA>" << XMLProtect ( curcta->valor ( "bloqueada" ) ) << "</BLOQUEADA>\n";
            stream << "\t<NODEBE>" << XMLProtect ( curcta->valor ( "nodebe" ) ) << "</NODEBE>\n";
            stream << "\t<NOHABER>" << XMLProtect ( curcta->valor ( "nohaber" ) ) << "</NOHABER>\n";
            stream << "\t<NOMBREENT_CUENTA>" << XMLProtect ( curcta->valor ( "nombreent_cuenta" ) ) << "</NOMBREENT_CUENTA>\n";
            stream << "\t<TIPOCUENTA>" << XMLProtect ( curcta->valor ( "tipocuenta" ) ) << "</TIPOCUENTA>\n";
            stream << "\t<WEBENT_CUENTA>" << XMLProtect ( curcta->valor ( "webent_cuenta" ) ) << "</WEBENT_CUENTA>\n";
            stream << "\t<EMAILENT_CUENTA>" << XMLProtect ( curcta->valor ( "emailent_cuenta" ) ) << "</EMAILENT_CUENTA>\n";
            stream << "\t<BANCOENT_CUENTA>" << XMLProtect ( curcta->valor ( "bancoent_cuenta" ) ) << "</BANCOENT_CUENTA>\n";
            stream << "\t<COMENT_CUENTA>" << XMLProtect ( curcta->valor ( "coment_cuenta" ) ) << "</COMENT_CUENTA>\n";
            stream << "\t<TELENT_CUENTA>" << XMLProtect ( curcta->valor ( "telent_cuenta" ) ) << "</TELENT_CUENTA>\n";
            stream << "\t<CPENT_CUENTA>" << XMLProtect ( curcta->valor ( "cpent_cuenta" ) ) << "</CPENT_CUENTA>\n";
            stream << "\t<REGULARIZACION>" << XMLProtect ( curcta->valor ( "regularizacion" ) ) << "</REGULARIZACION>\n";
            stream << "\t<IMPUTACION>" << XMLProtect ( curcta->valor ( "imputacion" ) ) << "</IMPUTACION>\n";
            stream << "</CUENTA>\n";
            curcta->nextRecord();
        } // end while
        delete curcta;
    } // end if

    /// Se va a comprobar si hay que exportar los tipos de IVA.
    if ( tipo & IMPORT_TIPOSIVA ) {
        /// Se vana exportar los tipos de IVA.
        query = "SELECT * from tipoiva LEFT JOIN cuenta ON cuenta.idcuenta = tipoiva.idcuenta";
        BlDbRecordSet *curtiva = dbConnection->loadQuery ( query );
        while ( !curtiva->eof() ) {
            stream << "<TIPOIVA>\n";
            stream << "\t<IDTIPOIVA>" << XMLProtect ( curtiva->valor ( "idtipoiva" ) ) << "</IDTIPOIVA>\n";
            stream << "\t<NOMBRETIPOIVA>" << XMLProtect ( curtiva->valor ( "nombretipoiva" ) ) << "</NOMBRETIPOIVA>\n";
            stream << "\t<PORCENTAJETIPOIVA>" << XMLProtect ( curtiva->valor ( "porcentajetipoiva" ) ) << "</PORCENTAJETIPOIVA>\n";
            stream << "\t<CUENTATIPOIVA>" << XMLProtect ( curtiva->valor ( "codigo" ) ) << "</CUENTATIPOIVA>\n";
            stream << "</TIPOIVA>\n";
            curtiva->nextRecord();
        } // end while
        delete curtiva;
    } // end if

    if ( tipo & IMPORT_ASIENTOS ) {
        /// Hacemos la exportacion de asientos
        /// Montamos el query
        query = "SELECT * FROM asiento WHERE 1 = 1 ";
        if ( m_fInicial != "" )
            query += " AND asiento.fecha >= '" + m_fInicial + "'";
        if ( m_fFinal != "" )
            query += " AND asiento.fecha <= '" + m_fFinal + "'";
        query += " ORDER BY asiento.fecha, asiento.idasiento ";
        BlDbRecordSet *curas = dbConnection->loadQuery ( query );
        int i = 0;
        /// Iteramos.
        numreg = curas->numregistros() + 1;
        while ( !curas->eof() ) {
            alerta ( i++, numreg );
            stream << "<ASIENTO>\n";
            stream << "\t<ORDENASIENTO>" << curas->valor ( "ordenasiento" ) << "</ORDENASIENTO>\n";
            QString fechas = curas->valor ( "fecha" );
            fechas = fechas.mid ( 6, 4 ) + fechas.mid ( 3, 2 ) + fechas.mid ( 0, 2 );
            stream << "\t<FECHA>" << fechas << "</FECHA>\n";
            query = "SELECT * FROM apunte LEFT JOIN cuenta ON apunte.idcuenta = cuenta.idcuenta ";
            query += "LEFT JOIN (SELECT nombre AS nomcanal, idcanal FROM canal) AS canal1 ON apunte.idcanal = canal1.idcanal ";
            query += "LEFT JOIN (SELECT nombre AS nc_coste, idc_coste FROM c_coste) AS c_coste1 ON c_coste1.idc_coste = apunte.idc_coste ";
            query += "LEFT JOIN (SELECT codigo AS codcontrapartida, idcuenta FROM cuenta) AS contra ON contra.idcuenta = apunte.contrapartida ";
            query += " WHERE " + curas->valor ( "idasiento" ) + " = apunte.idasiento ";
            BlDbRecordSet *curap = dbConnection->loadQuery ( query, "masquery1" );
            while ( !curap->eof() ) {
                stream << "\t<APUNTE>\n";
                QString fecha = curap->valor ( "fecha" );
                fecha = fecha.mid ( 6, 4 ) + fecha.mid ( 3, 2 ) + fecha.mid ( 0, 2 );
                stream << "\t\t<FECHA>" << XMLProtect ( fecha ) << "</FECHA>\n";
                stream << "\t\t<CODIGO>" << XMLProtect ( curap->valor ( "codigo" ) ) << "</CODIGO>\n";
                stream << "\t\t<DEBE>" << XMLProtect ( curap->valor ( "debe" ) ) << "</DEBE>\n";
                stream << "\t\t<HABER>" << XMLProtect ( curap->valor ( "haber" ) ) << "</HABER>\n";
                stream << "\t\t<CONCEPTOCONTABLE>" << XMLProtect ( curap->valor ( "conceptocontable" ) ) << "</CONCEPTOCONTABLE>\n";
                stream << "\t\t<IDCANAL>" << XMLProtect ( curap->valor ( "idcanal" ) ) << "</IDCANAL>\n";
                stream << "\t\t<CANAL>" << XMLProtect ( curap->valor ( "nomcanal" ) ) << "</CANAL>\n";
                stream << "\t\t<IDC_COSTE>" << XMLProtect ( curap->valor ( "idc_coste" ) ) << "</IDC_COSTE>\n";
                stream << "\t\t<C_COSTE>" << XMLProtect ( curap->valor ( "nc_coste" ) ) << "</C_COSTE>\n";
                stream << "\t\t<PUNTEO>" << XMLProtect ( curap->valor ( "punteo" ) ) << "</PUNTEO>\n";
                stream << "\t\t<ORDEN>" << XMLProtect ( curap->valor ( "orden" ) ) << "</ORDEN>\n";
                stream << "\t\t<CONTRAPARTIDA>" << XMLProtect ( curap->valor ( "codcontrapartida" ) ) << "</CONTRAPARTIDA>\n";
                /// Hacemos la exportacion de registros de IVA.
                query  = "SELECT * FROM registroiva";
                query += " LEFT JOIN (SELECT codigo, idcuenta FROM cuenta) AS t1 ON registroiva.contrapartida = t1.idcuenta ";
                query += " WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=" + curas->valor ( "idasiento" ) + " AND orden = " + curap->valor ( "orden" ) + ")";
                BlDbRecordSet *curreg = dbConnection->loadQuery ( query, "queryregiva" );
                while ( !curreg->eof() ) {
                    stream << "\t\t<REGISTROIVA>\n";
                    stream << "\t\t\t<CONTRAPARTIDA>" << XMLProtect ( curreg->valor ( "codigo" ) ) << "</CONTRAPARTIDA>\n";
                    stream << "\t\t\t<BASEIMP>" << XMLProtect ( curreg->valor ( "baseimp" ) ) << "</BASEIMP>\n";
                    stream << "\t\t\t<IVA>" << XMLProtect ( curreg->valor ( "iva" ) ) << "</IVA>\n";
                    stream << "\t\t\t<FFACTURA>" << XMLProtect ( curreg->valor ( "ffactura" ) ) << "</FFACTURA>\n";
                    stream << "\t\t\t<FACTURA>" << XMLProtect ( curreg->valor ( "factura" ) ) << "</FACTURA>\n";
                    stream << "\t\t\t<NUMORDEN>" << XMLProtect ( curreg->valor ( "numorden" ) ) << "</NUMORDEN>\n";
                    stream << "\t\t\t<CIF>" << XMLProtect ( curreg->valor ( "cif" ) ) << "</CIF>\n";
                    stream << "\t\t\t<IDFPAGO>" << XMLProtect ( curreg->valor ( "idfpago" ) ) << "</IDFPAGO>\n";
                    stream << "\t\t\t<RECTIFICAAREGISTROIVA>" << XMLProtect ( curreg->valor ( "rectificaaregistroiva" ) ) << "</RECTIFICAAREGISTROIVA>\n";
                    /// Hacemos la exportacion de IVAs.
                    query  = "SELECT * FROM iva ";
                    query += " LEFT JOIN tipoiva ON iva.idtipoiva = tipoiva.idtipoiva ";
                    query += " WHERE idregistroiva = " + curreg->valor ( "idregistroiva" );
                    BlDbRecordSet *curiva = dbConnection->loadQuery ( query, "queryiva" );
                    while ( !curiva->eof() ) {
                        stream << "\t\t\t<RIVA>\n";
                        stream << "\t\t\t\t<IDTIPOIVA>" << XMLProtect ( curiva->valor ( "idtipoiva" ) ) << "</IDTIPOIVA>\n";
                        stream << "\t\t\t\t<NOMBRETIPOIVA>" << XMLProtect ( curiva->valor ( "nombretipoiva" ) ) << "</NOMBRETIPOIVA>\n";
                        stream << "\t\t\t\t<BASEIVA>" << XMLProtect ( curiva->valor ( "baseiva" ) ) << "</BASEIVA>\n";
                        stream << "\t\t\t</RIVA>\n";
                        curiva->nextRecord();
                    } // end while
                    delete curiva;
                    stream << "\t\t</REGISTROIVA>\n";
                    curreg->nextRecord();
                } // end while
                delete curreg;
                mensajeria ( _( "Exportando :" ) + curap->valor ( "codigo" ) + "--" + fecha + "\n" );
                curap->nextRecord();
                stream << "\t</APUNTE>\n";
            } // end while
            delete curap;
            stream << "</ASIENTO>\n";
            curas->nextRecord();
        } // end while
        delete curas;
    } // end if
    stream << "</FUGIT>\n";
    alerta ( numreg, numreg );
    _depura ( "END BlImportExport::bulmages2XML", 0 );
    return 0;
}


/// Funcion para pasar de un archivo XML a Bulmages.
/** Crea un objeto del tipo \ref StructureParser (sistema de proceso de XML mediante SAX)
    y lo ejecuta para que haga la imporacion del archivo XML. */
/**
\param fichero
\param tip
\return
**/
int BlImportExport::XML2Bulmages ( QFile &fichero, unsigned long long int tip )
{
    _depura ( "BlImportExport::XML2Bulmages", 0 );
    StructureParser handler ( dbConnection, tip );
    QXmlInputSource source ( &fichero );
    QXmlSimpleReader reader;
    reader.setContentHandler ( &handler );
    reader.parse ( source );
    alerta ( 100, 100 );
    _depura ( "END BlImportExport::XML2Bulmages", 0 );
    return 0;
}


/// Funcion para pasar de un archivo XML a BulmaFact.
/** Crea un objeto del tipo \ref StructureParser (sistema de proceso de XML mediante SAX) y lo ejecuta para
    que haga la imporacion del archivo XML. */
/**
\param fichero
\param tip
\return
**/
int BlImportExport::XML2BulmaFact ( QFile &fichero, unsigned long long int tip )
{
    _depura ( "BlImportExport::XML2BulmaFact", 0 );
    bool noerror = TRUE;
    ImportBulmaFact handler ( this, dbConnection, tip );
    QXmlInputSource source ( &fichero );
    QXmlSimpleReader reader;
    reader.setContentHandler ( &handler );
    noerror = reader.parse ( source );
    if ( !noerror ) {
        QMessageBox::warning ( 0, _( "Application name" ), _( "Error en el XML.\nDocumento mal formado." ), "OK", 0, 0 );
    }
    alerta ( 100, 100 );
    _depura ( "END BlImportExport::XML2BulmaFact", 0 );
    return 0;
}


///
/**
\param con
\param tip
**/
StructureParser::StructureParser ( BlPostgreSqlClient *con, unsigned int tip )
{
    _depura ( "StructureParser::StructureParser", 0 );
    dbConnection = con;
    m_tipo = tip;
    dbConnection->begin();
    QString query = "INSERT INTO cuenta (codigo, descripcion) VALUES ('AUX', 'Una descripcion auxiliar de cuenta')";
    dbConnection->runQuery ( query );
    for ( int i = 0; i <= 12; i++ ) {
        QString query2 = "INSERT INTO ejercicios (ejercicio, periodo, bloqueado) VALUES (2005, " + QString::number ( i ) + ", FALSE)";
        dbConnection->runQuery ( query2 );
    } // end for
    dbConnection->commit();
    _depura ( "END StructureParser::StructureParser", 0 );
}


///
/**
**/
StructureParser::~StructureParser()
{
    _depura ( "StructureParser::~StructureParser", 0 );
    QString query = "DELETE FROM cuenta WHERE codigo = 'AUX'";
    dbConnection->runQuery ( query );
    _depura ( "END StructureParser::~StructureParser", 0 );
}

///
/**
\return
**/
bool StructureParser::startDocument()
{
    _depura ( "StructureParser::startDocument", 0 );
    indent = "";
    _depura ( "END StructureParser::startDocument", 0 );
    return TRUE;
}


///
/**
\param qName
\param QXmlAttributes
\return
**/
bool StructureParser::startElement ( const QString&, const QString&, const QString& qName, const QXmlAttributes& )
{
    _depura ( "StructureParser::startElement", 0, qName );
    indent += "..";
    if ( qName == "ASIENTO" && m_tipo & IMPORT_ASIENTOS ) {
        tagpadre = "ASIENTO";
        QString query = "INSERT INTO ASIENTO (descripcion, fecha) VALUES ('un nuevo asiento', '01/01/2005')";
        dbConnection->begin();
        dbConnection->runQuery ( query );
        BlDbRecordSet *cur = dbConnection->loadQuery ( "SELECT MAX(idasiento) AS max FROM asiento", "otroquery" );
        dbConnection->commit();
        if ( !cur->eof() ) {
            idasiento = cur->valor ( "max" );
        } // end if
        /// Iniciamos el orden para que los apuntes salgan en orden empezando desde cero.
        m_ordenapunte = 0;
        delete cur;
    } // end if
    if ( qName == "APUNTE" && m_tipo & IMPORT_ASIENTOS ) {
        QString query = "INSERT INTO borrador (idasiento, debe, haber, idcuenta, fecha, orden) VALUES (" + idasiento + ", 0, 0, id_cuenta('AUX'), '01/01/2003', " + QString::number ( m_ordenapunte++ ) + ")";
        dbConnection->begin();
        dbConnection->runQuery ( query );
        BlDbRecordSet *cur = dbConnection->loadQuery ( "SELECT MAX(idborrador) AS max FROM borrador", "otroquery1" );
        dbConnection->commit();
        if ( !cur->eof() ) {
            idborrador = cur->valor ( "max" );
        } // end if
        delete cur;
        tagpadre = "APUNTE";
    } // end if
    if ( qName == "REGISTROIVA" && m_tipo & IMPORT_FACTURAS ) {
        QString query = "INSERT INTO registroiva (contrapartida, idborrador) VALUES (id_cuenta('AUX'), " + idborrador + ")";
        dbConnection->begin();
        dbConnection->runQuery ( query );
        BlDbRecordSet *cur = dbConnection->loadQuery ( "SELECT MAX(idregistroiva) AS max FROM registroiva", "otroquery13" );
        dbConnection->commit();
        if ( !cur->eof() ) {
            m_idRegistroIva = cur->valor ( "max" );
        } // end if
        delete cur;
        tagpadre = "REGISTROIVA";
    } // end if
    if ( qName == "RIVA" && m_tipo & IMPORT_FACTURAS ) {
        tagpadre = "RIVA";
    } // end if
    if ( qName == "CUENTA" && m_tipo & IMPORT_CUENTAS ) {
        tagpadre = "CUENTA";
        /// Borramos todos los elementos para que no haya traspasos de informacion.
        codigocuenta = "";
        descripcioncuenta = "";
        codigopadre = "";
        m_bloqueadaCuenta = "";
        m_nodebeCuenta = "";
        m_nohaberCuenta = "";
        m_tipoCuenta = "";
    } // end if
    cadintermedia = "";
    _depura ( "END StructureParser::startElement", 0 );
    return TRUE;
}


///
/**
\param qName
\return
**/
bool StructureParser::endElement ( const QString&, const QString&, const QString& qName )
{
    _depura ( "StructureParser::endElement", 0, qName );
    indent.remove ( ( uint ) 0, 2 );
    /// Vamos a ir distinguiendo casos y actuando segun cada caso.
    /// En la mayoria de casos iremos actuando en consecuencia.
    /// Ha terminado un asiento, por tanto hacemos el update de los campos.
    if ( qName == "ASIENTO" && m_tipo & IMPORT_ASIENTOS ) {
        QString query = "UPDATE asiento set fecha = '" +
                        dbConnection->sanearCadena ( XMLDesProtect ( fechaasiento ) ) + "' WHERE idasiento = " +
                        dbConnection->sanearCadena ( XMLDesProtect ( idasiento ) );
        dbConnection->begin();
        dbConnection->runQuery ( query );
        BlDbRecordSet *cur = dbConnection->loadQuery ( "SELECT cierraasiento(" + idasiento + ")" );
        dbConnection->commit();
        delete cur;
    } // end if
    /// Si es un apunte hacemos su insercion.
    if ( qName == "APUNTE" && m_tipo & IMPORT_ASIENTOS ) {
        QString query = "UPDATE borrador SET debe = " +
                        dbConnection->sanearCadena ( debeapunte ) + ", haber = " +
                        dbConnection->sanearCadena ( haberapunte ) + ", idcuenta = id_cuenta('" +
                        dbConnection->sanearCadena ( codigocuentaapunte ) + "'), fecha = '" +
                        dbConnection->sanearCadena ( fechaapunte ) + "', conceptocontable = '" +
                        dbConnection->sanearCadena ( XMLDesProtect ( conceptocontableapunte ) ) + "' WHERE idborrador = " + idborrador;
        dbConnection->runQuery ( query );
    } // end if
    if ( qName == "FECHA" && tagpadre == "ASIENTO" )
        fechaasiento = cadintermedia;
    if ( qName == "FECHA" && tagpadre == "APUNTE" )
        fechaapunte = cadintermedia;
    if ( qName == "DEBE" && tagpadre == "APUNTE" )
        debeapunte = cadintermedia;
    if ( qName == "HABER" && tagpadre == "APUNTE" )
        haberapunte = cadintermedia;
    if ( qName == "CODIGO" && tagpadre == "APUNTE" )
        codigocuentaapunte = cadintermedia;
    if ( qName == "CONCEPTOCONTABLE" && tagpadre == "APUNTE" )
        conceptocontableapunte = cadintermedia;
    if ( qName == "CUENTA" && m_tipo & IMPORT_CUENTAS ) {
        /// Ha terminado una cuenta, por tanto hacemos la insercion de la misma.
        /// Podemos hacer la insercion y no un sistema de update pq la cuenta no tiene hijos en el XML.
        /// Nuevo Socio M.Mezo
        QString idgrupo = codigocuenta.left ( 1 );
        /// Primero debemos determinar si existe o no dicha cuenta para hacer la insercion o la modificacion.
        QString vidcuenta;
        if ( codigopadre != "" )  {
            vidcuenta = "id_cuenta('" + dbConnection->sanearCadena ( XMLDesProtect ( codigopadre ) ) + "')";
        } else {
            vidcuenta = "NULL";
        } // end if
        /// Si el tipo de cuenta esta vacio lo ponemos a NULL para que no haya error en la base de datos.
        if ( m_tipoCuenta == "" )
            m_tipoCuenta = "NULL";

        QString query = "SELECT * FROM cuenta WHERE codigo = '" + codigocuenta + "'";
        BlDbRecordSet *cur = dbConnection->loadQuery ( query );
        if ( cur->eof() ) {
            QString query = "INSERT INTO cuenta (tipocuenta, codigo, descripcion, padre, bloqueada, nodebe, nohaber) VALUES (" +
                            dbConnection->sanearCadena ( m_tipoCuenta ) + ",'" +
                            dbConnection->sanearCadena ( codigocuenta ) + "','" +
                            dbConnection->sanearCadena ( XMLDesProtect ( descripcioncuenta ) ) + "', " +
                            vidcuenta + ", '" +
                            dbConnection->sanearCadena ( m_bloqueadaCuenta ) + "','" +
                            dbConnection->sanearCadena ( m_nodebeCuenta ) + "','" +
                            dbConnection->sanearCadena ( m_nohaberCuenta ) + "')";
            dbConnection->runQuery ( query );
        } else {
            QString query = "UPDATE cuenta SET ";
            query += "descripcion = '" + dbConnection->sanearCadena ( XMLDesProtect ( descripcioncuenta ) ) + "'";
            query += ", tipocuenta = " + dbConnection->sanearCadena ( m_tipoCuenta );
            query += ", bloqueada = '" + dbConnection->sanearCadena ( m_bloqueadaCuenta ) + "'";
            query += ", nodebe = '" + dbConnection->sanearCadena ( m_nodebeCuenta ) + "'";
            query += ", nohaber = '" + dbConnection->sanearCadena ( m_nohaberCuenta ) + "'";
            query += " WHERE codigo = '" + dbConnection->sanearCadena ( codigocuenta ) + "'";
            dbConnection->runQuery ( query );
        } //end if
        delete cur;
    } // end if
    if ( qName == "CODIGO" && tagpadre == "CUENTA" )
        codigocuenta = cadintermedia;
    if ( qName == "DESCRIPCION" && tagpadre == "CUENTA" )
        descripcioncuenta = cadintermedia;
    if ( qName == "CODPADRE" && tagpadre == "CUENTA" )
        codigopadre = cadintermedia;
    if ( qName == "BLOQUEADA" && tagpadre == "CUENTA" )
        m_bloqueadaCuenta = cadintermedia;
    if ( qName == "NODEBE" && tagpadre == "CUENTA" )
        m_nodebeCuenta = cadintermedia;
    if ( qName == "NOHABER" && tagpadre == "CUENTA" )
        m_nohaberCuenta = cadintermedia;
    if ( qName == "TIPOCUENTA" && tagpadre == "CUENTA" )
        m_tipoCuenta = cadintermedia;
    /// Si es un registro de iva vamos a por el.
    if ( qName == "REGISTROIVA" && m_tipo & IMPORT_FACTURAS ) {
        QString query = "UPDATE registroiva SET contrapartida = id_cuenta('" + m_rIvaContrapartida + "'), ffactura = '" + m_rIvaFFactura + "'";
        if ( m_rIvaBaseImp != "" )
            query += ", baseimp = " + m_rIvaBaseImp;
        if ( m_rIvaIva != "" )
            query += ", iva = " + m_rIvaIva;
        query += " WHERE idregistroiva = " + m_idRegistroIva;
        dbConnection->runQuery ( query );
    } // end if
    if ( qName == "CONTRAPARTIDA" && tagpadre == "REGISTROIVA" )
        m_rIvaContrapartida = cadintermedia;
    if ( qName == "BASEIMP" && tagpadre == "REGISTROIVA" )
        m_rIvaBaseImp = cadintermedia;
    if ( qName == "IVA" && tagpadre == "REGISTROIVA" )
        m_rIvaIva = cadintermedia;
    if ( qName == "FFACTURA" && tagpadre == "REGISTROIVA" )
        m_rIvaFFactura = cadintermedia;
    if ( qName == "FACTURA" && tagpadre == "REGISTROIVA" )
        m_rIvaFactura = cadintermedia;
    if ( qName == "CIF" && tagpadre == "REGISTROIVA" )
        m_rIvaCIF = cadintermedia;
    if ( qName == "IDFPAGO" && tagpadre == "REGISTROIVA" )
        m_rIvaIdFPago = cadintermedia;
    if ( qName == "RECTIFICAAREGISTROIVA" && tagpadre == "REGISTROIVA" )
        m_rIvRecRegIva = cadintermedia;
    /// Insercion de IVA's dentro del registro de IVA.
    if ( qName == "RIVA" && m_tipo & IMPORT_FACTURAS ) {
        QString query1 = "SELECT idtipoiva FROM tipoiva WHERE nombretipoiva = '" + m_nombreTipoIva + "'";
        dbConnection->begin();
        BlDbRecordSet * cur = dbConnection->loadQuery ( query1, "elqueryd" );
        if ( !cur->eof() ) {
            QString query = "INSERT INTO IVA (idregistroiva, idtipoiva, baseiva) VALUES (" + m_idRegistroIva + ", " + cur->valor ( "idtipoiva" ) + ", " + m_baseIva + ")";
            dbConnection->runQuery ( query );
        } // end if
        delete cur;
        dbConnection->commit();
    } // end if
    if ( qName == "IDTIPOIVA" && tagpadre == "RIVA" )
        m_idTipoIva = cadintermedia;
    if ( qName == "NOMBRETIPOIVA" && tagpadre == "RIVA" )
        m_nombreTipoIva = cadintermedia;
    if ( qName == "BASEIVA" && tagpadre == "RIVA" )
        m_baseIva = cadintermedia;
    cadintermedia = "";
    _depura ( "END StructureParser::endElement", 0 );
    return TRUE;
}

///
/**
\param nl
\return
**/
bool StructureParser::characters ( const QString& n1 )
{
    _depura ( "StructureParser::characters", 0 );
    cadintermedia += n1;
    _depura ( "END StructureParser::characters", 0 );
    return TRUE;
}


/// ==================================================================================
///
/**
\param imp
\param con
\param tip
**/
ImportBulmaFact::ImportBulmaFact ( BlImportExport *imp, BlPostgreSqlClient *con, unsigned long long int tip )
{
    _depura ( "ImportBulmaFact::ImportBulmaFact", 0 );
    dbConnection = con;
    pgimport = imp;
    m_tipo = tip;
    _depura ( "END ImportBulmaFact::ImportBulmaFact", 0 );
}


///
/**
**/
ImportBulmaFact::~ImportBulmaFact()
{
    _depura ( "ImportBulmaFact::~ImportBulmaFact", 0 );
    _depura ( "END ImportBulmaFact::~ImportBulmaFact", 0 );
}


///
/**
\return
**/
bool ImportBulmaFact::startDocument()
{
    _depura ( "ImportBulmaFact::startDocument", 0 );
    indent = "";
    _depura ( "END ImportBulmaFact::startDocument", 0 );
    return TRUE;
}


///
/**
\return
**/
bool ImportBulmaFact::startElement ( const QString&, const QString&, const QString&, const QXmlAttributes& )
{
    _depura ( "ImportBulmaFact::startElement", 0 );
    cadintermedia = "";
    _depura ( "END ImportBulmaFact::startElement", 0 );
    return TRUE;
}


///
/**
\param qName
\return
**/
bool ImportBulmaFact::endElement ( const QString&, const QString&, const QString& qName )
{
    _depura ( "ImportBulmaFact::endElement", 0 );
    valores[qName] = cadintermedia;
    cadintermedia = "";
    fprintf ( stderr, "Tag de Cierre: %s\n", qName.toAscii().data() );
    if ( qName == "CLIENTE" )
        trataCliente();
    if ( qName == "PROVEEDOR" )
        trataProveedor();
    if ( qName == "FORMA_PAGO" )
        trataFormaPago();
    if ( qName == "ALMACEN" )
        trataAlmacen();
    if ( qName == "ARTICULO" )
        trataArticulo();
    if ( qName == "PRESUPUESTO" )
        trataPresupuesto();
    if ( qName == "LPRESUPUESTO" )
        trataLPresupuesto();
    if ( qName == "DPRESUPUESTO" )
        trataDPresupuesto();
    if ( qName == "FAMILIA" )
        trataFamilia();
    if ( qName == "FACTURA" )
        trataFactura();
    if ( qName == "LFACTURA" )
        trataLFactura();
    if ( qName == "DFACTURA" )
        trataDFactura();
    if ( qName == "ALBARAN" )
        trataAlbaran();
    if ( qName == "LALBARAN" )
        trataLAlbaran();
    if ( qName == "DALBARAN" )
        trataDAlbaran();
    if ( qName == "PEDIDOCLIENTE" )
        trataPedidoCliente();
    if ( qName == "LPEDIDOCLIENTE" )
        trataLPedidoCliente();
    if ( qName == "DPEDIDOCLIENTE" )
        trataDPedidoCliente();
    _depura ( "END ImportBulmaFact::endElement", 0 );
    return TRUE;
}


///
/**
\param nl
\return
**/
bool ImportBulmaFact::characters ( const QString& n1 )
{
    _depura ( "ImportBulmaFact::characters", 0 );
    QString val = n1;
    if ( val == "\n" )
        val = "";
    cadintermedia += val;
    _depura ( "END ImportBulmaFact::characters", 0 );
    return TRUE;
}


///
/**
**/
void ImportBulmaFact::printcontents()
{
    _depura ( "ImportBulmaFact::characters", 0 );
    fprintf ( stderr, "Impresion de contenidos\n" );
    tvalores::Iterator it;
    for ( it = valores.begin(); it != valores.end(); ++it ) {
        fprintf ( stderr, "Valores encontrados clave: %s Valor:%s\n",
                  it.key().toAscii().data(),
                  it.value().toAscii().data() );
    } // end for
    fprintf ( stderr, "Fin de impresion de contenidos\n" );
    _depura ( "END ImportBulmaFact::characters", 0 );
}


///
/**
\return
**/
int ImportBulmaFact::trataCliente()
{
    _depura ( "ImportBulmaFact::trataCliente", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( _( "<HR><B>Tratando el cliente " ) + valores["CIFCLIENTE"] + " " + valores["NOMCLIENTE"] + "</B><BR>" );
    /// En la importacion de un cliente hay que hacer la comprobacion del DNI para saber si existe o no.
    QString dcif = valores["CIFCLIENTE"];
    if ( dcif != "" ) {
        QString query = "SELECT * FROM cliente WHERE cifcliente SIMILAR TO '" + dcif + "'";
        BlDbRecordSet *cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            /// El cliente ya existe se pueden hacer modificaciones.
            pgimport->mensajeria ( _( "<LI> El cliente ya existe se pueden hacer modificaciones</LI>\n" ) );
            QString query1 = "UPDATE cliente SET nomcliente = '" + valores["NOMCLIENTE"] + "' WHERE cifcliente = '" + valores["CIFCLIENTE"] + "'";
            dbConnection->runQuery ( query1 );
        } else {
            /// El cliente no existe, se debe hacer una insercion de este.
            pgimport->mensajeria ( _( "<LI> El cliente no existe, se debe hacer una insercion de este</LI>\n" ) );
            QString query1 = "INSERT INTO cliente (cifcliente, nomcliente) VALUES ('" + valores["CIFCLIENTE"] + "','" + valores["NOMCLIENTE"] + "')";
            dbConnection->runQuery ( query1 );
        } // end if
        delete cur;
    } // end if
    pgimport->mensajeria ( "<HR>" );
    valores.clear();
    _depura ( "END ImportBulmaFact::trataCliente", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataProveedor()
{
    _depura ( "ImportBulmaFact::trataProveedor", 0 );

    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando el proveedor" ) + " " + valores["CIFPROVEEDOR"] + "</b><BR>" );
    /// En la importacion de un proveedor hay que hacer la comprobacion del DNI para saber si existe o no.
    QString cifprov = valores["CIFPROVEEDOR"];
    if ( cifprov != "" ) {
        QString query = "SELECT * FROM proveedor WHERE cifproveedor SIMILAR TO '" + cifprov + "'";
        BlDbRecordSet *cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            /// El cliente ya existe se pueden hacer modificaciones.
            pgimport->mensajeria ( "<LI>" + _( "El proveedor ya existe, se pueden hacer modificaciones") + "<LI>\n" );
            QString query1 = "UPDATE proveedor SET nomproveedor = '" + valores["NOMPROVEEDOR"] + "' WHERE cifproveedor = '" + valores["CIFPROVEEDOR"] + "'";
            dbConnection->runQuery ( query1 );
        } else {
            /// El cliente no existe, se debe hacer una insercion de este.
            pgimport->mensajeria ( "<LI>" +  _( "El proveedor no existe, se debe hacer una insercion de este" ) + "<LI>\n" );
            QString query1 = "INSERT INTO proveedor (cifproveedor, nomproveedor) VALUES ('" + valores["CIFPROVEEDOR"] + "','" + valores["NOMPROVEEDOR"] + "')";
            dbConnection->runQuery ( query1 );
        } // end if
        delete cur;
    } // end if
    pgimport->mensajeria ( "<HR>" );
    valores.clear();
    _depura ( "END ImportBulmaFact::trataProveedor", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataFormaPago()
{
    _depura ( "ImportBulmaFact::trataFormaPago", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<BR><B>" + _( "Tratando la forma de pago" ) + " " + valores["DESCFORMA_PAGO"] + "</B><BR>" );
    QString idforma_pago = valores["IDFORMA_PAGO"];
    QString descforma_pago = valores["DESCFORMA_PAGO"];
    QString dias1tforma_pago = valores["DIAS1TFORMA_PAGO"];
    QString descuentoforma_pago = valores["DESCUENTOFORMA_PAGO"];

    /// Comprobamos que hayan datos suficientes.
    if ( descforma_pago == "" || dias1tforma_pago == "" || descuentoforma_pago == "" ) {
        pgimport->mensajeria ( "<LI>" + _( "Datos insuficientes para tratar la forma de pago." ) );
        valores.clear();
        return 1;
    } // end if

    /// Comprobamos que no este ya creada una forma de pago de este tipo.
    QString query = "SELECT * FROM forma_pago WHERE dias1tforma_pago = " + dias1tforma_pago + " AND descuentoforma_pago = " + descuentoforma_pago;
    BlDbRecordSet *cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        pgimport->mensajeria ( "<LI>" + _( "Ya existe esta forma de pago." ) );
        delete cur;
        valores.clear();
        return 1;
    } // end if
    delete cur;

    /// Hacemos la insercion de la forma de Pago.
    query = "INSERT INTO forma_pago (descforma_pago, dias1tforma_pago, descuentoforma_pago) VALUES ('" + descforma_pago + "'," + dias1tforma_pago + "," + descuentoforma_pago + ")";
    dbConnection->runQuery ( query );
    pgimport->mensajeria ( "<LI>" + _( "Forma de pago" ) + " <B>" + descforma_pago + "</B> " + _( "Insertada" ) );
    pgimport->mensajeria ( "<HR>" );
    valores.clear();
    _depura ( "END ImportBulmaFact::trataFormaPago", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataAlmacen()
{
    _depura ( "ImportBulmaFact::trataAlmacen", 0 );
    /// En el XML se ha encontrado un tag de almacen que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando almacen" ) + " " + valores["CODIGOALMACEN"] + " " + valores["NOMALMACEN"] + "</B><BR>" );
    /// Primero hacemos la recoleccion de valores.
    QString idalmacen = valores["IDALMACEN"];
    QString codigoalmacen = valores["CODIGOALMACEN"];
    QString nomalmacen = valores["NOMALMACEN"];
    QString diralmacen = valores["DIRALMACEN"];
    QString poblalmacen = valores["POBLALMACEN"];
    QString cpalmacen = valores["CPALMACEN"];
    QString telalmacen = valores["TELALMACEN"];
    QString faxalmacen = valores["FAXALMACEN"];
    QString emailalmacen = valores["EMAILALMACEN"];
    QString inactivoalmacen = valores["INACTIVOALMACEN"];
    /// Comprobamos que hayan suficientes datos para procesar.
    if ( codigoalmacen == "" || nomalmacen == "" ) {
        pgimport->mensajeria ( "<LI>" + _( "Datos insuficientes para tratar el almacen." ) );
        valores.clear();
        return 1;
    } // end if
    /// Comprobamos que no este ya creada un almacen de este tipo.
    QString query = "SELECT * FROM almacen WHERE codigoalmacen = '" + codigoalmacen + "'";
    BlDbRecordSet *cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        pgimport->mensajeria ( "<LI>" + _( "Ya existe este almacen." ) );
        delete cur;
        valores.clear();
        return 1;
    } // end if
    delete cur;
    /// Hacemos la insercion del almacen.
    query = "INSERT INTO almacen (codigoalmacen, nomalmacen, diralmacen) VALUES ('" + codigoalmacen + "','" + nomalmacen + "','" + diralmacen + "')";
    dbConnection->runQuery ( query );
    pgimport->mensajeria ( "<LI> " + _( "Almacen" ) + " <B>" + codigoalmacen + "</B> " + _( "insertado" ) );
    /// Finalizamos.
    pgimport->mensajeria ( "<HR>" );
    valores.clear();
    _depura ( "END ImportBulmaFact::trataAlmacen", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataFamilia()
{
    _depura ( "ImportBulmaFact::trataFamilia", 0 );
    QString query;
    BlDbRecordSet *cur;
    /// En el XML se ha encontrado un tag de almacen que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando familia" ) + " " + valores["CODIGOCOMPLETOFAMILIA"] + " " + valores["NOMBREFAMILIA"] + "</B><BR>" );
    /// Primero hacemos la recoleccion de valores.
    QString idfamilia = valores["IDFAMILIA"];
    QString codigofamilia = valores["CODIGOFAMILIA"];
    QString nombrefamilia = valores["NOMBREFAMILIA"];
    QString descfamilia = valores["DESCFAMILIA"];
    QString padrefamilia = valores["PADREFAMILIA"];
    QString codigocompletofamilia = valores["CODIGOCOMPLETOFAMILIA"];
    /// Comprobamos que hayan suficientes datos para procesar.
    /// Comprobamos quien es el padre de la familia.
    QString codigopadre = codigocompletofamilia.left ( codigocompletofamilia.length() - codigofamilia.length() );
    QString idpadre = "";
    if ( codigopadre == "" ) {
        idpadre = "NULL";
    } else {
        query = "SELECT * FROM familia WHERE codigocompletofamilia = '" + codigopadre + "'";
        cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idpadre = cur->valor ( "idfamilia" );
        } else {
            pgimport->mensajeria ( "<P> " + _( "No se ha encontrado el padre de esta familia." ) + "</P>" );
        } // end if
        delete cur;
    } // end if
    if ( codigocompletofamilia == "" || nombrefamilia == "" || idpadre == "" ) {
        pgimport->mensajeria ( "<P>" + _( "Datos insuficientes para tratar la familia." ) + "</P>" );
        valores.clear();
        return 1;
    } // end if
    /// Comprobamos que no este ya creada un familia de este tipo.
    query = "SELECT * FROM familia WHERE codigocompletofamilia = '" + codigocompletofamilia + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        pgimport->mensajeria ( "<P>" + _( "Ya existe esta familia." ) + "</P>" );
        delete cur;
        valores.clear();
        return 1;
    } // end if
    delete cur;
    /// Hacemos la insercion del familia.
    query = "INSERT INTO familia (codigofamilia, nombrefamilia, padrefamilia) VALUES ('" + codigofamilia + "','" + nombrefamilia + "'," + idpadre + ")";
    dbConnection->runQuery ( query );
    pgimport->mensajeria ( "<P>" + _( "Familia" ) + " <B>" + codigocompletofamilia + "</B> " + _( "insertada" ) + "</P>" );
    /// Finalizamos.
    pgimport->mensajeria ( "<HR>" );
    valores.clear();
    _depura ( "END ImportBulmaFact::trataFamilia", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataArticulo()
{
    _depura ( "ImportBulmaFact::trataArticulo", 0 );
    QString query;
    BlDbRecordSet *cur;
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando articulo" ) + " " + valores["CODIGOCOMPLETOARTICULO"] + " </B><BR>" );
    /// Primero hacemos la recoleccion de valores.
    QString idarticulo = valores["IDARTICULO"];
    QString codarticulo =  valores["CODARTICULO"];
    QString nomarticulo = valores["NOMARTICULO"];
    QString abrevarticulo = valores["ABREVARTICULO"];
    QString obserarticulo = valores["OBSERARTICULO"];
    QString presentablearticulo = valores["PRESENTABLEARTICULO"];
    QString controlstockarticulo = valores["CONTROLSTOCKARTICULO"];
    QString idtipo_articulo = valores["IDTIPO_ARTICULO"];
    QString idtipo_iva = valores["IDTIPO_IVA"];
    QString codigocompletoarticulo = valores["CODIGOCOMPLETOARTICULO"];
    QString idfamilia = valores["IDFAMILIA"];
    QString stockarticulo = valores["STOCKARTICULO"];
    QString inactivoarticulo = valores["INACTIVOARTICULO"];
    QString pvparticulo = valores["PVPARTICULO"];
    QString codigocompletofamilia = valores["CODIGOCOMPLETOFAMILIA"];
    QString nombrefamilia = valores["NOMBREFAMILIA"];
    QString codtipo_articulo = valores["CODTIPO_ARTICULO"];
    QString desctipo_articulo = valores["DESCTIPO_ARTICULO"];
    QString desctipo_iva = valores["DESCTIPO_IVA"];

    query = "SELECT * FROM familia WHERE codigocompletofamilia = '" + codigocompletofamilia + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idfamilia = cur->valor ( "idfamilia" );
    } else {
        pgimport->mensajeria ( _( "La familia del articulo no existe" ) + "<BR>\n" );
        idfamilia = "";
    } // end if
    delete cur;

    query = "SELECT * from tipo_articulo WHERE codtipo_articulo = '" + codtipo_articulo + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idtipo_articulo = cur->valor ( "idtipo_articulo" );
    } else {
        idtipo_articulo = "NULL";
    } // end if
    delete cur;

    query = "SELECT * from tipo_iva WHERE desctipo_iva = '" + desctipo_iva + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idtipo_iva = cur->valor ( "idtipo_iva" );
    } else {
        idtipo_iva = "NULL";
    } // end if
    delete cur;

    /// Comprobamos que hayan suficientes datos para procesar.
    if ( codigocompletoarticulo == "" || nomarticulo == "" || idfamilia == "" ) {
        pgimport->mensajeria ( "<P>" + _( "Datos insuficientes para tratar el articulo." ) + "</P>" );
        valores.clear();
        return 1;
    } // end if

    /// Comprobamos que no este ya creada un articulo de este tipo.
    query = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompletoarticulo + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        pgimport->mensajeria ( "<P>" + _( "Ya existe este articulo." ) + "</P>" );
        delete cur;
        valores.clear();
        return 1;
    } // end if
    delete cur;

    /// Hacemos la insercion del articulo.
    query = "INSERT INTO articulo (codarticulo, nomarticulo, abrevarticulo, idfamilia, pvparticulo, idtipo_articulo, obserarticulo, presentablearticulo, inactivoarticulo, controlstockarticulo, idtipo_iva) VALUES (";
    query += "'" + codarticulo + "'";
    query += ",'" + nomarticulo + "'";
    query += ",'" + abrevarticulo;
    query += "'," + idfamilia;
    query += "," + pvparticulo;
    query += "," + idtipo_articulo;
    query += ",'" + obserarticulo + "'";
    query += ",'" + presentablearticulo + "'";
    query += ",'" + inactivoarticulo + "'";
    query += ",'" + controlstockarticulo + "'";
    query += "," + idtipo_iva;
    query += ")";
    dbConnection->runQuery ( query );
    pgimport->mensajeria ( "<P>" + _( "Articulo" ) + " <B>" + codigocompletoarticulo + "</B> " + _( "insertado" ) + "</P>" );
    valores.clear();
    _depura ( "END ImportBulmaFact::trataArticulo", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataLPedidoCliente()
{
    _depura ( "ImportBulmaFact::trataLPedidoCliente", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando LPedidoCliente" ) + " " + valores["IDLPEDIDOCLIENTE"] + "</B><BR>" );
    tvalores lpedidoclientemap;
    lpedidoclientemap.insert ( "IDLPEDIDOCLIENTE", valores["IDLPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "DESCLPEDIDOCLIENTE", valores["DESCLPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "CANTLPEDIDOCLIENTE", valores["CANTLPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "PVPLPEDIDOCLIENTE", valores["PVPLPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "IVALPEDIDOCLIENTE", valores["IVALPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "DESCUENTOLPEDIDOCLIENTE", valores["DESCUENTOLPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "IDPEDIDOCLIENTE", valores["IDPEDIDOCLIENTE"] );
    lpedidoclientemap.insert ( "IDARTICULO", valores["IDARTICULO"] );
    lpedidoclientemap.insert ( "CODARTICULO", valores["CODARTICULO"] );
    lpedidoclientemap.insert ( "NOMARTICULO", valores["NOMARTICULO"] );
    lpedidoclientemap.insert ( "ABREVARTICULO", valores["ABREVARTICULO"] );
    lpedidoclientemap.insert ( "CODIGOCOMPLETOARTICULO", valores["CODIGOCOMPLETOARTICULO"] );
    listalpedidocliente.append ( lpedidoclientemap );
    _depura ( "END ImportBulmaFact::trataLPedidoCliente", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataDPedidoCliente()
{
    _depura ( "ImportBulmaFact::trataDPedidoCliente", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando DPedidoCliente" ) + " " + valores["IDDPEDIDOCLIENTE"] + "</B><BR>" );
    tvalores dpedidoclientemap;
    dpedidoclientemap.insert ( "IDDPEDIDOCLIENTE", valores["IDDPEDIDOCLIENTE"] );
    dpedidoclientemap.insert ( "CONCEPTDPEDIDOCLIENTE", valores["CONCEPTDPEDIDOCLIENTE"] );
    dpedidoclientemap.insert ( "PROPORCIONDPEDIDOCLIENTE", valores["PROPORCIONDPEDIDOCLIENTE"] );
    listadpedidocliente.append ( dpedidoclientemap );
    _depura ( "END ImportBulmaFact::trataDPedidoCliente", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataPedidoCliente()
{
    _depura ( "ImportBulmaFact::trataPedidoCliente", 0 );
    QString query;
    BlDbRecordSet *cur;
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando pedido cliente" ) + " " + valores["IDPEDIDOCLIENTE"] + " " + valores["NOMBREFAMILIA"] + "</B><BR>" );
    QString idpedidocliente = valores["IDPEDIDOCLIENTE"];
    QString numpedidocliente = valores["NUMPEDIDOCLIENTE"];
    QString refpedidocliente = valores["REFPEDIDOCLIENTE"];
    QString fechapedidocliente = valores["FECHAPEDIDOCLIENTE"];
    QString descpedidocliente = valores["DESCPEDIDOCLIENTE"];
    QString idalmacen = valores["IDALMACEN"];
    QString comentpedidocliente = valores["COMENTPEDIDOCLIENTE"];
    QString procesadopedidocliente = valores["PROCESADOPEDIDOCLIENTE"];
    QString contactpedidocliente = valores["CONTACTPEDIDOCLIENTE"];
    QString telpedidocliente = valores["TELPEDIDOCLIENTE"];
    QString idusuari = valores["IDUSUARI"];
    QString idcliente = valores["IDCLIENTE"];
    QString idforma_pago = valores["IDFORMA_PAGO"];
    QString idtrabajador = valores["IDTRABAJADOR"];
    QString codigoalmacen = valores["CODIGOALMACEN"];
    QString nomalmacen = valores["NOMALMACEN"];
    QString diralmacen = valores["DIRALMACEN"];
    QString poblalmacen = valores["POBLALMACEN"];
    QString cpalmacen = valores["CPALMACEN"];
    QString telalmacen = valores["TELALMACEN"];
    QString nomcliente = valores["NOMCLIENTE"];
    QString nomaltcliente = valores["NOMALTCLIENTE"];
    QString cifcliente = valores["CIFCLIENTE"];
    QString bancocliente = valores["BANCOCLIENTE"];
    QString dircliente = valores["DIRCLIENTE"];
    QString poblcliente = valores["POBLCLIENTE"];
    QString cpcliente = valores["CPCLIENTE"];
    QString telcliente = valores["TELCLIENTE"];
    QString faxcliente = valores["FAXCLIENTE"];
    QString mailcliente = valores["MAILCLIENTE"];
    QString urlcliente = valores["URLCLIENTE"];
    QString provcliente = valores["PROVCLIENTE"];
    QString nomtrabajador = valores["NOMTRABAJADOR"];
    QString apellidostrabajador = valores["APELLIDOSTRABAJADOR"];
    QString dirtrabajador = valores["DIRTRABAJADOR"];
    QString nsstrabajador = valores["NSSTRABAJADOR"];
    QString teltrabajador = valores["TELTRABAJADOR"];
    QString moviltrabajador = valores["MOVILTRABAJADOR"];
    QString emailtrabajador = valores["EMAILTRABAJADOR"];
    QString descforma_pago = valores["DESCFORMA_PAGO"];
    QString dias1tforma_pago = valores["DIAS1TFORMA_PAGO"];
    QString descuentoforma_pago = valores["DESCUENTOFORMA_PAGO"];

    /// Completamos datos que puedan faltar.
    query = "SELECT * FROM almacen WHERE codigoalmacen = '" + codigoalmacen + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idalmacen = cur->valor ( "idalmacen" );
    } else {
        pgimport->mensajeria ( _( "El almacen del presupuesto no existe" ) + "<BR>\n" );
        idalmacen = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM cliente WHERE cifcliente = '" + cifcliente + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idcliente = cur->valor ( "idcliente" );
    } else {
        pgimport->mensajeria ( _( "El cliente del presupuesto no existe" ) + "<BR>\n" );
        idcliente = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM forma_pago WHERE descforma_pago = '" + descforma_pago + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idforma_pago = cur->valor ( "idforma_pago" );
    } else {
        pgimport->mensajeria ( _( "La forma de pago del presupuesto no existe" ) + "<BR>\n" );
        idforma_pago = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM trabajador WHERE nomtrabajador = '" + nomtrabajador + "' AND nsstrabajador = '" + nsstrabajador + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idtrabajador = cur->valor ( "idtrabajador" );
    } else {
        pgimport->mensajeria ( _( "El empleado del presupuesto no existe" ) + "<BR>\n" );
        idtrabajador = "NULL";
    } // end if
    delete cur;

    /// Hacemos la insercion.
    query = " INSERT INTO pedidocliente (numpedidocliente, refpedidocliente, fechapedidocliente, descpedidocliente, contactpedidocliente, telpedidocliente, comentpedidocliente, idusuari, procesadopedidocliente, idcliente, idalmacen, idforma_pago, idtrabajador) VALUES (";
    query += "NULL";
    query += ", '" + refpedidocliente + "'";
    query += ", '" + fechapedidocliente + "'";
    query += ", '" + descpedidocliente + "'";
    query += ", '" + contactpedidocliente + "'";
    query += ", '" + telpedidocliente + "'";
    query += ", '" + comentpedidocliente + "'";
    query += ", NULL";
    query += ",'" + procesadopedidocliente + "'";
    query += "," + idcliente;
    query += "," + idalmacen;
    query += "," + idforma_pago;
    query += "," + idtrabajador;
    query += ")";

    dbConnection->begin();
    dbConnection->runQuery ( query );
    cur = dbConnection->loadQuery ( "SELECT MAX(idpedidocliente) AS id FROM pedidocliente" );
    idpedidocliente = cur->valor ( "id" );
    delete cur;
    dbConnection->commit();
    /// Tratamos la insercion de las lineas de albaran.
    tvalores *lpedidoclientemap;

    QMutableListIterator<tvalores> ilistalpedidocliente ( listalpedidocliente );

    for ( lpedidoclientemap = &listalpedidocliente.first(); lpedidoclientemap; lpedidoclientemap = &ilistalpedidocliente.next() ) {
        QString idlpedidocliente = ( *lpedidoclientemap ) ["IDLPEDIDOCLIENTE"];
        QString desclpedidocliente = ( *lpedidoclientemap ) ["DESCLPEDIDOCLIENTE"];
        QString cantlpedidocliente = ( *lpedidoclientemap ) ["CANTLPEDIDOCLIENTE"];
        QString pvplpedidocliente = ( *lpedidoclientemap ) ["PVPLPEDIDOCLIENTE"];
        QString ivalpedidocliente = ( *lpedidoclientemap ) ["IVALPEDIDOCLIENTE"];
        QString descuentolpedidocliente = ( *lpedidoclientemap ) ["DESCUENTOLPEDIDOCLIENTE"];
        QString idarticulo = ( *lpedidoclientemap ) ["IDARTICULO"];
        QString codarticulo = ( *lpedidoclientemap ) ["CODARTICULO"];
        QString nomarticulo = ( *lpedidoclientemap ) ["NOMARTICULO"];
        QString abrevarticulo = ( *lpedidoclientemap ) ["ABREVARTICULO"];
        QString codigocompletoarticulo = ( *lpedidoclientemap ) ["CODIGOCOMPLETOARTICULO"];
        /// Completamos datos de la linea de presupuesto.
        query = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompletoarticulo + "'";
        cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idarticulo = cur->valor ( "idarticulo" );
        } else {
            pgimport->mensajeria ( _( "El articulo de la linea de presupuesto no existe" ) + "<BR>\n" );
            idarticulo = "NULL";
        } // end if
        delete cur;
        /// Hacemos la insercion.
        query = " INSERT INTO lpedidocliente (desclpedidocliente, cantlpedidocliente, pvplpedidocliente, ivalpedidocliente, descuentolpedidocliente, idpedidocliente, idarticulo) VALUES (";
        query += "'" + desclpedidocliente + "'";
        query += ", " + cantlpedidocliente;
        query += ", " + pvplpedidocliente;
        query += ", " + ivalpedidocliente;
        query += ", " + descuentolpedidocliente;
        query += ", " + idpedidocliente;
        query += ", " + idarticulo;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    /// Tratamos la insercion de los descuentos de presupuesto.
    tvalores *dpedidoclientemap;

    QMutableListIterator<tvalores> ilistadpedidocliente ( listadpedidocliente );

    for ( dpedidoclientemap = &listadpedidocliente.first(); dpedidoclientemap; dpedidoclientemap = &ilistadpedidocliente.next() ) {
        QString iddpedidocliente = ( *dpedidoclientemap ) ["IDDPEDIDOCLIENTE"];
        QString conceptdpedidocliente = ( *dpedidoclientemap ) ["CONCEPTDPEDIDOCLIENTE"];
        QString proporciondpedidocliente = ( *dpedidoclientemap ) ["PROPORCIONDPEDIDOCLIENTE"];
        /// Hacemos la insercion.
        query = " INSERT INTO dpedidocliente (conceptdpedidocliente, proporciondpedidocliente, idpedidocliente) VALUES (";
        query += "'" + conceptdpedidocliente + "'";
        query += ", " + proporciondpedidocliente;
        query += ", " + idpedidocliente;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    valores.clear();
    _depura ( "END ImportBulmaFact::trataPedidoCliente", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataLAlbaran()
{
    _depura ( "ImportBulmaFact::trataLAlbaran", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando LAlbaran" ) + " " + valores["IDLALBARAN"] + "</B><BR>" );
    tvalores lalbaranmap;
    lalbaranmap.insert ( "IDLALBARAN", valores["IDLALBARAN"] );
    lalbaranmap.insert ( "DESCLALBARAN", valores["DESCLALBARAN"] );
    lalbaranmap.insert ( "CANTLALBARAN", valores["CANTLALBARAN"] );
    lalbaranmap.insert ( "PVPLALBARAN", valores["PVPLALBARAN"] );
    lalbaranmap.insert ( "IVALALBARAN", valores["IVALALBARAN"] );
    lalbaranmap.insert ( "DESCUENTOLALBARAN", valores["DESCUENTOLALBARAN"] );
    lalbaranmap.insert ( "IDALBARAN", valores["IDALBARAN"] );
    lalbaranmap.insert ( "IDARTICULO", valores["IDARTICULO"] );
    lalbaranmap.insert ( "CODARTICULO", valores["CODARTICULO"] );
    lalbaranmap.insert ( "NOMARTICULO", valores["NOMARTICULO"] );
    lalbaranmap.insert ( "ABREVARTICULO", valores["ABREVARTICULO"] );
    lalbaranmap.insert ( "CODIGOCOMPLETOARTICULO", valores["CODIGOCOMPLETOARTICULO"] );
    listalalbaran.append ( lalbaranmap );
    _depura ( "END ImportBulmaFact::trataLAlbaran", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataDAlbaran()
{
    _depura ( "ImportBulmaFact::trataDAlbaran", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando DAlbaran" ) + " " + valores["IDDALBARAN"] + "</B><BR>" );
    tvalores dalbaranmap;
    dalbaranmap.insert ( "IDDALBARAN", valores["IDDALBARAN"] );
    dalbaranmap.insert ( "CONCEPTDALBARAN", valores["CONCEPTDALBARAN"] );
    dalbaranmap.insert ( "PROPORCIONDALBARAN", valores["PROPORCIONDALBARAN"] );
    listadalbaran.append ( dalbaranmap );
    _depura ( "END ImportBulmaFact::trataDAlbaran", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataAlbaran()
{
    _depura ( "ImportBulmaFact::trataAlbaran", 0 );
    QString query;
    BlDbRecordSet *cur;
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando albaran" ) + " " + valores["IDALBARAN"] + " " + valores["NOMBREFAMILIA"] + "</B><BR>" );
    QString idalbaran = valores["IDALBARAN"];
    QString numalbaran = valores["NUMALBARAN"];
    QString refalbaran = valores["REFALBARAN"];
    QString fechaalbaran = valores["FECHAALBARAN"];
    QString descalbaran = valores["DESCALBARAN"];
    QString idalmacen = valores["IDALMACEN"];
    QString contactfactura = valores["CONTACTFACTURA"];
    QString telfactura = valores["TELFACTURA"];
    QString comentalbaran = valores["COMENTALBARAN"];
    QString procesadoalbaran = valores["PROCESADOALBARAN"];
    QString contactalbaran = valores["CONTACTALBARAN"];
    QString telalbaran = valores["TELALBARAN"];
    QString idusuari = valores["IDUSUARI"];
    QString idcliente = valores["IDCLIENTE"];
    QString idforma_pago = valores["IDFORMA_PAGO"];
    QString idtrabajador = valores["IDTRABAJADOR"];
    QString codigoalmacen = valores["CODIGOALMACEN"];
    QString nomalmacen = valores["NOMALMACEN"];
    QString diralmacen = valores["DIRALMACEN"];
    QString poblalmacen = valores["POBLALMACEN"];
    QString cpalmacen = valores["CPALMACEN"];
    QString telalmacen = valores["TELALMACEN"];
    QString nomcliente = valores["NOMCLIENTE"];
    QString nomaltcliente = valores["NOMALTCLIENTE"];
    QString cifcliente = valores["CIFCLIENTE"];
    QString bancocliente = valores["BANCOCLIENTE"];
    QString dircliente = valores["DIRCLIENTE"];
    QString poblcliente = valores["POBLCLIENTE"];
    QString cpcliente = valores["CPCLIENTE"];
    QString telcliente = valores["TELCLIENTE"];
    QString faxcliente = valores["FAXCLIENTE"];
    QString mailcliente = valores["MAILCLIENTE"];
    QString urlcliente = valores["URLCLIENTE"];
    QString provcliente = valores["PROVCLIENTE"];
    QString nomtrabajador = valores["NOMTRABAJADOR"];
    QString apellidostrabajador = valores["APELLIDOSTRABAJADOR"];
    QString dirtrabajador = valores["DIRTRABAJADOR"];
    QString nsstrabajador = valores["NSSTRABAJADOR"];
    QString teltrabajador = valores["TELTRABAJADOR"];
    QString moviltrabajador = valores["MOVILTRABAJADOR"];
    QString emailtrabajador = valores["EMAILTRABAJADOR"];
    QString descforma_pago = valores["DESCFORMA_PAGO"];
    QString dias1tforma_pago = valores["DIAS1TFORMA_PAGO"];
    QString descuentoforma_pago = valores["DESCUENTOFORMA_PAGO"];

    /// Completamos datos que puedan faltar.
    query = "SELECT * FROM almacen WHERE codigoalmacen = '" + codigoalmacen + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idalmacen = cur->valor ( "idalmacen" );
    } else {
        pgimport->mensajeria ( _( "El almacen del presupuesto no existe" ) + "<BR>\n" );
        idalmacen = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM cliente WHERE cifcliente = '" + cifcliente + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idcliente = cur->valor ( "idcliente" );
    } else {
        pgimport->mensajeria ( _( "El cliente del presupuesto no existe" ) + "<BR>\n" );
        idcliente = "NULL";
    } // end if
    delete cur;
    query = "SELECT * FROM forma_pago WHERE descforma_pago = '" + descforma_pago + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idforma_pago = cur->valor ( "idforma_pago" );
    } else {
        pgimport->mensajeria ( _( "La forma de pago del presupuesto no existe" ) + "<BR>\n" );
        idforma_pago = "NULL";
    } // end if
    delete cur;
    query = "SELECT * FROM trabajador WHERE nomtrabajador = '" + nomtrabajador + "' AND nsstrabajador = '" + nsstrabajador + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idtrabajador = cur->valor ( "idtrabajador" );
    } else {
        pgimport->mensajeria ( _( "El empleado del presupuesto no existe" ) + "<BR>\n" );
        idtrabajador = "NULL";
    } // end if
    delete cur;
    /// Hacemos la insercion.
    query = " INSERT INTO albaran (numalbaran, refalbaran, fechaalbaran, descalbaran, contactalbaran , telalbaran  , comentalbaran , idusuari, procesadoalbaran, idcliente, idalmacen, idforma_pago, idtrabajador) VALUES (";
    query += "NULL";
    query += ", '" + refalbaran + "'";
    query += ", '" + fechaalbaran + "'";
    query += ", '" + descalbaran + "'";
    query += ", '" + contactalbaran + "'";
    query += ", '" + telalbaran + "'";
    query += ", '" + comentalbaran + "'";
    query += ", NULL";
    query += ",'" + procesadoalbaran + "'";
    query += "," + idcliente;
    query += "," + idalmacen;
    query += "," + idforma_pago;
    query += "," + idtrabajador;
    query += ")";

    dbConnection->begin();
    dbConnection->runQuery ( query );
    cur = dbConnection->loadQuery ( "SELECT MAX(idalbaran) AS id FROM albaran" );
    idalbaran = cur->valor ( "id" );
    delete cur;
    dbConnection->commit();
    /// Tratamos la insercion de las lineas de albaran.
    tvalores *lalbaranmap;

    QMutableListIterator<tvalores> ilistalalbaran ( listalalbaran );

    for ( lalbaranmap = &listalalbaran.first(); lalbaranmap; lalbaranmap = &ilistalalbaran.next() ) {
        QString idlalbaran = ( *lalbaranmap ) ["IDLALBARAN"];
        QString desclalbaran = ( *lalbaranmap ) ["DESCLALBARAN"];
        QString cantlalbaran = ( *lalbaranmap ) ["CANTLALBARAN"];
        QString pvplalbaran = ( *lalbaranmap ) ["PVPLALBARAN"];
        QString ivalalbaran = ( *lalbaranmap ) ["IVALALBARAN"];
        QString descuentolalbaran = ( *lalbaranmap ) ["DESCUENTOLALBARAN"];
        QString idarticulo = ( *lalbaranmap ) ["IDARTICULO"];
        QString codarticulo = ( *lalbaranmap ) ["CODARTICULO"];
        QString nomarticulo = ( *lalbaranmap ) ["NOMARTICULO"];
        QString abrevarticulo = ( *lalbaranmap ) ["ABREVARTICULO"];
        QString codigocompletoarticulo = ( *lalbaranmap ) ["CODIGOCOMPLETOARTICULO"];
        /// Completamos datos de la linea de presupuesto.
        query = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompletoarticulo + "'";
        cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idarticulo = cur->valor ( "idarticulo" );
        } else {
            pgimport->mensajeria ( _( "El articulo de la linea de presupuesto no existe" ) + "<BR>\n" );
            idarticulo = "NULL";
        } // end if
        delete cur;
        /// Hacemos la insercion.
        query = " INSERT INTO lalbaran(desclalbaran, cantlalbaran, pvplalbaran, ivalalbaran, descuentolalbaran, idalbaran, idarticulo) VALUES (";
        query += "'" + desclalbaran + "'";
        query += ", " + cantlalbaran;
        query += ", " + pvplalbaran;
        query += ", " + ivalalbaran;
        query += ", " + descuentolalbaran;
        query += ", " + idalbaran;
        query += ", " + idarticulo;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    /// Tratamos la insercion de los descuentos de presupuesto.
    tvalores *dalbaranmap;

    QMutableListIterator<tvalores> ilistadalbaran ( listadalbaran );

    for ( dalbaranmap = &listadalbaran.first(); dalbaranmap; dalbaranmap = &ilistadalbaran.next() ) {
        QString iddalbaran = ( *dalbaranmap ) ["IDDALBARAN"];
        QString conceptdalbaran = ( *dalbaranmap ) ["CONCEPTDALBARAN"];
        QString proporciondalbaran = ( *dalbaranmap ) ["PROPORCIONDALBARAN"];
        /// Hacemos la insercion.
        query = " INSERT INTO dalbaran(conceptdalbaran, proporciondalbaran, idalbaran) VALUES (";
        query += "'" + conceptdalbaran + "'";
        query += ", " + proporciondalbaran;
        query += ", " + idalbaran;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    valores.clear();
    _depura ( "END ImportBulmaFact::trataAlbaran", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataLFactura()
{
    _depura ( "ImportBulmaFact::trataLFactura", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando LFactura" ) + " " + valores["IDLFACTURA"] + "</B><BR>" );
    tvalores lfacturamap;
    lfacturamap.insert ( "IDLFACTURA", valores["IDLFACTURA"] );
    lfacturamap.insert ( "DESCLFACTURA", valores["DESCLFACTURA"] );
    lfacturamap.insert ( "CANTLFACTURA", valores["CANTLFACTURA"] );
    lfacturamap.insert ( "PVPLFACTURA", valores["PVPLFACTURA"] );
    lfacturamap.insert ( "IVALFACTURA", valores["IVALFACTURA"] );
    lfacturamap.insert ( "DESCUENTOLFACTURA", valores["DESCUENTOLFACTURA"] );
    lfacturamap.insert ( "IDFACTURA", valores["IDFACTURA"] );
    lfacturamap.insert ( "IDARTICULO", valores["IDARTICULO"] );
    lfacturamap.insert ( "CODARTICULO", valores["CODARTICULO"] );
    lfacturamap.insert ( "NOMARTICULO", valores["NOMARTICULO"] );
    lfacturamap.insert ( "ABREVARTICULO", valores["ABREVARTICULO"] );
    lfacturamap.insert ( "CODIGOCOMPLETOARTICULO", valores["CODIGOCOMPLETOARTICULO"] );
    listalfactura.append ( lfacturamap );
    _depura ( "END ImportBulmaFact::trataLFactura", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataDFactura()
{
    _depura ( "ImportBulmaFact::trataDFactura", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando DFactura" ) + " " + valores["IDDFACTURA"] + "</B><BR>" );
    tvalores dfacturamap;
    dfacturamap.insert ( "IDDFACTURA", valores["IDDFACTURA"] );
    dfacturamap.insert ( "CONCEPTDFACTURA", valores["CONCEPTDFACTURA"] );
    dfacturamap.insert ( "PROPORCIONDFACTURA", valores["PROPORCIONDFACTURA"] );
    listadfactura.append ( dfacturamap );
    _depura ( "END ImportBulmaFact::trataDFactura", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataFactura()
{
    _depura ( "ImportBulmaFact::trataFactura", 0 );
    QString query;
    BlDbRecordSet *cur;
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando factura" ) + " " + valores["IDFACTURA"] + " " + valores["NOMBREFAMILIA"] + "</B><BR>" );
    QString idfactura = valores["IDFACTURA"];
    QString codigoserie_factura = valores["CODIGOSERIE_FACTURA"];
    QString numfactura = valores["NUMFACTURA"];
    QString reffactura = valores["REFFACTURA"];
    QString ffactura = valores["FFACTURA"];
    QString descfactura = valores["DESCFACTURA"];
    QString idalmacen = valores["IDALMACEN"];
    QString contactfactura = valores["CONTACTFACTURA"];
    QString telfactura = valores["TELFACTURA"];
    QString comentfactura = valores["COMENTFACTURA"];
    QString procesadafactura = valores["PROCESADAFACTURA"];
    QString idusuari = valores["IDUSUARI"];
    QString idcliente = valores["IDCLIENTE"];
    QString idforma_pago = valores["IDFORMA_PAGO"];
    QString idtrabajador = valores["IDTRABAJADOR"];
    QString codigoalmacen = valores["CODIGOALMACEN"];
    QString nomalmacen = valores["NOMALMACEN"];
    QString diralmacen = valores["DIRALMACEN"];
    QString poblalmacen = valores["POBLALMACEN"];
    QString cpalmacen = valores["CPALMACEN"];
    QString telalmacen = valores["TELALMACEN"];
    QString nomcliente = valores["NOMCLIENTE"];
    QString nomaltcliente = valores["NOMALTCLIENTE"];
    QString cifcliente = valores["CIFCLIENTE"];
    QString bancocliente = valores["BANCOCLIENTE"];
    QString dircliente = valores["DIRCLIENTE"];
    QString poblcliente = valores["POBLCLIENTE"];
    QString cpcliente = valores["CPCLIENTE"];
    QString telcliente = valores["TELCLIENTE"];
    QString faxcliente = valores["FAXCLIENTE"];
    QString mailcliente = valores["MAILCLIENTE"];
    QString urlcliente = valores["URLCLIENTE"];
    QString provcliente = valores["PROVCLIENTE"];
    QString nomtrabajador = valores["NOMTRABAJADOR"];
    QString apellidostrabajador = valores["APELLIDOSTRABAJADOR"];
    QString dirtrabajador = valores["DIRTRABAJADOR"];
    QString nsstrabajador = valores["NSSTRABAJADOR"];
    QString teltrabajador = valores["TELTRABAJADOR"];
    QString moviltrabajador = valores["MOVILTRABAJADOR"];
    QString emailtrabajador = valores["EMAILTRABAJADOR"];
    QString descforma_pago = valores["DESCFORMA_PAGO"];
    QString dias1tforma_pago = valores["DIAS1TFORMA_PAGO"];
    QString descuentoforma_pago = valores["DESCUENTOFORMA_PAGO"];

    /// Completamos datos que puedan faltar.
    query = "SELECT * FROM almacen WHERE codigoalmacen = '" + codigoalmacen + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idalmacen = cur->valor ( "idalmacen" );
    } else {
        pgimport->mensajeria ( _( "El almacen del presupuesto no existe" ) + "<BR>\n" );
        idalmacen = "NULL";
    } // end if
    delete cur;
    query = "SELECT * FROM cliente WHERE cifcliente = '" + cifcliente + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idcliente = cur->valor ( "idcliente" );
    } else {
        pgimport->mensajeria ( _( "El cliente del presupuesto no existe" ) + "<BR>\n" );
        idcliente = "NULL";
    } // end if
    delete cur;
    query = "SELECT * FROM forma_pago WHERE descforma_pago = '" + descforma_pago + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idforma_pago = cur->valor ( "idforma_pago" );
    } else {
        pgimport->mensajeria ( _( "La forma de pago del presupuesto no existe" ) + "<BR>\n" );
        idforma_pago = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM trabajador WHERE nomtrabajador = '" + nomtrabajador + "' AND nsstrabajador = '" + nsstrabajador + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idtrabajador = cur->valor ( "idtrabajador" );
    } else {
        pgimport->mensajeria ( _( "El empleado del presupuesto no existe" ) + "<BR>\n" );
        idtrabajador = "NULL";
    } // end if
    delete cur;

    /// Hacemos comprobaciones de integridad para saber si la factura es insertable o no.
    query = "SELECT * FROM factura WHERE numfactura = '" + numfactura + "' AND codigoserie_factura = '" + codigoserie_factura + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        delete cur;
        pgimport->mensajeria ( _( "La factura ya existe y no se va a insertar." ) );
        valores.clear();
        return 1;
    } // end if
    delete cur;

    /// Hacemos la insercion.
    query = " INSERT INTO factura(numfactura, reffactura, ffactura, descfactura, contactfactura, telfactura, comentfactura, idusuari, procesadafactura, idcliente, idalmacen, idforma_pago, idtrabajador, codigoserie_factura) VALUES (";
    query += numfactura;
    query += ", '" + reffactura + "'";
    query += ", '" + ffactura + "'";
    query += ", '" + descfactura + "'";
    query += ", '" + contactfactura + "'";
    query += ", '" + telfactura + "'";
    query += ", '" + comentfactura + "'";
    query += ", NULL";
    query += ",'" + procesadafactura + "'";
    query += "," + idcliente;
    query += "," + idalmacen;
    query += "," + idforma_pago;
    query += "," + idtrabajador;
    query += ",'" + codigoserie_factura + "'";
    query += ")";

    dbConnection->begin();
    dbConnection->runQuery ( query );
    cur = dbConnection->loadQuery ( "SELECT MAX(idfactura) AS id FROM factura" );
    idfactura = cur->valor ( "id" );
    delete cur;
    dbConnection->commit();
    /// Tratamos la insercion de las lineas de presupuesto.
    tvalores *lfacturamap;

    QMutableListIterator<tvalores> ilistalfactura ( listalfactura );

    for ( lfacturamap = &listalfactura.first(); lfacturamap; lfacturamap = &ilistalfactura.next() ) {
        QString idlfactura = ( *lfacturamap ) ["IDLFACTURA"];
        QString desclfactura = ( *lfacturamap ) ["DESCLFACTURA"];
        QString cantlfactura = ( *lfacturamap ) ["CANTLFACTURA"];
        QString pvplfactura = ( *lfacturamap ) ["PVPLFACTURA"];
        QString ivalfactura = ( *lfacturamap ) ["IVALFACTURA"];
        QString descuentolfactura = ( *lfacturamap ) ["DESCUENTOLFACTURA"];
        QString idarticulo = ( *lfacturamap ) ["IDARTICULO"];
        QString codarticulo = ( *lfacturamap ) ["CODARTICULO"];
        QString nomarticulo = ( *lfacturamap ) ["NOMARTICULO"];
        QString abrevarticulo = ( *lfacturamap ) ["ABREVARTICULO"];
        QString codigocompletoarticulo = ( *lfacturamap ) ["CODIGOCOMPLETOARTICULO"];
        /// Completamos datos de la linea de presupuesto.
        query = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompletoarticulo + "'";
        cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idarticulo = cur->valor ( "idarticulo" );
        } else {
            pgimport->mensajeria ( _( "El articulo de la linea de presupuesto no existe" ) + "<BR>\n" );
            idarticulo = "NULL";
        } // end if
        delete cur;
        /// Hacemos la insercion.
        query = " INSERT INTO lfactura(desclfactura, cantlfactura, pvplfactura, ivalfactura, descuentolfactura, idfactura, idarticulo) VALUES (";
        query += "'" + desclfactura + "'";
        query += ", " + cantlfactura;
        query += ", " + pvplfactura;
        query += ", " + ivalfactura;
        query += ", " + descuentolfactura;
        query += ", " + idfactura;
        query += ", " + idarticulo;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    /// Tratamos la insercion de los descuentos de presupuesto.
    tvalores *dfacturamap;

    QMutableListIterator<tvalores> ilistadfactura ( listadfactura );

    for ( dfacturamap = &listadfactura.first(); dfacturamap; dfacturamap = &ilistadfactura.next() ) {
        QString iddfactura = ( *dfacturamap ) ["IDDFACTURA"];
        QString conceptdfactura = ( *dfacturamap ) ["CONCEPTDFACTURA"];
        QString proporciondfactura = ( *dfacturamap ) ["PROPORCIONDFACTURA"];
        /// Hacemos la insercion.
        query = " INSERT INTO dfactura(conceptdfactura, proporciondfactura, idfactura) VALUES (";
        query += "'" + conceptdfactura + "'";
        query += ", " + proporciondfactura;
        query += ", " + idfactura;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    valores.clear();
    _depura ( "END ImportBulmaFact::trataFactura", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataLPresupuesto()
{
    _depura ( "ImportBulmaFact::trataLPresupuesto", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando LPresupuesto" ) + " " + valores["IDLPRESUPUESTO"] + "</B><BR>" );
    tvalores lpresupuestomap;
    lpresupuestomap.insert ( "IDLPRESUPUESTO", valores["IDLPRESUPUESTO"] );
    lpresupuestomap.insert ( "DESCLPRESUPUESTO", valores["DESCLPRESUPUESTO"] );
    lpresupuestomap.insert ( "CANTLPRESUPUESTO", valores["CANTLPRESUPUESTO"] );
    lpresupuestomap.insert ( "PVPLPRESUPUESTO", valores["PVPLPRESUPUESTO"] );
    lpresupuestomap.insert ( "IVALPRESUPUESTO", valores["IVALPRESUPUESTO"] );
    lpresupuestomap.insert ( "DESCUENTOLPRESUPUESTO", valores["DESCUENTOLPRESUPUESTO"] );
    lpresupuestomap.insert ( "IDPRESUPUESTO", valores["IDPRESUPUESTO"] );
    lpresupuestomap.insert ( "IDARTICULO", valores["IDARTICULO"] );
    lpresupuestomap.insert ( "CODARTICULO", valores["CODARTICULO"] );
    lpresupuestomap.insert ( "NOMARTICULO", valores["NOMARTICULO"] );
    lpresupuestomap.insert ( "ABREVARTICULO", valores["ABREVARTICULO"] );
    lpresupuestomap.insert ( "CODIGOCOMPLETOARTICULO", valores["CODIGOCOMPLETOARTICULO"] );
    listalpresupuesto.append ( lpresupuestomap );
    _depura ( "END ImportBulmaFact::trataLPresupuesto", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataDPresupuesto()
{
    _depura ( "ImportBulmaFact::trataDPresupuesto", 0 );
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando DPresupuesto" ) + " " + valores["IDDPRESUPUESTO"] + "</B><BR>" );
    tvalores dpresupuestomap;
    dpresupuestomap.insert ( "IDDPRESUPUESTO", valores["IDDPRESUPUESTO"] );
    dpresupuestomap.insert ( "CONCEPTDPRESUPUESTO", valores["CONCEPTDPRESUPUESTO"] );
    dpresupuestomap.insert ( "PROPORCIONDPRESUPUESTO", valores["PROPORCIONDPRESUPUESTO"] );
    listadpresupuesto.append ( dpresupuestomap );
    _depura ( "END ImportBulmaFact::trataDPresupuesto", 0 );
    return 0;
}


///
/**
\return
**/
int ImportBulmaFact::trataPresupuesto()
{
    _depura ( "ImportBulmaFact::trataPresupuesto", 0 );
    QString query;
    BlDbRecordSet *cur;
    /// En el XML se ha encontrado un tag de cliente que esta almacenado en la estructura valores.
    pgimport->mensajeria ( "<HR><B>" + _( "Tratando presupuesto" ) + " " + valores["IDPRESUPUESTO"] + "</B><BR>" );
    /// Primero hacemos la recoleccion de valores.
    QString idpresupuesto = valores["IDPRESUPUESTO"];
    QString numpresupuesto = valores["NUMPRESUPUESTO"];
    QString refpresupuesto = valores["REFPRESUPUESTO"];
    QString fpresupuesto = valores["FPRESUPUESTO"];
    QString descpresupuesto = valores["DESCPRESUPUESTO"];
    QString contactpresupuesto = valores["CONTACTPRESUPUESTO"];
    QString telpresupuesto = valores["TELPRESUPUESTO"];
    QString vencpresupuesto = valores["VENCPRESUPUESTO"];
    QString comentpresupuesto = valores["COMENTPRESUPUESTO"];
    QString idusuari = valores["IDUSUARI"];
    QString procesadopresupuesto = valores["PROCESADOPRESUPUESTO"];
    QString idcliente = valores["IDCLIENTE"];
    QString idalmacen = valores["IDALMACEN"];
    QString idforma_pago = valores["IDFORMA_PAGO"];
    QString idtrabajador = valores["IDTRABAJADOR"];
    QString codigoalmacen = valores["CODIGOALMACEN"];
    QString nomalmacen = valores["NOMALMACEN"];
    QString diralmacen = valores["DIRALMACEN"];
    QString poblalamacen = valores["POBLALMACEN"];
    QString cpalmacen = valores["CPALMACEN"];
    QString telalmacen = valores["TELALMACEN"];
    QString nomcliente = valores["NOMCLIENTE"];
    QString nomaltcliente = valores["NOMALTCLIENTE"];
    QString cifcliente = valores["CIFCLIENTE"];
    QString bancocliente = valores["BANCOCLIENTE"];
    QString dircliente = valores["DIRCLIENTE"];
    QString poblcliente = valores["POBLCLIENTE"];
    QString cpcliente = valores["CPCLIENTE"];
    QString telcliente = valores["TELCLIENTE"];
    QString faxcliente = valores["FAXCLIENTE"];
    QString mailcliente = valores["MAILCLIENTE"];
    QString urlcliente = valores["URLCLIENTE"];
    QString provcliente = valores["PROVCLIENTE"];
    QString nomtrabajador = valores["NOMTRABAJADOR"];
    QString apellidostrabajador = valores["APELLIDOSTRABAJADOR"];
    QString dirtrabajador = valores["DIRTRABAJADOR"];
    QString nsstrabajador = valores["NSSTRABAJADOR"];
    QString teltrabajador = valores["TELTRABAJADOR"];
    QString moviltrabajador = valores["MOVILTRABAJADOR"];
    QString emailtrabajador = valores["EMAILTRABAJADOR"];
    QString descforma_pago = valores["DESCFORMA_PAGO"];
    QString dias1tforma_pago = valores["DIAS1TFORMA_PAGO"];
    QString descuentoforma_pago = valores["DESCUENTOFORMA_PAGO"];
    /// Completamos datos que puedan faltar.
    query = "SELECT * FROM almacen WHERE codigoalmacen = '" + codigoalmacen + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idalmacen = cur->valor ( "idalmacen" );
    } else {
        pgimport->mensajeria ( _( "El almacen del presupuesto no existe" ) + "<BR>\n" );
        idalmacen = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM cliente WHERE cifcliente='" + cifcliente + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idcliente = cur->valor ( "idcliente" );
    } else {
        pgimport->mensajeria ( _( "El cliente del presupuesto no existe" ) + "<BR>\n" );
        idcliente = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM forma_pago WHERE descforma_pago = '" + descforma_pago + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idforma_pago = cur->valor ( "idforma_pago" );
    } else {
        pgimport->mensajeria ( _( "La forma de pago del presupuesto no existe" ) + "<BR>\n" );
        idforma_pago = "NULL";
    } // end if
    delete cur;

    query = "SELECT * FROM trabajador WHERE nomtrabajador = '" + nomtrabajador + "' AND nsstrabajador = '" + nsstrabajador + "'";
    cur = dbConnection->loadQuery ( query );
    if ( !cur->eof() ) {
        idtrabajador = cur->valor ( "idtrabajador" );
    } else {
        pgimport->mensajeria ( _( "El empleado del presupuesto no existe" ) + "<BR>\n" );
        idtrabajador = "NULL";
    } // end if
    delete cur;
    /// Hacemos la insercion.
    query = " INSERT INTO presupuesto(numpresupuesto, refpresupuesto, fpresupuesto, descpresupuesto, contactpresupuesto, telpresupuesto, vencpresupuesto, comentpresupuesto, idusuari, procesadopresupuesto, idcliente, idalmacen, idforma_pago, idtrabajador) VALUES (";
    query += "NULL";
    query += ", '" + refpresupuesto + "'";
    query += ", '" + fpresupuesto + "'";
    query += ", '" + descpresupuesto + "'";
    query += ", '" + contactpresupuesto + "'";
    query += ", '" + telpresupuesto + "'";
    query += ", '" + vencpresupuesto + "'";
    query += ", '" + comentpresupuesto + "'";
    query += ", NULL";
    query += ",'" + procesadopresupuesto + "'";
    query += "," + idcliente;
    query += "," + idalmacen;
    query += "," + idforma_pago;
    query += "," + idtrabajador;
    query += ")";

    dbConnection->begin();
    dbConnection->runQuery ( query );
    cur = dbConnection->loadQuery ( "SELECT MAX(idpresupuesto) AS id FROM presupuesto" );
    idpresupuesto = cur->valor ( "id" );
    delete cur;
    dbConnection->commit();

    /// Tratamos la insercion de las lineas de presupuesto.
    tvalores *lpresupuestomap;

    QMutableListIterator<tvalores> ilistalpresupuesto ( listalpresupuesto );

    for ( lpresupuestomap = &listalpresupuesto.first(); lpresupuestomap; lpresupuestomap = &ilistalpresupuesto.next() ) {
        QString idlpresupuesto = ( *lpresupuestomap ) ["IDLPRESUPUESTO"];
        QString desclpresupuesto = ( *lpresupuestomap ) ["DESCLPRESUPUESTO"];
        QString cantlpresupuesto = ( *lpresupuestomap ) ["CANTLPRESUPUESTO"];
        QString pvplpresupuesto = ( *lpresupuestomap ) ["PVPLPRESUPUESTO"];
        QString ivalpresupuesto = ( *lpresupuestomap ) ["IVALPRESUPUESTO"];
        QString descuentolpresupuesto = ( *lpresupuestomap ) ["DESCUENTOLPRESUPUESTO"];
        QString idarticulo = ( *lpresupuestomap ) ["IDARTICULO"];
        QString codarticulo = ( *lpresupuestomap ) ["CODARTICULO"];
        QString nomarticulo = ( *lpresupuestomap ) ["NOMARTICULO"];
        QString abrevarticulo = ( *lpresupuestomap ) ["ABREVARTICULO"];
        QString codigocompletoarticulo = ( *lpresupuestomap ) ["CODIGOCOMPLETOARTICULO"];
        /// Completamos datos de la linea de presupuesto.
        query = "SELECT * FROM articulo WHERE codigocompletoarticulo = '" + codigocompletoarticulo + "'";
        cur = dbConnection->loadQuery ( query );
        if ( !cur->eof() ) {
            idarticulo = cur->valor ( "idarticulo" );
        } else {
            pgimport->mensajeria ( _( "El articulo de la linea de presupuesto no existe" ) + "<BR>\n" );
            idarticulo = "NULL";
        } // end if
        delete cur;
        /// Hacemos la insercion.
        query = " INSERT INTO lpresupuesto(desclpresupuesto, cantlpresupuesto, pvplpresupuesto, ivalpresupuesto, descuentolpresupuesto, idpresupuesto, idarticulo) VALUES (";
        query += "'" + desclpresupuesto + "'";
        query += ", " + cantlpresupuesto;
        query += ", " + pvplpresupuesto;
        query += ", " + ivalpresupuesto;
        query += ", " + descuentolpresupuesto;
        query += ", " + idpresupuesto;
        query += ", " + idarticulo;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    /// Tratamos la insercion de los descuentos de presupuesto.
    tvalores *dpresupuestomap;

    QMutableListIterator<tvalores> ilistadpresupuesto ( listadpresupuesto );

    for ( dpresupuestomap = &listadpresupuesto.first(); dpresupuestomap; dpresupuestomap = &ilistadpresupuesto.next() ) {
        QString iddpresupuesto = ( *dpresupuestomap ) ["IDDPRESUPUESTO"];
        QString conceptdpresupuesto = ( *dpresupuestomap ) ["CONCEPTDPRESUPUESTO"];
        QString proporciondpresupuesto = ( *dpresupuestomap ) ["PROPORCIONDPRESUPUESTO"];
        /// Hacemos la insercion.
        query = " INSERT INTO dpresupuesto(conceptdpresupuesto, proporciondpresupuesto, idpresupuesto) VALUES (";
        query += "'"  + conceptdpresupuesto + "'";
        query += ", " + proporciondpresupuesto;
        query += ", " + idpresupuesto;
        query += ")";
        dbConnection->runQuery ( query );
    } // end for
    valores.clear();
    _depura ( "END ImportBulmaFact::trataPresupuesto", 0 );
    return 0;
}

