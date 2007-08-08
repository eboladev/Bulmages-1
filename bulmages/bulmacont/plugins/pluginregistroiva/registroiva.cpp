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
#include "empresa.h"
#include "configuracion.h"
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"

typedef QMap<QString, Fixed> base;


RegistroIva::RegistroIva(empresa *comp, QWidget *parent) : FichaBc(comp, parent) {
    _depura("RegistroIva::RegistroIva", 0);
    setDBTableName("registroiva");
    setDBCampoId("idregistroiva");
    addDBCampo("idregistroiva", DBCampo::DBint, DBCampo::DBPrimaryKey, "Identificador");
    addDBCampo("contrapartida", DBCampo::DBint, DBCampo::DBNotNull, "Contrapartida");
    addDBCampo("baseimp", DBCampo::DBnumeric, DBCampo::DBNotNull, "Base Imponible");
    addDBCampo("iva", DBCampo::DBnumeric, DBCampo::DBNotNull, "Iva");
    addDBCampo("ffactura", DBCampo::DBdate, DBCampo::DBNotNull, "Fecha Factura");
    addDBCampo("idborrador", DBCampo::DBint, DBCampo::DBNotNull, "Borrador");
    addDBCampo("numorden", DBCampo::DBint, DBCampo::DBNotNull, "Orden");
    addDBCampo("cif", DBCampo::DBvarchar, DBCampo::DBNotNull, "CIF");
    addDBCampo("idfpago", DBCampo::DBint, DBCampo::DBNothing, "Forma de Pago");
    addDBCampo("incregistro", DBCampo::DBboolean, DBCampo::DBNotNull, "incregistro");
    addDBCampo("regularizacion", DBCampo::DBboolean, DBCampo::DBNothing, "regularizacion");
    addDBCampo("plan349", DBCampo::DBboolean, DBCampo::DBNothing, "plan 349");
    addDBCampo("factemitida", DBCampo::DBboolean, DBCampo::DBNotNull, "Factura Emitida");
    addDBCampo("rectificaaregistroiva", DBCampo::DBint, DBCampo::DBNothing, "Rectifica a");
    addDBCampo("factura", DBCampo::DBvarchar, DBCampo::DBNothing, "Factura");
    addDBCampo("femisionregistroiva", DBCampo::DBdate, DBCampo::DBNotNull, "Fecha Emision");
    addDBCampo("serieregistroiva", DBCampo::DBvarchar, DBCampo::DBNothing, "Serie");
    _depura("END RegistroIva::RegistroIva", 0);
}


RegistroIva::~RegistroIva() {}


int RegistroIva::borrar() {
    _depura("RegistroIva::borraRegistroIva", 0);
    if (DBvalue("idregistroiva") != "") {
        empresaBase()->begin();
        try {
            int error = empresaBase()->ejecuta("DELETE FROM iva WHERE idregistroiva = " + DBvalue("idregistroiva"));
            if (error) throw -1;

            error = empresaBase()->ejecuta("DELETE FROM registroiva WHERE idregistroiva = " + DBvalue("idregistroiva"));
            if (error) throw -1;
            empresaBase()->commit();
            _depura("Registro borrado satisfactoriamente", 2);
	    dialogChanges_cargaInicial();
	    close();
        } catch (...) {
            _depura("No se pudo borrar el registro de IVA", 2);
            empresaBase()->rollback();
        } // end try
    } // end if
    _depura("END RegistroIva::borraRegistroIva", 0);
    return 0;
}


void RegistroIva::vaciaRegistroIva() {
    DBclear();
}


