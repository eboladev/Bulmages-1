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
#include "bfempresa.h"
#define __VERSION_ACTUAL__ "bulmafact 0.0.1"

/*******************************************************************************/
/* Clase BfEmpresa                                                               */
/*******************************************************************************/

BfEmpresa::BfEmpresa() {
}


BfEmpresa::~BfEmpresa() {
}

BfCursor* BfEmpresa::pg_database(QString* usuario, QString* passwd) {
    QString query;
    pgIface* DbTemp;
    BfCursor* curTempVersion;
    BfCursor* curTempUsuario;
    BfCursor* curTempNombreEmpresa;
    pgIface* pg_template1 = new pgIface("template1");
    pg_template1->begin();
    pg_template1->ejecuta("CREATE LOCAL TEMPORARY TABLE misEmpresas (nombreDB varchar(20), nombreEmpresa varchar(50))");
    BfCursor* currTemplate1 = pg_template1->cargaCursor("SELECT datname FROM pg_database","listaEmpresas");
    pg_template1->commit();
    pg_template1->begin();
    while (!currTemplate1->eof()) {
        DbTemp = new pgIface(currTemplate1->valor(0));
        DbTemp->begin();
        curTempVersion = DbTemp->cargaCursor("SELECT valor FROM meta WHERE id='version'","buscaDB");
        query.sprintf("SELECT login, clave FROM usuarios WHERE login='%s' AND clave='%s'",usuario->ascii(),passwd->ascii());
        curTempUsuario = DbTemp->cargaCursor(query,"buscaUser");
        if (!curTempVersion->eof() && !curTempUsuario->eof()) {
            fprintf(stderr,"Encontrado Usuario: " +  curTempUsuario->valor(0) + "\n"); 
            if (curTempVersion->valor(0)==__VERSION_ACTUAL__)  {
                //fprintf(stderr," " +  currTemplate1->valor(0) + "\n"); 
                curTempNombreEmpresa = DbTemp->cargaCursor("SELECT valor FROM meta WHERE id='empresa'","buscaNombre");
                query.sprintf("INSERT INTO misEmpresas (nombreDB, nombreEmpresa) VALUES ('%s', '%s')",currTemplate1->valor(0).ascii(),curTempNombreEmpresa->valor(0).ascii());
                pg_template1->ejecuta(query);
                }
        }
        DbTemp->commit();
        currTemplate1->siguienteregistro();
    }
    BfCursor* lista = pg_template1->cargaCursor("SELECT * FROM misEmpresas","misEmpresas");
    pg_template1->commit();
    delete pg_template1;
    
    if (lista->eof()) return 0;
    else return lista;
}

/*******************************************************************************/
/* Clase BfCursor                                                                */
/*******************************************************************************/
BfCursor::BfCursor(QString nombre,PGconn *conn1, QString SQLQuery){
    conn = conn1;
    nomcursor = nombre;
    nregistros=0;
    registroactual=0;
    QString Query = "DECLARE "+nomcursor+" CURSOR FOR "+SQLQuery;
    result = PQexec(conn, Query.ascii());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
//      if (!result || PQresultStatus(result) == PGRES_FATAL_ERROR || PQresultStatus(result) == PGRES_NONFATAL_ERROR ) {
         fprintf(stderr,"%s\n", PQerrorMessage(conn));
         fprintf(stderr, "DECLARE CURSOR command failed\n");
         //QMessageBox::warning(NULL, "postgresiface","Error...","", "postgresiface","Ocurrió un error con la carga de un query de la base de datos\n"+Query+"\n"+PQerrorMessage(conn),"", "postgresiface","Aceptar","");
         PQclear(result);
         return;
      }// end if
    PQclear(result);
    QString fech = "FETCH ALL in "+nomcursor;
    result = PQexec(conn, fech.ascii());
    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        fprintf(stderr, "FETCH ALL command didn't return tuples properly\n");
        PQclear(result);
        return;
    } // end if
    nregistros = PQntuples(result);
    ncampos = PQnfields(result);
    registroactual=0;
}// end cursor

      
BfCursor::~BfCursor(){
    cerrar();
}

void BfCursor::cerrar() {
     QString cadena = "CLOSE "+ nomcursor;
     result = PQexec(conn, cadena);
     PQclear(result);
}// end cerrar
         
int BfCursor::numregistros(){
  return (nregistros);
}

int BfCursor::numcampos() {
  return(ncampos);
}

QString BfCursor::nomcampo(int campo) {
  return((QString) PQfname(result, campo));
}// end if

/**********************************************************************
 * Esta funcion devuelve el valor del campo posicion del registro
 * pasado, si se pasa -1 como registro se devuelve el registro actual
 **********************************************************************/   
QString BfCursor::valor(int posicion, int registro){
  if (registro == -1) {
   registro = registroactual;
  }// end if
  return (PQgetvalue(result, registro, posicion));
}// end valor
   
QString BfCursor::valor(QString campo, int registro){
  int i=0;
  if (registro == -1) {
   registro = registroactual;
  }// end if
  while (i<numcampos() && campo != nomcampo(i) ){
    i++;
  }// end while
  return(PQgetvalue(result, registro, i));
}// end valor
   
