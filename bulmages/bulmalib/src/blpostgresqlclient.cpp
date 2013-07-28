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

/// Contiene la implementacion de las clases 'BlDbRecordSet' y 'BlPostgreSqlClient' que proveen
/// acceso a las bases de datos de postgres de forma sencilla y eficiente.
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#include <QtCore/QLocale>

#include <climits>
#include <cmath>

#include "blpostgresqlclient.h"
#include "blerrordialog.h"


int g_num_begins=0;
int g_global_commit=true;

///
/**
\return
**/
bool BlDbRecordSet::error()
{
    BL_FUNC_DEBUG
    
    return m_error;
}


///
/**
\return
**/
QString BlDbRecordSet::query()
{
    BL_FUNC_DEBUG
    
    return m_query;
}

QString BlDbRecordSet::pristineQuery()
{
    BL_FUNC_DEBUG
    
    return m_pristineQuery;
}

///
/**
\return
**/
int BlDbRecordSet::currentRecord()
{
    BL_FUNC_DEBUG
    
    return registroactual;
}


/// Realiza la consulta en la base de datos y almacena el resultado en las variables de
/// clase para poder ser manupuladas.
/// Tambien almacena en variables globales algunos resultados para poder acelerar las
/// consultas (nregistros y ncampos).
/// Si todo falla (y en funcion de la configuracion) Da un mensaje de alerta o no.
/// \param nombre Nombre que obtendra el query (OBSOLETO)
/// \param conn1 Conexion con la base de datos (Inicializada en \ref BlPostgreSqlClient
/// \param SQLQuery Query en formato SQL a realizar en la base de datos.
BlDbRecordSet::BlDbRecordSet ( QString nombre, PGconn *conn1, QString SQLQuery, QString pristineQuery )
{
    inicializa ( nombre, conn1, SQLQuery, 0, ( char ** ) NULL, pristineQuery );
}


BlDbRecordSet::BlDbRecordSet ( QString nombre, PGconn *conn1, QString SQLQuery, int numParams,
                               const QString *paramValues, QString pristineQuery
                             )
{
    char *charValues[numParams];
    QByteArray qbaValues[numParams]; //si sabes C++ i Qt sabria si no cal ?
    for ( int i = 0; i < numParams ; i++ ) {
        qbaValues[i] = paramValues[i].toUtf8();
        charValues[i] = qbaValues[i].data();
    };
    inicializa ( nombre, conn1, SQLQuery, numParams, charValues, pristineQuery );
}


BlDbRecordSet::BlDbRecordSet ( QString nombre, PGconn *conn1, QString SQLQuery, int numParams,
                               const char * const *paramValues, QString pristineQuery
                             )
{
    inicializa ( nombre, conn1, SQLQuery, numParams, paramValues, pristineQuery );
}


void BlDbRecordSet::inicializa ( QString nombre, PGconn *conn1, QString SQLQuery, int numParams,
                                 const char * const *paramValues, QString pristineQuery
                               )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::BlDbRecordSet", 0, SQLQuery );
    for ( int i = 0; i < numParams; i++ ) {
        BlDebug::blDebug ( " param=", 0, QString::fromUtf8 ( paramValues[i] ) );
    } ;
    try {
        conn = conn1;
        m_error = false;
        m_query = SQLQuery;
        if ( pristineQuery.isNull() ) {
            m_pristineQuery = SQLQuery; // a falta de + info. Qui ens cridi ja ho sobreescriurà
        } else {
            m_pristineQuery = pristineQuery;
        }
        nomcursor = nombre;
        nregistros = 0;
        registroactual = 0;
        BlDebug::blDebug ( SQLQuery, 0 );
        result = PQexecParams ( conn, SQLQuery.toUtf8().data(), numParams, NULL,
                                ( numParams == 0 ? NULL : paramValues ), NULL, NULL, 0  );
        switch ( PQresultStatus ( result ) ) {
        case PGRES_NONFATAL_ERROR:
        case PGRES_FATAL_ERROR:
        case NULL:
            m_error = true;
            BlDebug::blDebug ( PQerrorMessage ( conn ) );
            BlDebug::blDebug ( "QUERY command failed [" + SQLQuery + "]", 10 );
            if ( g_confpr->valueTrue( CONF_ALERTAS_DB )) {
                BlErrorDialog ( _ ( "Error al hacer la consulta con la base de datos." ) + QString ( "\n:: " ) + QString ( PQresultErrorField ( result, PG_DIAG_MESSAGE_PRIMARY ) ) + " ::", SQLQuery + QString ( "\n" ) + ( QString ) PQerrorMessage ( conn ) );
            } // end if
            PQclear ( result );
            throw - 1;
            break;
        default:
            break;
        } // end switch

        nregistros = PQntuples ( result );
        ncampos = PQnfields ( result );
        registroactual = 0;

        /// Rellenamos el hash que luego nos sirve en la consulta.
        m_campos.clear();
        for ( int i = 0; i < ncampos; i++ ) {
            if ( !m_campos.contains ( fieldName ( i ) ) )
                m_campos[fieldName ( i ) ] = i;
        } // end for

        BlDebug::blDebug ( "------------ RESULTADO DE LA CONSULTA -----------------" );
        BlDebug::blDebug ( "Numero de registros: " + QString::number ( nregistros ) + ", Numero de campos: " + QString::number ( ncampos ) );
        BlDebug::blDebug ( "--------- FIN RESULTADO DE LA CONSULTA ----------------" );
    } catch ( ... ) {
        BlDebug::blDebug ( "BlDbRecordSet::BlDbRecordSet: Error en la consulta: " + SQLQuery, 2 );
        throw - 1;
    } // end try
    
}