void RegistroIva::pintaRegistroIva() {
    _depura("RegistroIva::pintaRegistroIva", 0);
    pintaidregistroiva(DBvalue("idregistroiva"));
    pintacontrapartida(DBvalue("contrapartida"));
    pintabaseimp(DBvalue("baseimp"));
    pintaiva(DBvalue("iva"));
    pintaffactura(DBvalue("ffactura"));
    pintafactura(DBvalue("factura"));
    pintaidborrador(DBvalue("idborrador"));
    pintaincregistro(DBvalue("incregistro"));
    pintaregularizacion(DBvalue("regularizacion"));
    pintaplan349(DBvalue("plan349"));
    pintanumorden(DBvalue("numorden"));
    pintacif(DBvalue("cif"));
    pintaidfpago(DBvalue("idfpago"));
    pintafactemitida(DBvalue("factemitida"));
    pintarectificaaregistroiva(DBvalue("rectificaaregistroiva"));
    pintafemisionregistroiva(DBvalue("femisionregistroiva"));
    pintaserieregistroiva(DBvalue("serieregistroiva"));
    calculaypintatotales();
    _depura("END RegistroIva::pintaRegistroIva", 0);
}


/// Esta funci&oacute;n carga un presupuesto.
int RegistroIva::cargar(QString id) {
    _depura("RegistroIva::cargaRegistroIva", 0);
    int error = 0;
    QString query = "SELECT * FROM registroiva WHERE idregistroiva = " + id;
    cursor2 *cur= empresaBase()->cargacursor(query);
    if (cur->error())
        error = 1;
    if (!cur->eof()) {
        DBload(cur);
    } // end if
    delete cur;
    /// Tratamiento de excepciones.
    if (error) {
        _depura("Error en la carga del registro\n", 0);
        return -1;
    } // end if
    dialogChanges_cargaInicial();
    _depura("END RegistroIva::cargaRegistroIva", 0);
    return 0;
}


int RegistroIva::guardar() {
    _depura("RegistroIva::guardaRegistroIva", 0);
    QString id;
    try {
        DBsave(id);
        setidregistroiva(id);
    } catch (...) {
        _depura("RegistroIva::guardar Error al guardar", 2);
        throw -1;
    } // end try
    _depura("END RegistroIva::guardaRegistroIva", 0);
    return 0;
}


/// Busca la cuenta del servicio.
/** Si hay varios servicios entonces tb deben pasarse en forma de lista. Aunque solo se
    necesite uno.
    \todo Los servicios no son todos los del grupo 6 y 7 son los 6x y 7x esto provoca que
    aparezca la ventana más veces de las deseadas.
    En estos momentos sólo se utiliza para calcular el total de la base imponible.
    Si todo va bien devuelve el idregistro
    Si hay errores devuelve -1
    Si no ha encontrado el registro pero no hay errores devuelve 0 **/
