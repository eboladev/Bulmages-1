/***************************************************************************
                          postgresiface2.cpp  -  description
                             -------------------
    begin                : Tue Nov 26 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
    email                : tborras@conetxia.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
/** \file postgresifac2.cpp
  * Contiene la implementaci� de las clases \ref cursor2 y \ref postgresiface2 que proveen acceso a las bases de datos
  * de postgres de forma sencilla y eficiente.
  */

#include "postgresiface2.h"
#include <qmessagebox.h>
#include <qapplication.h>

#include "msgerror.h"
#include "funcaux.h"


/** Constructor de la clase
  * Realiza la consulta en la base de datos y almacena el resultado en las variables de clase para poder ser manupuladas.
  * Tambi� almacena en variables globales algunos resultados para poder acelerar las consultas (nregistros y ncampos).
  * Si todo falla (y en funci� de la configuraci�) Da un mensaje de alerta o no.
  * \param nombre Nombre que obtendr�el query (OBSOLETO)
  * \param conn1 Conexi� con la base de datos (Inicializada en \ref postgresiface2
  * \param SQLQuery Query en formato SQL a realizar en la base de datos.
  */
cursor2::cursor2(QString nombre,PGconn *conn1, QString SQLQuery) {
    _depura("cursor2::cursor2",0);
    conn = conn1;
    m_error = FALSE;
    m_query = SQLQuery;
    nomcursor = nombre;
    nregistros=0;
    registroactual=0;
    _depura(SQLQuery,0);
    result = PQexec(conn, SQLQuery.toAscii().data());
    if (!result ) {
        m_error = TRUE;
        _depura( PQerrorMessage(conn));
        _depura( "QUERY command failed ["+SQLQuery+"]");
        if (confpr->valor(CONF_ALERTAS_DB) == "Yes")
            msgError(QString("Ha ocurrido un error al hacer una consulta con la base de datos."),SQLQuery+QString("\n")+(QString)PQerrorMessage(conn));
        PQclear(result);
        return;
    }// end if
    nregistros = PQntuples(result);
    ncampos = PQnfields(result);
    registroactual=0;
    /// Depuramos todo
    _depura("--------- RESULTADO DE QUERY ----------------");
    QString err;
    err.sprintf("Num. Registros: %d, Num. Campos: %d",nregistros, ncampos);
    _depura(err);
    _depura("--------- FIN RESULTADO DE QUERY ----------------");
    _depura("END cursor2::cursor2",0);
}// end cursor2


/** Destructor de clase, antes de destruirse limpia los posibles buffers intermedios
  */
cursor2::~cursor2() {
    cerrar();
}


/** Limpia los buffers intermedios que puedan estar chupando memoria
  */
void cursor2::cerrar() {
    PQclear(result);
}// end cerrar


/** \return Devuelve el nmero de registros de la consulta.
  */
int cursor2::numregistros() {
    return (nregistros);
}// end numregistros


/** \return Devuelve el nmero de campos de la consulta
  */
int cursor2::numcampos() {
    return(ncampos);
}

/** \return Devuelve el nombre del campo de una posici� determinada.
  * \param campo Posici� de la que se quiere que devuelva el nombre.
  */
QString cursor2::nomcampo(int campo) {
    return((QString) PQfname(result, campo));
}// end if


/**
  * Dado un campo esta funci� devuelve devuelve la posici� que le
  * corresponde, y si no le corresponde ninguno devuelve -1
  * \param campo Nombre del campo.
  * \return Devuelve la posici� si el nombre del campo existe, y si no devuelve -1
  * \todo Tal vez deber� crearse una estructura intermedia que indexe los nombres con las posiciones para hacer la bsqueda m� r�ida, pero al ser el nmero de registros siempre muy reducido seguramente no arreglariamos nada de nada.
  */
int cursor2::numcampo(QString campo) {
    int i=0;
    while (i<numcampos() && campo != nomcampo(i) ) {
        i++;
    }// end while
    if (i== numcampos()) {
        _depura("Campo: "+campo+" no existe en la consulta: "+m_query, 2);
        return(-1);
    }
    return(i);
}// end numcampo


