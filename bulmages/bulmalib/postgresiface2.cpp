/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras                                    *
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

/// Contiene la implementacion de las clases 'cursor2' y 'postgresiface2' que proveen
/// acceso a las bases de datos de postgres de forma sencilla y eficiente.
#include <QMessageBox>
#include <QApplication>

#include "postgresiface2.h"

#include "msgerror.h"
#include "funcaux.h"


bool cursor2::error() {
    return m_error;
}


QString cursor2::query() {
    return m_query;
}


int cursor2::regactual() {
    return registroactual;
}


/// Realiza la consulta en la base de datos y almacena el resultado en las variables de
/// clase para poder ser manupuladas.
/// Tambien almacena en variables globales algunos resultados para poder acelerar las
/// consultas (nregistros y ncampos).
/// Si todo falla (y en funcion de la configuracion) Da un mensaje de alerta o no.
/// \param nombre Nombre que obtendra el query (OBSOLETO)
/// \param conn1 Conexion con la base de datos (Inicializada en \ref postgresiface2
/// \param SQLQuery Query en formato SQL a realizar en la base de datos.
cursor2::cursor2(QString nombre, PGconn *conn1, QString SQLQuery) {
    _depura("cursor2::cursor2", 0, SQLQuery);
    try {
        conn = conn1;
        m_error = FALSE;
        m_query = SQLQuery;
        nomcursor = nombre;
        nregistros = 0;
        registroactual = 0;
        _depura(SQLQuery, 0);
        result = PQexec(conn, SQLQuery.toAscii().data());

        /// TODO: Switch en pruebas 9/5/07. Si todo OK borrarlo pasado 9/6/07.
        switch (PQresultStatus(result)) {
            case PGRES_COMMAND_OK:
                break;
            case PGRES_NONFATAL_ERROR:
            case PGRES_FATAL_ERROR:
            case NULL:
                m_error = TRUE;
                _depura(PQerrorMessage(conn));
                _depura("QUERY command failed [" + SQLQuery + "]", 10);
                if (confpr->valor(CONF_ALERTAS_DB) == "Yes") {
                    msgError(QString(QObject::tr("Error al hacer la consulta con la base de datos.")) + "\n:: " + QString(PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY)) + " ::", SQLQuery + QString("\n") + (QString) PQerrorMessage(conn));
                } // end if
                g_main->statusBar()->showMessage(QString(PQresultErrorField(result, PG_DIAG_MESSAGE_PRIMARY)), 4000);
                PQclear(result);
                throw -1;
                break;
            default:
                break;
        } // end switch

        nregistros = PQntuples(result);
        ncampos = PQnfields(result);
        registroactual = 0;
        /// Depuramos todo.
        _depura("------------ RESULTADO DE LA CONSULTA -----------------");
        QString err;
        err.sprintf("Numero de registros: %d, Numero de campos: %d", nregistros, ncampos);
        _depura(err);
        _depura("--------- FIN RESULTADO DE LA CONSULTA ----------------");
    } catch (...) {
        _depura("cursor2::cursor2: Error en la consulta: " + SQLQuery, 3);
        throw -1;
    } // end try
    _depura("END cursor2::cursor2", 0, " Numero de registros: " + QString::number(nregistros) + ", Numero de campos: " + QString::number(ncampos));
}


/// Destructor de clase, antes de destruirse limpia los posibles buffers intermedios.
cursor2::~cursor2() {
    cerrar();
}


/// Limpia los buffers intermedios que puedan estar chupando memoria.
void cursor2::cerrar() {
    PQclear(result);
}


/// Devuelve el nmero de registros de la consulta.
int cursor2::numregistros() {
    return nregistros;
}


/// Devuelve el nmero de campos de la consulta.
int cursor2::numcampos() {
    return ncampos;
}


/// Devuelve el nombre del campo de una posicion determinada.
/// \param campo Posicion de la que se quiere que devuelva el nombre.
QString cursor2::nomcampo(int campo) {
    return ((QString)PQfname(result, campo));
}


