/***************************************************************************
                          postgresiface1.cpp  -  description
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

#include "postgresiface1.h"

cursor1::cursor1(string nombre,PGconn *conn1, string SQLQuery){
    conn = conn1;
    nomcursor = nombre;
    nregistros=0;
    registroactual=0;
    string Query = "DECLARE "+nomcursor+" CURSOR FOR "+SQLQuery;
    result = PQexec(conn, Query.c_str());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DECLARE CURSOR command failed\n");
        PQclear(result);
        return;
    }// end if
    PQclear(result);
    string fech = "FETCH ALL in "+nomcursor;
    result = PQexec(conn, fech.c_str());
    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "FETCH ALL command didn't return tuples properly\n");
        PQclear(result);
        return;
    } // end if
    nregistros = PQntuples(result);
    ncampos = PQnfields(result);
    registroactual=0;
}// end cursor1

      
cursor1::~cursor1(){
}

void cursor1::cerrar() {
     string cadena = "CLOSE "+ nomcursor;
     result = PQexec(conn, cadena.c_str());
     PQclear(result);
}// end cerrar
         
int cursor1::numregistros(){
  return (nregistros);
}

int cursor1::numcampos() {
  return(ncampos);
}

string cursor1::nomcampo(int campo) {
  return((string) PQfname(result, campo));
}// end if

/**********************************************************************
 * Esta funcion devuelve el valor del campo posicion del registro
 * pasado, si se pasa -1 como registro se devuelve el registro actual
 **********************************************************************/   
string cursor1::valor(int posicion, int registro){
  if (registro == -1) {
   registro = registroactual;
  }// end if
  return (PQgetvalue(result, registro, posicion));
}// end valor
   
string cursor1::valor(string campo, int registro){
  int i=0;
  if (registro == -1) {
   registro = registroactual;
  }// end if
  while (i<numcampos() && campo != nomcampo(i) ){
    i++;
  }// end while
  return(PQgetvalue(result, registro, i));
}// end valor
   
   int cursor1::siguienteregistro(){
     return (++registroactual);
   }
   
   int cursor1::registroanterior(){
     return (--registroactual);
   }
   
   int cursor1::primerregistro(){
     registroactual=0;
     return (0);
   }
   
   int cursor1::ultimoregistro(){
     registroactual = nregistros-1;
     return (registroactual);
   }

   bool cursor1::eof() {
     if (nregistros == 0) {
       return(true);
     }// end if
     return(registroactual >= nregistros);
   }// end if

   bool cursor1::bof() {
     if (nregistros == 0) {
       return(true);
     }// end if
     return(registroactual < 0);
   }// end if

   bool cursor1::esultimoregistro() {
     return (registroactual == nregistros-1);
   }// end ultimoregistro
   
   bool cursor1::esprimerregistro() {
     return (registroactual == 0);
   }// end ultimoregistro


// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------

postgresiface1::postgresiface1(){
}// end postgresiface1

postgresiface1::~postgresiface1(){
      /* close the connection to the database and cleanup */
    PQfinish(conn);
}// end -postgresiface1

int postgresiface1::inicializa(char *nomdb) {
    dbName=nomdb;
    pghost = confpr->valor(CONF_SERVIDOR);              /* host name of the backend server */
    pgport = confpr->valor(CONF_PUERTO);              /* port of the backend server */
    pgoptions = "";           /* special options to start up the backend server */
    pgtty = "";               /* debugging tty for the backend server */
    char conexion[300];

// Esto ya esta obsoleto, ahora dejamos la autentificacion al sistema
//    sprintf(conexion, "dbname=%s user=%s", dbName, "ubulmages");

    if (pghost == "localhost") {
       sprintf(conexion,"dbname=%s",dbName.c_str());
    } else {
       sprintf(conexion, "hostaddr=%s port=%s dbname=%s",pghost.c_str(),pgport.c_str(), dbName.c_str());
    }// end if   
    fprintf(stderr,"%s\n",conexion);

//    fprintf(stderr,"Inicializamos la nueva definicion de la base de datos\n");
//    fprintf(stderr,"-----------------------------------------------------\n");
//    conn = PQsetdb(pghost, pgport, pgoptions, pgtty, dbName);
    conn = PQconnectdb(conexion);
    if (PQstatus(conn) == CONNECTION_BAD)  {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.c_str());
        fprintf(stderr, "%s", PQerrorMessage(conn));
        return(1);
    }// end if
    formatofecha();
    return(0);
}// end inicializa


