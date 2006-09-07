//
// C++ Implementation: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "registroiva.h"
#include "empresa.h"
#include "configuracion.h"
#include <QFile>
#include <QTextStream>
#include "fixed.h"
#include "funcaux.h"
#include "plugins.h"

/*
CREATE TABLE registroiva (
    idregistroiva serial PRIMARY KEY,
    contrapartida integer REFERENCES cuenta(idcuenta),
    baseimp numeric(12,2),
    iva numeric(12,2),		-- De solo lectura
    ffactura date,
    factura character varying(70),
    idborrador integer NOT NULL REFERENCES borrador(idborrador),
    incregistro boolean,
    regularizacion boolean,
    plan349 boolean,
    numorden character varying(50),
    cif character varying(25),
    idfpago integer REFERENCES fpago(idfpago),
    factemitida boolean NOT NULL,
    rectificaaregistroiva integer REFERENCES registroiva(idregistroiva)
);
*/




typedef QMap<QString, Fixed> base;

RegistroIva::RegistroIva(empresa *comp) : DBRecord(comp) {
    _depura("RegistroIva::RegistroIva", 0);
    m_companyact=comp;
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
    _depura("RegistroIva::borraRegistroIva",0);
    if (DBvalue("idpresupuesto") != "") {
        m_listalineas->borrar();
        m_listadescuentos->borrar();
        m_companyact->begin();
        int error = m_companyact->ejecuta("DELETE FROM registroiva WHERE idregistroiva="+DBvalue("idregistroiva"));
        if (error) {
            m_companyact->rollback();
            return -1;
        }// end if
        m_companyact->commit();
        vaciaRegistroIva();
        pintaRegistroIva();
    }// end if
    _depura("END RegistroIva::borraRegistroIva",0);
    return 0;
}


void RegistroIva::vaciaRegistroIva() {
    DBclear();
}

void RegistroIva::pintaRegistroIva() {
    _depura("RegistroIva::pintaRegistroIva",0);
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

    // Pinta el subformulario de detalle del presupuesto.
    m_listalineas->pintaListIva();
    m_listadescuentos->pintaListLinPrevCobro();

    calculaypintatotales();
    _depura("END RegistroIva::pintaRegistroIva",0);
}


// Esta funci� carga un presupuesto.
int RegistroIva::cargar(QString id) {
    _depura("RegistroIva::cargaRegistroIva",0);
    int error = 0;
    QString query = "SELECT * FROM registroiva WHERE idregistroiva="+id;
    cursor2 * cur= m_companyact->cargacursor(query);
    if (cur->error())
        error =1;
    if (!cur->eof()) {
        DBload(cur);
    }// end if
    delete cur;

    error += m_listalineas->cargaListIva(id);
    m_listadescuentos->setidregistroiva(id);
    error += m_listadescuentos->chargeBudgetLines();

    /// Tratamiento de excepciones
    if (error) {
        _depura("Error en la carga del registro\n",0);
        return -1;
    }// end if

    _depura("END RegistroIva::cargaRegistroIva",0);

    //    pintaRegistroIva();
    return 0;
}


int RegistroIva::guardar() {
    _depura("RegistroIva::guardaRegistroIva",0);
    QString id;
    m_companyact->begin();
    int error = DBsave(id);
    if (error ) {
        m_companyact->rollback();
        return -1;
    }// end if
    setidregistroiva(id);
    m_listalineas->setidregistroiva(id);
    m_companyact->commit();
    m_listalineas->guardaListIva();
    m_listadescuentos->guardaListLinPrevCobro();
    //    cargaRegistroIva(id);
    _depura("END RegistroIva::guardaRegistroIva",0);
    return 0;
}






/** \brief busca la cuenta del servicio
  *
  * Si hay varios servicios entonces tb deben pasarse en forma de lista. Aunque sólo se necesite uno.
  * \todo Los servicios no son todos los del grupo 6 y 7 son los 6x y 7x esto provoca que aparezca la ventana más veces de las deseadas.
  * En estos momentos sólo se utiliza para calcular el total de la base imponible.
  * Si todo va bien devuelve el idregistro
  * Si hay errores devuelve -1
  * Si no ha encontrado el registro pero no hay errores devuelve 0
  */