/// Dado un campo esta funcion devuelve devuelve la posicion que le
/// corresponde, y si no le corresponde ninguno devuelve -1
/// \param campo Nombre del campo.
/// \return Devuelve la posicion si el nombre del campo existe, y si no devuelve -1
/// \todo Tal vez deberia crearse una estructura intermedia que indexe los nombres con
/// las posiciones para hacer la busqueda mas rapida, pero al ser el numero de registros
/// siempre muy reducido seguramente no arreglariamos nada de nada.
int cursor2::numcampo(QString campo) {
    _depura("cursor2::numcampo", 0);
    int i = 0;
    while (i < numcampos() && campo != nomcampo(i)) {
        i++;
    } // end while
    if (i == numcampos()) {
        //_depura("cursor2::numcampo", 2, "Campo: " + campo + " no existe en la consulta: " + m_query);
        return -1;
    } // end if
    _depura("END cursor2::numcampo", 0);
    return i;
}


/// Esta funcion devuelve el valor del campo posicion del registro
/// pasado, si se pasa -1 como registro se devuelve el registro actual
/// \param posicion El nmero de campo del que se quiere la posicion.
/// \param registro El registro del que se quiere devolver el campo.
/// Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
/// \return El valor de la posicion.
QString cursor2::valor(int posicion, int registro) {
    _depura("cursor2::valor", 0, QString::number(posicion) + QString::number(registro));
    if (registro == -1) {
        registro = registroactual;
    } // end if
    _depura("END cursor2::valor", 0);
    return (QString::fromUtf8(PQgetvalue(result, registro, posicion)));
}


/// Esta funci&oacute;n devuelve el valor del campo especificado (por nombre) del registro
/// pasado, si se pasa como registro -1 se devuelve el registro actual.
/// \param campo Nombre del campo a devolver
/// \param registro El registro del que se quiere devolver el campo.
/// Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
/// \return El valor de la posici&oacute;n.
QString cursor2::valor(QString campo, int registro) {
    _depura("cursor2::valor", 0, campo + " " + QString::number(registro));
    int i = 0;
    if (registro == -1) {
        registro = registroactual;
    } // end if
    i = numcampo(campo);
    if (i == -1)
        return "";
    _depura("END cursor2::valor ", 0, "campo:" + campo + " ----- Valor:" + PQgetvalue(result, registro, i));
    return (QString::fromUtf8(PQgetvalue(result, registro, i)));
}


/// Devuelve la posici&oacute;n siguiente al registro que se est&aacute; recorriendo.
int cursor2::siguienteregistro() {
    _depura("cursor2::siguienteregistro", 0, "Registro actual: " + QString::number(registroactual) + " Numero de registros: " + QString::number(nregistros));
    _depura("END cursor2::siguienteregistro", 0);
    return ++registroactual;
}


/// Devuelve la posici&oacute;n anterior al registro que se est&aacute; recorriendo.
int cursor2::registroanterior() {
    _depura("cursor2::registroanterior", 0, "Registro actual: " + QString::number(registroactual) + " Numero de registros: " + QString::number(nregistros));
    return --registroactual;
}


/// Devuelve la posici&oacute;n del primer registro de la tabla de registros.
int cursor2::primerregistro() {
    _depura("cursor2::primerregistro", 0, "Registro actual: " + QString::number(registroactual) + " Numero de registros: " + QString::number(nregistros));
    registroactual = 0;
    _depura("END cursor2::primerregistro", 0);
    return 0;
}


/// Devuelve el &uacute;ltimo registro de la tabla de registros.
int cursor2::ultimoregistro() {
    _depura("cursor2::ultimoregistro", 0, "Registro actual: " + QString::number(registroactual)+" Numero de registros: " + QString::number(nregistros));
    registroactual = nregistros - 1;
    return registroactual;
}


/// Devuelve TRUE si el registro est&aacute; en la posici&oacute;n final, o si est&aacute; vacio.
bool cursor2::eof() {
    _depura("cursor2::eof", 0);
    if (nregistros == 0) {
        return (true);
    } // end if
    _depura("END cursor2::eof", 0);
    return (registroactual >= nregistros);
}