/***********************************************************************
 * Cambia el formato de fecha de la base de datos para que usemos la
 * fecha española dd/mm/yyyy
 ***********************************************************************/
int postgresiface1::formatofecha() {
    char query[400]="";
    PGresult   *res;
    begin();
    sprintf(query,"SET DateStyle TO 'SQL'");
    res = PQexec(conn, query);
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DECLARE CURSOR command failed\n");
    }// end if
    PQclear(res);
    commit();
//    n=1;
    return(0);
}// end formatofecha

int postgresiface1::begin() {
   PGresult   *res;
   res = PQexec(conn, "BEGIN");
   if (!res || PQresultStatus(res) != PGRES_COMMAND_OK)  {
       fprintf(stderr, "BEGIN command failed\n");
       PQclear(res);
       return (1);
   }// end if
   PQclear(res);
   return(0);
}// end begin

void postgresiface1::commit() {
    PGresult   *res;
    res = PQexec(conn, "COMMIT");
    PQclear(res);
}// end commit

void postgresiface1::rollback() {
    PGresult   *res;
    res = PQexec(conn, "ROLLBACK");
    PQclear(res);
}// end rollback

cursor1 *postgresiface1::cargacursor(string Query, string nomcursor) {
  cursor1 *cur=new cursor1(nomcursor,conn,Query);
  return(cur);
}// end cargacursor

int postgresiface1::ejecuta(string Query) {
    PGresult *result;
    result = PQexec(conn, Query.c_str());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "SQL command failed\n");
        PQclear(result);
        return(1);
    }// end if
    PQclear(result);
    return(0);
}// end ejecuta


int postgresiface1::nuevoborrador(int idcuenta, int idasiento, char *concepto, char *descripcion, float debe, float haber, char *fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal) {
    char query[400]="";
    char textcuenta[40];
    char textcontrapartida[40];
    char textidccoste[40];
    char textidcanal[30];
    if (idcuenta == 0) {
       strcpy(textcuenta,"NULL");
    } else {
       sprintf(textcuenta,"%d",idcuenta);
    }// endif
    if (idcontrapartida == 0) {
       strcpy(textcontrapartida,"NULL");
    } else {
       sprintf(textcontrapartida,"%d",idcontrapartida);
    }// endif
    if (idccoste == 0) {
      strcpy(textidccoste,"NULL");
    } else {
      sprintf(textidccoste,"%d",idccoste);
    }// end if
    if (idcanal == 0) {
      strcpy(textidcanal,"NULL");
    } else {
      sprintf(textidcanal,"%d",idcanal);
    }// end if
    sprintf(query,"INSERT INTO borrador (idcuenta,idasiento,conceptocontable, descripcion, debe, haber, fecha, contrapartida, idtipoiva, idc_coste, idcanal) VALUES (%s, %d,'%s','%s', %2.2f, %2.2f,'%s', %s, %d, %s, %s)",textcuenta, idasiento, concepto, descripcion, debe, haber, fecha, textcontrapartida, idtipoiva, textidccoste, textidcanal);
    fprintf(stderr,"%s\n",query);
    return(ejecuta(query));
}// end nuevoborrador