/** Esta funcion devuelve el valor del campo posicion del registro
  * pasado, si se pasa -1 como registro se devuelve el registro actual
  * \param posicion El nmero de campo del que se quiere la posici�.
  * \param registro El registro del que se quiere devolver el campo. Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
  * \return El valor de la posicion.
  */
QString cursor2::valor(int posicion, int registro) {
    if (registro == -1) {
        registro = registroactual;
    }// end if
    //    return (QString::fromLatin1(PQgetvalue(result, registro, posicion)));
    return (QString::fromUtf8(PQgetvalue(result, registro, posicion)));

}// end valor


/** Esta funcion devuelve el valor del campo especificado (por nombre) del registro
  * pasado, si se pasa como registro -1 se devuelve el registro actual.
  * \param campo Nombre del campo a devolver
  * \param registro El registro del que se quiere devolver el campo. Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
  * \return El valor de la posicion.
  */
QString cursor2::valor(QString campo, int registro) {
    int i=0;
    if (registro == -1) {
        registro = registroactual;
    }// end if

    i = numcampo(campo);
    if ( i == -1)
        return "";

    return(QString::fromUtf8(PQgetvalue(result, registro, i)));
}// end valor


/** \return Devuelve la posicion siguiente al registro que se est�recorriendo
  */
int cursor2::siguienteregistro() {
    return (++registroactual);
}

/** \return Devuelve la posicion anterior al registro que se est�recorriendo
  */
int cursor2::registroanterior() {
    return (--registroactual);
}

/** \return Devuelve la posicion del primer registro de la tabla de registros
  */
int cursor2::primerregistro() {
    registroactual=0;
    return (0);
}// end primerregistro


/** \return Devuelve el ultimo registro de la tabla de registros
  */
int cursor2::ultimoregistro() {
    registroactual = nregistros-1;
    return (registroactual);
}// end ultimoregistro


/** \return Devuelve TRUE si el registro est�en la posicion final, o si esta vacio
  */
bool cursor2::eof() {
    if (nregistros == 0) {
        return(true);
    }// end if
    return(registroactual >= nregistros);
}// end if


/** \return Devuelve TRUE si el registro esta en la posicion inicial, o si esta vacio
  */
bool cursor2::bof() {
    if (nregistros == 0) {
        return(true);
    }// end if
    return(registroactual < 0);
}// end if

/** \return devuelve TRUE si es el ltimo registro a considerar */
bool cursor2::esultimoregistro() {
    return (registroactual == nregistros-1);
}// end ultimoregistro

/** \return devuelve TRUE si es el primer registro a consear e el query. */
bool cursor2::esprimerregistro() {
    return (registroactual == 0);
}// end ultimoregistro


// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------

/** Constructor de la clase, no hace nada de nada de nada */
postgresiface2::postgresiface2() {}// end postgresiface2

/** Finaliza la conexi� con la base de datos */
void postgresiface2::terminar() {
    PQfinish(conn);
}// end terminar

/** Destructor de la clase que al igual que \ref terminar termina la conexi� con la base de datos */
postgresiface2::~postgresiface2() {
    /** close the connection to the database and cleanup */
    PQfinish(conn);
}// end -postgresiface2

/** Inicializa la conexi� con la base de datos mediante los par�etro especificados
  * Precisamente no lo hace el constructor debido a la ausencia de dichos datos.
  * \param nomdb Indica el nombre de la base de datos
  * \param user Indica el usuario que hace la operaci� a ojos de la base de datos.
  * \param passwd Indica la contrase� que utiliza el usuario para autentificarse
  * \return Si todo va bien devuelve 0, en caso contrario devuelve 1
  */