/// Destructor de clase, antes de destruirse limpia los posibles buffers intermedios.
/**
**/
BlDbRecordSet::~BlDbRecordSet()
{
    BL_FUNC_DEBUG
    cerrar();
    
}


/// Limpia los buffers intermedios que puedan estar chupando memoria.
/**
**/
void BlDbRecordSet::cerrar()
{
    BL_FUNC_DEBUG
    PQclear ( result );
    
}


/// Devuelve el nmero de registros de la consulta.
/**
\return
**/
int BlDbRecordSet::numregistros()
{
    BL_FUNC_DEBUG
    
    return nregistros;
}


/// Devuelve el nmero de campos de la consulta.
///
/**
\return
**/
int BlDbRecordSet::numcampos()
{
    BL_FUNC_DEBUG
    
    return ncampos;
}


/// Devuelve el nombre del campo de una posicion determinada.
/// \param campo Posicion de la que se quiere que devuelva el nombre.
/**
\return
**/
QString BlDbRecordSet::fieldName ( int campo )
{
    BL_FUNC_DEBUG
    
    if ( campo >= 0 ) {
        return ( ( QString ) PQfname ( result, campo ) );
    } else {
        return "--Campo no soportado--";
    } // end if
}


/// Dado un campo esta funcion devuelve devuelve la posicion que le
/// corresponde, y si no le corresponde ninguno devuelve -1
/// \param campo Nombre del campo.
/// \return Devuelve la posicion si el nombre del campo existe, y si no devuelve -1
/// \todo Tal vez deberia crearse una estructura intermedia que indexe los nombres con
/// las posiciones para hacer la busqueda mas rapida, pero al ser el numero de registros
/// siempre muy reducido seguramente no arreglariamos nada de nada.
int BlDbRecordSet::numcampo ( const QString &campo )
{
    BL_FUNC_DEBUG
    int val = -1;
    if ( m_campos.contains ( campo ) ) {
        val =  m_campos.value ( campo );
    } // end if
    return val;
}


/// Esta funcion devuelve el tipo del campo posicion
/// \return un OID con el indice a la tabla pg_type (que puede considerarse una constante).
int BlDbRecordSet::fieldType ( int posicion )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::tipo", 0, QString::number ( posicion ) );
    
    return ( PQftype ( result, posicion ) );
}


/// Esta funcion devuelve el valor del campo posicion del registro
/// pasado, si se pasa -1 como registro se devuelve el registro actual
/// \param posicion El nmero de campo del que se quiere la posicion.
/// \param registro El registro del que se quiere devolver el campo.
/// Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
/// \return El valor de la posicion.
QString BlDbRecordSet::value( int posicion, int registro, bool localeformat )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::valor", 0, QString::number ( posicion ) + QString::number ( registro ) );

    QLocale locale;

    if ( registro == -1 ) {
        registro = registroactual;
    } // end if
    QString val = QString::fromUtf8 ( PQgetvalue ( result, registro, posicion ) );
    /// Si el campo es del tipo numeric y esmos con locales lo parseamos.
    if ( localeformat ) {
        if ( fieldType ( posicion ) == 1700 ) {
            /// La base de datos solo devuelve valores numericos con tipoDecimal el . y por eso solo tratamos este caso.
	    // Perdona que te haga estos cambios Aron. Pero estan probando el programa y da muchos fallos.
	    // Lo dejo medio apanyado y disculpa la intromision
	    // No podemos usar el tipo de datos double ya que es punto flotante y en nuestro
	    // caso debemos usar punto fijo (no implementado en C++. De ahi la clase BlFixed) para evitar errores de redondeo 
	    // y de precision.
            //val = locale.toString((locale.toDouble(val))); //ARON
	    // Esto mas o menos funciona
	    
	    /*
	    val.replace('.',locale.decimalPoint());
	    QString parteentera = val.left(val.indexOf(locale.decimalPoint()));
	    QString decimales = val.right(val.length()- val.indexOf(locale.decimalPoint()));
	    // Si lo podemos usar formateo de locales con numeros enteros.
	    QString val1 = locale.toString(locale.toInt(parteentera));
	    val = val1 + decimales;
	    */
	    // Hasta aqui
	    
        } // end if
    } // end if
    
    return ( val );
}