/// Devuelve TRUE si el registro est&aacute; en la posici&oacute;n inicial, o si est&aacute; vacio.
bool cursor2::bof() {
    if (nregistros == 0) {
        return(true);
    } // end if
    return (registroactual < 0);
}


/// Devuelve TRUE si es el &uacute;ltimo registro a considerar.
bool cursor2::esultimoregistro() {
    _depura("cursor2::esultimoregistro", 0);
    return (registroactual == nregistros - 1);
}


/// devuelve TRUE si es el primer registro a consear e el query.
bool cursor2::esprimerregistro() {
    return (registroactual == 0);
}


/// ----------------------------------------------------------------- ///


QString postgresiface2::nameDB() {
    return dbName;
}


/// Constructor de la clase, no hace nada de nada de nada.
postgresiface2::postgresiface2() {
    m_transaccion = FALSE;
}


/// Finaliza la conexi&oacute;n con la base de datos.
void postgresiface2::terminar() {
    PQfinish(conn);
}


/// Destructor de la clase que al igual que \ref terminar termina la conexi&oacute;n
/// con la base de datos.
postgresiface2::~postgresiface2() {
    _depura("postgresiface2::~postgresiface2", 0);
    /// close the connection to the database and cleanup.
    PQfinish(conn);
    _depura("END postgresiface2::~postgresiface2", 0);
}


/// Inicializa la conexi&oacute;n con la base de datos mediante los par&aacute;metro
/// especificados. Precisamente no lo hace el constructor debido a la ausencia de
/// dichos datos.
/// \param nomdb Indica el nombre de la base de datos.
/// \param user Indica el usuario que hace la operacion a ojos de la base de datos.
/// \param passwd Indica la contrasenya que utiliza el usuario para autentificarse.
/// \return Si todo va bien devuelve 0, en caso contrario devuelve 1.
int postgresiface2::inicializa(QString nomdb) {
    _depura("postgresiface2::inicializa", 0, nomdb);
    dbName = nomdb;
    pghost = confpr->valor(CONF_SERVIDOR); /// host name of the backend server.
    pgport = confpr->valor(CONF_PUERTO); /// port of the backend server.
    pgoptions = ""; /// special options to start up the backend server.
    pgtty = ""; /// debugging tty for the backend server.
    QString conexion;

    QString user = confpr->valor(CONF_LOGIN_USER);
    QString passwd = confpr->valor(CONF_PASSWORD_USER);
    /// Antes no resolvia bien en caso de querer hacer conexiones al ordenador local.
    /// Ahora si se pone -- se considera conexion local.
    if (pghost != "--")
        conexion = "host = " + pghost;
    conexion += " port = " + pgport;
    conexion += " dbname = " + dbName;
    if (user != "")
        conexion += " user = " + user;
    if (passwd != "")
        conexion += " password = " + passwd;

    _depura(conexion, 0);
    conn = PQconnectdb(conexion.toAscii().data());
    if (PQstatus(conn) == CONNECTION_BAD) {
        _depura("La conexion con la base de datos '" + dbName + "' ha fallado.\n", 0);
        if (passwd != "" && confpr->valor(CONF_ALERTAS_DB) == "Yes") {
            _depura(PQerrorMessage(conn), 2);
        } else {
            _depura(PQerrorMessage(conn), 0);
        } // end if
        return 1;
    } // end if
    _depura("La conexion con la base de datos ha ido bien, ahora vamos a por la fecha", 0);
    formatofecha();
    _depura("END postgresiface2::inicializa", 0, nomdb);
    return 0;
}


/// Cambia el formato de fecha de la base de datos para que usemos la
/// fecha espayola dd/mm/yyyy
/// \return Devuelve 0 si no ha habido problemas, en caso contrario devuelve 1.
int postgresiface2::formatofecha() {
    _depura("postgresiface2::formatofecha", 0);
    QString query = "";
    PGresult *res;
    query = "SET DATESTYLE TO SQL, European";
    res = PQexec(conn, query.toAscii().data());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        _depura( "Cambio del formato de fecha command failed");
    } // end if
    PQclear(res);

    /// Establecemos la codificacion por defecto a UNICODE.
    /// Pero con los problemas que está teniendo el UNICODE lo vamos a dejar en
    /// SQL_ASCII QUE funciona bastante mejor.
    query = "SET client_encoding = 'UTF8'";
    res = PQexec(conn, query.toAscii().data());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        _depura( "Cambio del formato de codificacion");
    } // end if
    PQclear(res);
    _depura("END postgresiface2::formatofecha", 0);
    return 0;
}


