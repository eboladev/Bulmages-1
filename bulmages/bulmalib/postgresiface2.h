/***************************************************************************
                          postgresiface2.h  -  description
                             -------------------
    begin                : Tue Nov 25 2003
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
 /** \file postgresiface2.h
   * Fichero de deficiniciones de las clases \ref cursor2 y \ref postgresiface2
   * los cuales definen la interficie de comunicación con el sistema motor de bases de datos
   * Estas clases se basan en la clase libpq.
   * \author Tomeu Borrás Riera
   */
#ifndef POSTGRESIFACE2_H
#define POSTGRESIFACE2_H

#include <qstring.h>

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

/** Esta clase provee toda la interacción necesaria con las consultas de base de datos
  * Se carga normalmente a partir de la clase \ref postgresiface2 con el método \ref cargacursor
  * Y crea las estructuras necesarias para recorrer un recordset de postgres
  */
class cursor2 {
private:
   /// El nombre del cursor, (OBSOLETE)
   QString nomcursor;
   /// Estructura en la que se almacena el resultado
   PGresult *result;
   /// Estructura en la que se almacenan los datos de conexión
   PGconn     *conn;
   /// Para recorrer un query simulamos una cola, esta variable global indica el registro en que nos encontramos situados.
   int registroactual;
   /// Indica el numero de registros que tiene el query. (La componente vertical del query)
   int nregistros;
   /// Indica el número de campos que tiene el query. (La componente horizontal del query).
   int ncampos;
   
public:
   /// Constructor, inicializa la estructura y realiza la consulta.
   cursor2(QString nombre, PGconn *conn1, QString SQLQuery);
   /// Destructor, elimina la memoria ocupada.
   ~cursor2();
   /// Devuelve el número de registros del cursor
   int numregistros();
   /// Devuelve el valor de una determinada posición del query
   QString valor(int posicion, int registro=-1);
public:
   /// Devuelve el valor de una determinada posición del query
   QString valor(QString campo, int registro=-1);
   /// Avanza el puntero de recorrido en forma de cola una posición
   int siguienteregistro();
   /// Retrocede el puntero de recorrido en forma de cola una posición
   int registroanterior();
   /// Establece la posición del puntero de recorrido de cola al primer registro
   int primerregistro();
   /// Establece la posición del puntero de recorrido de cola en el último registro
   int ultimoregistro();
   /// Devuelve la posición del registro actual
   int regactual () {return registroactual;};
   /// Libera la memoria que se ha ocupado en las consultas
   void cerrar();
   /// Devuelve el nombre del campo de una columna determinada del query
   QString nomcampo(int);
   /// Devuelve el número de columna que tiene una columna referenciada por el nombre
   int numcampo(QString);
   /// Devuelve el número total de columnas que tiene el query.
   int numcampos();
   /// Indica que se ha alcanzado el primer registro del query
   bool eof();
   /// Indica que se ha alcanzado el último registro del query
   bool bof();
   /// Informa sobre si el registro actual es el ultimo registro del query
   bool esultimoregistro();
   /// Informa sobre si el registro actual es el primer registro del query
   bool esprimerregistro();
};


class postgresiface2 {
private:
    QString      pghost,	/// Indica cual es el host de las bases de datos
                 pgport,	/// Indica por que puerto acceder a dicho host
                 pgoptions,	/// Indica las opciones especiales que pueda tener la conexión
                 pgtty;		/// Indica algunos parametros de la conexión (Desgraciadamente no se cuales)
    QString      dbName;	/// Indica el nombre de la base de datos con la que se conecta
    int          nFields;	/// Indica el número de campos que tiene algo (no se que)
    PGconn       *conn;		/// Representa la conexión.
    int          open; 		/// indica si el postgresiface esta abierto o no.
private:
  /// Establece en la base de datos cual va a ser el formato de fecha a utilizar.
  int formatofecha();
public:
  /// \brief Constructor de la clase que inicializa variables.
  postgresiface2();
  /// \brief Destructor de la clase que libera memoria.
  ~postgresiface2();
  /// \brief la clase debe ser inicializada con algunos parametros  para poder funcionar.
  int inicializa(QString nomdb);
  /// \brief Inicia una transacción.
  int begin();
  /// \brief Termina una transacción
  void commit();
  /// \brief Termina una transacción de forma abrupta
  void rollback();
  /// Carga el cursor con un query.
  cursor2 *cargacursor(QString Query, QString nomcursor="");
  /// Ejecuta una sentencia de código SQL en la base de datos
  int ejecuta(QString);
  /// 
  int nuevoborrador(int idcuenta, int idasiento, QString concepto, QString descripcion, float debe, float haber, QString fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal);
  int modificaborrador(int idborrador, int idcuenta, float idebe, float ihaber, QString concepto, QString fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal);
  cursor2 *cargacuenta(int idcuenta, QString ccuenta="");
  cursor2 *cargaasiento(int idasiento);
  cursor2 *cargaapuntes(int tidasiento);
  cursor2 *cargaborradores(int tidasiento);
  cursor2 *cargacuentas(int padre);
  cursor2 *cargagrupos();
  cursor2 *cargasaldoscuentafecha(int , QString);
  cursor2 *cargaapuntesctafecha(int , QString, QString);
  cursor2 *cargacuentascodigo(int , QString, QString);
  cursor2 *cargaasientosfecha(QString, QString);
  int cierraasiento(int idasiento);
  int borrarasiento(int idasiento);
  int borrarborrador(int idborrador);
  int abreasiento(int idasiento);
  int borrarcuenta(int idcuenta);
  int modificacuenta(int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool);
  int nuevacuenta(QString desccuenta, QString codigo, int padre, int idgrupo, QString, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool);
  int nuevoasiento(QString nombre, QString fecha, int numasiento=0, int clase=1);
  /// Hace la carga de una empresa determinada
  int cargaempresa(QString nomempresa, QString login, QString password);
  /// Carga en un query las empresas especificadas.
  cursor2 *cargaempresas();
  /// Busca en una cadena código malicioso para SQL y lo elimina (previene el SQLInjection)
  static QString sanearCadena(QString cadena);
  void terminar();
  QString propiedadempresa(QString );
  /// Returns the parent of a determinated account code.
  QString searchParent(QString);
  /// Returns the name of the database opened, if none as open then returns ""
  QString nameDB() {return dbName;};  
};

#endif