/// Esta funci&oacute;n devuelve el valor del campo especificado (por nombre) del registro
/// pasado, si se pasa como registro -1 se devuelve el registro actual.
/// \param campo Nombre del campo a devolver
/// \param registro El registro del que se quiere devolver el campo.
/// Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
/// \return El valor de la posici&oacute;n.
QString BlDbRecordSet::value( const QString &campo, int registro, bool localeformat )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::valor", 0, campo + " " + QString::number ( registro ) );
    int i = 0;
    if ( registro == -1 ) {
        registro = registroactual;
    } // end if
    i = numcampo ( campo );
    if ( i == -1 ) {
        
        return "";
    } // end if
    
    return value( i, registro, localeformat );
}

/// Esta funci&oacute;n devuelve el valor entero del campo posicion del registro
/// pasado, o siNull si el campo es null .
/// Si se pasa -1 como registro se devuelve el registro actual.
/// Si el campo no es entero no la llames, explotar&aacute;.
/// \param posicion El n&uacute;mero de campo del que se quiere la posicion.
/// \param registro El registro del que se quiere devolver el campo.
/// Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
/// \param siNull Valor a devolver cuando el campo este a nulo
/// \return El valor de la posicion.
int BlDbRecordSet::valorInt ( int posicion, int registro , int siNull )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::valor", 0, QString::number ( posicion ) + QString::number ( registro ) );
    if ( registro == -1 ) {
        registro = registroactual;
    } // end if

    if ( PQgetisnull ( result, registro, posicion ) ) {
        
        return siNull;
    }

    
    return ( atoi ( PQgetvalue ( result, registro, posicion  ) ) );
}

/// Esta funci&oacute;n devuelve el valor entero del campo especificado
/// (por nombre) del registro pasado, o siNull en caso que el campo sea null.
/// Si se pasa como registro -1 se devuelve el registro actual.
/// Si el campo no es entero no la llames, explotar&aacute;.
/// \param campo Nombre del campo a devolver
/// \param registro El registro del que se quiere devolver el campo.
/// Si vale -1 entonces se usa el recorrido  en forma de lista de campos para hacerlo.
/// \param siNull Valor a devolver cuando el campo este a nulo
/// \return El valor de la posici&oacute;n.
int BlDbRecordSet::valorInt ( const QString &campo, int registro , int siNull )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::valor", 0, campo + " " + QString::number ( registro ) );
    int i = 0;
    i = numcampo ( campo );
    if ( i == -1 ) {
        
        return siNull;
    } // end if

    
    return valorInt ( i, registro, siNull );
}

/// Devuelve la posici&oacute;n siguiente al registro que se est&aacute; recorriendo.
/**
\return
**/
int BlDbRecordSet::nextRecord()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::nextRecord", 0, "Registro actual: " + QString::number ( registroactual ) + " Numero de registros: " + QString::number ( nregistros ) );
    
    return ++registroactual;
}


/// Devuelve la posici&oacute;n anterior al registro que se est&aacute; recorriendo.
/**
\return
**/
int BlDbRecordSet::previousRecord()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::previousRecord", 0, "Registro actual: " + QString::number ( registroactual ) + " Numero de registros: " + QString::number ( nregistros ) );
    return --registroactual;
}


/// Devuelve la posici&oacute;n del primer registro de la tabla de registros.
/**
\return
**/
int BlDbRecordSet::firstRecord()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::firstRecord", 0, "Registro actual: " + QString::number ( registroactual ) + " Numero de registros: " + QString::number ( nregistros ) );
    registroactual = 0;
    
    return 0;
}


/// Devuelve el &uacute;ltimo registro de la tabla de registros.
/**
\return
**/
int BlDbRecordSet::lastRecord()
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlDbRecordSet::lastRecord", 0, "Registro actual: " + QString::number ( registroactual ) + " Numero de registros: " + QString::number ( nregistros ) );
    registroactual = nregistros - 1;
    
    return registroactual;
}


/// Devuelve true si el registro est&aacute; en la posici&oacute;n final, o si est&aacute; vacio.
/**
\return
**/
bool BlDbRecordSet::eof()
{
    BL_FUNC_DEBUG
    
    if (this == NULL) return true;
    
    bool result = false;
    if ( nregistros == 0 ) {
        result = true;
    } // end if
    result = registroactual >= nregistros;
    
    return result;
}


/// Devuelve true si el registro est&aacute; en la posici&oacute;n inicial, o si est&aacute; vacio.
/**
\return
**/
bool BlDbRecordSet::bof()
{
    BL_FUNC_DEBUG
    if ( nregistros == 0 ) {
        return ( true );
    } // end if
    
    return ( registroactual < 0 );
}