int RegistroIva::buscaborradorservicio(int idborrador) {
    _depura("RegistroIva::buscaborradorservicio", 0);
    QString SQLQuery;
    int registro = 0;
    try {
        SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.debe - borrador.haber AS baseimp FROM borrador, cuenta where borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = %d)", idborrador);
        empresaBase()->begin();
        int error = empresaBase()->ejecuta(SQLQuery);
        SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
        error = empresaBase()->ejecuta(SQLQuery);
        if (error) {
	    _depura("Error en la creacion del temporary table", 2);
            empresaBase()->rollback();
            return -1;
        } // end if
        /// Cogemos de la configuracion las cuentas que queremos que se apunten.
        /// Montamos los querys en base a la cadena cuentas.
        QString cuentas = "";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasIngresos'";
        cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
        if (!cur->eof()) {
            cuentas += cur->valor("valor");
        } // end if
        delete cur;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasGastos'";
        cur = empresaBase()->cargacursor(SQLQuery);
        if (!cur->eof()) {
            cuentas += ";" + cur->valor("valor");
        } // end if
        delete cur;
        cuentas.replace(';',"%|^");
        cuentas = "'^" + cuentas + "%'";
        /// Vamos a tener en cuenta tambi&eacute;n las cuentas de IVA.
        QString cuentasIVA = "";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'RegistroEmitida'";
        cur=empresaBase()->cargacursor(SQLQuery);
        if (!cur->eof()) {
            cuentasIVA += cur->valor("valor");
        } // end if
        delete cur;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'RegistroSoportada'";
        cur = empresaBase()->cargacursor(SQLQuery);
        if (!cur->eof()) {
            cuentasIVA += ";" + cur->valor("valor");
        } // end if
        delete cur;
        cuentasIVA.replace(';', "%|^");
        cuentasIVA = "'^" + cuentasIVA + "%'";
        /// Atentos que aqu&iacute; es donde se incorpora el par&aacute;metro.
        SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO " + cuentas + " OR codigo SIMILAR TO " + cuentasIVA;
        cur = empresaBase()->cargacursor(SQLQuery);
        while (!cur->eof() ) {
            fprintf(stderr, "idborrador: %s contrapartida: %s cuenta: %s\n", cur->valor("idborrador").toAscii().constData(), cur->valor("contrapartida").toAscii().constData(), cur->valor("codigo").toAscii().constData());
            registro = atoi(cur->valor("idborrador").toAscii().constData());
            cur->siguienteregistro();
        } //end while
        delete cur;
        /// Se calcula el total.
        /// El c&aacute;lculo se compara con el formato conocido de IVA
        /// (16, 7, 4: por tanto, formato de 2 dígitos con 0 decimales).
        SQLQuery = "SELECT abs(sum(baseimp)) AS subtotal FROM lacosa, (SELECT baseimp AS iva FROM lacosa WHERE codigo SIMILAR TO "+cuentasIVA+") AS iva WHERE codigo SIMILAR TO " + cuentas + " AND (iva.iva*100/baseimp)::NUMERIC(2,0) IN (SELECT porcentajetipoiva FROM tipoiva)";
        cur = empresaBase()->cargacursor(SQLQuery);
        if (!cur->eof() ) {
            setbaseimp(cur->valor("subtotal"));
        } // end while
        delete cur;
        SQLQuery = "DROP TABLE lacosa";
        error = empresaBase()->ejecuta(SQLQuery);
        empresaBase()->commit();
    } catch (...) {
        _depura("RegistroIva:: Error en transaccion", 2);
        empresaBase()->rollback();
    } // end try
    _depura("END RegistroIva::buscaborradorservicio", 0);
    return registro;
}


/// Busca la cuenta del cliente o del proveedor en el apunte que se ha seleccionado.
/** Se basa en el supuesto de que si la cuenta no es el cliente entonces la contrapartida
    de la cuenta seguro que es el cliente. **/
int RegistroIva::buscaborradorcliente(int idborrador) {
    _depura("RegistroIva::buscaborradorcliente", 0);
    QString SQLQuery;
    int registro = 0;
    try {
        empresaBase()->begin();
        SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.cifent_cuenta, cuenta.idcuenta AS idcuenta, codigo, borrador.debe AS debe, borrador.haber AS haber, borrador.debe+borrador.haber AS totalfactura FROM borrador LEFT JOIN cuenta ON borrador.idcuenta=cuenta.idcuenta where borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = %d)", idborrador);
        int error = empresaBase()->ejecuta(SQLQuery);

        SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
        error = empresaBase()->ejecuta(SQLQuery);
        /// Cogemos de la configuracion las cuentas que queremos que se apunten.
        /// Montamos los querys en base a la cadena cuentas.
        /// Se consideran cuentas de Derechos y de Obligaciones a Clientes y Proveedores,
        /// respectivamente.
        /// Los campos sirven para encontrar la cuenta que corresponde a quien hace el pago
        /// de la factura.
        QString cuentas = "";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasDerechos'";
        cursor2 *cur1 = empresaBase()->cargacursor(SQLQuery);
        if (!cur1->eof()) {
            cuentas += cur1->valor("valor");
        } // end if
        delete cur1;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre = 'CuentasObligaciones'";
        cur1 = empresaBase()->cargacursor(SQLQuery);
        if (!cur1->eof()) {
            cuentas += ";" + cur1->valor("valor");
        } // end if
        delete cur1;
        cuentas.replace(';', "%|^");
        cuentas = "'^" + cuentas + "%'";

        /// Atentos que aqu&iacute; es donde se incorpora el par&aacute;metro.
        SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO " + cuentas;
        cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
        while (!cur->eof()) {
            /// Ponemos la cuenta de cliente y los valores adyacentes.
            setcontrapartida(cur->valor("idcuenta"));
            setcif(cur->valor("cifent_cuenta"));
            /// Comprobamos si es un cliente o un proveedor y segun sea actuamos en
            /// consecuencia.
            if (cur->valor("codigo").left(2) == "43") {
                setfactemitida("t");
            } else {
                setfactemitida( "f");
            } // end if
            registro = cur->valor("idborrador").toInt();
            cur->siguienteregistro();
        } // end while
        delete cur;
        SQLQuery = "DROP TABLE lacosa";
        error = empresaBase()->ejecuta(SQLQuery);
        empresaBase()->commit();
    } catch (...) {
        _depura("RegistroIva:: Error en buscaborradorcliente", 2);
        empresaBase()->rollback();
    } // end try
    _depura("ENd RegistroIva::buscaborradorcliente", 0);
    return registro;
}


