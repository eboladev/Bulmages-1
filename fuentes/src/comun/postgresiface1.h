/***************************************************************************
                          postgresiface1.h  -  description
                             -------------------
    begin                : Tue Jan 14 2003
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
#ifndef POSTGRESIFACE1_H
#define POSTGRESIFACE1_H

#include <stdio.h>

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

#include <stdio.h>
#include <string>
using std::string;
#include "configuracion.h"

/** *@author Tomeu Borrás Riera */

class cursor1 {
private:
   string nomcursor;
   PGresult *result;
   PGconn     *conn;
   int registroactual;
   int nregistros;
   int ncampos;
public:
   cursor1(string nombre, PGconn *conn1, string SQLQuery);
   ~cursor1();
   int numregistros();
   string valor(int posicion, int registro=-1);
   string valor(string campo, int registro=-1);
   int siguienteregistro();
   int registroanterior();
   int primerregistro();
   int ultimoregistro();
   void cerrar();
   string nomcampo(int);
   int numcampos();
   bool eof();
   bool bof();
   bool esultimoregistro();
   bool esprimerregistro();
};


class postgresiface1 {
private:
    string      pghost,
                pgport,
                pgoptions,
                pgtty;
    string       dbName;
    int         nFields;
    PGconn     *conn;
    int open; // indica si el postgresiface esta abierto o no.
public:
  postgresiface1();
  ~postgresiface1();
  int inicializa(char *);
  int begin();
  void commit();
  void rollback();
  cursor1 *cargacursor(string , string);
  int ejecuta(string);
  int nuevoborrador(int idcuenta, int idasiento, char *concepto, char *descripcion, float debe, float haber, char *fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal);
  int modificaborrador(int idborrador, int idcuenta, float idebe, float ihaber, char *concepto, char *fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal);
  cursor1 *cargacuenta(int idcuenta, char *ccuenta="");
  cursor1 *cargaasiento(int idasiento);
  cursor1 *cargaapuntes(int tidasiento);
  cursor1 *cargaborradores(int tidasiento);
  cursor1 *cargacuentas(int padre);
  cursor1 *cargagrupos();
  cursor1 *cargasaldoscuentafecha(int , char *);
  cursor1 *cargaapuntesctafecha(int , char *, char *);
  cursor1 *cargacuentascodigo(int , char *, char *);
  cursor1 *cargaasientosfecha(char *, char *);
  int cierraasiento(int idasiento);
  int borrarasiento(int idasiento);
  int borrarborrador(int idborrador);
  int abreasiento(int idasiento);
  int borrarcuenta(int idcuenta);
  int modificacuenta(int idcuenta, char *desccuenta, char *codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, char *, char *, char *, char *, char *, char *, char *, char *, int);
  int nuevacuenta(char *desccuenta, char *codigo, int padre, int idgrupo, char *, char *, char *, char *, char *, char *, char *, char *, int);
  int nuevoasiento(char *nombre, char *fecha, int numasiento=0);
  int cargaempresa(char *nomempresa, char *login, char *password);
  cursor1 *cargaempresas();

private:
  int formatofecha();

};

#endif