/// Devuelve true si es el &uacute;ltimo registro a considerar.
/**
\return
**/
bool BlDbRecordSet::isLastRecord()
{
    BL_FUNC_DEBUG
    
    return ( registroactual == nregistros - 1 );
}


/// devuelve true si es el primer registro a consear e el query.
/**
\return
**/
bool BlDbRecordSet::isFirstRecord()
{
    BL_FUNC_DEBUG
    
    return ( registroactual == 0 );
}


/// ----------------------------------------------------------------- ///


///
/**
\return
**/
QString BlPostgreSqlClient::dbName()
{
    BL_FUNC_DEBUG
    
    return m_pgDbName;
}


/// Constructor de la clase, no hace nada de nada de nada.
/**
**/
BlPostgreSqlClient::BlPostgreSqlClient()
{
    BL_FUNC_DEBUG
    conn = NULL;
}


/// Finaliza la conexi&oacute;n con la base de datos.
/**
**/
void BlPostgreSqlClient::terminar()
{
    BL_FUNC_DEBUG
    PQfinish ( conn );
    
}


/// Destructor de la clase que al igual que \ref terminar termina la conexi&oacute;n
/// con la base de datos.
/**
**/
BlPostgreSqlClient::~BlPostgreSqlClient()
{
    BL_FUNC_DEBUG
    /// close the connection to the database and cleanup.
    PQfinish ( conn );
    
}

// escapa una cadena per a posar-la a la cadena de connexió a PostgreSQL
QString & BlPostgreSqlClient::escCadConn(QString t) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlPostgreSqlClient::escCadConn", 0, t );
    
    return (t.isNull()? t : t.replace(QRegExp("([\\\'])"),"\\\\1"));
   
}
/// Inicializa la conexi&oacute;n con la base de datos mediante los par&aacute;metro
/// especificados. Precisamente no lo hace el constructor debido a la ausencia de
/// dichos datos.
/// \param nomdb Indica el nombre de la base de datos.
/// \param user Indica el usuario que hace la operacion a ojos de la base de datos.
/// \param passwd Indica la contrasenya que utiliza el usuario para autentificarse.
/// \return Si todo va bien devuelve 0, en caso contrario devuelve 1.
int BlPostgreSqlClient::inicializa ( QString nomdb )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlPostgreSqlClient::inicializa", 0, nomdb );
    m_pgDbName = nomdb;
    m_pgHost = g_confpr->value( CONF_SERVIDOR ); /// host name of the backend server.
    m_pgPort = g_confpr->value( CONF_PUERTO ); /// port of the backend server.
    m_pgOptions = ""; /// special options to start up the backend server.
    m_pgTty = ""; /// debugging tty for the backend server.
    QString conexion;

    QString user = g_confpr->value( CONF_LOGIN_USER );
    QString passwd = g_confpr->value( CONF_PASSWORD_USER );
    BlDebug::blDebug(" usuari "+user,0);
    try {
        /// Antes no resolvia bien en caso de querer hacer conexiones al ordenador local.
        /// Ahora si se pone -- se considera conexion local.
        if ( m_pgHost != "--" ) {
            conexion = "host = '" + escCadConn(m_pgHost)+"'";
        } // end if
        conexion += " port = '" + escCadConn(m_pgPort)+"'";
        conexion += " dbname = '" + escCadConn(m_pgDbName)+"'";
        if ( user != "" ) {
            conexion += " user = '" + escCadConn(user)+"'";
        } // end if
        if ( passwd != "" ) {
            conexion += " password = '" + escCadConn(passwd)+"'";
        } // end if

        BlDebug::blDebug ( conexion, 0 );
        conn = PQconnectdb ( conexion.toLatin1().data() );
        if ( PQstatus ( conn ) == CONNECTION_BAD ) {
            BlDebug::blDebug ( "La conexion con la base de datos '" + m_pgDbName + "' ha fallado.\n", 0 );
            if ( passwd != "" && g_confpr->valueTrue( CONF_ALERTAS_DB )) {
                BlDebug::blDebug ( PQerrorMessage ( conn ), 2 );
            } else {
                BlDebug::blDebug ( PQerrorMessage ( conn ), 0 );
            } // end if
            return 1;
        } // end if
        BlDebug::blDebug ( "La conexion con la base de datos ha ido bien, ahora vamos a por la fecha", 0 );
        formatofecha();

        /// Buscamos cual es el usuario ejecutando y lo almacenamos.
        BlDbRecordSet *rs = loadQuery ( "SELECT current_user" );
        if ( !rs->eof() ) {
            m_currentUser = rs->value( "current_user" );
        } else {
            m_currentUser = "";
        } // end if
        delete rs;
    } catch ( ... ) {
        BlDebug::blDebug ( "Error en la conexion postgresifcace2::inicializa", 2 );
    } // end try

    
    return 0;
}