int RegistroIva::buscaborradorservicio(int idborrador) {
    _depura("RegistroIva::buscaborradorservicio", 0);
    QString SQLQuery;
    int registro=0;
    try {
	SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.debe - borrador.haber AS baseimp FROM borrador, cuenta where borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador=%d)", idborrador);
	m_companyact->begin();
	int error = m_companyact->ejecuta(SQLQuery);
	SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
	error = m_companyact->ejecuta(SQLQuery);
	if (error) {
		m_companyact->rollback();
		return -1;
	}// end if
	/// Cogemos de la configuracion las cuentas que queremos que se apunten.
	/// Montamos los querys en base a la cadena cuentas.
	QString cuentas="";
	SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasIngresos'";
	cursor2 *cur=m_companyact->cargacursor(SQLQuery);
	if (!cur->eof()) {
		cuentas += cur->valor("valor");
	}// end if
	delete cur;
	SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasGastos'";
	cur=m_companyact->cargacursor(SQLQuery);
	if (!cur->eof()) {
		cuentas += ";"+cur->valor("valor");
	}// end if
	delete cur;
	cuentas.replace(';',"%|^");
	cuentas = "'^"+cuentas+"%'";
	
	/// Vamos a tener en cuenta también las cuentas de iva
	QString cuentasIVA="";
	SQLQuery = "SELECT valor FROM configuracion WHERE nombre='RegistroEmitida'";
	cur=m_companyact->cargacursor(SQLQuery);
	if (!cur->eof()) {
		cuentasIVA += cur->valor("valor");
	}// end if
	delete cur;
	SQLQuery = "SELECT valor FROM configuracion WHERE nombre='RegistroSoportada'";
	cur=m_companyact->cargacursor(SQLQuery);
	if (!cur->eof()) {
		cuentasIVA += ";"+cur->valor("valor");
	}// end if
	delete cur;
	cuentasIVA.replace(';',"%|^");
	cuentasIVA = "'^"+cuentasIVA+"%'";
	
	// Atentos que aquí es donde se incorpora el parametro.
	SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO "+cuentas+" OR codigo SIMILAR TO "+cuentasIVA;
	cur=m_companyact->cargacursor(SQLQuery);
	while (! cur->eof() ) {
		fprintf(stderr,"idborrador: %s contrapartida: %s cuenta: %s\n",cur->valor("idborrador").ascii(), cur->valor("contrapartida").ascii(), cur->valor("codigo").ascii());
		registro = atoi(cur->valor("idborrador").ascii());
		cur->siguienteregistro();
	}//end while
	delete cur;
	/// se calcula el total
	/// El cálculo se compara con el formato conocido de IVA (16, 7, 4: por tanto, formato de 2 dígitos con 0 decimales)
	SQLQuery = "SELECT abs(sum(baseimp)) AS subtotal FROM lacosa, (SELECT baseimp AS iva FROM lacosa WHERE codigo SIMILAR TO "+cuentasIVA+") AS iva WHERE codigo SIMILAR TO "+cuentas+" AND (iva.iva*100/baseimp)::NUMERIC(2,0) IN (SELECT porcentajetipoiva FROM tipoiva)";
	cur=m_companyact->cargacursor(SQLQuery);
	if (! cur->eof() ) {
		setbaseimp( cur->valor("subtotal"));
	}// end while
	delete cur;
	SQLQuery = "DROP TABLE lacosa";
	error = m_companyact->ejecuta(SQLQuery);
	m_companyact->commit();
    } catch(...) {
	_depura("RegistroIva:: Error en transaccion", 2);
	m_companyact->rollback();
    } // end try
    _depura("END RegistroIva::buscaborradorservicio", 0);
    return registro;
}


/** \brief busca la cuenta del cliente o del proveedor en el apunte que se ha seleccionado
  *
  * Se basa en el supuesto de que si la cuenta no es el cliente entonces la contrapartida de la cuenta
  * Seguro que es el cliente.
  */