/// Con esta sentencia se inicia una transaccion en la base de datos
/// Las transacciones lo que indican es que el usuario se ha apoderado de la base de datos
/// durante un tiempo y que la operacion que va a transcurrir debe hacerse sin concurrencia.
/// \return Si todo ha funcionado bien devuelve un 0, en caso contrario devuelve un 1.
int postgresiface2::begin() {
    _depura("postgresiface2::begin", 0);
    if (m_transaccion) {
        _depura("Ya estamos dentro de una transaccion", 0);
        return -1;
    } // end if
    PGresult *res;
    res = PQexec(conn, "BEGIN");
    _depura("-- BEGIN TRANSACTION --");
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        _depura("BEGIN command failed");
        PQclear(res);
        return -1;
    } // end if
    PQclear(res);
    m_transaccion = TRUE;
    _depura("END postgresiface2::begin", 0);
    return(0);
}


/// Con esta sentencia se termina un bloque de transaccion dando por buenos todos los
/// resultados que se han almacenado dentro. Y quedando almacenados en la base
/// de datos de forma definitiva.
void postgresiface2::commit() {
    _depura("postgresiface2::commit", 0);
    if (!m_transaccion) {
        _depura("No estamos en ninguna transaccion", 0);
        return;
    } // end if
    PGresult *res;
    _depura("-- COMMIT TRANSACTION --");
    res = PQexec(conn, "COMMIT");
    PQclear(res);
    m_transaccion = FALSE;
    _depura("postgresiface2::commit", 0);
}


/// Con esta sentencia se termina un bloque de transaccion dando por malos los resultados
/// de la operacion y dejandose la base de datos en el mismo estado que cuando se
/// inicio la transaccion.
void postgresiface2::rollback() {
    _depura("postgresiface2::rollback", 0);
    if (!m_transaccion) {
        _depura("No estamos en ninguna transaccion", 0);
        return;
    } // end if
    PGresult *res;
    res = PQexec(conn, "ROLLBACK");
    PQclear(res);
    m_transaccion = FALSE;
    _depura("END postgresiface2::rollback", 0);
}


/// Se encarga de generar un objeto del tipo cursor2 y de iniciarlo con un query concreto
/// NOTA: Este metodo crea memoria, con lo que esta debe ser liberada posteriormente.
/// \return Devuelve un apuntador al objeto \ref cursor2 generado e inicializado con la
/// respuesta al query.
cursor2 *postgresiface2::cargacursor(QString Query, QString nomcursor) {
    _depura ("postgresiface2::cargacursor", 0, Query);
    cursor2 *cur = NULL;
    try {
        cur = new cursor2(nomcursor, conn, Query);
    } catch (...) {
        _depura("postgresiface2::cargacursor La base de datos genero un error: ", 10);
        _depura(Query, 10);
        delete cur;
        throw -1;
    } // end try
    _depura ("END postgresiface2::cargacursor", 0, nomcursor);
    return cur;
}


/// Ejecuta un comando SQL
/// \param Query Comando a ejecutar. Debe ser un comando que no devuelva ningun valor
/// (como \c select).
/// \retval 0 Si la ejecucion fue correcta
/// \retval 1 en caso contrario

#include <qtextcodec.h>