/// Cambia el formato de fecha de la base de datos para que usemos la
/// fecha espayola dd/mm/yyyy
/// \return Devuelve 0 si no ha habido problemas, en caso contrario devuelve 1.
/**
**/
int BlPostgreSqlClient::formatofecha()
{
    BL_FUNC_DEBUG
    QString query = "";
    PGresult *res;
    query = "SET DATESTYLE TO SQL, European";
    res = PQexec ( conn, query.toLatin1().data() );
    if ( !res || PQresultStatus ( res ) != PGRES_COMMAND_OK ) {
        BlDebug::blDebug ( "Cambio del formato de fecha command failed" );
    } // end if
    PQclear ( res );

    /// Establecemos la codificacion por defecto a UNICODE.
    query = "SET client_encoding = 'UTF8'";
    res = PQexec ( conn, query.toLatin1().data() );
    if ( !res || PQresultStatus ( res ) != PGRES_COMMAND_OK ) {
        BlDebug::blDebug ( "Cambio del formato de codificacion" );
    } // end if
    PQclear ( res );

    /// Establecemos la zona horaria de Madrid
    query = "SET TIME ZONE  'Europe/Madrid'";
    res = PQexec ( conn, query.toLatin1().data() );
    if ( !res || PQresultStatus ( res ) != PGRES_COMMAND_OK ) {
        BlDebug::blDebug ( "Cambio de zona horaria" );
    } // end if
    PQclear ( res );

    
    return 0;
}


/// Con esta sentencia se inicia una transaccion en la base de datos
/// Las transacciones lo que indican es que el usuario se ha apoderado de la base de datos
/// durante un tiempo y que la operacion que va a transcurrir debe hacerse sin concurrencia.
/// \return Si todo ha funcionado bien devuelve un 0, en caso contrario devuelve un 1.
int BlPostgreSqlClient::begin()
{
    BL_FUNC_DEBUG
    /// Podemos hacer transacciones en cascada, en cuyo caso las previas son obviadas. Salvo en caso de hacer un rollback.
    /// Esto es debido a que un formulario puede solicitar el borrado de otro formulario y ambos harán begin. Interesa que si uno de los dos falla
    /// Se obvie toda la transaccion.
    if (g_num_begins == 0) {
      /// Es la primera transaccion y ponemos el commit a true, porque para empezar no habrá rollback.
      g_global_commit = true;
      BlDebug::blDebug("=============== BEGIN ==================");
      PGresult *res;
      res = PQexec ( conn, "BEGIN" );
      if ( !res || PQresultStatus ( res ) != PGRES_COMMAND_OK ) {
	  BlDebug::blDebug ( "BEGIN command failed" );
	  blMsgError("No se pudo iniciar la transaccion");
	  PQclear ( res );
	  g_num_begins = 0;
	  return -1;
      } // end if
    } else {
          BlDebug::blDebug("=============== BEGIN ANIDADO ==================");
    } // end if
    g_num_begins ++;
    
    // PQclear ( res );
    
    return ( 0 );
}


/// Con esta sentencia se termina un bloque de transaccion dando por buenos todos los
/// resultados que se han almacenado dentro. Y quedando almacenados en la base
/// de datos de forma definitiva.
/**
\return
**/
void BlPostgreSqlClient::commit()
{
    BL_FUNC_DEBUG
    g_num_begins--;
    if (g_num_begins == 0) {
	 if (g_global_commit) {
	      BlDebug::blDebug("=============== COMMIT ==================");
	      PGresult *res;
	      res = PQexec ( conn, "COMMIT" );
	      PQclear ( res );
	 } else {
	      BlDebug::blDebug("=============== ROLLBACK DENTRO DE TRANSACCION ANIDADA ==================");
	      PGresult *res;
	      res = PQexec ( conn, "ROLLBACK" );
	      PQclear ( res );
	 } // end if
    } // end if
}


/// Con esta sentencia se termina un bloque de transaccion dando por malos los resultados
/// de la operacion y dejandose la base de datos en el mismo estado que cuando se
/// inicio la transaccion.
/**
\return
**/
void BlPostgreSqlClient::rollback()
{
    BL_FUNC_DEBUG
    g_num_begins--;
    if ( g_num_begins == 0) {
	BlDebug::blDebug("=============== ROLLBACK ==================");
	PGresult *res;
	res = PQexec ( conn, "ROLLBACK" );
	PQclear ( res );
    } else {
	g_global_commit= false;
    } // end if
}


