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
#ifndef POSTGRESIFACE2_H
#define POSTGRESIFACE2_H

#include <qstring.h>

#ifdef DISTRO_DEBIAN
    #include <postgresql/libpq-fe.h>
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

/** *@author Tomeu Borrás Riera */

class cursor2 {
private:
   QString nomcursor;
   PGresult *result;
   PGconn     *conn;
   int registroactual;
   int nregistros;
   int ncampos;
public:
   cursor2(QString nombre, PGconn *conn1, QString SQLQuery);
   ~cursor2();
   int numregistros();
   QString valor(int posicion, int registro=-1);
   QString valor(QString campo, int registro=-1);
   int siguienteregistro();
   int registroanterior();
   int primerregistro();
   int ultimoregistro();
   void cerrar();
   QString nomcampo(int);
   int numcampo(QString);
   int numcampos();
   bool eof();
   bool bof();
   bool esultimoregistro();
   bool esprimerregistro();
};


class postgresiface2 {
private:
    QString      pghost,
                     pgport,
                     pgoptions,
                     pgtty;
    QString       dbName;
    int         nFields;
    PGconn     *conn;
    int open; // indica si el postgresiface esta abierto o no.
public:
  postgresiface2();
  ~postgresiface2();
  int inicializa(QString);
  int begin();
  void commit();
  void rollback();
  cursor2 *cargacursor(QString , QString);
  int ejecuta(QString);
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
  int modificacuenta(int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool);
  int nuevacuenta(QString desccuenta, QString codigo, int padre, int idgrupo, QString, QString, QString, QString, QString, QString, QString, QString, int, bool, bool);
  int nuevoasiento(QString nombre, QString fecha, int numasiento=0, int clase=1);
  int cargaempresa(QString nomempresa, QString login, QString password);
  cursor2 *cargaempresas();
  
  static QString sanearCadena(QString cadena);

private:
  int formatofecha();

};

#endif
