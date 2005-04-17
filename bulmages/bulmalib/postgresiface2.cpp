/***************************************************************************
                          postgresiface2.cpp  -  description
                             -------------------
    begin                : Tue Nov 26 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
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
  * Contiene la implementación de las clases \ref cursor2 y \ref postgresiface2 que proveen acceso a las bases de datos
  * de postgres de forma sencilla y eficiente.
  */
  
#include "postgresiface2.h"
#include <qmessagebox.h>
#include <qapplication.h>


/** Constructor de la clase 
  * Realiza la consulta en la base de datos y almacena el resultado en las variables de clase para poder ser manupuladas.
  * También almacena en variables globales algunos resultados para poder acelerar las consultas (nregistros y ncampos).
  * Si todo falla (y en función de la configuración) Da un mensaje de alerta o no.
  * \param nombre Nombre que obtendrá el query (OBSOLETO)
  * \param conn1 Conexión con la base de datos (Inicializada en \ref postgresiface2
  * \param SQLQuery Query en formato SQL a realizar en la base de datos.
  */
cursor2::cursor2(QString nombre,PGconn *conn1, QString SQLQuery) {
    conn = conn1;
    nomcursor = nombre;
    nregistros=0;
    registroactual=0;
    QString Query = SQLQuery;
    result = PQexec(conn, Query.ascii());
    if (!result ) {
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        fprintf(stderr, "QUERY command failed [%s]\n", Query.ascii());
        if (confpr->valor(CONF_ALERTAS_DB) == "Yes")
            QMessageBox::warning(NULL, theApp->translate("postgresiface","Error...",""), theApp->translate("postgresiface","Ocurrió un error con la carga de un query de la base de datos\n"+Query+"\n"+PQerrorMessage(conn),""), theApp->translate("postgresiface","Aceptar",""));
        PQclear(result);
        return;
    }// end if
    nregistros = PQntuples(result);
    ncampos = PQnfields(result);
    registroactual=0;
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


/** \return Devuelve el número de registros de la consulta.
  */
int cursor2::numregistros() {
    return (nregistros);
}// end numregistros


/** \return Devuelve el número de campos de la consulta
  */
int cursor2::numcampos() {
    return(ncampos);
}

/** \return Devuelve el nombre del campo de una posición determinada.
  * \param campo Posición de la que se quiere que devuelva el nombre.
  */
QString cursor2::nomcampo(int campo) {
    return((QString) PQfname(result, campo));
}// end if


/**
  * Dado un campo esta función devuelve devuelve la posición que le
  * corresponde, y si no le corresponde ninguno devuelve -1
  * \param campo Nombre del campo.
  * \return Devuelve la posición si el nombre del campo existe, y si no devuelve -1
  * \todo Tal vez debería crearse una estructura intermedia que indexe los nombres con las posiciones para hacer la búsqueda más rápida, pero al ser el número de registros siempre muy reducido seguramente no arreglariamos nada de nada.
  */
int cursor2::numcampo(QString campo) {
    int i=0;
    while (i<numcampos() && campo != nomcampo(i) ) {
        i++;
    }// end while
    if (i== numcampos())
        return(-1);
    return(i);
}// end numcampo


/** Esta funcion devuelve el valor del campo posicion del registro
  * pasado, si se pasa -1 como registro se devuelve el registro actual
  * \param posicion El número de campo del que se quiere la posición.
  * \param registro El registro del que se quiere devolver el campo. Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
  * \return El valor de la posición.
  */
QString cursor2::valor(int posicion, int registro) {
    if (registro == -1) {
        registro = registroactual;
    }// end if
    return (PQgetvalue(result, registro, posicion));
}// end valor


/** Esta función devuelve el valor del campo especificado (por nombre) del registro
  * pasado, si se pasa como registro -1 se devuelve el registro actual.
  * \param campo Nombre del campo a devolver
  * \param registro El registro del que se quiere devolver el campo. Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
  * \return El valor de la posición.
  */
QString cursor2::valor(QString campo, int registro) {
    int i=0;
    if (registro == -1) {
        registro = registroactual;
    }// end if
    while (i<numcampos() && campo != nomcampo(i) ) {
        i++;
    }// end while
    return(PQgetvalue(result, registro, i));
}// end valor


/** \return Devuelve la posición siguiente al registro que se está recorriendo
  */
int cursor2::siguienteregistro() {
    return (++registroactual);
}

/** \return Devuelve la posición anterior al registro que se está recorriendo
  */
int cursor2::registroanterior() {
    return (--registroactual);
}

/** \return Devuelve la posición del primer registro de la tabla de registros
  */
int cursor2::primerregistro() {
    registroactual=0;
    return (0);
}// end primerregistro


/** \return Devuelve el último registro de la tabla de registros
  */
int cursor2::ultimoregistro() {
    registroactual = nregistros-1;
    return (registroactual);
}// end ultimoregistro


/** \return Devuelve TRUE si el registro está en la posición final, o si está vacio
  */
bool cursor2::eof() {
    if (nregistros == 0) {
        return(true);
    }// end if
    return(registroactual >= nregistros);
}// end if


/** \return Devuelve TRUE si el registro está en la posición inicial, o si está vacio
  */
bool cursor2::bof() {
    if (nregistros == 0) {
        return(true);
    }// end if
    return(registroactual < 0);
}// end if

/** \return devuelve TRUE si es el último registro a considerar */
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

/** Finaliza la conexión con la base de datos */
void postgresiface2::terminar() {
    PQfinish(conn);
}// end terminar

/** Destructor de la clase que al igual que \ref terminar termina la conexión con la base de datos */
postgresiface2::~postgresiface2() {
    /** close the connection to the database and cleanup */
    PQfinish(conn);
}// end -postgresiface2

/** Inicializa la conexión con la base de datos mediante los parámetro especificados
  * Precisamente no lo hace el constructor debido a la ausencia de dichos datos.
  * \param nomdb Indica el nombre de la base de datos
  * \param user Indica el usuario que hace la operación a ojos de la base de datos.
  * \param passwd Indica la contraseña que utiliza el usuario para autentificarse
  * \return Si todo va bien devuelve 0, en caso contrario devuelve 1
  */
int postgresiface2::inicializa(QString nomdb, QString user, QString passwd) {
    dbName=nomdb;
    pghost = confpr->valor(CONF_SERVIDOR);             /** host name of the backend server */
    pgport = confpr->valor(CONF_PUERTO);              /** port of the backend server */
    pgoptions = "";           /** special options to start up the backend server */
    pgtty = "";               /** debugging tty for the backend server */
    QString conexion;

    if (pghost != "localhost") {
        conexion = "hostaddr="+pghost+" port="+pgport;
    }// end if
    conexion += " dbname="+dbName;
    if (user != "") {
        conexion += " user="+user;
    }// end if
    if (passwd != "") {
        conexion += " password="+passwd;
    }// end if

    fprintf(stderr,"%s\n",conexion.ascii());
    conn = PQconnectdb(conexion);
    if (PQstatus(conn) == CONNECTION_BAD)  {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.ascii());
        fprintf(stderr, "%s", PQerrorMessage(conn));
        return(1);
    }// end if
    fprintf(stderr,"La conexion con la base de datos ha ido bien, ahora vamos a por la fecha\n");
    formatofecha();
    return(0);
}// end inicializa