/// Se encarga de generar un objeto del tipo BlDbRecordSet y de iniciarlo con un query concreto
/// NOTA: Este metodo crea memoria, con lo que esta debe ser liberada posteriormente.
/// \return Devuelve un apuntador al objeto \ref BlDbRecordSet generado e inicializado con la
/// respuesta al query.
/**
\param Query La sentencia SELECT en formato SQL.
\param nomcursor Nombre que desea asignar a la consulta. Puede estar vacia.
**/
BlDbRecordSet *BlPostgreSqlClient::loadQuery ( QString query, QString nomcursor, int limit, int offset )
{
    return loadQuery ( query, 0, NULL, nomcursor, limit, offset );
}
/// carga el cursor con los valores de parámetros $1, $2 , $3 ... que se pasan como QString al final
// no funciona por ejemplo con "select 'precio = $2' where id = $1" porque $2 no es un parámetro
BlDbRecordSet *BlPostgreSqlClient::load( QString query,  
QString dolar1, QString dolar2, QString dolar3, QString dolar4, QString dolar5,
QString dolar6, QString dolar7, QString dolar8, QString dolar9, QString dolar10,
QString dolar11, QString dolar12, QString dolar13, QString dolar14, QString dolar15,
QString dolar16, QString dolar17, QString dolar18, QString dolar19, QString dolar20
 ) {
    QString params[20] = { dolar1 , dolar2, dolar3, dolar4 , dolar5, 
                           dolar6 , dolar7, dolar8, dolar9 , dolar10, 
                           dolar11 , dolar12, dolar13, dolar14 , dolar15, 
                           dolar16 , dolar17, dolar18, dolar19 , dolar20 };
    return loadQuery(query,numParams(query),params,"",0,0); 
}

int BlPostgreSqlClient::numParams(QString query) {
    QRegExp rx("\\$(\\d+)");
    int numParams=0;
    int pos = 0;
    // busco màxim número de paràmetre, per exemple a select $3, a from b where c=$1
    // seria 3 perquè m'han de passar 3 paràmetres encara que no faci servir el segon 
    while ((pos = rx.indexIn(query, pos)) != -1) {
      int param =  rx.cap(1).toInt();
      if (param > numParams) {
           numParams = param;
      }
      pos += rx.matchedLength();
    }
  return numParams;
}

const size_t digitsInt = 1 + int ( ceil ( log10 ( 1.0 + INT_MAX ) ) );

BlDbRecordSet *BlPostgreSqlClient::loadQuery ( QString query, int numParams,
        QString *paramValues, QString nomcursor,
        int limit, int offset )
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlPostgreSqlClient::loadQuery", 0, query );

    BlDbRecordSet *rs = NULL;
    /// Iniciamos la depuracion.
    try {
        QString pristineQuery = query;
        //digitsInt >= longitud expressio decimal d'un int positiu qualsevol
        int midaParams = numParams + ( limit != 0 ? 1 : 0 ) + ( offset != 0 ? 1 : 0 );
        QString newParamValues[midaParams];
        for ( int i = 0; i < numParams ; i++ ) {
            newParamValues[i] = paramValues[i];
        };
        /// Si hay establecidas clausulas limit o offset modificamos el query
        /// y anyadimos parametros
        if ( limit != 0 ) {
            newParamValues[numParams] =  QString::number ( limit );
            query += " LIMIT $" + QString::number ( ++numParams ) + "::int4";
        };
        if ( offset != 0 ) {
            newParamValues[numParams] =  QString::number ( offset );
            query += " OFFSET $" + QString::number ( ++numParams ) + "::int4";
        };

        rs = new BlDbRecordSet ( nomcursor, conn, query, numParams, newParamValues, pristineQuery );
        

    } catch ( ... ) {
        if ( rs ) delete rs;
	
        rs = NULL;
    } // end try
    return rs;
}


/// Ejecuta un comando SQL
/// \param Query Comando a ejecutar. Debe ser un comando que no devuelva ningun valor
/// (como \c select).
/// \retval 0 Si la ejecucion fue correcta
/// \retval 1 en caso contrario

#include <QtCore/QTextCodec>

///
/**
\param Query
**/
int BlPostgreSqlClient::run ( QString query,  
QString dolar1, QString dolar2, QString dolar3, QString dolar4, QString dolar5,
QString dolar6, QString dolar7, QString dolar8, QString dolar9, QString dolar10,
QString dolar11, QString dolar12, QString dolar13, QString dolar14, QString dolar15,
QString dolar16, QString dolar17, QString dolar18, QString dolar19, QString dolar20 ) {
   QString params[20] = { dolar1 , dolar2, dolar3, dolar4 , dolar5, 
                            dolar6 , dolar7, dolar8, dolar9 , dolar10, 
                            dolar11 , dolar12, dolar13, dolar14 , 
                            dolar15, dolar16 , dolar17, dolar18, 
                            dolar19 , dolar20 };

   run(query, numParams(query), params); 
   return 0;
}