int postgresiface2::inicializa(QString nomdb) {
    dbName=nomdb;
    pghost = confpr->valor(CONF_SERVIDOR);             /** host name of the backend server */
    pgport = confpr->valor(CONF_PUERTO);              /** port of the backend server */
    pgoptions = "";           /** special options to start up the backend server */
    pgtty = "";               /** debugging tty for the backend server */
    QString conexion;

    QString user = confpr->valor(CONF_LOGIN_USER);
    QString passwd = confpr->valor(CONF_PASSWORD_USER);



    /// Antes no resolvia bien en caso de querer hacer conexiones al ordenador local.
    /// Ahora si se pone -- se considera conexion� local.
    if (pghost != "--")
        conexion = "host="+pghost;
    conexion += " port="+pgport;
    conexion += " dbname="+dbName;
    if (user != "")
        conexion += " user="+user;
    if (passwd != "")
        conexion += " password="+passwd;

    _depura(conexion,0);
    conn = PQconnectdb(conexion.toAscii().data());
    if (PQstatus(conn) == CONNECTION_BAD)  {
        _depura("Connection to database '"+dbName+"' failed.\n",0);
        if (passwd != "" && confpr->valor(CONF_ALERTAS_DB)== "Yes")
            _depura(PQerrorMessage(conn),2);
        else
            _depura(PQerrorMessage(conn),0);
        return(1);
    }// end if
    _depura("La conexion con la base de datos ha ido bien, ahora vamos a por la fecha",0);
    formatofecha();
    return(0);
}// end inicializa


/** Cambia el formato de fecha de la base de datos para que usemos la
  * fecha espa�la dd/mm/yyyy
  * \return Devuelve 0 si no ha habido problemas, en caso contrario devuelve 1
  */
int postgresiface2::formatofecha() {
    QString query="";
    PGresult   *res;
    //    begin();
    query= "SET DATESTYLE TO SQL,European";
    res = PQexec(conn, query.toAscii().data());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        _depura( "Cambio del formato de fecha command failed");
    }// end if
    PQclear(res);

    /// Establecemos la codificación por defecto a UNICODE.
    /// Pero con los problemas que está teniendo el UNICODE lo vamos a dejar en SQL_ASCII QUE funciona bastante mejor.

    query = "SET client_encoding = 'UTF8'";
    res = PQexec(conn, query.toAscii().data());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        _depura( "Cambio del formato de Codificación");
    }// end if
    PQclear(res);

    //    commit();
    return(0);
}// end formatofecha


/** Con esta sentencia se inicia una transacci� en la base de datos
  * Las transacciones lo que indican es que el usuario se ha apoderado de la base de datos durante
  * un tiempo y que la operaci� que va a transcurrir debe hacerse sin concurrencia.
  * \return Si todo ha funcionado bien devuelve un 0, en caso contrario devuelve un 1
  */
int postgresiface2::begin() {
    PGresult   *res;
    res = PQexec(conn, "BEGIN");
    _depura("-- BEGIN TRANSACTION --");
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)  {
        _depura( "BEGIN command failed");
        PQclear(res);
        return (1);
    }// end if
    PQclear(res);
    return(0);
}// end begin


/** Con esta sentencia se termina un bloque de transacci� dando por buenos todos los resultados que
  * Se han almacenado dentro de �te. Y quedando almacenados en la base de datos de forma definitiva.
  */
void postgresiface2::commit() {
    PGresult   *res;
    _depura("-- COMMIT TRANSACTION --");
    res = PQexec(conn, "COMMIT");
    PQclear(res);
}// end commit


/** Con esta sentencia se termina un bloque de transacci� dando por malos los resultados de la operaci�
  * Y dejandose la base de datos en el mismo estado que cuando se inici�la transacci�.
  */
void postgresiface2::rollback() {
    PGresult   *res;
    res = PQexec(conn, "ROLLBACK");
    PQclear(res);
}// end rollback


/** Se encarga de generar un objeto del tipo cursor2 y de iniciarlo con un query concreto
  * NOTA: Este m�odo crea mem�ia, con lo que �ta debe ser liberada posteriormente.
  * \return Devuelve un apuntador al objeto \ref cursor2 generado e inicializado con la respuesta al query.
  */