int postgresiface2::ejecuta(QString Query) {
    _depura("postgresiface2::ejecuta", 0, Query);
    PGresult *result = NULL;
    try {
        /// Prova de control de permisos.
        if (confpr->valor(CONF_PRIVILEGIOS_USUARIO) != "1" && (Query.left(6) == "DELETE" || Query.left(6) == "UPDATE" || Query.left(6) == "INSERT"))
            throw 42501;
        /// Fi prova. Nota: 42501 = INSUFFICIENT PRIVILEGE en SQL Standard.
        result = PQexec(conn, (const char *) Query.toUtf8());
        if (!result || PQresultStatus(result) != PGRES_COMMAND_OK)
            throw -1;
        PQclear(result);
        _depura("postgresiface2::ejecuta", 0);
        return 0;
    } catch (int e) {
        if (e == 42501) {
            _depura("SQL command failed: " + Query);
            fprintf(stderr,"%s\n", PQerrorMessage(conn));
            QString mensaje = "No tiene permisos suficientes para ejecutar el comando SQL:\n";
            msgError(mensaje + (QString)PQerrorMessage(conn), Query + "\n" + (QString)PQerrorMessage(conn));
            PQclear(result);
            throw -1;
        } else {
            _depura("SQL command failed: " + Query);
            fprintf(stderr,"%s\n", PQerrorMessage(conn));
            QString mensaje = "Error al intentar modificar la base de datos:\n";
            msgError(mensaje + (QString)PQerrorMessage(conn), Query + "\n" + (QString)PQerrorMessage(conn));
            PQclear(result);
            throw -1;
        } // end if
    } catch (...) {
        throw -1;
    } // end try
    _depura("END postgresiface2::ejecuta", 0, Query);
}


/// This function search in the database the account parent of the account selected
/// if there are not parent returns NULL
/// else returns the code of the parent account.
QString postgresiface2::searchParent(QString cod) {
    QString padre = "NULL"; /// Almacena el padre de la cuenta.
    QString query;
    int i = 2;
    int fin = 0;
    while (!fin) {
        query = "SELECT * FROM cuenta WHERE codigo = '" + cod.left(i) + "'";
        begin();
        cursor2 *cur = cargacursor(query, "unquery");
        commit();
        if (!cur->eof()) {
            padre = cur->valor("codigo");
        } else {
            fin = 1;
        } // end if
        delete cur;
        i++;
    } // end while
    return padre;
}


int postgresiface2::nuevoborrador(int idcuenta, int idasiento, QString concepto, QString descripcion, float debe, float haber, QString fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal) {
    QString query = "";
    QString textcuenta;
    QString textcontrapartida;
    QString textidccoste;
    QString textidcanal;
    if (idcuenta == 0) {
        textcuenta = "NULL";
    } else {
        textcuenta.sprintf("%d", idcuenta);
    } // end if
    if (idcontrapartida == 0) {
        textcontrapartida = "NULL";
    } else {
        textcontrapartida.sprintf("%d", idcontrapartida);
    } // end if
    if (idccoste == 0) {
        textidccoste = "NULL";
    } else {
        textidccoste.sprintf("%d", idccoste);
    } // end if
    if (idcanal == 0) {
        textidcanal = "NULL";
    } else {
        textidcanal.sprintf("%d", idcanal);
    } // end if
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
    return (ejecuta(query));
}


/// Esta funcion se encarga de modificar el apunte con el identificador
/// correspondiente.
int postgresiface2::modificaborrador(int idborrador, int idcuenta, float idebe, float ihaber, QString concepto, QString fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal) {
    QString query = "";
    QString textidccoste;
    QString textcontrapartida;
    QString textocanal;
    if (idccoste == 0) {
        textidccoste = "NULL";
    } else {
        textidccoste.sprintf("%d", idccoste);
    } // end if
    if (contrapartida == 0) {
        textcontrapartida = "NULL";
    } else {
        textcontrapartida.sprintf("%d", contrapartida);
    } // end if
    if (idcanal == 0) {
        textocanal = "NULL";
    } else {
        textocanal.sprintf("%d", idcanal);
    } // end if

    query.sprintf("UPDATE borrador SET idcuenta = %d, debe = %2.2f, haber = %2.2f, conceptocontable = '%s', fecha = '%s', contrapartida = %s, idtipoiva = %d, idc_coste = %s, idcanal = %s WHERE idborrador = %d", idcuenta, idebe, ihaber, concepto.toAscii().data(), fecha.toAscii().data(), textcontrapartida.toAscii().data(), idtipoiva, textidccoste.toAscii().data(), textocanal.toAscii().data(), idborrador);
    _depura(query);
    return(ejecuta(query));
}


