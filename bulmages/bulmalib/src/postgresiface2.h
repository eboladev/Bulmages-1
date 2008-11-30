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

/// Fichero de definiciones de las clases 'cursor2' y 'postgresiface2'.
/// Definen la interficie de comunicacion con el sistema motor de bases de datos.
/// Estas clases se basan en la clase libpq.
#ifndef POSTGRESIFACE2_H
#define POSTGRESIFACE2_H

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

#include "configuracion.h"


/// Esta clase provee toda la interacci&oacute;n necesaria para las consultas de base de datos.
/** Se carga normalmente a partir de la clase 'postgresiface2' con el m&eacute;todo 'cargacursor'.
    Crea las estructuras necesarias para recorrer un recordset de postgres. */
class cursor2
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
    QHash<QString, int> m_campos;
//      QList<QString, int>m_campos;
//    QMap<QString, int> m_campos;
public:
    /// Constructor, inicializa la estructura y realiza la consulta.
    cursor2 ( QString nombre, PGconn *conn1, QString SQLQuery );
    /// Destructor, elimina la memoria ocupada.
    ~cursor2();
    /// Devuelve el n&uacute;mero de registros del cursor.
    int numregistros();
    /// Devuelve el valor de una determinada posici&oacute;n del query.
    QString valor ( int posicion, int registro = -1 );
    bool error();
    QString query();

public:
    /// Devuelve el valor de una determinada posici&oacute;n del query.
    QString valor ( const QString &campo, int registro = -1 );
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


class postgresiface2
{
private:
    QString pghost; /// Indica cual es el host de las bases de datos.
    QString pgport; /// Indica por que puerto acceder a dicho host.
    QString pgoptions; /// Indica las opciones especiales que pueda tener la conexion.
    QString pgtty; /// Indica algunos par&aacute;metros de la conexi&oacute;n (Desgraciadamente no se cuales).
    QString dbName; /// Indica el nombre de la base de datos con la que se conecta.
    int nFields; /// Indica el n&uacute;mero de campos que tiene algo (no se que).
    PGconn *conn; /// Representa la conexi&oacute;n.
    int open; /// Indica si el postgresiface esta abierto o no.
    bool m_transaccion; /// Indica si estamos dentro de una transacci&oacute;n.
    QString m_currentUser;   /// Indica el usuario que se ha conectado.

private:
    /// Establece en la base de datos cual va a ser el formato de fecha a utilizar.
    int formatofecha();

public:
    /// Constructor de la clase que inicializa variables.
    postgresiface2();
    /// Destructor de la clase que libera memoria.
    ~postgresiface2();
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
    cursor2 *cargacursor ( QString query, QString nomcursor = "", int limit = 0, int offset = 0 );
    /// Ejecuta una sentencia de c&oacute;digo SQL en la base de datos.
    int ejecuta ( QString );
    int nuevoborrador ( int idcuenta, int idasiento, QString concepto, QString descripcion, float debe, float haber, QString fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal );
    int modificaborrador ( int idborrador, int idcuenta, float idebe, float ihaber, QString concepto, QString fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal );
    cursor2 *cargacuenta ( int idcuenta, QString ccuenta = "" );
    cursor2 *cargaasiento ( int idasiento );
    cursor2 *cargaapuntes ( int tidasiento );
    cursor2 *cargaborradores ( int tidasiento );
    cursor2 *cargacuentas ( int padre );
    cursor2 *cargagrupos();
    cursor2 *cargasaldoscuentafecha ( int, QString );
    cursor2 *cargaapuntesctafecha ( int, QString, QString );
    cursor2 *cargacuentascodigo ( int, QString, QString );
    cursor2 *cargaasientosfecha ( QString, QString );
    int cierraasiento ( int idasiento );
    int borrarasiento ( int idasiento );
    int borrarborrador ( int idborrador );
    int abreasiento ( int idasiento );
    int borrarcuenta ( int idcuenta );
    int modificacuenta ( int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool );
    int nuevacuenta ( QString desccuenta, QString codigo, int padre, int idgrupo, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool );
    /// Carga en un query las empresas especificadas.
    cursor2 *cargaempresas();
    /// Busca en una cadena c&oacute;digo malicioso para SQL y lo elimina (previene el SQLInjection).
    static QString sanearCadena ( QString cadena );
    void terminar();
    QString propiedadempresa ( QString );
    /// Returns the parent of a determinated account code.
    QString searchParent ( QString );
    /// Returns the name of the database opened, if none as open then returns "".
    QString nameDB();
    /// Returns table privileges of current user
    bool has_table_privilege ( QString tabla, QString permiso );
    /// Evalua expresiones con el motor de calculo de la base de datos
    QString PGEval(QString evalexp, int precision = 2 );
};

#endif