cursor2 *postgresiface2::cargacursor(QString Query, QString nomcursor) {
    _depura ("postgresiface2::cargacursor",0);
    cursor2 *cur=new cursor2(nomcursor,conn,Query);
    _depura ("END postgresiface2::cargacursor",0);
    return(cur);
}// end cargacursor


/** \brief Ejecuta un comando SQL
\param Query Comando a ejecutar. Debe ser un comando que no devuelva ningn valor (como \c select).
\retval 0 Si la ejecucion fue correcta
\retval 1 en caso contrario
*/
#include <qtextcodec.h>
int postgresiface2::ejecuta(QString Query) {
    _depura("postgresiface2::ejecuta",0);
    PGresult *result=NULL;
    try {
        //Prova de control de permisos
        if (confpr->valor(CONF_PRIVILEGIOS_USUARIO) != "1" && (Query.left(6)=="DELETE" || Query.left(6)=="UPDATE" || Query.left(6)=="INSERT"))
            throw (42501);
        //Fi prova. Nota: 42501 = INSUFFICIENT PRIVILEGE en SQL Standard
        result = PQexec(conn,  (const char *) Query.utf8());
        if (!result || PQresultStatus(result) != PGRES_COMMAND_OK)
            throw -1;
        PQclear(result);
        _depura("postgresiface2::ejecuta",0);
        return 0;
    }// end try

    catch (...) {
        _depura("SQL command failed: "+Query);
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        QString mensaje = "Error al intentar modificar la base de datos:\n";
        msgError(mensaje+(QString)PQerrorMessage(conn),Query+"\n"+(QString)PQerrorMessage(conn));
        PQclear(result);
        throw -1;
    }// end catch
}




/**
  * This function search in the database the account parent of the account selected
  * if there are not parent returns NULL
  * else returns the code of the parent account
  */
QString postgresiface2::searchParent(QString cod) {
    QString padre="NULL"; //almacena el padre de la cuenta.
    QString query;
    int i = 2;
    int fin=0;
    while (!fin) {
        query = "SELECT * FROM cuenta WHERE codigo = '"+cod.left(i)+"'";
        begin();
        cursor2 *cur = cargacursor(query,"unquery");
        commit();
        if (!cur->eof()) {
            padre = cur->valor("codigo");
        } else {
            fin=1;
        }// end if
        delete cur;
        i++;
    }// end while
    return padre;
}// end searchParent


int postgresiface2::nuevoborrador(int idcuenta, int idasiento, QString concepto, QString descripcion, float debe, float haber, QString fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal) {
    QString query="";
    QString textcuenta;
    QString textcontrapartida;
    QString textidccoste;
    QString textidcanal;
    if (idcuenta == 0) {
        textcuenta="NULL";
    } else {
        textcuenta.sprintf("%d",idcuenta);
    }// endif
    if (idcontrapartida == 0) {
        textcontrapartida="NULL";
    } else {
        textcontrapartida.sprintf("%d",idcontrapartida);
    }// endif
    if (idccoste == 0) {
        textidccoste="NULL";
    } else {
        textidccoste.sprintf("%d",idccoste);
    }// end if
    if (idcanal == 0) {
        textidcanal="NULL";
    } else {
        textidcanal.sprintf("%d",idcanal);
    }// end if
    query.sprintf("INSERT INTO borrador (idcuenta,idasiento,conceptocontable, descripcion, debe, haber, fecha, contrapartida, idtipoiva, idc_coste, idcanal) VALUES (%s, %d,'%s','%s', %2.2f, %2.2f,'%s', %s, %d, %s, %s)",
                  sanearCadena(textcuenta).toAscii().data(),
                  idasiento,
                  sanearCadena(concepto).toAscii().data(),
                  sanearCadena(descripcion).toAscii().data(),
                  debe,
                  haber,
                  sanearCadena(fecha).toAscii().data(),
                  sanearCadena(textcontrapartida).toAscii().data(),
                  idtipoiva,
                  sanearCadena(textidccoste).toAscii().data(),
                  sanearCadena(textidcanal).toAscii().data());
    return(ejecuta(query));
}// end nuevoborrador