/// Esta funcion carga de la base de datos la cuenta especificada
/// si idcuenta != 0 se intenta cargar la cuenta con idcuenta especificada
/// si idcuenta == 0 se intenta cargar la cuenta con codigo especificado
cursor2 *postgresiface2::cargacuenta(int idcuenta, QString ccuenta) {
    QString query = "";
    if ( idcuenta != 0) {
        query.sprintf("SELECT * FROM cuenta WHERE idcuenta = %d", idcuenta);
    }  else  {
        query.sprintf("SELECT * FROM cuenta WHERE codigo LIKE '%s' ORDER BY codigo", ccuenta.toAscii().data());
    } // end if
    cursor2 *cur = cargacursor(query, "cargacuenta");
    return cur;
}


/// Esta funcion carga de la base de datos el asiento con idasiento especificado.
cursor2 *postgresiface2::cargaasiento(int idasiento) {
    QString query = "";
    query.sprintf("SELECT * FROM asiento WHERE idasiento = %d", idasiento);
    cursor2 *cur = cargacursor(query, "cargaasiento");
    return cur;
}


/// Esta funcion carga de la base de datos los apuntes
/// tidasiento --> El asiento del que se deben cargar los apuntes.
cursor2 *postgresiface2::cargaapuntes(int tidasiento) {
    QString query = "";
    query.sprintf("SELECT * FROM apunte where idasiento = %d ORDER BY idapunte", tidasiento);
    cursor2 *cur = cargacursor(query, "cargaapuntes");
    return cur;
}


/// Esta funcion carga de la base de datos los apuntes
/// tidasiento --> El asiento del que se deben cargar los apuntes.
cursor2 *postgresiface2::cargaborradores(int tidasiento) {
    QString query = "";
    query.sprintf("SELECT * FROM borrador where idasiento = %d ORDER BY idborrador", tidasiento);
    cursor2 *cur = cargacursor(query, "cargaborradores");
    return cur;
}


/// Esta funcion carga de la base de datos las cuentas
/// padre : = 0 carga las cuentas de nivel 0
/// padre : = -1 carga todas las cuentas sin excepcion
/// padre : else carga las cuentas cuyo padre es el valor especificado.
cursor2 *postgresiface2::cargacuentas(int padre) {
    QString query = "";
    if (padre != 0 && padre != -1 && padre != -2) {
        query.sprintf("SELECT * FROM cuenta WHERE padre=%d ORDER BY padre", padre);
    } else if (padre == 0) {
        query.sprintf("SELECT * FROM cuenta WHERE padre isnull ORDER BY padre ");
    } else if (padre == -1) {
        query.sprintf("SELECT * FROM cuenta ORDER BY padre ");
    } else if (padre == -2) {
        query.sprintf("SELECT * FROM cuenta WHERE NOT padre isnull ORDER BY padre ");
    }// end if
    cursor2 *cur = cargacursor(query, "cargaborradores");
    return cur;
}


/// Esta funcion carga de la base de datos los grupos.
cursor2 *postgresiface2::cargagrupos() {
    QString query = "SELECT * FROM grupo";
    cursor2 *cur = cargacursor(query, "cargagrupos");
    return cur;
}


/// Esta funcion carga de la base de datos los apuntes
/// entre las fechas indicadas. Ambas fechas inclusive.
cursor2 *postgresiface2::cargaapuntesctafecha(int tidcuenta, QString fechainicial, QString fechafinal) {
    QString query = "";
    query.sprintf("SELECT * FROM apunte where idcuenta = %d AND fecha >= '%s' AND fecha <= '%s' ORDER BY fecha",tidcuenta, fechainicial.toAscii().data(), fechafinal.toAscii().data());
    cursor2 *cur = cargacursor(query, "cargasaldoscuentafecha");
    return cur;
}


/// Esta funcion carga los saldos de una cuenta dada antes de la fecha
/// limite especificada. Los apuntes de la fecha no estan incluidos.
cursor2 *postgresiface2::cargasaldoscuentafecha(int idcuenta, QString fecha) {
    QString query = "";
    query.sprintf("SELECT sum(debe) as tdebe, sum(haber)as thaber FROM apunte WHERE idcuenta = %d AND fecha <'%s'",idcuenta, fecha.toAscii().data());
    cursor2 *cur = cargacursor(query, "cargasaldoscuentafecha");
    return(cur);
}