/** Cambia el formato de fecha de la base de datos para que usemos la
  * fecha española dd/mm/yyyy
  * \return Devuelve 0 si no ha habido problemas, en caso contrario devuelve 1
  */
int postgresiface2::formatofecha() {
    QString query="";
    PGresult   *res;
    begin();
    query.sprintf("SET DateStyle TO 'SQL'");
    res = PQexec(conn, query.ascii());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Cambio del formato de fecha command failed\n");
    }// end if
    PQclear(res);
    commit();
    return(0);
}// end formatofecha


/** Con esta sentencia se inicia una transacción en la base de datos
  * Las transacciones lo que indican es que el usuario se ha apoderado de la base de datos durante
  * un tiempo y que la operación que va a transcurrir debe hacerse sin concurrencia.
  * \return Si todo ha funcionado bien devuelve un 0, en caso contrario devuelve un 1
  */
int postgresiface2::begin() {
    PGresult   *res;
    res = PQexec(conn, "BEGIN");
    fprintf(stderr,"-- BEGIN TRANSACTION --\n");
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)  {
        fprintf(stderr, "BEGIN command failed\n");
        PQclear(res);
        return (1);
    }// end if
    PQclear(res);
    return(0);
}// end begin


/** Con esta sentencia se termina un bloque de transacción dando por buenos todos los resultados que
  * Se han almacenado dentro de éste. Y quedando almacenados en la base de datos de forma definitiva.
  */