int RegistroIva::buscaborradorcliente(int idborrador) {
    _depura("RegistroIva::buscaborradorcliente", 0);
    QString SQLQuery;
    int registro=0;
    try {
        m_companyact->begin();
        SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.cifent_cuenta, cuenta.idcuenta AS idcuenta, codigo, borrador.debe AS debe, borrador.haber AS haber, borrador.debe+borrador.haber AS totalfactura FROM borrador LEFT JOIN cuenta ON borrador.idcuenta=cuenta.idcuenta where borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador=%d)", idborrador);
        int error = m_companyact->ejecuta(SQLQuery);
        SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
        error = m_companyact->ejecuta(SQLQuery);
        /// Cogemos de la configuracion las cuentas que queremos que se apunten.
        /// Montamos los querys en base a la cadena cuentas.
        /// Se consideran cuentas de Derechos y de Obligaciones a Clientes y Proveedores, respectivamente.
        /// Los campos sirven para encontrar la cuenta que corresponde a quien hace el pago de la factura.
        QString cuentas="";
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasDerechos'";
        cursor2 *cur1=m_companyact->cargacursor(SQLQuery);
        if (!cur1->eof()) {
            cuentas += cur1->valor("valor");
        }// end if
        delete cur1;
        SQLQuery = "SELECT valor FROM configuracion WHERE nombre='CuentasObligaciones'";
        cur1=m_companyact->cargacursor(SQLQuery);
        if (!cur1->eof()) {
            cuentas += ";"+cur1->valor("valor");
        }// end if
        delete cur1;
        cuentas.replace(';',"%|^");
        cuentas = "'^"+cuentas+"%'";

        /// Atentos que aqui es donde se incorpora el parametro.
        SQLQuery = "SELECT * FROM lacosa WHERE codigo SIMILAR TO "+cuentas;
        cursor2 * cur=m_companyact->cargacursor(SQLQuery, "buscaapunte");
        while (! cur->eof() ) {
            /// Ponemos la cuenta de Cliente y los valores adyacentes
            setcontrapartida(cur->valor("idcuenta"));
            setcif(cur->valor("cifent_cuenta"));
            /// Comprobamos si es un cliente o un proveedor y segun sea actuamos en consecuencia.
            if (cur->valor("codigo").left(2) == "43") {
                setfactemitida("t");
            } else {
                setfactemitida( "f");
            }// end if
            registro = cur->valor("idborrador").toInt();
            cur->siguienteregistro();
        }// end while
        delete cur;
        SQLQuery = "DROP TABLE lacosa";
        error = m_companyact->ejecuta(SQLQuery);
        m_companyact->commit();
    } catch (...) {
        _depura("RegistroIva:: Error en buscaborradorcliente", 2);
        m_companyact->rollback();
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
    _depura("RegistroIva::inicializa1",0);
    QString query, cadena;
    int idborrador;

    /// Busca si a este apunte le corresponde un borrador
    idborrador = buscaborradorcliente(idapunte1);
    /// En el caso de que no existiese una cuenta de servicio a la que asociar se
    /// Asocia directamente al apunte seleccionado.
    if (idborrador <= 0)
        idborrador = idapunte1;
    ///Busca entradas de IVA en la tabla registroiva
    query.sprintf ( "SELECT * FROM registroiva WHERE idborrador=%d",idborrador);
    cursor2 *cursoriva = m_companyact->cargacursor(query);
    if (!cursoriva->eof()) {
        // El registro ya existe
        cargar( cursoriva->valor("idregistroiva"));
    } else {
        // El registro no existe y hay que hacer la propuesta más acertada de registro.
        ///buscamos en todo el asiento las cuentas de IVA y lo reflejamos
        buscaborradoriva(idapunte1);
        /// Buscamos la fecha que le corresponderá teoricamente a la factura.
        buscafecha(idapunte1);
        /// Buscamos la cuenta de servicio.
        buscaborradorservicio(idapunte1);
        buscaNumFactura(idapunte1);
        setidborrador(QString::number(idapunte1));
    }// end if
    delete cursoriva;

    pintaRegistroIva();
    // Hacemos la carga de los cobros.
    //    cargacobros();
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
    try {
        m_companyact->begin();
        QString SQLQuery;
        SQLQuery.sprintf("CREATE TEMPORARY TABLE lacosa AS SELECT borrador.debe AS ivadebe, borrador.haber AS ivahaber, idborrador, bcontrapartidaborr(idborrador) AS contrapartida , cuenta.idcuenta AS idcuenta, codigo, borrador.fecha AS fecha  FROM borrador, cuenta WHERE borrador.idcuenta=cuenta.idcuenta AND borrador.idasiento IN (SELECT idasiento FROM borrador WHERE idborrador=%d)", idborrador);
        int error = m_companyact->ejecuta(SQLQuery);

        SQLQuery.sprintf("DELETE FROM lacosa WHERE idborrador NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d) AND contrapartida NOT IN (SELECT idborrador FROM lacosa WHERE idborrador = %d UNION SELECT contrapartida AS idborrador FROM lacosa WHERE idborrador = %d)", idborrador, idborrador, idborrador, idborrador);
        error = m_companyact->ejecuta(SQLQuery);


        /// Cargamos los registros que quedan pq seguro que son de IVA.
        SQLQuery = "SELECT * FROM tipoiva LEFT JOIN lacosa ON tipoiva.idcuenta=lacosa.idcuenta LEFT JOIN (SELECT idcuenta, contrapartida, IVAdebe AS debe, IVAhaber AS haber FROM lacosa) AS base ON (base.debe*porcentajetipoiva/100)::NUMERIC(12,1)=lacosa.ivadebe::NUMERIC(12,1) AND (base.haber*porcentajetipoiva/100)::NUMERIC(12,2)=lacosa.ivahaber::NUMERIC(12,1) ORDER BY codigo";


        cursor2 * cur=m_companyact->cargacursor(SQLQuery);
        while (! cur->eof() ) {
            Iva *iva = new Iva(m_companyact);
            iva->setidtipoiva(cur->valor("idtipoiva"));
            if (cur->valor("debe") != "0.00") {
                iva->setivaiva(cur->valor("ivadebe"));
                iva->setbaseiva(cur->valor("debe"));
            } else {
                iva->setivaiva(cur->valor("ivahaber"));
                iva->setbaseiva(cur->valor("haber"));
            }// end if
            m_listalineas->append(iva);
            cur->siguienteregistro();
        }// end while
        delete cur;
        _depura("limpiamos la base de datos");
        SQLQuery = "DROP TABLE lacosa";
        error = m_companyact->ejecuta(SQLQuery);
        m_companyact->commit();
    } catch(...) {
        _depura("Error al buscar el borrador", 2);
        m_companyact->rollback();
    } // end try
    _depura("END RegistroIva::buscaborradoriva", 0);
    return 0;
}