/// Esta funcion carga de la base de datos los asientos.
cursor2 *postgresiface2::cargaasientosfecha(QString fechini, QString fechfin) {
    QString query = "";
    query.sprintf("SELECT * FROM asiento WHERE fecha >= '%s' AND fecha <= '%s' ORDER BY fecha", fechini.toAscii().data(),fechfin.toAscii().data());
    cursor2 *cur=cargacursor(query, "cargaasientosfecha");
    return cur;
}


/// Esta funcion carga de la base de datos las cuentas entre los codigos especificados
/// padre : = 0 carga las cuentas de nivel 0
/// padre : = -1 carga todas las cuentas sin excepcion
/// padre : else carga las cuentas cuyo padre es el valor especificado.
/// codigoinicial: el codigo inicial a partir del cual cargar cuentas
/// codigofinal: el codigo hasta donde saldran cuentas.
cursor2 *postgresiface2::cargacuentascodigo(int padre, QString codigoinicial, QString codigofinal) {
    QString query = "";
    if (padre != 0 && padre != -1) {
        query.sprintf("SELECT * FROM cuenta WHERE padre = %d AND codigo >= '%s' AND codigo <= '%s' ORDER BY codigo", padre, codigoinicial.toAscii().data(), codigofinal.toAscii().data());
    } else if (padre== 0) {
        query.sprintf("SELECT * FROM cuenta WHERE padre isnull AND codigo >= '%s' AND codigo <= '%s' ORDER BY codigo", codigoinicial.toAscii().data(), codigofinal.toAscii().data());
    } else if (padre ==-1) {
        query.sprintf("SELECT * FROM cuenta WHERE codigo >= '%s' AND codigo <= '%s' ORDER BY codigo", codigoinicial.toAscii().data(), codigofinal.toAscii().data());
    } // end if
    cursor2 *cur = cargacursor(query, "cargasaldoscuentafecha");
    return cur;
}


int postgresiface2::cierraasiento(int idasiento) {
    QString query;
    query.sprintf("SELECT cierraasiento(%d)", idasiento);
    cursor2 *cur = cargacursor(query, "abreasientos");
    delete cur;
    return 1;
}


/// Esta funcion se encarga de borrar el apunte con el identificador
/// correspondiente.
/// Si el asiento no esta vacio entonces no se borra.
int postgresiface2::borrarasiento(int idasiento) {
    QString query = "";
    query.sprintf("DELETE FROM asiento WHERE idasiento = %d", idasiento);
    return (ejecuta(query));
}


/// Esta funcion se encarga de borrar el apunte con el identificador
/// correspondiente.
int postgresiface2::borrarborrador(int idborrador) {
    QString query = "";
    query.sprintf("DELETE FROM borrador WHERE idborrador = %d", idborrador);
    return (ejecuta(query));
}


/// Esta funcion se encarga de borrar el apunte con el identificador
/// correspondiente.
int postgresiface2::borrarcuenta(int idcuenta) {
    QString query = "";
    query.sprintf("DELETE FROM cuenta WHERE idcuenta = %d", idcuenta);
    return (ejecuta(query));
}


/// Esta funcion se encarga de abrir el asiento pasado como parametro
/// correspondiente.
int postgresiface2::abreasiento(int idasiento) {
    _depura("Funcion abreasiento\n");
    QString query = "";
    query.sprintf("SELECT abreasiento(%d)", idasiento);
    cursor2 *cur = cargacursor(query, "abreasientos");
    delete cur;
    return 1;
}