/***********************************************************************
 * Esta funcion se encarga de modificar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface2::modificaborrador(int idborrador, int idcuenta, float idebe, float ihaber, QString concepto, QString fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal) {
    QString query="";
    QString textidccoste;
    QString textcontrapartida;
    QString textocanal;
    if (idccoste == 0) {
        textidccoste="NULL";
    } else {
        textidccoste.sprintf("%d",idccoste);
    }// end if
    if (contrapartida == 0) {
        textcontrapartida="NULL";
    } else {
        textcontrapartida.sprintf("%d",contrapartida);
    }// end if
    if (idcanal == 0) {
        textocanal="NULL";
    } else {
        textocanal.sprintf("%d",idcanal);
    }// end if

    query.sprintf("UPDATE borrador SET idcuenta=%d, debe=%2.2f, haber=%2.2f, conceptocontable='%s', fecha='%s', contrapartida=%s, idtipoiva=%d, idc_coste=%s, idcanal=%s WHERE idborrador=%d",idcuenta,idebe,ihaber,concepto.toAscii().data(),fecha.toAscii().data(),textcontrapartida.toAscii().data(),idtipoiva,textidccoste.toAscii().data(),textocanal.toAscii().data(), idborrador);
    _depura(query);
    return(ejecuta(query));
}// end modificaborrador


/* **********************************************************
 * Esta funcion carga de la base de datos la cuenta especificada   *
 * si idcuenta != 0 se intenta cargar la cuenta con idcuenta especificada
 * si idcuenta == 0 se intenta cargar la cuenta con codigo especificado  *
 ************************************************************/
cursor2 *postgresiface2::cargacuenta(int idcuenta, QString ccuenta) {
    QString query="";
    if ( idcuenta != 0) {
        query.sprintf("SELECT * FROM cuenta WHERE idcuenta=%d",idcuenta);
    }  else  {
        query.sprintf("SELECT * FROM cuenta WHERE codigo LIKE '%s' ORDER BY codigo",ccuenta.toAscii().data());
    }// end if
    cursor2 *cur=cargacursor(query,"cargacuenta");
    return(cur);
}// end cargacuenta


/* *****************************************************
 * Esta funcion carga de la base de datos el asiento con idasiento especificado
 *******************************************************/
cursor2 *postgresiface2::cargaasiento(int idasiento) {
    QString query="";
    query.sprintf("SELECT * FROM asiento WHERE idasiento=%d",idasiento);
    cursor2 *cur=cargacursor(query,"cargaasiento");
    return(cur);
}// end cargaasiento


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * tidasiento --> El asiento del que se deben cargar los apuntes
 *****************************************************************/
cursor2 *postgresiface2::cargaapuntes(int tidasiento) {
    QString query="";
    query.sprintf("SELECT * FROM apunte where idasiento=%d ORDER BY idapunte",tidasiento);
    cursor2 *cur=cargacursor(query,"cargaapuntes");
    return(cur);
}// end cargaapuntes


/**
 * Esta funcion carga de la base de datos los apuntes
 * tidasiento --> El asiento del que se deben cargar los apuntes
 */
cursor2 *postgresiface2::cargaborradores(int tidasiento) {
    QString query="";
    query.sprintf("SELECT * FROM borrador where idasiento=%d ORDER BY idborrador",tidasiento);
    cursor2 *cur=cargacursor(query,"cargaborradores");
    return(cur);
}// end cargaborradores


/**
 * Esta funcion carga de la base de datos las cuentas
 * padre : = 0 carga las cuentas de nivel 0
 * padre : = -1 carga todas las cuentas sin excepcion
 * padre : else carga las cuentas cuyo padre es el valor especificado.
 */
