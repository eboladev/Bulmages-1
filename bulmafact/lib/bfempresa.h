/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/
#ifndef BFEMPRESA_H
#define BFEMPRESA_H

#ifdef DISTRO_DEBIAN
    #include <postgresql/libpq-fe.h>
#elif  DISTRO_RED_HAT
    #include <pgsql/libpq-fe.h>
#elif  DISTRO_GENTOO
    #include <postgresql/pgsql/libpq-fe.h>
#else
    #include <libpq-fe.h>
#endif

#include <math.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qtextedit.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qmessagebox.h>


class BClientes;
class BArticulos;
class BfCursor;

/** @author Josep Burcion */

/*******************************************************************************/
/* Clase BfEmpresa                                                             */
/*******************************************************************************/
class BfEmpresa{
public:
    BfEmpresa();
    ~BfEmpresa();
    void cargaEmpresa(QString*);
    BfCursor* pg_database(QString* usuario=0, QString* passwd=0);
    //Funciones relacionadas con clientes
    int nuevoCliente(QString* codigo=NULL);
    int eliminarCliente(QString*);
    int salvaCliente(BClientes*);
    int cargaCliente(BClientes*);
    //Funciones relacionadas con Articulos
    int nuevoArticulo(QString*);
    int eliminarArticulo(QString*);
    int salvaArticulo(BArticulos*);
    int cargaArticulo(BArticulos*);

    BfCursor* buscarParecidos(QString tabla, QString argumentoBusqueda="", QString opcionesBusqueda="ILIKE '%");
    QString fechaPresente();
    int roundI(double);
    
    //int ventanas;

private:
    QString * nombreDB;
    
};


/** @author Tomeu Borrás Riera */
/*******************************************************************************/
/* Clase cursor                                                                */
/*******************************************************************************/
class BfCursor {
private:
   QString nomcursor;
   PGresult *result;
   PGconn     *conn;
   int registroactual;
   int nregistros;
   int ncampos;
public:
   BfCursor(QString nombre, PGconn *conn1, QString SQLQuery);
   ~BfCursor();
   int numregistros();
   QString valor(int posicion, int registro=-1);
   QString valor(QString campo, int registro=-1);
   int siguienteregistro();
   int registroanterior();
   int primerregistro();
   int ultimoregistro();
   void cerrar();
   QString nomcampo(int);
   int numcampos();
   bool eof();
   bool bof();
   bool esultimoregistro();
   bool esprimerregistro();
};

/*******************************************************************************/
/* Clase pgIface                                                               */
/*******************************************************************************/
class pgIface {
private:
    QString      pghost,
                 pgport,
                 pgoptions,
                 pgtty;
    QString      dbName;
    int         nFields;
    PGconn     *conn;
    int open; // indica si el pgIface esta abierto o no.
public:
  pgIface(QString);
  ~pgIface();
  //int inicializa(QString);
  int begin();
  void commit();
  void rollback();
  BfCursor* cargaCursor(QString , QString);
  int ejecuta(QString);

private:
  int formatofecha();

};


#endif