int postgresiface2::modificacuenta(int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString nombreent, QString cifent, QString dir, QString cp, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
    QString cadena;
    cadena.sprintf("%d", idcuenta);
    QString query = "";
    QString bloqueada = cbloqueada ? (char *) "TRUE" : (char *) "FALSE";
    QString activo = cactivo ?  "TRUE" :  "FALSE";
    QString imputacion = cimputacion ?  "TRUE" :  "FALSE";
    QString nodebe = cnodebe ? "TRUE" : "FALSE";
    QString nohaber = cnohaber ? "TRUE" : "FALSE";
    query.sprintf("UPDATE cuenta SET descripcion = '%s', codigo = '%s', imputacion = %s, bloqueada = %s, idgrupo = %d, activo = %s, nombreent_cuenta = '%s', cifent_cuenta = '%s', dirent_cuenta = '%s', cpent_cuenta = '%s', telent_cuenta = '%s', coment_cuenta = '%s', bancoent_cuenta = '%s', emailent_cuenta = '%s', webent_cuenta = '%s', tipocuenta = %d, nodebe = %s, nohaber = %s WHERE idcuenta = %d\n", desccuenta.toAscii().data(), codigo.toAscii().data(), imputacion.toAscii().data(), bloqueada.toAscii().data(),idgrupo, activo.toAscii().data(), nombreent.toAscii().data(), cifent.toAscii().data(), dir.toAscii().data(), cp.toAscii().data(), tel.toAscii().data(), comm.toAscii().data(), banco.toAscii().data(), email.toAscii().data(), web.toAscii().data(), tipocuenta,nodebe.toAscii().data(), nohaber.toAscii().data(), idcuenta);
    _depura(query);
    return (ejecuta(query));
}


int postgresiface2::nuevacuenta(QString desccuenta, QString codigo, int padre, int idgrupo, QString nombreent, QString cifent, QString dir, QString cp, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
    QString query = "";
    QString tpadre;
    if (padre == 0) {
        tpadre = "NULL";
    } else {
        tpadre.sprintf("%d", padre);
    } // end if
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
                  sanearCadena(nohaber).toAscii().data());
    return (ejecuta(query));
}


cursor2 *postgresiface2::cargaempresas() {
    QString query;
    query = "SELECT * FROM empresa";
    cursor2 *cur = cargacursor(query, "cargaempresas");
    return cur;
}


/// Esta funci&oacute;n est&eacute;tica devuelve una cadena "saneada" para pasarsela
/// a Postgresql. Neutraliza (escapes) los car&aacute;cteres problem&aacute;ticos por
/// ser car&aacute;cteres especiales de Postgresql. Ejemplo, comillas, barras invertidas, ...
QString postgresiface2::sanearCadena(QString cadena) {
    int longitud = 0;
    char *buffer = "";
    QString cadenaLimpia = "";
    longitud = cadena.size();
    /// Reservamos (la funci&oacute;n de postgres lo necesita) un buffer del
    /// doble de car&aacute;cteres + 1 que la cadena original.
    buffer = (char *) malloc((sizeof(char) * longitud * 2) + 1);
    PQescapeString(buffer, cadena.toAscii().constData(), cadena.toAscii().size());
    cadenaLimpia = QString::fromAscii(buffer);
    free(buffer);
    return cadenaLimpia;
}


/// Devuelve el valor de una propiedad de la empresa
/// Las propiedades de la empresa son valores asociados a esta de la forma Nombre = Valor.
/// De esta forma se guardan datos como el nombre fiscal de la empresa, CIF, domicilio, etc.
/// \param nombre Nombre de la propiedad.
/// \return Valor de la propiedad.
QString postgresiface2::propiedadempresa(QString nombre) {
    PGresult *result;
    QString value;
    int num;
    QString Query = "select * from configuracion where nombre = '" + nombre + "'";
    fprintf(stderr, "%s\n", Query.toAscii().data());
    result = PQexec(conn, Query.toAscii().data());
    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SQL command failed: %s\n", Query.toAscii().data());
        fprintf(stderr, "%s\n", PQerrorMessage(conn));
        PQclear(result);
        return "";
    } // end if
    num = PQntuples(result);
    if (num > 1)
        fprintf(stderr, "Aviso: Hay %d valores para el campo %s en la tabla configuracion\n", num, nombre.toAscii().data());
    if (num == 0)
        value = "";
    else
        value = PQgetvalue(result, 0, 2);
    PQclear(result);
    return value;
}