cursor2 *postgresiface2::cargacuentas(int padre) {
    QString query="";
    if (padre!=0 && padre != -1 && padre!=-2) {
        query.sprintf("SELECT * FROM cuenta WHERE padre=%d ORDER BY padre",padre);
    } else if (padre== 0) {
        query.sprintf("SELECT * FROM cuenta WHERE padre isnull ORDER BY padre ");
    } else if (padre ==-1) {
        query.sprintf("SELECT * FROM cuenta ORDER BY padre ");
    } else if (padre ==-2) {
        query.sprintf("SELECT * FROM cuenta WHERE NOT padre isnull ORDER BY padre ");
    }// end if
    cursor2 *cur=cargacursor(query,"cargaborradores");
    return(cur);
}



/**
 * Esta funcion carga de la base de datos los grupos
 */
cursor2 *postgresiface2::cargagrupos() {
    QString query="SELECT * FROM grupo";
    cursor2 *cur=cargacursor(query,"cargagrupos");
    return(cur);
}// end cargagrupos


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * entre las fechas indicadas. Ambas fechas inclusive.
 *****************************************************************/
cursor2 *postgresiface2::cargaapuntesctafecha(int tidcuenta, QString fechainicial, QString fechafinal) {
    QString query="";
    query.sprintf("SELECT * FROM apunte where idcuenta=%d AND fecha>='%s' AND fecha<='%s' ORDER BY fecha",tidcuenta, fechainicial.toAscii().data(), fechafinal.toAscii().data());
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargaapuntesctafecha


/* **********************************************************
 * Esta funcion carga los saldos de una cuenta dada antes de la fecha
 * limite especificada. Los apuntes de la fecha no estan incluidos. *
 ************************************************************/
cursor2 *postgresiface2::cargasaldoscuentafecha(int idcuenta, QString fecha) {
    QString query="";
    query.sprintf("SELECT sum(debe) as tdebe, sum(haber)as thaber FROM apunte WHERE idcuenta=%d AND fecha <'%s'",idcuenta, fecha.toAscii().data());
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargasaldoscuentafecha


/* *****************************************************
 * Esta funcion carga de la base de datos los asientos
 *******************************************************/
cursor2 *postgresiface2::cargaasientosfecha(QString fechini, QString fechfin) {
    QString query="";
    query.sprintf("SELECT * FROM asiento WHERE fecha >= '%s' AND fecha <= '%s' ORDER BY fecha", fechini.toAscii().data(),fechfin.toAscii().data());
    cursor2 *cur=cargacursor(query,"cargaasientosfecha");
    return(cur);
}// end cargaasientosfecha

/* *****************************************************
 * Esta funcion carga de la base de datos las cuentas entre los codigos especificados
 * padre : = 0 carga las cuentas de nivel 0
 * padre : = -1 carga todas las cuentas sin excepcion
 * padre : else carga las cuentas cuyo padre es el valor especificado.
 * codigoinicial: el codigo inicial a partir del cual cargar cuentas
 * codigofinal: el codigo hasta donde saldran cuentas.
 *
 */
cursor2 *postgresiface2::cargacuentascodigo(int padre, QString codigoinicial, QString codigofinal) {
    QString query="";
    if (padre!=0 && padre != -1) {
        query.sprintf("SELECT * FROM cuenta WHERE padre=%d AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",padre, codigoinicial.toAscii().data(), codigofinal.toAscii().data());
    } else if (padre== 0) {
        query.sprintf("SELECT * FROM cuenta WHERE padre isnull AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial.toAscii().data(), codigofinal.toAscii().data());
    } else if (padre ==-1) {
        query.sprintf("SELECT * FROM cuenta WHERE codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial.toAscii().data(), codigofinal.toAscii().data());
    }// end if
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargacuentascodigo



int postgresiface2::cierraasiento(int idasiento) {
    QString query;
    query.sprintf("SELECT cierraasiento(%d)",idasiento);
    cursor2 *cur=cargacursor(query,"abreasientos");
    delete cur;
    return(1);
}// end cierraasiento


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 * Si el asiento no esta vacio entonces no se borra.
 ***********************************************************************/
int postgresiface2::borrarasiento(int idasiento) {
    QString query="";
    query.sprintf("DELETE FROM asiento WHERE idasiento=%d",idasiento);
    return(ejecuta(query));
}// end borrarasiento


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface2::borrarborrador(int idborrador) {
    QString query="";
    query.sprintf("DELETE FROM borrador WHERE idborrador=%d",idborrador);
    return(ejecuta(query));
}// end borrarborrador


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface2::borrarcuenta(int idcuenta) {
    QString query="";
    query.sprintf("DELETE FROM cuenta WHERE idcuenta=%d",idcuenta);
    return(ejecuta(query));
}// end borrarcuenta

/***********************************************************************
 * Esta funcion se encarga de abrir el asiento pasado como parametro
 * correspondiente.
 ***********************************************************************/
int postgresiface2::abreasiento(int idasiento) {
    _depura("Funcion abreasiento\n");
    QString query="";

    query.sprintf("SELECT abreasiento(%d)",idasiento);
    cursor2 *cur=cargacursor(query,"abreasientos");
    delete cur;
    return(1);
}// end abreasiento


int postgresiface2::modificacuenta(int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString nombreent, QString cifent, QString dir, QString cp, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
    QString cadena;
    cadena.sprintf("%d",idcuenta);
    QString query="";
    QString bloqueada = cbloqueada ? (char *) "TRUE" : (char *) "FALSE";
    QString activo = cactivo ?  "TRUE" :  "FALSE";
    QString imputacion = cimputacion ?  "TRUE" :  "FALSE";
    QString nodebe = cnodebe ? "TRUE" : "FALSE";
    QString nohaber = cnohaber ? "TRUE" : "FALSE";
    query.sprintf("UPDATE cuenta SET descripcion='%s', codigo='%s', imputacion=%s, bloqueada=%s, idgrupo=%d, activo=%s, nombreent_cuenta='%s', cifent_cuenta='%s', dirent_cuenta='%s', cpent_cuenta='%s', telent_cuenta='%s', coment_cuenta='%s', bancoent_cuenta='%s', emailent_cuenta='%s', webent_cuenta='%s', tipocuenta=%d, nodebe=%s, nohaber = %s WHERE idcuenta=%d\n",desccuenta.toAscii().data(),codigo.toAscii().data(),imputacion.toAscii().data(),bloqueada.toAscii().data(),idgrupo,activo.toAscii().data(),nombreent.toAscii().data(), cifent.toAscii().data(),dir.toAscii().data(),cp.toAscii().data(),tel.toAscii().data(),comm.toAscii().data(),banco.toAscii().data(), email.toAscii().data(), web.toAscii().data(),tipocuenta,nodebe.toAscii().data(), nohaber.toAscii().data(), idcuenta);
    _depura(query);
    return(ejecuta(query));
}// end modificacuenta


int postgresiface2::nuevacuenta(QString desccuenta, QString codigo, int padre, int idgrupo, QString nombreent, QString cifent, QString dir, QString cp, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
    QString query="";
    QString tpadre;
    if (padre == 0) {
        tpadre="NULL";
    }  else  {
        tpadre.sprintf("%d",padre);
    }// end if
    QString nodebe = cnodebe ? "TRUE" : "FALSE";
    QString nohaber = cnohaber ? "TRUE" : "FALSE";

    query.sprintf("INSERT INTO cuenta (descripcion, padre,codigo, idgrupo, nombreent_cuenta, cifent_cuenta, dirent_cuenta, cpent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta, tipocuenta, nodebe, nohaber) VALUES('%s',%s,'%s',%d, '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, %s, %s)",
                  sanearCadena(desccuenta).toAscii().data(),
                  sanearCadena(tpadre).toAscii().data(),
                  sanearCadena(codigo).toAscii().data(),
                  idgrupo,
                  sanearCadena(nombreent).toAscii().data(),
                  sanearCadena(cifent).toAscii().data(),
                  sanearCadena(dir).toAscii().data(),
                  sanearCadena(cp).toAscii().data(),
                  sanearCadena(tel).toAscii().data() ,
                  sanearCadena(comm).toAscii().data(),
                  sanearCadena(banco).toAscii().data(),
                  sanearCadena(email).toAscii().data(),
                  sanearCadena(web).toAscii().data(),
                  tipocuenta,
                  sanearCadena(nodebe).toAscii().data(),
                  sanearCadena(nohaber).toAscii().data() );
    return(ejecuta(query));
}// end nuevacuenta


int postgresiface2::nuevoasiento(QString nombre, QString fecha, int numasiento, int clase) {
    // clase => 0=Apertura, 1=Normal, 98=Regularizaci� 99=Cierre
    QString query="";
    int val;
    int ordenasiento;
    //0=Apertura, 1=Normal, 98=Regularizaci� 99=Cierre
    if (numasiento == 0) {
        query="SELECT max(idasiento) FROM asiento";
        cursor2 *cur=cargacursor(query,"cargaasientoseq");
        if (!cur->eof())
            val = atoi(cur->valor(0).toAscii().data());
        else
            val=0;
        val++;
        delete cur;
    } else {
        val = numasiento;
    }// end if
    query.sprintf("SELECT MAX(ordenasiento) FROM asiento WHERE EXTRACT(YEAR FROM fecha)='%s'",fecha.right(4).toAscii().data());
    cursor2 *cur=cargacursor(query,"cargaasientos");
    if (!cur->eof())
        ordenasiento = atoi(cur->valor(0).toAscii().data());
    else
        ordenasiento=0;
    ordenasiento++;
    delete cur;
    query.sprintf("INSERT INTO asiento (idasiento,descripcion, fecha, ordenasiento, clase) VALUES (%d,'%s','%s', %d, %d)",
                  val,
                  sanearCadena(nombre).toAscii().data(),
                  sanearCadena(fecha).toAscii().data(),
                  ordenasiento,
                  clase);
    int error = ejecuta(query);
    if (error)
        return -1;
    return(val);
}// end nuevoasiento


cursor2 *postgresiface2::cargaempresas() {
    QString query;
    query= "SELECT * FROM empresa";
    cursor2 *cur=cargacursor(query,"cargaempresas");
    return(cur);
}// end cargaempresas



/** Esta funci� est�ica devuelve una cadena "saneada" para pasarsela a Postgresql.
  * Neutraliza (escapes) los caracteres problem�icos por ser caracteres especiales
  * de Postgresql. Ejemplo, comillas, contrabarras,...
  */
QString postgresiface2::sanearCadena(QString cadena) {
    int longitud;
    char *buffer;
    QString cadenaLimpia;
    longitud = cadena.length();
    // Reservamos (la funcion de postgres lo necesita) un buffer del
    // doble de caracteres + 1 que la cadena original
    buffer = (char *)malloc(sizeof(char)*longitud*3);
    PQescapeString(buffer, cadena.utf8(), strlen(cadena.utf8()));
    cadenaLimpia = buffer;
    free(buffer);
    return cadenaLimpia;
}

/** Devuelve el valor de una propiedad de la empresa
  * Las propiedades de la empresa son valores asociados a �ta de la forma Nombre=Valor. 
  *De esta forma se guardan datos como el nombre fiscal de la empresa, CIF, domicilio, etc.
  * \param nombre Nombre de la propiedad
  * \return Valor de la propiedad
*/
QString postgresiface2::propiedadempresa(QString nombre) {
    PGresult *result;
    QString value;
    int num;
    QString Query="select * from configuracion where nombre='"+nombre+"'";
    fprintf(stderr,"%s\n",Query.toAscii().data());
    result = PQexec(conn, Query.toAscii().data());
    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SQL command failed: %s\n", Query.toAscii().data());
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        PQclear(result);
        return("");
    }// end if
    num=PQntuples(result);
    if (num>1)
        fprintf(stderr,"Aviso: Hay %d valores para el campo %s en la tabla configuracion\n",num,nombre.toAscii().data());
    if (num==0)
        value="";
    else
        value= PQgetvalue(result, 0, 2);
    PQclear(result);
    return(value);
}// end propiedadempresa