/**
  * \brief Inicializa el registro de iva. Calculando los datos que son buscables (O encontrables a partir del asiento).
  * Esquema:
  * 1.- Comprobamos que no haya ya un registro con la factura
  * 2.- Si hay factura la ponemos, si no la hay sacamos los datos que podemos y los presentamos
  */
void RegistroIva::inicializa1(int idapunte1) {
    _depura("RegistroIva::inicializa1", 0);
    QString query, cadena;
    int idborrador;
    /// Busca si a este apunte le corresponde un borrador.
    idborrador = buscaborradorcliente(idapunte1);
    /// En el caso de que no existiese una cuenta de servicio a la que asociar se
    /// Asocia directamente al apunte seleccionado.
    if (idborrador <= 0)
        idborrador = idapunte1;
    ///Busca entradas de IVA en la tabla registroiva.
    query.sprintf ( "SELECT * FROM registroiva WHERE idborrador = %d", idborrador);
    cursor2 *cursoriva = empresaBase()->cargacursor(query);
    if (!cursoriva->eof()) {
        /// El registro ya existe.
        cargar( cursoriva->valor("idregistroiva"));
    } else {
        /// El registro no existe y hay que hacer la propuesta m&aacute;s acertada de registro.
        /// Buscamos en todo el asiento las cuentas de IVA y lo reflejamos.
        buscaborradoriva(idapunte1);
        /// Buscamos la fecha que le corresponderá teoricamente a la factura.
        buscafecha(idapunte1);
        /// Buscamos la cuenta de servicio.
        buscaborradorservicio(idapunte1);
        buscaNumFactura(idapunte1);
        setidborrador(QString::number(idapunte1));
    } // end if
    delete cursoriva;
    pintaRegistroIva();
    _depura("END RegistroIva::inicializa1", 0);
}


/** \brief busca las cuenta de IVA en el apunte que se ha seleccionado.
  * Esta funcion carga, dado un apunte y un asiento todas las cuentas de iva en el registro de iva 
  * que corresponden con la partida del asiento.
  * Los pasa en la tabla m_listIva. Tambien busca la fecha del asiento y la pone en m_ffactura
  * Devuelve 0 si todo va bien
  * Devuelve 1 si ha ocurrido algún error.
  */
int RegistroIva::buscaborradoriva(int idborrador) {
    _depura("RegistroIva::buscaborradoriva", 0);
    int error = 0;
    try {
        empresaBase()->begin();
        QString SQLQuery;
        SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT borrador.debe AS ivadebe, borrador.haber AS ivahaber, idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.fecha AS fecha  FROM borrador, cuenta WHERE borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = %d)", idborrador);
        error = empresaBase()->ejecuta(SQLQuery);
	if (error) {
		_depura("error en la base de datos", 2);
		_depura(SQLQuery, 2);
	} // end if

        SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
        error = empresaBase()->ejecuta(SQLQuery);
	if (error) {
		_depura("error en la base de datos", 2);
		_depura(SQLQuery, 2);
	} // end if
        /// Cargamos los registros que quedan porque seguro que son de IVA.
        SQLQuery =  "SELECT *, GREATEST(debe*100/porcentajetipoiva, haber*100/porcentajetipoiva)::NUMERIC(12,1) AS baseiva, GREATEST (ivadebe, ivahaber) AS ivaiva FROM tipoiva ";
	SQLQuery += " LEFT JOIN (SELECT *, ivadebe AS debe, ivahaber AS haber FROM lacosa ) AS lacosa ON tipoiva.idcuenta=lacosa.idcuenta ";
        cursor2 *cur = empresaBase()->cargacursor(SQLQuery);
        m_lineas->cargar(cur);
	recalculaIva();
        _depura("limpiamos la base de datos");
        SQLQuery = "DROP TABLE lacosa";
        error = empresaBase()->ejecuta(SQLQuery);
        empresaBase()->commit();
    } catch (...) {
        _depura("Error al buscar el borrador", 2);
        empresaBase()->rollback();
    } // end try
    _depura("END RegistroIva::buscaborradoriva", 0);
    return 0;
}