/** \brief SLOT que se dispara cuando se ha hecho el bot� de bsqueda de una fecha.
  */
void RegistroIva::buscafecha(int idborrador) {
    _depura("RegistroIva::buscafecha", 0);
    QString SQLQuery;
    cursor2 * cur = m_companyact->cargacursor("SELECT fecha from borrador WHERE idborrador = "+QString::number(idborrador));
    if (!cur->eof()) {
        setffactura(cur->valor("fecha").left(10));
    }// end if
    delete cur;
    _depura("END RegistroIva::buscafecha", 0);
}



//Proponemos un número de factura si se trata de una venta y proponemos un número de orden si se trata de una compra
//Realmente esto se tendria que implementar con contadores en la base de datos.
//Primero comprobamos si esta factura ya tiene un apunte de iva distinto y cojemos el mismo numero de factura

void RegistroIva::buscaNumFactura(int idborrador) {

    QString query;
    cursor2 * recordset;
    int numfact;
    QString cadena;
    int numord;
    query.sprintf("SELECT factura, numorden FROM registroiva WHERE idborrador IN (SELECT idborrador FROM borrador WHERE idasiento=(SELECT idasiento FROM borrador WHERE idborrador='%i'))",idborrador);

    recordset = m_companyact->cargacursor(query);

    if (!recordset->eof()) {
        if (factemitida() == "t") {  //Se trata de una Venta
            setfactura( recordset->valor("factura"));
        } else { //Se trata de una compra
            setfactura(recordset->valor("factura"));
            setnumorden( recordset->valor("numorden"));
        }// end if
    }  else {   //La factura no existe, entonces proponemos el siguiente número de factura
        if (factemitida() == "t") {  //Se trata de una Venta
            query.sprintf("SELECT MAX(to_number(factura,'99999')) AS factura FROM registroiva WHERE numorden=''");

            recordset = m_companyact->cargacursor(query,"recordset");

            if (!recordset->eof()) {
                numfact = 1 + atoi(recordset->valor("factura").ascii());
            } else {
                numfact= 1;
            }// end if
            cadena.sprintf("%i",numfact);
            setfactura(cadena);
        } else { //Se trata de una compra
            /// Vemos si podemos extraer de la descripción del apunte el número de fra.
            query.sprintf("SELECT * FROM borrador WHERE idasiento IN (SELECT idasiento FROM borrador WHERE idborrador='%i') AND debe=0", idborrador);

            recordset = m_companyact->cargacursor(query,"recordset");

            QString num_fra = recordset->valor("conceptocontable");
            QRegExp patron("^S/Fra\\. [0-9]+$");
            if(num_fra.contains(patron) == 1) { // Si se cumple el patrón
                num_fra = num_fra.remove("S/Fra. ");
            } else { // Si no saco el nº de fra. lo pongo a cero, por poner algo
                num_fra.sprintf("0");
            }
            setfactura("num_fra");
            /// Buscamos qué número de orden poner
            query.sprintf("SELECT MAX(to_number(numorden,'99999')) AS numorden FROM registroiva WHERE numorden<>''");

            recordset = m_companyact->cargacursor(query,"recordset");

            if (!recordset->eof()) {
                numord = 1 + atoi(recordset->valor("numorden").ascii());
            } else {
                numord = 1;
            }// end if
            setnumorden(QString::number(numord));
        }// end if
    }// end if
    delete recordset;//Fin proposicion numeros factura y orden.
}

