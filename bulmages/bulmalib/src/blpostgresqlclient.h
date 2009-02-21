/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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

/// Fichero de definiciones de las clases 'BlDbRecordSet' y 'BlPostgreSqlClient'.
/// Definen la interficie de comunicacion con el sistema motor de bases de datos.
/// Estas clases se basan en la clase libpq.
#ifndef BLPOSTGRESQLCLIENT_H
#define BLPOSTGRESQLCLIENT_H

#include <QString>
#include <QHash>

#ifdef DISTRO_DEBIAN
#include <postgresql/libpq-fe.h>
#elif DISTRO_DEBIAN_8_0
#include <postgresql/8.0/libpq-fe.h>
#elif  DISTRO_RED_HAT
#include <pgsql/libpq-fe.h>
#elif  DISTRO_GENTOO
#include <postgresql/pgsql/libpq-fe.h>
#elif  DISTRO_NO_SE_QUE
#include <no_se_que/pgsql/libpq-fe.h>
#else
#include <libpq-fe.h>
#endif

#include "blconfiguration.h"
#include "blfunctions.h"


/// Esta clase provee toda la interacci&oacute;n necesaria para las consultas de base de datos.
/** Se carga normalmente a partir de la clase 'BlPostgreSqlClient' con el m&eacute;todo 'loadQuery'.
    Crea las estructuras necesarias para recorrer un recordset de postgres. */
class BlDbRecordSet
{
private:
    /// El nombre del cursor, (OBSOLETE).
    QString nomcursor;
    /// Estructura en la que se almacena el resultado.
    PGresult *result;
    /// Estructura en la que se almacenan los datos de conexi&oacute;n.
    PGconn *conn;
    /// Para recorrer una consulta (query) simulamos una cola, esta variable global indica el
    /// registro en que nos encontramos situados.
    int registroactual;
    /// Indica el n&uacute;mero de registros que tiene la consulta. (La componente vertical del query).
    int nregistros;
    /// Indica el n&uacute;mero de campos que tiene la consulta. (La componente horizontal del query).
    int ncampos;
    bool m_error;
    QString m_query;
    QString m_pristineQuery;

    QHash<QString, int> m_campos;
//      QList<QString, int>m_campos;
//    QMap<QString, int> m_campos;

    void inicializa ( QString nombre, PGconn *conn1, QString SQLQuery, int numParams,
                       const char * const *paramValues, QString pristineQuery=NULL
                       );
public:
    /// Constructor, inicializa la estructura y realiza la consulta.
    BlDbRecordSet ( QString nombre, PGconn *conn1, QString SQLQuery, QString pristineQuery=NULL );

    /**
     * Constructor para consultas parametrizadas (menos escapes y formateos, 
     * algo más de seguridad y dejamos puertas abiertas para futuras optimizaciones
     * con consultas precompiladas)
     * @param nombre nombre para el cursor
     * @param conn1 conexión de BD
     * @param SQLQuery sentencia SQL a enviar a base de datos. Contendrá tantos 
     *                  $1, $2, etc. como numParams. Pueden incluir sufijo con el
     *                   tipo de datos psql, como $1::int4, $2::varchar etc.
     * @param numParams número de parámetros ($) en la consulta y número de elementos de 
     *                  paramValues.
     * @param paramValues Valores para los parámetros. Sólo aceptamos formato texto
     *                    no el formato binario
     */
     BlDbRecordSet ( QString nombre, PGconn *conn1, QString SQLQuery, int numParams,
                       const QString *paramValues, QString pristineQuery=NULL
                       ) ;
  

    /**
     * Constructor para consultas parametrizadas (menos escapes y formateos, 
     * algo más de seguridad y dejamos puertas abiertas para futuras optimizaciones
     * con consultas precompiladas)
     * @param nombre nombre para el cursor
     * @param conn1 conexión de BD
     * @param SQLQuery sentencia SQL a enviar a base de datos. Contendrá tantos 
     *                  $1, $2, etc. como numParams. Pueden incluir sufijo con el
     *                   tipo de datos psql, como $1::int4, $2::varchar etc.
     * @param numParams número de parámetros ($) en la consulta y número de elementos de 
     *                  paramValues.
     * @param paramValues Valores para los parámetros. Sólo aceptamos formato texto
     *                    no el formato binario. Sólo los leemos, no los destruimos.
     */
    BlDbRecordSet ( QString nombre, PGconn *conn1, QString SQLQuery, int numParams,
                       const char *const * const paramValues, QString pristineQuery=NULL);


    /// Destructor, elimina la memoria ocupada.
    ~BlDbRecordSet();
    /// Devuelve el n&uacute;mero de registros del cursor.
    int numregistros();
    /// Devuelve el valor de una determinada posici&oacute;n del query.
    QString valor ( int posicion, int registro = -1, bool localeformat = FALSE );
	/// Devuelve el tipo de dato de una determinada posicion de un query
	int tipo(int posicion);
    bool error();
    QString query();
    QString pristineQuery();

    /// Esta funci&oacute;n devuelve el valor entero del campo posicion del registro
    /// pasado, o siNull si el campo es null .
    int valorInt ( int posicion, int registro = -1, int siNull = 0);