/// SLOT que se dispara cuando se ha hecho el bot&oacute;n de b&uacute;squeda de una fecha.
void RegistroIva::buscafecha(int idborrador) {
    _depura("RegistroIva::buscafecha", 0);
    QString SQLQuery;
    cursor2 *cur = empresaBase()->cargacursor("SELECT fecha from borrador WHERE idborrador = " + QString::number(idborrador));
    if (!cur->eof()) {
        setffactura(cur->valor("fecha").left(10));
        setfemisionregistroiva(cur->valor("fecha").left(10));
    } // end if
    delete cur;
    _depura("END RegistroIva::buscafecha", 0);
}


/// Proponemos un n&uacute;mero de factura si se trata de una venta y proponemos un
/// n&uacute;mero de orden si se trata de una compra.
/// Realmente esto se tendria que implementar con contadores en la base de datos.
/// Primero comprobamos si esta factura ya tiene un apunte de IVA distinto y cogemos
/// el mismo n&uacute;mero de factura.
void RegistroIva::buscaNumFactura(int idborrador) {
    QString query;
    cursor2 *recordset;
    QString cadena;
    int numord;
    query.sprintf("SELECT factura, numorden FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=(SELECT idasiento FROM borrador WHERE idborrador = '%i'))", idborrador);
    recordset = empresaBase()->cargacursor(query);
    if (!recordset->eof()) {
            setfactura( recordset->valor("factura"));
            setnumorden( recordset->valor("numorden"));
    } else { /// La factura no existe, entonces proponemos el siguiente n&uacute;mero de factura.
            /// Vemos si podemos extraer de la descripci&oacute;n del apunte el
            /// n&uacute;mero de factura.
            query.sprintf("SELECT * FROM borrador WHERE idasiento IN (SELECT idasiento FROM borrador WHERE idborrador = '%i') AND debe = 0", idborrador);
            recordset = empresaBase()->cargacursor(query, "recordset");

            QString num_ser = recordset->valor("conceptocontable");
            QRegExp patrons(" ([A-Z]*)[0-9]+$");
            if (patrons.indexIn(num_ser) > -1) { /// Si se cumple el patr&oacute;n
                num_ser = patrons.cap(1);
            } else { /// Si no saco el nº de fra. lo pongo a cero, por poner algo.
                num_ser = "Normal";
            }
            setserieregistroiva(num_ser);

            QString num_fra = recordset->valor("conceptocontable");
            QRegExp patron(" [A-Z]*([0-9]+)$");
            if (patron.indexIn(num_fra) > -1) { /// Si se cumple el patr&oacute;n
                num_fra = patron.cap(1);
            } else { /// Si no saco el nº de fra. lo pongo a cero, por poner algo.
                num_fra = "0";
            }
            setfactura(num_fra);
            /// Buscamos qu&eacute; n&uacute;mero de orden poner.
            query.sprintf("SELECT MAX(to_number(numorden,'99999')) AS numorden FROM registroiva WHERE numorden <> ''");
            recordset = empresaBase()->cargacursor(query, "recordset");
            if (!recordset->eof()) {
                numord = 1 + atoi(recordset->valor("numorden").toAscii().constData());
            } else {
                numord = 1;
            } // end if
            setnumorden(QString::number(numord));
    } // end if
    delete recordset; /// Fin proposici&oacute;n n&uacute;meros factura y orden.
}