void postgresiface2::commit() {
    PGresult   *res;
    fprintf(stderr,"-- COMMIT TRANSACTION --\n");
    res = PQexec(conn, "COMMIT");
    PQclear(res);
}// end commit


/** Con esta sentencia se termina un bloque de transacción dando por malos los resultados de la operación
  * Y dejandose la base de datos en el mismo estado que cuando se inició la transacción.
  */
void postgresiface2::rollback() {
    PGresult   *res;
    res = PQexec(conn, "ROLLBACK");
    PQclear(res);
}// end rollback


/** Se encarga de generar un objeto del tipo cursor2 y de iniciarlo con un query concreto
  * NOTA: Este método crea memória, con lo que ésta debe ser liberada posteriormente.
  * \return Devuelve un apuntador al objeto \ref cursor2 generado e inicializado con la respuesta al query.
  */
cursor2 *postgresiface2::cargacursor(QString Query, QString nomcursor) {
    fprintf(stderr,"%s\n",Query.ascii());
    cursor2 *cur=new cursor2(nomcursor,conn,Query);
    return(cur);
}// end cargacursor


/** \brief Ejecuta un comando SQL
\param Query Comando a ejecutar. Debe ser un comando que no devuelva ningún valor (como \c select).
\retval 0 Si la ejecución fue correcta
\retval 1 en caso contrario
*/
int postgresiface2::ejecuta(QString Query) {
    PGresult *result;
    fprintf(stderr,"%s\n",Query.ascii());
    //Prova de control de permisos
    if (confpr->valor(CONF_PRIVILEGIOS_USUARIO) != "1" && (Query.left(6)=="DELETE" || Query.left(6)=="UPDATE" || Query.left(6)=="INSERT"))
        return (42501);
    //Fi prova. Nota: 42501 = INSUFFICIENT PRIVILEGE en SQL Standard
    result = PQexec(conn, Query.ascii());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "SQL command failed: %s\n", Query.ascii());
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        QMessageBox::warning(NULL, theApp->translate("postgresiface","Error...",""), theApp->translate("postgresiface","Ocurrió un error con la Base de Datos:\n"+Query+"\n"+PQerrorMessage(conn),""), theApp->translate("postgresiface","Aceptar",""));
        PQclear(result);
        return(1);
    }// end if
    PQclear(result);
    return(0);
}// end ejecuta



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
    query.sprintf("INSERT INTO borrador (idcuenta,idasiento,conceptocontable, descripcion, debe, haber, fecha, contrapartida, idtipoiva, idc_coste, idcanal) VALUES (%s, %d,'%s','%s', %2.2f, %2.2f,'%s', %s, %d, %s, %s)",textcuenta.ascii(), idasiento, concepto.ascii(), descripcion.ascii(), debe, haber, fecha.ascii(), textcontrapartida.ascii(), idtipoiva, textidccoste.ascii(), textidcanal.ascii());
    fprintf(stderr,"%s\n",query.ascii());
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

    query.sprintf("UPDATE borrador SET idcuenta=%d, debe=%2.2f, haber=%2.2f, conceptocontable='%s', fecha='%s', contrapartida=%s, idtipoiva=%d, idc_coste=%s, idcanal=%s WHERE idborrador=%d",idcuenta,idebe,ihaber,concepto.ascii(),fecha.ascii(),textcontrapartida.ascii(),idtipoiva,textidccoste.ascii(),textocanal.ascii(), idborrador);
    fprintf(stderr,"%s\n",query.ascii());
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
        query.sprintf("SELECT * FROM cuenta WHERE codigo LIKE '%s' ORDER BY codigo",ccuenta.ascii());
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
    query.sprintf("SELECT * FROM apunte where idcuenta=%d AND fecha>='%s' AND fecha<='%s' ORDER BY fecha",tidcuenta, fechainicial.ascii(), fechafinal.ascii());
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargaapuntesctafecha


/* **********************************************************
 * Esta funcion carga los saldos de una cuenta dada antes de la fecha
 * limite especificada. Los apuntes de la fecha no estan incluidos. *
 ************************************************************/