    /// Esta funci&oacute;n devuelve el valor entero del campo especificado 
    /// (por nombre) del registro pasado, o siNull en caso que el campo sea null.
    int valorInt ( const QString &campo, int registro=-1 , int siNull = 0);

public:
    /// Devuelve el valor de una determinada posici&oacute;n del query.
    QString valor ( const QString &campo, int registro = -1, bool localeformat = FALSE );
    /// Avanza el puntero de recorrido en forma de cola una posici&oacute;n.
    int siguienteregistro();
    /// Retrocede el puntero de recorrido en forma de cola una posici&oacute;n.
    int registroanterior();
    /// Establece la posici&oacute;n del puntero de recorrido de cola al primer registro.
    int primerregistro();
    /// Establece la posici&oacute;n del puntero de recorrido de cola en el &uacute;ltimo registro.
    int ultimoregistro();
    /// Devuelve la posici&oacute;n del registro actual.
    int regactual();
    /// Libera la memoria que se ha ocupado en las consultas.
    void cerrar();
    /// Devuelve el nombre del campo de una columna determinada de la consulta.
    QString nomcampo ( int );
    /// Devuelve el n&uacute;mero de columna que tiene una columna referenciada por el nombre.
    int numcampo ( const QString & );
    /// Devuelve el n&uacute;mero total de columnas que tiene la consulta.
    int numcampos();
    /// Indica que se ha alcanzado el primer registro de la consulta.
    bool eof();
    /// Indica que se ha alcanzado el &uacute;ltimo registro de la consulta.
    bool bof();
    /// Informa sobre si el registro actual es el &uacute;ltimo registro de la consulta.
    bool esultimoregistro();
    /// Informa sobre si el registro actual es el primer registro de la consulta.
    bool esprimerregistro();
};


class BlPostgreSqlClient
{
private:
    QString m_pgHost; /// Indica cual es el host de las bases de datos.
    QString m_pgPort; /// Indica por que puerto acceder a dicho host.
    QString m_pgOptions; /// Indica las opciones especiales que pueda tener la conexion.
    QString m_pgTty; /// Indica algunos par&aacute;metros de la conexi&oacute;n (Desgraciadamente no se cuales).
    QString m_pgDbName; /// Indica el nombre de la base de datos con la que se conecta.
    PGconn *conn; /// Representa la conexi&oacute;n.
    bool m_insideTransaction; /// Indica si estamos dentro de una transacci&oacute;n.
    QString m_currentUser;   /// Indica el usuario que se ha conectado.

private:
    /// Establece en la base de datos cual va a ser el formato de fecha a utilizar.
    int formatofecha();

public:
    /// Constructor de la clase que inicializa variables.
    BlPostgreSqlClient();
    /// Destructor de la clase que libera memoria.
    ~BlPostgreSqlClient();
    const QString currentUser();
    /// La clase debe ser inicializada con algunos par&aacute;metros para poder funcionar.
    int inicializa ( QString nomdb );
    /// Inicia una transacci&oacute;n.
    int begin();
    /// Termina una transacci&oacute;n.
    void commit();
    /// Termina una transacci&oacute;n de forma abrupta.
    void rollback();
    /// Carga el cursor con una consulta.
    BlDbRecordSet *loadQuery ( QString query, QString nomcursor = "", int limit = 0, int offset = 0 );
    BlDbRecordSet *loadQuery ( QString query, int numParams,
                       QString  * paramValues, QString nomcursor = "", 
                       int limit =0, int offset = 0 );

    /// Ejecuta una sentencia de c&oacute;digo SQL en la base de datos.
    int ejecuta ( QString );
    int nuevoborrador ( int idcuenta, int idasiento, QString concepto, QString descripcion, float debe, float haber, QString fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal );
    int modificaborrador ( int idborrador, int idcuenta, float idebe, float ihaber, QString concepto, QString fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal );
    BlDbRecordSet *cargacuenta ( int idcuenta, QString ccuenta = "" );
    BlDbRecordSet *cargaasiento ( int idasiento );
    BlDbRecordSet *cargaapuntes ( int tidasiento );
    BlDbRecordSet *cargaborradores ( int tidasiento );
    BlDbRecordSet *cargacuentas ( int padre );
    BlDbRecordSet *cargagrupos();
    BlDbRecordSet *cargasaldoscuentafecha ( int, QString );
    BlDbRecordSet *cargaapuntesctafecha ( int, QString, QString );
    BlDbRecordSet *cargacuentascodigo ( int, QString, QString );
    BlDbRecordSet *cargaasientosfecha ( QString, QString );
    int cierraasiento ( int idasiento );
    int borrarasiento ( int idasiento );
    int borrarborrador ( int idborrador );
    int abreasiento ( int idasiento );
    int borrarcuenta ( int idcuenta );
    int modificacuenta ( int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool );
    int nuevacuenta ( QString desccuenta, QString codigo, int padre, int idgrupo, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool );
    /// Carga en un query las empresas especificadas.
    BlDbRecordSet *cargaempresas();
    /// Busca en una cadena c&oacute;digo malicioso para SQL y lo elimina (previene el SQLInjection).
    static QString sanearCadena ( QString cadena );
    void terminar();
    QString propiedadempresa ( QString );
    /// Returns the parent of a determinated account code.
    QString searchParent ( QString );
    /// Returns the name of the database opened, if none as open then returns "".
    QString dbName();
    /// Returns table privileges of current user
    bool hasTablePrivilege ( QString table, QString privilege );
    /// Evalua expresiones con el motor de calculo de la base de datos
    QString PGEval(QString evalexp, int precision = 2 );
};

#endif