/***********************************************************************
 * Esta funcion se encarga de modificar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface1::modificaborrador(int idborrador, int idcuenta, float idebe, float ihaber, char *concepto, char *fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal) {
    char query[400]="";
    char textidccoste[40];
    char textcontrapartida[40];
    char textocanal[20];
    if (idccoste == 0) {
      strcpy(textidccoste,"NULL");
    } else {
      sprintf(textidccoste,"%d",idccoste);
    }// end if
    if (contrapartida == 0) {
      strcpy(textcontrapartida,"NULL");
    } else {
      sprintf(textcontrapartida,"%d",contrapartida);
    }// end if
    if (idcanal == 0) {
      strcpy(textocanal,"NULL");
    } else {
      sprintf(textocanal,"%d",idcanal);
    }// end if

    sprintf(query,"UPDATE borrador SET idcuenta=%d, debe=%2.2f, haber=%2.2f, conceptocontable='%s', fecha='%s', contrapartida=%s, idtipoiva=%d, idc_coste=%s, idcanal=%s WHERE idborrador=%d",idcuenta,idebe,ihaber,concepto,fecha,textcontrapartida,idtipoiva,textidccoste,textocanal, idborrador);
    fprintf(stderr,"%s\n",query);
    return(ejecuta(query));
}// end modificaborrador


/* **********************************************************
 * Esta funcion carga de la base de datos la cuenta especificada   *
 * si idcuenta != 0 se intenta cargar la cuenta con idcuenta especificada
 * si idcuenta == 0 se intenta cargar la cuenta con codigo especificado  *
 ************************************************************/
cursor1 *postgresiface1::cargacuenta(int idcuenta, char *ccuenta) {
    char query[400]="";
    if ( idcuenta != 0) {
       sprintf(query,"SELECT * FROM cuenta WHERE idcuenta=%d",idcuenta);
    }  else  {
       sprintf(query,"SELECT * FROM cuenta WHERE codigo LIKE '%s' ORDER BY codigo",ccuenta);
    }// end if
    cursor1 *cur=cargacursor(query,"cargacuenta");
    return(cur);
}// end cargacuenta


/* *****************************************************
 * Esta funcion carga de la base de datos el asiento con idasiento especificado
 *******************************************************/
cursor1 *postgresiface1::cargaasiento(int idasiento) {
    char query[400]="";
    sprintf(query,"SELECT * FROM asiento WHERE idasiento=%d",idasiento);
    cursor1 *cur=cargacursor(query,"cargaasiento");
    return(cur);
}// end cargaasiento


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * tidasiento --> El asiento del que se deben cargar los apuntes
 *****************************************************************/
cursor1 *postgresiface1::cargaapuntes(int tidasiento) {
    char query[400]="";
    sprintf(query,"SELECT * FROM apunte where idasiento=%d ORDER BY idapunte",tidasiento);
    cursor1 *cur=cargacursor(query,"cargaapuntes");
    return(cur);
}// end cargaapuntes


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * tidasiento --> El asiento del que se deben cargar los apuntes
 *****************************************************************/
cursor1 *postgresiface1::cargaborradores(int tidasiento) {
    char query[400]="";
    sprintf(query,"SELECT * FROM borrador where idasiento=%d ORDER BY idborrador",tidasiento);
    cursor1 *cur=cargacursor(query,"cargaborradores");
    return(cur);
}// end cargaborradores


/* *****************************************************
 * Esta funcion carga de la base de datos las cuentas
 * padre : = 0 carga las cuentas de nivel 0
 * padre : = -1 carga todas las cuentas sin excepcion
 * padre : else carga las cuentas cuyo padre es el valor especificado.
 */
cursor1 *postgresiface1::cargacuentas(int padre) {
    char query[400]="";
   if (padre!=0 && padre != -1) {
      sprintf(query,"SELECT * FROM cuenta WHERE padre=%d ORDER BY padre",padre);
    } else if (padre== 0) {
      sprintf(query,"SELECT * FROM cuenta WHERE padre isnull ORDER BY padre ");
    } else if (padre ==-1){
      sprintf(query,"SELECT * FROM cuenta ORDER BY padre ");
    } else if (padre ==-2){
      sprintf(query,"SELECT * FROM cuenta WHERE padre isnotnull ORDER BY padre ");
    }// end if
    cursor1 *cur=cargacursor(query,"cargaborradores");
    return(cur);
}



/* *****************************************************
 * Esta funcion carga de la base de datos los grupos
 *******************************************************/
cursor1 *postgresiface1::cargagrupos() {
    char query[400]="";
    sprintf(query,"SELECT * FROM grupo");
    cursor1 *cur=cargacursor(query,"cargagrupos");
    return(cur);
}// end cargagrupos


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * entre las fechas indicadas. Ambas fechas inclusive.
 *****************************************************************/