cursor2 *postgresiface2::cargasaldoscuentafecha(int idcuenta, QString fecha) {
    QString query="";
    query.sprintf("SELECT sum(debe) as tdebe, sum(haber)as thaber FROM apunte WHERE idcuenta=%d AND fecha <'%s'",idcuenta, fecha.ascii());
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargasaldoscuentafecha


/* *****************************************************
 * Esta funcion carga de la base de datos los asientos
 *******************************************************/
cursor2 *postgresiface2::cargaasientosfecha(QString fechini, QString fechfin) {
    QString query="";
    query.sprintf("SELECT * FROM asiento WHERE fecha >= '%s' AND fecha <= '%s' ORDER BY fecha", fechini.ascii(),fechfin.ascii());
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
        query.sprintf("SELECT * FROM cuenta WHERE padre=%d AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",padre, codigoinicial.ascii(), codigofinal.ascii());
    } else if (padre== 0) {
        query.sprintf("SELECT * FROM cuenta WHERE padre isnull AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial.ascii(), codigofinal.ascii());
    } else if (padre ==-1) {
        query.sprintf("SELECT * FROM cuenta WHERE codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial.ascii(), codigofinal.ascii());
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
    fprintf(stderr,"Funcion abreasiento\n");
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
    query.sprintf("UPDATE cuenta SET descripcion='%s', codigo='%s', imputacion=%s, bloqueada=%s, idgrupo=%d, activo=%s, nombreent_cuenta='%s', cifent_cuenta='%s', dirent_cuenta='%s', cpent_cuenta='%s', telent_cuenta='%s', coment_cuenta='%s', bancoent_cuenta='%s', emailent_cuenta='%s', webent_cuenta='%s', tipocuenta=%d, nodebe=%s, nohaber = %s WHERE idcuenta=%d\n",desccuenta.ascii(),codigo.ascii(),imputacion.ascii(),bloqueada.ascii(),idgrupo,activo.ascii(),nombreent.ascii(), cifent.ascii(),dir.ascii(),cp.ascii(),tel.ascii(),comm.ascii(),banco.ascii(), email.ascii(), web.ascii(),tipocuenta,nodebe.ascii(), nohaber.ascii(), idcuenta);
    fprintf(stderr,"%s\n",query.ascii());
    return(ejecuta(query));
}// end modificacuenta


int postgresiface2::nuevacuenta(QString desccuenta, QString codigo, int padre, int idgrupo, QString nombreent, QString cifent, QString dir, QString cp, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
    QString query="";
    QString tpadre;
    if (padre == 0) {
        tpadre="NULL";
    }  else  {
        tpadre.sprintf("'%d'",padre);
    }// end if
    QString nodebe = cnodebe ? "TRUE" : "FALSE";
    QString nohaber = cnohaber ? "TRUE" : "FALSE";

    query.sprintf("INSERT INTO cuenta (descripcion, padre,codigo, idgrupo, nombreent_cuenta, cifent_cuenta, dirent_cuenta, cpent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta, tipocuenta, nodebe, nohaber) VALUES('%s',%s,'%s',%d, '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, %s, %s)",desccuenta.ascii(),tpadre.ascii(), codigo.ascii(), idgrupo, nombreent.ascii(), cifent.ascii(), dir.ascii(), cp.ascii(), tel.ascii() ,comm.ascii(), banco.ascii(), email.ascii(), web.ascii(), tipocuenta, nodebe.ascii(), nohaber.ascii() );
    fprintf(stderr,"%s\n",query.ascii());
    return(ejecuta(query));
}// end nuevacuenta


int postgresiface2::nuevoasiento(QString nombre, QString fecha, int numasiento, int clase) {
    // clase => 0=Apertura, 1=Normal, 98=Regularización 99=Cierre
    QString query="";
    int val;
    int ordenasiento;
    //0=Apertura, 1=Normal, 98=Regularización 99=Cierre
    if (numasiento == 0) {
        query="SELECT max(idasiento) FROM asiento";
        cursor2 *cur=cargacursor(query,"cargaasientoseq");
        if (!cur->eof())
            val = atoi(cur->valor(0).ascii());
        else
            val=0;
        val++;
        delete cur;
    } else {
        val = numasiento;
    }// end if
    query.sprintf("SELECT MAX(ordenasiento) FROM asiento WHERE EXTRACT(YEAR FROM fecha)='%s'",fecha.right(4).ascii());
    cursor2 *cur=cargacursor(query,"cargaasientos");
    if (!cur->eof())
        ordenasiento = atoi(cur->valor(0).ascii());
    else
        ordenasiento=0;
    ordenasiento++;
    delete cur;
    query.sprintf("INSERT INTO asiento (idasiento,descripcion, fecha, ordenasiento, clase) VALUES (%d,'%s','%s', %d, %d)",val,nombre.ascii(), fecha.ascii(), ordenasiento, clase);
    ejecuta(query);
    return(val);
}// end nuevoasiento


cursor2 *postgresiface2::cargaempresas() {
    QString query;
    query= "SELECT * FROM empresa";
    cursor2 *cur=cargacursor(query,"cargaempresas");
    return(cur);
}// end cargaempresas



/** Esta función carga de la metabase la combinación usuario password y basde de datos y
  * devuelve el número de tuplas encontrados.
  * Sirve como comprobación de que los datos introducidos (usuario/password y la empresa seleccionada) son verídicos.
  */
int postgresiface2::cargaempresa(QString nomempresa, QString login, QString password) {
    fprintf(stderr,"postgresiface2::cargaempresa\n");
    QString query="";
    PGresult   *res;
    /* make a connection to the database */
    QString conexion;

    if (confpr->valor(CONF_SERVIDOR) == "localhost") {
        conexion.sprintf( "dbname = %s", (char *)confpr->valor(CONF_METABASE).ascii());
    } else {
        conexion.sprintf("hostaddr=%s port=%s dbname=%s", (char *) confpr->valor(CONF_SERVIDOR).ascii(), (char *) confpr->valor(CONF_PUERTO).ascii(), (char *) confpr->valor(CONF_METABASE).ascii());
    }// end if

    conn = PQconnectdb(conexion);

    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.ascii());
        fprintf(stderr, "%s", PQerrorMessage(conn));
    }// end if
    begin();

    /** fetch rows from the pg_database, the system catalog of
     * databases
     */
    query.sprintf("DECLARE mycursor CURSOR FOR SELECT * FROM empresa, usuario, usuario_empresa where usuario.idusuario=usuario_empresa.idusuario AND empresa.idempresa=usuario_empresa.idempresa AND usuario.login='%s' AND empresa.nombredb='%s' AND usuario.password='%s'",login.ascii(), nomempresa.ascii(), password.ascii());
    res = PQexec(conn, query.ascii());
    commit();
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DECLARE CURSOR command failed\n");
        PQclear(res);
    }// end if
    PQclear(res);
    res = PQexec(conn, "FETCH ALL in mycursor");
    if (!res || PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "FETCH ALL command didn't return tuples properly\n");
        PQclear(res);
    } // end if
    nFields = PQntuples(res);
    open=1;
    return nFields;
}// end cargaempresa


