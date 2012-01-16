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

#include <QFile>
#include <QTextStream>

#include "registroiva.h"
#include "bccompany.h"
#include "blconfiguration.h"
#include "blfixed.h"
#include "blfunctions.h"
#include "blplugins.h"


typedef QMap<QString, BlFixed> base;


///
/**
\param comp
\param parent
**/
RegistroIva::RegistroIva ( BcCompany *comp, QWidget *parent ) : BcForm ( comp, parent )
{
    BL_FUNC_DEBUG
    setTitleName ( _ ( "Registro IVA" ) );
    setDbTableName ( "registroiva" );
    setDbFieldId ( "idregistroiva" );
    addDbField ( "idregistroiva", BlDbField::DbInt, BlDbField::DbPrimaryKey, _ ( "Identificador" ) );
    addDbField ( "contrapartida", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Contrapartida" ) );
    addDbField ( "baseimp", BlDbField::DbNumeric, BlDbField::DbNotNull, _ ( "Base Imponible" ) );
    addDbField ( "iva", BlDbField::DbNumeric, BlDbField::DbNotNull, _ ( "IVA" ) );
    addDbField ( "ffactura", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha factura" ) );
    addDbField ( "idborrador", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Borrador" ) );
    addDbField ( "numorden", BlDbField::DbInt, BlDbField::DbNotNull, _ ( "Orden" ) );
    addDbField ( "cif", BlDbField::DbVarChar, BlDbField::DbNotNull, _ ( "CIF" ) );
    addDbField ( "idfpago", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Forma de pago" ) );
    addDbField ( "incregistro", BlDbField::DbBoolean, BlDbField::DbNotNull, _ ( "Inc registro" ) );
    addDbField ( "regularizacion", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "Regularizacion" ) );
    addDbField ( "plan349", BlDbField::DbBoolean, BlDbField::DbNothing, _ ( "plan 349" ) );
    addDbField ( "factemitida", BlDbField::DbBoolean, BlDbField::DbNotNull, _ ( "Factura emitida" ) );
    addDbField ( "rectificaaregistroiva", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Rectifica a" ) );
    addDbField ( "factura", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Factura" ) );
    addDbField ( "femisionregistroiva", BlDbField::DbDate, BlDbField::DbNotNull, _ ( "Fecha emision" ) );
    addDbField ( "serieregistroiva", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Serie" ) );
    /// Llamamos a los scripts
    blScript(this);

    
}


///
/**
**/
RegistroIva::~RegistroIva()
{
    BL_FUNC_DEBUG
    
}


///
/**
\return
**/
int RegistroIva::remove()
{
    BL_FUNC_DEBUG
    if ( dbValue ( "idregistroiva" ) != "" ) {
        mainCompany() ->begin();
        try {
            int error = mainCompany() ->runQuery ( "DELETE FROM iva WHERE idregistroiva = " + dbValue ( "idregistroiva" ) );

            if ( error ) throw - 1;

            error = mainCompany() ->runQuery ( "DELETE FROM registroiva WHERE idregistroiva = " + dbValue ( "idregistroiva" ) );

            if ( error ) throw - 1;

            mainCompany() ->commit();
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Registro borrado correctamente.") );
            dialogChanges_readValues();
            close();
        } catch ( ... ) {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error. No se pudo borrar el registro de IVA.") );
            mainCompany() ->rollback();
        } // end try
    } // end if
    
    return 0;
}


///
/**
**/
void RegistroIva::vaciaRegistroIva()
{
    BL_FUNC_DEBUG
    DBclear();
    
}


///
/**
**/
void RegistroIva::pintaRegistroIva()
{
    BL_FUNC_DEBUG
    pintaidregistroiva ( dbValue ( "idregistroiva" ) );
    pintacontrapartida ( dbValue ( "contrapartida" ) );
    pintabaseimp ( dbValue ( "baseimp" ) );
    pintaiva ( dbValue ( "iva" ) );
    pintaffactura ( dbValue ( "ffactura" ) );
    pintafactura ( dbValue ( "factura" ) );
    pintaidborrador ( dbValue ( "idborrador" ) );
    pintaincregistro ( dbValue ( "incregistro" ) );
    pintaregularizacion ( dbValue ( "regularizacion" ) );
    pintaplan349 ( dbValue ( "plan349" ) );
    pintanumorden ( dbValue ( "numorden" ) );
    pintacif ( dbValue ( "cif" ) );
    pintaidfpago ( dbValue ( "idfpago" ) );
    pintafactemitida ( dbValue ( "factemitida" ) );
    pintarectificaaregistroiva ( dbValue ( "rectificaaregistroiva" ) );
    pintafemisionregistroiva ( dbValue ( "femisionregistroiva" ) );
    pintaserieregistroiva ( dbValue ( "serieregistroiva" ) );
    calculaPintaTotales();
    
}


/// Esta funci&oacute;n carga un presupuesto.
/**
\param id
\return
**/
int RegistroIva::load ( QString id )
{
    BL_FUNC_DEBUG
    int error = 0;
    QString query = "SELECT * FROM registroiva WHERE idregistroiva = " + id;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( query );

    if ( cur->error() ) error = 1;

    if ( !cur->eof() ) {
        DBload ( cur );
    } // end if

    delete cur;

    /// Tratamiento de excepciones.
    if ( error ) {
	
        return -1;
    } // end if

    dialogChanges_readValues();
    
    return 0;
}


///
/**
\param return
**/
int RegistroIva::save()
{
    BL_FUNC_DEBUG
    QString id;
    try {
        dbSave ( id );
        setidregistroiva ( id );
    } catch ( ... ) {
	
        throw - 1;
    } // end try
    
    return 0;
}


/// Busca la cuenta del servicio.
/** Si hay varios servicios entonces tb deben pasarse en forma de lista. Aunque solo se
    necesite uno.
    \todo Los servicios no son todos los del grupo 6 y 7 son los 6x y 7x esto provoca que
    aparezca la ventana mas veces de las deseadas.
    En estos momentos solo se utiliza para calcular el total de la base imponible.
    Si todo va bien devuelve el idregistro
    Si hay errores devuelve -1
    Si no ha encontrado el registro pero no hay errores devuelve 0 **/
/**
\param idborrador
\return
**/
int RegistroIva::buscaborradorservicio ( int idborrador )
{
    BL_FUNC_DEBUG
    QString SQLQuery;
    int registro = 0;
    try {
        SQLQuery.sprintf ( "CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.debe - borrador.haber AS baseimp FROM borrador, cuenta where borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = %d)", idborrador );
        mainCompany() ->begin();
        int error = mainCompany() ->runQuery ( SQLQuery );
        SQLQuery.sprintf ( "DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador );
        error = mainCompany() ->runQuery ( SQLQuery );

        if ( error ) {
	    
            mainCompany() ->rollback();
            return -1;
        } // end if

        /// Cogemos de la configuracion las cuentas que queremos que se apunten.
        /// Montamos los querys en base a la cadena cuentas.
        QString cuentas = "";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasIngresos'";
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            cuentas += cur->value( "valor" );
        } // end if

        delete cur;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasGastos'";
        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            cuentas += ";" + cur->value( "valor" );
        } // end if

        delete cur;
        cuentas.replace ( ';', "%|^" );
        cuentas = "'^" + cuentas + "%'";
        /// Vamos a tener en cuenta tambi&eacute;n las cuentas de IVA.
        QString cuentasIVA = "";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'RegistroEmitida'";
        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            cuentasIVA += cur->value( "valor" );
        } // end if

        delete cur;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'RegistroSoportada'";
        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            cuentasIVA += ";" + cur->value( "valor" );
        } // end if

        delete cur;
        cuentasIVA.replace ( ';', "%|^" );
        cuentasIVA = "'^" + cuentasIVA + "%'";
        /// Atentos que aqu&iacute; es donde se incorpora el par&aacute;metro.
        SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO " + cuentas + " OR codigo SIMILAR TO " + cuentasIVA;
        cur = mainCompany() ->loadQuery ( SQLQuery );

        while ( !cur->eof() ) {
            fprintf ( stderr, "idborrador: %s contrapartida: %s cuenta: %s\n", cur->value( "idborrador" ).toAscii().constData(), cur->value( "contrapartida" ).toAscii().constData(), cur->value( "codigo" ).toAscii().constData() );
            registro = atoi ( cur->value( "idborrador" ).toAscii().constData() );
            cur->nextRecord();
        } //end while

        delete cur;
        /// Se calcula el total.
        /// El c&aacute;lculo se compara con el formato conocido de IVA
        /// (16, 7, 4: por tanto, formato de 2 digitos con 0 decimales).
        SQLQuery = "SELECT abs(sum(baseimp)) AS subtotal FROM lacosa, (SELECT baseimp AS iva FROM lacosa WHERE codigo SIMILAR TO " + cuentasIVA + ") AS iva WHERE codigo SIMILAR TO " + cuentas + " AND (iva.iva*100/baseimp)::NUMERIC(2,0) IN (SELECT porcentajetipoiva FROM tipoiva)";
        cur = mainCompany() ->loadQuery ( SQLQuery );

        if ( !cur->eof() ) {
            setbaseimp ( cur->value( "subtotal" ) );
        } // end while

        delete cur;
        SQLQuery = "DROP TABLE lacosa";
        error = mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error en la transaccion.") );
        mainCompany() ->rollback();
    } // end try
    
    return registro;
}


/// Busca la cuenta del cliente o del proveedor en el apunte que se ha seleccionado.
/** Se basa en el supuesto de que si la cuenta no es el cliente entonces la contrapartida
    de la cuenta seguro que es el cliente. **/
/**
\param idborrador
\return
**/
int RegistroIva::buscaborradorcliente ( int idborrador )
{
    BL_FUNC_DEBUG
    QString SQLQuery;
    int registro = 0;
    try {
        mainCompany() ->begin();
        SQLQuery.sprintf ( "CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.cifent_cuenta, cuenta.idcuenta AS idcuenta, codigo, borrador.debe AS debe, borrador.haber AS haber, borrador.debe+borrador.haber AS totalfactura FROM borrador LEFT JOIN cuenta ON borrador.idcuenta=cuenta.idcuenta where borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = %d)", idborrador );
        int error = mainCompany() ->runQuery ( SQLQuery );

        SQLQuery.sprintf ( "DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador );
        error = mainCompany() ->runQuery ( SQLQuery );
        /// Cogemos de la configuracion las cuentas que queremos que se apunten.
        /// Montamos los querys en base a la cadena cuentas.
        /// Se consideran cuentas de Derechos y de Obligaciones a Clientes y Proveedores,
        /// respectivamente.
        /// Los campos sirven para encontrar la cuenta que corresponde a quien hace el pago
        /// de la factura.
        QString cuentas = "";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasDerechos'";
        BlDbRecordSet *cur1 = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur1->eof() ) {
            cuentas += cur1->value( "valor" );
        } // end if
        delete cur1;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasObligaciones'";
        cur1 = mainCompany() ->loadQuery ( SQLQuery );
        if ( !cur1->eof() ) {
            cuentas += ";" + cur1->value( "valor" );
        } // end if
        delete cur1;
        cuentas.replace ( ';', "%|^" );
        cuentas = "'^" + cuentas + "%'";

        /// Atentos que aqu&iacute; es donde se incorpora el par&aacute;metro.
        SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO " + cuentas;
        BlDbRecordSet *cur = mainCompany() ->loadQuery ( SQLQuery );
        while ( !cur->eof() ) {
            /// Ponemos la cuenta de cliente y los valores adyacentes.
            setcontrapartida ( cur->value( "idcuenta" ) );
            setcif ( cur->value( "cifent_cuenta" ) );
            /// Comprobamos si es un cliente o un proveedor y segun sea actuamos en
            /// consecuencia.
            if ( cur->value( "codigo" ).left ( 2 ) == "43" ) {
                setfactemitida ( "t" );
            } else {
                setfactemitida ( "f" );
            } // end if
            registro = cur->value( "idborrador" ).toInt();
            cur->nextRecord();
        } // end while
        delete cur;
        SQLQuery = "DROP TABLE lacosa";
        error = mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error en la busqueda.") );
        mainCompany() ->rollback();
    } // end try

    
    return registro;
}


/**
  * \brief Inicializa el registro de iva. Calculando los datos que son buscables (O encontrables a partir del asiento).
  * Esquema:
  * 1.- Comprobamos que no haya ya un registro con la factura
  * 2.- Si hay factura la ponemos, si no la hay sacamos los datos que podemos y los presentamos
  */


///
/**
\param idapunte1
**/
void RegistroIva::inicializa1 ( int idapunte1 )
{
    BL_FUNC_DEBUG
    QString query, cadena;
    int idborrador;
    /// Busca si a este apunte le corresponde un borrador.
    idborrador = buscaborradorcliente ( idapunte1 );
    /// En el caso de que no existiese una cuenta de servicio a la que asociar se
    /// Asocia directamente al apunte seleccionado.
    if ( idborrador <= 0 )
        idborrador = idapunte1;
    ///Busca entradas de IVA en la tabla registroiva.
    query.sprintf ( "SELECT * FROM registroiva WHERE idborrador = %d", idborrador );
    BlDbRecordSet *cursoriva = mainCompany() ->loadQuery ( query );
    if ( !cursoriva->eof() ) {
        /// El registro ya existe.
        load ( cursoriva->value( "idregistroiva" ) );
    } else {
        /// El registro no existe y hay que hacer la propuesta m&aacute;s acertada de registro.
        /// Buscamos en todo el asiento las cuentas de IVA y lo reflejamos.
        buscaborradoriva ( idapunte1 );
        /// Buscamos la fecha que le correspondera teoricamente a la factura.
        buscafecha ( idapunte1 );
        /// Buscamos la cuenta de servicio.
        buscaborradorservicio ( idapunte1 );
        buscaNumFactura ( idapunte1 );
        setidborrador ( QString::number ( idapunte1 ) );
    } // end if
    delete cursoriva;
    pintaRegistroIva();
    
}


/** \brief busca las cuenta de IVA en el apunte que se ha seleccionado.
  * Esta funcion carga, dado un apunte y un asiento todas las cuentas de iva en el registro de iva
  * que corresponden con la partida del asiento.
  * Los pasa en la tabla m_listIva. Tambien busca la fecha del asiento y la pone en m_ffactura
  * Devuelve 0 si todo va bien
  * Devuelve 1 si ha ocurrido algun error.
  */


///
/**
\param idborrador
\return
**/
int RegistroIva::buscaborradoriva ( int idborrador )
{
    BL_FUNC_DEBUG
    int error = 0;
    try {
        mainCompany() ->begin();
        QString SQLQuery;
        SQLQuery.sprintf ( "CREATE TEMPORARY TABLE lacosa AS SELECT borrador.debe AS ivadebe, borrador.haber AS ivahaber, idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.fecha AS fecha  FROM borrador, cuenta WHERE borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = %d)", idborrador );
        error = mainCompany() ->runQuery ( SQLQuery );
        if ( error ) {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Error en la consulta: '%1'.")).arg(SQLQuery) );
        } // end if

        SQLQuery.sprintf ( "DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador );
        error = mainCompany() ->runQuery ( SQLQuery );
        if ( error ) {
	    BlDebug::blDebug ( Q_FUNC_INFO, 0, QString(_("Error en la consulta: '%1'.")).arg(SQLQuery) );
        } // end if

        /// Cargamos los registros que quedan porque seguro que son de IVA.
        SQLQuery =  "SELECT *, GREATEST(debe * 100 / porcentajetipoiva, haber * 100 / porcentajetipoiva)::NUMERIC(12, 1) AS baseiva, GREATEST (ivadebe, ivahaber) AS ivaiva FROM tipoiva ";
        SQLQuery += " LEFT JOIN (SELECT *, ivadebe AS debe, ivahaber AS haber FROM lacosa) AS lacosa ON tipoiva.idcuenta = lacosa.idcuenta ";
        //22/09/07 Ahora se pasa el query
        //BlDbRecordSet *cur = mainCompany()->loadQuery(SQLQuery);
        m_lineas->load ( SQLQuery );
        recalculaIva();

	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Limpiamos la base de datos.") );
        SQLQuery = "DROP TABLE lacosa";
        error = mainCompany() ->runQuery ( SQLQuery );
        mainCompany() ->commit();
    } catch ( ... ) {
	BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Error en la busqueda.") );
        mainCompany() ->rollback();
    } // end try
    
    return 0;
}


/// SLOT que se dispara cuando se ha hecho el bot&oacute;n de b&uacute;squeda de una fecha.
/**
\param idborrador
**/
void RegistroIva::buscafecha ( int idborrador )
{
    BL_FUNC_DEBUG
    QString SQLQuery;
    BlDbRecordSet *cur = mainCompany() ->loadQuery ( "SELECT fecha from borrador WHERE idborrador = " + QString::number ( idborrador ) );
    if ( !cur->eof() ) {
        setffactura ( cur->value( "fecha" ).left ( 10 ) );
        setfemisionregistroiva ( cur->value( "fecha" ).left ( 10 ) );
    } // end if
    delete cur;
    
}


/// Proponemos un n&uacute;mero de factura si se trata de una venta y proponemos un
/// n&uacute;mero de orden si se trata de una compra.
/// Realmente esto se tendria que implementar con contadores en la base de datos.
/// Primero comprobamos si esta factura ya tiene un apunte de IVA distinto y cogemos
/// el mismo n&uacute;mero de factura.
/**
\param idborrador
**/
void RegistroIva::buscaNumFactura ( int idborrador )
{
    BL_FUNC_DEBUG
    QString query;
    BlDbRecordSet *recordset;
    QString cadena;
    int numord;
    query.sprintf ( "SELECT factura, numorden FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=(SELECT idasiento FROM borrador WHERE idborrador = '%i'))", idborrador );
    recordset = mainCompany() ->loadQuery ( query );
    if ( !recordset->eof() ) {
        setfactura ( recordset->value( "factura" ) );
        setnumorden ( recordset->value( "numorden" ) );
    } else {
        /// La factura no existe, entonces proponemos el siguiente n&uacute;mero de factura.
        /// Vemos si podemos extraer de la descripci&oacute;n del apunte el
        /// n&uacute;mero de factura.
        query.sprintf ( "SELECT * FROM borrador WHERE idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = '%i') AND debe = 0", idborrador );
        recordset = mainCompany() ->loadQuery ( query, "recordset" );

        QString num_ser = recordset->value( "conceptocontable" );
        QRegExp patrons ( " ([A-Z]*)[0-9]+$" );
        if ( patrons.indexIn ( num_ser ) > -1 ) { /// Si se cumple el patr&oacute;n
            num_ser = patrons.cap ( 1 );
        } else { /// Si no saco el nº de fra. lo pongo a cero, por poner algo.
            num_ser = "Normal";
        } // end if
        setserieregistroiva ( num_ser );

        QString num_fra = recordset->value( "conceptocontable" );
        QRegExp patron ( " [A-Z]*([0-9]+)$" );
        if ( patron.indexIn ( num_fra ) > -1 ) { /// Si se cumple el patr&oacute;n
            num_fra = patron.cap ( 1 );
        } else { /// Si no saco el nº de fra. lo pongo a cero, por poner algo.
            num_fra = "0";
        } // end if
        setfactura ( num_fra );
        /// Buscamos qu&eacute; n&uacute;mero de orden poner.
        query.sprintf ( "SELECT MAX(to_number(numorden,'99999')) AS numorden FROM registroiva WHERE numorden <> ''" );
        recordset = mainCompany() ->loadQuery ( query, "recordset" );
        if ( !recordset->eof() ) {
            numord = 1 + atoi ( recordset->value( "numorden" ).toAscii().constData() );
        } else {
            numord = 1;
        } // end if
        setnumorden ( QString::number ( numord ) );
    } // end if
    delete recordset; /// Fin proposici&oacute;n n&uacute;meros factura y orden.
    
}