int BlPostgreSqlClient::run ( QString query,  int numParams, QString params[])
{
    BL_FUNC_DEBUG
    const char *charValues[numParams];
    QByteArray qbaValues[numParams]; //si sabes C++ i Qt sabria si no cal ?
    for ( int i = 0; i < numParams ; i++ ) {
        BlDebug::blDebug(" param "+QString::number(i)+":"+params[i],0); 
        qbaValues[i] = params[i].toUtf8();
        charValues[i] = qbaValues[i].data();
    };
   /*
    for ( int i = 0; i < numParams ; i++ ) {
        BlDebug::blDebug(" param "+i+":"+params[i],0); 
        charValues[i] = params[i].toUtf8().constData();
    };
   */
   run(query, numParams, charValues);
   return 0;
}

int 
BlPostgreSqlClient::runQuery ( QString query ) {
   return run(query, 0, (const char * const *) NULL);
}

int BlPostgreSqlClient::run ( QString Query,  int numParams, const char * const * params)
{
    BL_FUNC_DEBUG
    BlDebug::blDebug ( "BlPostgreSqlClient::run", 0, Query );
    PGresult *result = NULL;
    try {
      
        if (!conn) {
	  throw -1;
	} // end if
      
        /// Prova de control de permisos.
        if ( g_confpr->value( CONF_PRIVILEGIOS_USUARIO ) != "1" && ( Query.left ( 6 ) == "DELETE" || Query.left ( 6 ) == "UPDATE" || Query.left ( 6 ) == "INSERT" ) )
            throw 42501;
	
	
        /// Nota: 42501 = INSUFFICIENT PRIVILEGE en SQL Standard.
        result=PQexecParams(  conn, ( const char * ) Query.toUtf8()  , 
                     numParams, NULL, params, NULL, NULL, 0);
        

	if ( !result )
            throw - 1;
        if ( PQresultStatus ( result ) != PGRES_COMMAND_OK && PQresultStatus ( result ) != 2 )
            throw - 1;

        PQclear ( result );
	
	
        
        return 0;
    } catch ( int e ) {
        if ( e == 42501 ) {
	    
            QString mensaje = "No tiene permisos suficientes para ejecutar el comando SQL:\n";
            BlErrorDialog ( mensaje + ( QString ) PQerrorMessage ( conn ), Query + "\n" + ( QString ) PQerrorMessage ( conn ) );
            PQclear ( result );
            throw - 1;
        } else {
	    
            QString mensaje = "Error al intentar modificar la base de datos:\n Codigo de error: " + QString::number ( PQresultStatus ( result ) ) + "\n";
            BlErrorDialog ( mensaje + ( QString ) PQerrorMessage ( conn ), Query + "\n" + ( QString ) PQerrorMessage ( conn ) );
            PQclear ( result );
            throw - 1;
        } // end if
    } catch ( ... ) {
	
        throw - 1;
    } // end try
}


/// This function search in the database the account parent of the account selected
/// if there are not parent returns NULL
/// else returns the code of the parent account.
/**
\param cod
\return
**/
QString BlPostgreSqlClient::searchParent ( QString cod )
{
    BL_FUNC_DEBUG
    QString padre = "NULL"; /// Almacena el padre de la cuenta.
    QString query;
    int i = 2;
    int fin = 0;
    while ( !fin ) {
        query = "SELECT * FROM cuenta WHERE codigo = '" + cod.left ( i ) + "'";
        begin();
        BlDbRecordSet *rs = loadQuery ( query, "unquery" );
        commit();
        if ( !rs->eof() ) {
            padre = rs->value( "codigo" );
        } else {
            fin = 1;
        } // end if
        delete rs;
        i++;
    } // end while
    
    return padre;
}


/// Esta funci&oacute;n est&eacute;tica devuelve una cadena "saneada" para pasarsela
/// a Postgresql. Neutraliza (escapes) los car&aacute;cteres problem&aacute;ticos por
/// ser car&aacute;cteres especiales de Postgresql. Ejemplo, comillas, barras invertidas, ...
/**
\param cadena
\return
**/
QString BlPostgreSqlClient::sanearCadena ( QString cadena )
{
    BL_FUNC_DEBUG
    int longitud = 0;
    char *buffer = NULL;
    QString cadenaLimpia = "";
    longitud = cadena.size();
    /// Reservamos (la funci&oacute;n de postgres lo necesita) un buffer del
    /// doble de car&aacute;cteres + 1 que la cadena original.
    buffer = ( char * ) malloc ( ( sizeof ( char ) * longitud * 2 ) + 1 );
    PQescapeString ( buffer, cadena.toLatin1().constData(), cadena.toLatin1().size() );
    cadenaLimpia = QString::fromLatin1 ( buffer );
    free ( buffer );
    
    return cadenaLimpia;
}