/** Esta función estática devuelve una cadena "saneada" para pasarsela a Postgresql.
  * Neutraliza (escapes) los caracteres problemáticos por ser caracteres especiales
  * de Postgresql. Ejemplo, comillas, contrabarras,...
  */
QString postgresiface2::sanearCadena(QString cadena) {
    int longitud;
    char *buffer;
    QString cadenaLimpia;
    longitud = cadena.length();
    // Reservamos (la funcion de postgres lo necesita) un buffer del
    // doble de caracteres + 1 que la cadena original
    buffer = (char *)malloc(sizeof(char)*longitud*2+1);
    PQescapeString(buffer, cadena.ascii(), longitud);
    cadenaLimpia = buffer;
    free(buffer);
    return cadenaLimpia;
}

/** Devuelve el valor de una propiedad de la empresa
  * Las propiedades de la empresa son valores asociados a ésta de la forma Nombre=Valor. 
  *De esta forma se guardan datos como el nombre fiscal de la empresa, CIF, domicilio, etc.
  * \param nombre Nombre de la propiedad
  * \return Valor de la propiedad
*/
QString postgresiface2::propiedadempresa(QString nombre) {
    PGresult *result;
    QString value;
    int num;
    QString Query="select * from configuracion where nombre='"+nombre+"'";
    fprintf(stderr,"%s\n",Query.ascii());
    result = PQexec(conn, Query.ascii());
    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SQL command failed: %s\n", Query.ascii());
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        PQclear(result);
        return("");
    }// end if
    num=PQntuples(result);
    if (num>1)
        fprintf(stderr,"Aviso: Hay %d valores para el campo %s en la tabla configuracion\n",num,nombre.ascii());
    if (num==0)
        value="";
    else
        value= PQgetvalue(result, 0, 2);
    PQclear(result);
    return(value);
}// end propiedadempresa