int BfCursor::siguienteregistro(){
     return (++registroactual);
}
   
int BfCursor::registroanterior(){
     return (--registroactual);
}
   
int BfCursor::primerregistro(){
     registroactual=0;
     return (0);
}
   
int BfCursor::ultimoregistro(){
     registroactual = nregistros-1;
     return (registroactual);
   }

bool BfCursor::eof() {
     if (nregistros == 0) {
       return(true);
     }// end if
     return(registroactual >= nregistros);
   }// end if

bool BfCursor::bof() {
     if (nregistros == 0) {
       return(true);
     }// end if
     return(registroactual < 0);
   }// end if

bool BfCursor::esultimoregistro() {
     return (registroactual == nregistros-1);
   }// end ultimoregistro
   
bool BfCursor::esprimerregistro() {
     return (registroactual == 0);
   }// end ultimoregistro


/*******************************************************************************/
/* Clase pgIface                                                               */
/*******************************************************************************/

pgIface::pgIface(QString nomdb){
    dbName=nomdb;
    pghost = "localhost";     /* host name of the backend server */
    pgport = "5432";          /* port of the backend server */
    pgoptions = "";           /* special options to start up the backend server */
    pgtty = "";               /* debugging tty for the backend server */
    QString conexion;

    if (pghost == "localhost") {
       conexion.sprintf("dbname=%s",dbName.ascii());
    } else {
       conexion.sprintf("hostaddr=%s port=%s dbname=%s",pghost.ascii(),pgport.ascii(), dbName.ascii());
    }// end if   
    fprintf(stderr,"%s\n",conexion.ascii());
    
    conn = PQconnectdb(conexion);
    if (PQstatus(conn) == CONNECTION_BAD)  {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.ascii());
        fprintf(stderr, "%s", PQerrorMessage(conn));
        //delete this;
    }// end if

}// end postgresiface

pgIface::~pgIface(){
      /* close the connection to the database and cleanup */
    PQfinish(conn);
}// end -postgresiface


/***********************************************************************
 * Cambia el formato de fecha de la base de datos para que usemos la
 * fecha española dd/mm/yyyy
 ***********************************************************************/
int pgIface::formatofecha() {
    QString query="";
    PGresult   *res;
    begin();
    query.sprintf("SET DateStyle TO 'SQL'");
    res = PQexec(conn, query.ascii());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DECLARE CURSOR command failed\n");
    }// end if
    PQclear(res);
    commit();
//    n=1;
    return(0);
}// end formatofecha

int pgIface::begin() {
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

void pgIface::commit() {
    PGresult   *res;
    fprintf(stderr,"-- COMMIT TRANSACTION --\n");
    res = PQexec(conn, "COMMIT");
    PQclear(res);
}// end commit

void pgIface::rollback() {
    PGresult   *res;
    res = PQexec(conn, "ROLLBACK");
    PQclear(res);
}// end rollback

BfCursor* pgIface::cargaCursor(QString Query, QString nomcursor) {
  fprintf(stderr,"%s\n",Query.ascii());
  BfCursor* cur=new BfCursor(nomcursor,conn,Query);
  return(cur);
}// end cargacursor

int pgIface::ejecuta(QString Query) {
    PGresult *result;
    fprintf(stderr,"%s\n",Query.ascii());
    //Prova de control de permisos     
    //if (confpr->valor(PRIVILEGIOS_USUARIO) != "1" && (Query.left(6)=="DELETE" || Query.left(6)=="UPDATE" || Query.left(6)=="INSERT")) return (42501);
    //Fi prova. Nota: 42501 = INSUFFICIENT PRIVILEGE en SQL Standard
    result = PQexec(conn, Query.ascii());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "SQL command failed: %s\n", Query.ascii());
        fprintf(stderr,"%s\n", PQerrorMessage(conn));
        //QMessageBox::warning(NULL, theApp->translate("postgresiface","Error...",""), theApp->translate("postgresiface","Ocurrió un error con la Base de Datos:\n"+Query+"\n"+PQerrorMessage(conn),""), theApp->translate("postgresiface","Aceptar",""));
        PQclear(result);
        return(1);
    }// end if
    PQclear(result);
    return(0);
}// end ejecuta

/*
int pgIface::inicializa(QString nomdb) {
    dbName=nomdb;
    pghost = "localhost";     // host name of the backend server 
    pgport = "5432";          // port of the backend server 
    pgoptions = "";           // special options to start up the backend server 
    pgtty = "";               // debugging tty for the backend server 
    QString conexion;

    if (pghost == "localhost") {
       conexion.sprintf("dbname=%s",dbName.ascii());
    } else {
       conexion.sprintf("hostaddr=%s port=%s dbname=%s",pghost.ascii(),pgport.ascii(), dbName.ascii());
    }// end if   
    fprintf(stderr,"%s\n",conexion.ascii());
    
    conn = PQconnectdb(conexion);
    if (PQstatus(conn) == CONNECTION_BAD)  {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.ascii());
        fprintf(stderr, "%s", PQerrorMessage(conn));
        return(1);
    }// end if
    formatofecha();
    return(0);
}// end inicializa
*/