/// Esta funci&oacute;n est&eacute;tica devuelve una cadena "saneada" para pasarsela
/// a Postgresql. Neutraliza (escapes) los car&aacute;cteres problem&aacute;tics por
/// ser car&aacute;cteres especiales de Postgresql. Ejemplo, comillas, barras ivertidas, ...
/// Però respeta los caràcters no ascii o no en ISO-8859-1.
/**
\param cadena
\return
**/
QString BlPostgreSqlClient::sanearCadenaUtf8 ( QString cadena )
{
    BL_FUNC_DEBUG
    int longitud = 0;
    char *buffer = NULL;
    QString cadenaLimpia = "";
    QByteArray ba = cadena.toUtf8();
    longitud = ba.size();
    /// Reservamos (la funci&oacute;n de postgres lo necesita) un buffer del
    /// doble de car&aacute;cteres + 1 que la cadena original.
    buffer = ( char * ) malloc ( ( sizeof ( char ) * longitud * 2 ) + 1 );
    PQescapeStringConn ( conn, buffer, ba.constData(), longitud , 0 );
    cadenaLimpia = QString::fromUtf8 ( buffer );
    free ( buffer );
    
    return cadenaLimpia;
}


/// Devuelve el valor de una propiedad de la empresa
/// Las propiedades de la empresa son valores asociados a esta de la forma Nombre = Valor.
/// De esta forma se guardan datos como el nombre fiscal de la empresa, CIF, domicilio, etc.
/// \param nombre Nombre de la propiedad.
/// \return Valor de la propiedad.
QString BlPostgreSqlClient::propiedadempresa ( QString nombre )
{
    BL_FUNC_DEBUG
    PGresult *result;
    QString value;
    int num;
    QString Query = "select * from configuracion where nombre = '" + sanearCadenaUtf8(nombre) + "'";
    result = PQexec ( conn, Query.toLatin1().data() );
    if ( !result || PQresultStatus ( result ) != PGRES_TUPLES_OK ) {
#ifdef CONFIG_DEBUG
        fprintf ( stderr, "SQL command failed: %s\n", Query.toLatin1().data() );
        fprintf ( stderr, "%s\n", PQerrorMessage ( conn ) );
#endif
        PQclear ( result );
        return "";
    } // end if
    num = PQntuples ( result );
    if ( num > 1 ) {
#ifdef CONFIG_DEBUG
        fprintf ( stderr, "Aviso: Hay %d valores para el campo %s en la tabla configuracion\n", num, nombre.toLatin1().data() );
#endif
    } // end if
    if ( num == 0 ) {
        value = "";
    } else {
        value = PQgetvalue ( result, 0, 2 );
    } // end if
    PQclear ( result );
    
    return value;
}


/// Comprueba si el usuario actual tiene permisos para actuar sobre una tabla y
/// devuelve true o false.
/// \param table La tabla que se quiere consultar.
/// \param privilege El tipo de permiso "SELECT", "INSERT" o "UPDATE".
/// \return true si se tiene permiso, false si no se lo tiene.
bool BlPostgreSqlClient::hasTablePrivilege ( QString table, QString privilege )
{
    BL_FUNC_DEBUG
    /// Comprobamos que tengamos permisos para trabajar con articulos.
    BlDbRecordSet *rs = loadQuery ( "SELECT has_table_privilege('" + table + "', '" + privilege + "') AS pins" );
    bool hasPrivilege = false;
    if ( rs ) {
        if ( rs->value( "pins" ) == "t" ) {
            hasPrivilege = true;
        } // end if
        delete rs;
    } // end if
    
    return hasPrivilege;
}


/// Comprueba si existe alg&uacute;n registro en la tabla que cumpla cierta condici&oacute;n
/// \param table
/// \param cond Condición, por defecto ninguna (se comprueba si hay algún registro en la tabla)
/// \return true si existe algún registro
bool BlPostgreSqlClient::existsAnyRecord ( QString table, QString cond )
{
   BL_FUNC_DEBUG

   QString cons = QString ( "SELECT * FROM %1" )
            .arg ( table );

   /// Agregamos la condici&oacute;n si es que la hay
   if ( !cond.isEmpty() )
   {
    cons += QString ( " WHERE %1" )
            .arg ( cond );
   }

   /// Con un resultado positivo nos basta
   cons += " LIMIT 1";

   BlDbRecordSet rs = *loadQuery ( cons );

   
   return ( !rs.eof() );
}


/// Evaluacion de formulas usando la base de dato como motor de calculo
QString BlPostgreSqlClient::PGEval ( QString evalexp, int precision )
{
    QString res = "";
    /// Ninguna expresion numerica acepta comas.
    evalexp.replace ( ",", "." );
    QString query = "SELECT (" + evalexp + ")::NUMERIC(12," + QString::number ( precision ) + ") AS res";
    BlDbRecordSet *rs = loadQuery ( query );
    if ( rs ) {
        res = rs->value( "res" );
        delete rs;
    } // end if
    return res;
}

const QString BlPostgreSqlClient::currentUser()
{
    return m_currentUser;
}