cursor1 *postgresiface1::cargaapuntesctafecha(int tidcuenta, char *fechainicial, char *fechafinal) {
    char query[400]="";
    sprintf(query,"SELECT * FROM apunte where idcuenta=%d AND fecha>='%s' AND fecha<='%s' ORDER BY fecha",tidcuenta, fechainicial, fechafinal);
    cursor1 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargaapuntesctafecha


/* **********************************************************
 * Esta funcion carga los saldos de una cuenta dada antes de la fecha
 * limite especificada. Los apuntes de la fecha no estan incluidos. *
 ************************************************************/
cursor1 *postgresiface1::cargasaldoscuentafecha(int idcuenta, char *fecha) {
    char query[400]="";
    sprintf(query,"SELECT sum(debe) as tdebe, sum(haber)as thaber FROM apunte WHERE idcuenta=%d AND fecha <'%s'",idcuenta, fecha);
    cursor1 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargasaldoscuentafecha


/* *****************************************************
 * Esta funcion carga de la base de datos los asientos
 *******************************************************/
cursor1 *postgresiface1::cargaasientosfecha(char *fechini, char *fechfin) {
    char query[400]="";
    sprintf(query,"SELECT * FROM asiento WHERE fecha >= '%s' AND fecha <= '%s' ORDER BY fecha", fechini,fechfin);
    cursor1 *cur=cargacursor(query,"cargaasientosfecha");
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
cursor1 *postgresiface1::cargacuentascodigo(int padre, char *codigoinicial, char *codigofinal) {
   char query[400]="";
   if (padre!=0 && padre != -1) {
      sprintf(query,"SELECT * FROM cuenta WHERE padre=%d AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",padre, codigoinicial, codigofinal);
   } else if (padre== 0) {
      sprintf(query,"SELECT * FROM cuenta WHERE padre isnull AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial, codigofinal);
   } else if (padre ==-1){
      sprintf(query,"SELECT * FROM cuenta WHERE codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial, codigofinal);
   }// end if
   cursor1 *cur=cargacursor(query,"cargasaldoscuentafecha");
   return(cur);
}// end cargacuentascodigo



int postgresiface1::cierraasiento(int idasiento) {
    char query[400]="";
    sprintf(query,"SELECT cierraasiento(%d)",idasiento);
    return(ejecuta(query));
}// end cierraasiento


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 * Si el asiento no esta vacio entonces no se borra.
 ***********************************************************************/
int postgresiface1::borrarasiento(int idasiento) {
    char query[400]="";
    sprintf(query,"DELETE FROM asiento WHERE idasiento=%d",idasiento);
    return(ejecuta(query));
}// end borrarasiento


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface1::borrarborrador(int idborrador) {
    char query[400]="";
    sprintf(query,"DELETE FROM borrador WHERE idborrador=%d",idborrador);
    return(ejecuta(query));
}// end borrarborrador


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface1::borrarcuenta(int idcuenta) {
    char query[400]="";
    sprintf(query,"DELETE FROM cuenta WHERE idcuenta=%d",idcuenta);
    return(ejecuta(query));
}// end borrarcuenta

/***********************************************************************
 * Esta funcion se encarga de abrir el asiento pasado como parametro
 * correspondiente.
 ***********************************************************************/
int postgresiface1::abreasiento(int idasiento) {
    char query[400]="";
    sprintf(query,"SELECT abreasiento(%d)",idasiento);
    return(ejecuta(query));
}// end abreasiento


int postgresiface1::modificacuenta(int idcuenta, char *desccuenta, char *codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, char *nombreent, char *cifent, char *dir, char*tel, char *comm, char *banco, char *email, char *web, int tipocuenta) {
   char cadena[300];
   sprintf(cadena,"%d",idcuenta);
    char query[400]="";
    char *imputacion;
    char * bloqueada = cbloqueada ? (char *) "TRUE" : (char *) "FALSE";
    char * activo = cactivo ? (char *) "TRUE" : (char *) "FALSE";
    imputacion = cimputacion ? (char *) "TRUE" : (char *) "FALSE";
    sprintf(query,"UPDATE cuenta SET descripcion='%s', codigo='%s', imputacion=%s, bloqueada=%s, idgrupo=%d, activo=%s, nombreent_cuenta='%s', cifent_cuenta='%s', dirent_cuenta='%s', telent_cuenta='%s', coment_cuenta='%s', bancoent_cuenta='%s', emailent_cuenta='%s', webent_cuenta='%s', tipocuenta=%d WHERE idcuenta=%d\n",desccuenta,codigo,imputacion,bloqueada,idgrupo,activo,nombreent, cifent,dir, tel, comm,banco, email, web,tipocuenta, idcuenta);
    fprintf(stderr,"%s\n",query);
    return(ejecuta(query));  
}// end modificacuenta


int postgresiface1::nuevacuenta(char *desccuenta, char *codigo, int padre, int idgrupo, char *nombreent, char *cifent, char *dir, char*tel, char *comm, char *banco, char * email, char *web, int tipocuenta) {
    char query[800]="";
    char tpadre[10];
    if (padre == 0) {
       strcpy(tpadre,"NULL");
    }  else  {
       sprintf(tpadre,"'%d'",padre);
    }// end if
    sprintf(query,"INSERT INTO cuenta (descripcion, padre,codigo, idgrupo, nombreent_cuenta, cifent_cuenta, dirent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta, tipocuenta) VALUES('%s',%s,'%s',%d, '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', %d)",desccuenta,tpadre, codigo, idgrupo, nombreent, cifent, dir, tel ,comm, banco, email, web, tipocuenta );
    fprintf(stderr,"%s\n",query);
    return(ejecuta(query));    
}// end nuevacuenta

int postgresiface1::nuevoasiento(char *nombre, char *fecha, int numasiento) {
    char query[400]="";
    int val;
    int ordenasiento;
    if (numasiento == 0) {
      sprintf(query,"SELECT max(idasiento) FROM asiento");
      cursor1 *cur=cargacursor(query,"cargaasientoseq");
      val = atoi(cur->valor(0).c_str());
      val++;
      delete cur;
    } else {
      val = numasiento;
    }// end if
    sprintf(query,"SELECT max(ordenasiento) FROM asiento");
    cursor1 *cur=cargacursor(query,"cargaasientos");
    ordenasiento = atoi(cur->valor(0).c_str());
    ordenasiento++;
    delete cur;

    sprintf(query,"INSERT INTO asiento (idasiento,descripcion, fecha, ordenasiento) VALUES (%d,'%s','%s', %d)",val,nombre, fecha, ordenasiento);
    ejecuta(query);
    return(val);
}// end nuevoasiento


cursor1 *postgresiface1::cargaempresas() {
   char query[400];
   sprintf(query, "SELECT * FROM empresa");
   cursor1 *cur=cargacursor(query,"cargaempresas");
   return(cur);
}// end cargaempresas


int postgresiface1::cargaempresa(char *nomempresa, char *login, char *password) {
    fprintf(stderr,"postgresiface1::cargaempresa\n");
    char query[400]="";
    PGresult   *res;
 /* make a connection to the database */
    char conexion[300];

    if (confpr->valor(CONF_SERVIDOR) == "localhost") {
       sprintf(conexion, "dbname = %s", (char *)confpr->valor(CONF_METABASE).c_str());
    } else {
       sprintf(conexion,"hostaddr=%s port=%s dbname=%s", (char *) confpr->valor(CONF_SERVIDOR).c_str(), (char *) confpr->valor(CONF_PUERTO).c_str(), (char *) confpr->valor(CONF_METABASE).c_str());
    }// end if

    conn = PQconnectdb(conexion);

    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.c_str());
        fprintf(stderr, "%s", PQerrorMessage(conn));
    }// end if
    begin();

    /*
     * fetch rows from the pg_database, the system catalog of
     * databases
     */
    sprintf(query,"DECLARE mycursor CURSOR FOR SELECT * FROM empresa, usuario, usuario_empresa where usuario.idusuario=usuario_empresa.idusuario AND empresa.idempresa=usuario_empresa.idempresa AND usuario.login='%s' AND empresa.nombredb='%s' AND usuario.password='%s'",login, nomempresa, password);
    res = PQexec(conn, query);
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

