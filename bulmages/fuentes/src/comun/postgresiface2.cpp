/***************************************************************************
                          postgresiface2.cpp  -  description
                             -------------------
    begin                : Tue Nov 26 2003
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

#include "postgresiface2.h"

cursor2::cursor2(QString nombre,PGconn *conn1, QString SQLQuery){
    conn = conn1;
    nomcursor = nombre;
    nregistros=0;
    registroactual=0;
    QString Query = "DECLARE "+nomcursor+" CURSOR FOR "+SQLQuery;
    result = PQexec(conn, Query.latin1());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DECLARE CURSOR command failed\n");
        PQclear(result);
        return;
    }// end if
    PQclear(result);
    QString fech = "FETCH ALL in "+nomcursor;
    result = PQexec(conn, fech.latin1());
    if (!result || PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "FETCH ALL command didn't return tuples properly\n");
        PQclear(result);
        return;
    } // end if
    nregistros = PQntuples(result);
    ncampos = PQnfields(result);
    registroactual=0;
}// end cursor2

      
cursor2::~cursor2(){
}

void cursor2::cerrar() {
     QString cadena = "CLOSE "+ nomcursor;
     result = PQexec(conn, cadena);
     PQclear(result);
}// end cerrar
         
int cursor2::numregistros(){
  return (nregistros);
}

int cursor2::numcampos() {
  return(ncampos);
}

QString cursor2::nomcampo(int campo) {
  return((QString) PQfname(result, campo));
}// end if

/**********************************************************************
 * Esta funcion devuelve el valor del campo posicion del registro
 * pasado, si se pasa -1 como registro se devuelve el registro actual
 **********************************************************************/   
QString cursor2::valor(int posicion, int registro){
  if (registro == -1) {
   registro = registroactual;
  }// end if
  return (PQgetvalue(result, registro, posicion));
}// end valor
   
QString cursor2::valor(QString campo, int registro){
  int i=0;
  if (registro == -1) {
   registro = registroactual;
  }// end if
  while (i<numcampos() && campo != nomcampo(i) ){
    i++;
  }// end while
  return(PQgetvalue(result, registro, i));
}// end valor
   
   int cursor2::siguienteregistro(){
     return (++registroactual);
   }
   
   int cursor2::registroanterior(){
     return (--registroactual);
   }
   
   int cursor2::primerregistro(){
     registroactual=0;
     return (0);
   }
   
   int cursor2::ultimoregistro(){
     registroactual = nregistros-1;
     return (registroactual);
   }

   bool cursor2::eof() {
     if (nregistros == 0) {
       return(true);
     }// end if
     return(registroactual >= nregistros);
   }// end if

   bool cursor2::bof() {
     if (nregistros == 0) {
       return(true);
     }// end if
     return(registroactual < 0);
   }// end if

   bool cursor2::esultimoregistro() {
     return (registroactual == nregistros-1);
   }// end ultimoregistro
   
   bool cursor2::esprimerregistro() {
     return (registroactual == 0);
   }// end ultimoregistro


// -----------------------------------------------------------------
// -----------------------------------------------------------------
// -----------------------------------------------------------------

postgresiface2::postgresiface2(){
}// end postgresiface2

postgresiface2::~postgresiface2(){
      /* close the connection to the database and cleanup */
    PQfinish(conn);
}// end -postgresiface2

int postgresiface2::inicializa(QString nomdb) {
    dbName=nomdb;
    pghost = confpr->valor(CONF_SERVIDOR).c_str();              /* host name of the backend server */
    pgport = confpr->valor(CONF_PUERTO).c_str();              /* port of the backend server */
    pgoptions = "";           /* special options to start up the backend server */
    pgtty = "";               /* debugging tty for the backend server */
    QString conexion;

// Esto ya esta obsoleto, ahora dejamos la autentificacion al sistema
//    sprintf(conexion, "dbname=%s user=%s", dbName, "ubulmages");

    if (pghost == "localhost") {
       conexion.sprintf("dbname=%s",dbName.latin1());
    } else {
       conexion.sprintf("hostaddr=%s port=%s dbname=%s",pghost.latin1(),pgport.latin1(), dbName.latin1());
    }// end if   
    fprintf(stderr,"%s\n",conexion.latin1());

//    fprintf(stderr,"Inicializamos la nueva definicion de la base de datos\n");
//    fprintf(stderr,"-----------------------------------------------------\n");
//    conn = PQsetdb(pghost, pgport, pgoptions, pgtty, dbName);
    conn = PQconnectdb(conexion);
    if (PQstatus(conn) == CONNECTION_BAD)  {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.latin1());
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
int postgresiface2::formatofecha() {
    QString query="";
    PGresult   *res;
    begin();
    query.sprintf("SET DateStyle TO 'SQL'");
    res = PQexec(conn, query.latin1());
    if (!res || PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "DECLARE CURSOR command failed\n");
    }// end if
    PQclear(res);
    commit();
//    n=1;
    return(0);
}// end formatofecha

int postgresiface2::begin() {
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

void postgresiface2::commit() {
    PGresult   *res;
    res = PQexec(conn, "COMMIT");
    PQclear(res);
}// end commit

void postgresiface2::rollback() {
    PGresult   *res;
    res = PQexec(conn, "ROLLBACK");
    PQclear(res);
}// end rollback

cursor2 *postgresiface2::cargacursor(QString Query, QString nomcursor) {
	fprintf(stderr,"%s\n",Query.latin1());
  cursor2 *cur=new cursor2(nomcursor,conn,Query);
  return(cur);
}// end cargacursor

int postgresiface2::ejecuta(QString Query) {
    PGresult *result;
	 fprintf(stderr,"%s\n",Query.latin1());
    result = PQexec(conn, Query.latin1());
    if (!result || PQresultStatus(result) != PGRES_COMMAND_OK) {
        fprintf(stderr, "SQL command failed: %s\n", Query.latin1());
        PQclear(result);
        return(1);
    }// end if
    PQclear(result);
    return(0);
}// end ejecuta


int postgresiface2::nuevoborrador(int idcuenta, int idasiento, QString concepto, QString descripcion, float debe, float haber, QString fecha, int idcontrapartida, int idtipoiva, int idccoste, int idcanal) {
    QString query="";
    QString textcuenta;
    QString textcontrapartida;
    QString textidccoste;
    QString textidcanal;
    if (idcuenta == 0) {
       textcuenta="NULL";
    } else {
       textcuenta.sprintf("%d",idcuenta);
    }// endif
    if (idcontrapartida == 0) {
       textcontrapartida="NULL";
    } else {
       textcontrapartida.sprintf("%d",idcontrapartida);
    }// endif
    if (idccoste == 0) {
      textidccoste="NULL";
    } else {
      textidccoste.sprintf("%d",idccoste);
    }// end if
    if (idcanal == 0) {
      textidcanal="NULL";
    } else {
      textidcanal.sprintf("%d",idcanal);
    }// end if
    query.sprintf("INSERT INTO borrador (idcuenta,idasiento,conceptocontable, descripcion, debe, haber, fecha, contrapartida, idtipoiva, idc_coste, idcanal) VALUES (%s, %d,'%s','%s', %2.2f, %2.2f,'%s', %s, %d, %s, %s)",textcuenta.latin1(), idasiento, concepto.latin1(), descripcion.latin1(), debe, haber, fecha.latin1(), textcontrapartida.latin1(), idtipoiva, textidccoste.latin1(), textidcanal.latin1());
    fprintf(stderr,"%s\n",query.latin1());
    return(ejecuta(query));
}// end nuevoborrador


/***********************************************************************
 * Esta funcion se encarga de modificar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface2::modificaborrador(int idborrador, int idcuenta, float idebe, float ihaber, QString concepto, QString fecha, int contrapartida, int idtipoiva, int idccoste, int idcanal) {
    QString query="";
    QString textidccoste;
    QString textcontrapartida;
    QString textocanal;
    if (idccoste == 0) {
      textidccoste="NULL";
    } else {
      textidccoste.sprintf("%d",idccoste);
    }// end if
    if (contrapartida == 0) {
      textcontrapartida="NULL";
    } else {
      textcontrapartida.sprintf("%d",contrapartida);
    }// end if
    if (idcanal == 0) {
      textocanal="NULL";
    } else {
      textocanal.sprintf("%d",idcanal);
    }// end if

    query.sprintf("UPDATE borrador SET idcuenta=%d, debe=%2.2f, haber=%2.2f, conceptocontable='%s', fecha='%s', contrapartida=%s, idtipoiva=%d, idc_coste=%s, idcanal=%s WHERE idborrador=%d",idcuenta,idebe,ihaber,concepto.latin1(),fecha.latin1(),textcontrapartida.latin1(),idtipoiva,textidccoste.latin1(),textocanal.latin1(), idborrador);
    fprintf(stderr,"%s\n",query.latin1());
    return(ejecuta(query));
}// end modificaborrador


/* **********************************************************
 * Esta funcion carga de la base de datos la cuenta especificada   *
 * si idcuenta != 0 se intenta cargar la cuenta con idcuenta especificada
 * si idcuenta == 0 se intenta cargar la cuenta con codigo especificado  *
 ************************************************************/
cursor2 *postgresiface2::cargacuenta(int idcuenta, QString ccuenta) {
    QString query="";
    if ( idcuenta != 0) {
       query.sprintf("SELECT * FROM cuenta WHERE idcuenta=%d",idcuenta);
    }  else  {
       query.sprintf("SELECT * FROM cuenta WHERE codigo LIKE '%s' ORDER BY codigo",ccuenta.latin1());
    }// end if
    cursor2 *cur=cargacursor(query,"cargacuenta");
    return(cur);
}// end cargacuenta


/* *****************************************************
 * Esta funcion carga de la base de datos el asiento con idasiento especificado
 *******************************************************/
cursor2 *postgresiface2::cargaasiento(int idasiento) {
    QString query="";
    query.sprintf("SELECT * FROM asiento WHERE idasiento=%d",idasiento);
    cursor2 *cur=cargacursor(query,"cargaasiento");
    return(cur);
}// end cargaasiento


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * tidasiento --> El asiento del que se deben cargar los apuntes
 *****************************************************************/
cursor2 *postgresiface2::cargaapuntes(int tidasiento) {
    QString query="";
    query.sprintf("SELECT * FROM apunte where idasiento=%d ORDER BY idapunte",tidasiento);
    cursor2 *cur=cargacursor(query,"cargaapuntes");
    return(cur);
}// end cargaapuntes


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * tidasiento --> El asiento del que se deben cargar los apuntes
 *****************************************************************/
cursor2 *postgresiface2::cargaborradores(int tidasiento) {
    QString query="";
    query.sprintf("SELECT * FROM borrador where idasiento=%d ORDER BY idborrador",tidasiento);
    cursor2 *cur=cargacursor(query,"cargaborradores");
    return(cur);
}// end cargaborradores


/* *****************************************************
 * Esta funcion carga de la base de datos las cuentas
 * padre : = 0 carga las cuentas de nivel 0
 * padre : = -1 carga todas las cuentas sin excepcion
 * padre : else carga las cuentas cuyo padre es el valor especificado.
 */
cursor2 *postgresiface2::cargacuentas(int padre) {
    QString query="";
   if (padre!=0 && padre != -1 && padre!=-2) {
      query.sprintf("SELECT * FROM cuenta WHERE padre=%d ORDER BY padre",padre);
    } else if (padre== 0) {
      query.sprintf("SELECT * FROM cuenta WHERE padre isnull ORDER BY padre ");
    } else if (padre ==-1){
      query.sprintf("SELECT * FROM cuenta ORDER BY padre ");
    } else if (padre ==-2){
      query.sprintf("SELECT * FROM cuenta WHERE NOT padre isnull ORDER BY padre ");
    }// end if
    cursor2 *cur=cargacursor(query,"cargaborradores");
    return(cur);
}



/* *****************************************************
 * Esta funcion carga de la base de datos los grupos
 *******************************************************/
cursor2 *postgresiface2::cargagrupos() {
    QString query="SELECT * FROM grupo";
    cursor2 *cur=cargacursor(query,"cargagrupos");
    return(cur);
}// end cargagrupos


/* ***************************************************************
 * Esta funcion carga de la base de datos los apuntes
 * entre las fechas indicadas. Ambas fechas inclusive.
 *****************************************************************/
cursor2 *postgresiface2::cargaapuntesctafecha(int tidcuenta, QString fechainicial, QString fechafinal) {
    QString query="";
    query.sprintf("SELECT * FROM apunte where idcuenta=%d AND fecha>='%s' AND fecha<='%s' ORDER BY fecha",tidcuenta, fechainicial.latin1(), fechafinal.latin1());
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargaapuntesctafecha


/* **********************************************************
 * Esta funcion carga los saldos de una cuenta dada antes de la fecha
 * limite especificada. Los apuntes de la fecha no estan incluidos. *
 ************************************************************/
cursor2 *postgresiface2::cargasaldoscuentafecha(int idcuenta, QString fecha) {
    QString query="";
    query.sprintf("SELECT sum(debe) as tdebe, sum(haber)as thaber FROM apunte WHERE idcuenta=%d AND fecha <'%s'",idcuenta, fecha.latin1());
    cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
    return(cur);
}// end cargasaldoscuentafecha


/* *****************************************************
 * Esta funcion carga de la base de datos los asientos
 *******************************************************/
cursor2 *postgresiface2::cargaasientosfecha(QString fechini, QString fechfin) {
    QString query="";
    query.sprintf("SELECT * FROM asiento WHERE fecha >= '%s' AND fecha <= '%s' ORDER BY fecha", fechini.latin1(),fechfin.latin1());
    cursor2 *cur=cargacursor(query,"cargaasientosfecha");
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
cursor2 *postgresiface2::cargacuentascodigo(int padre, QString codigoinicial, QString codigofinal) {
   QString query="";
   if (padre!=0 && padre != -1) {
      query.sprintf("SELECT * FROM cuenta WHERE padre=%d AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",padre, codigoinicial.latin1(), codigofinal.latin1());
   } else if (padre== 0) {
      query.sprintf("SELECT * FROM cuenta WHERE padre isnull AND codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial.latin1(), codigofinal.latin1());
   } else if (padre ==-1){
      query.sprintf("SELECT * FROM cuenta WHERE codigo>='%s' AND codigo <='%s' ORDER BY codigo",codigoinicial.latin1(), codigofinal.latin1());
   }// end if
   cursor2 *cur=cargacursor(query,"cargasaldoscuentafecha");
   return(cur);
}// end cargacuentascodigo



int postgresiface2::cierraasiento(int idasiento) {
    QString query="";
    query.sprintf("SELECT cierraasiento(%d)",idasiento);
    return(ejecuta(query));
}// end cierraasiento


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 * Si el asiento no esta vacio entonces no se borra.
 ***********************************************************************/
int postgresiface2::borrarasiento(int idasiento) {
    QString query="";
    query.sprintf("DELETE FROM asiento WHERE idasiento=%d",idasiento);
    return(ejecuta(query));
}// end borrarasiento


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface2::borrarborrador(int idborrador) {
    QString query="";
    query.sprintf("DELETE FROM borrador WHERE idborrador=%d",idborrador);
    return(ejecuta(query));
}// end borrarborrador


/***********************************************************************
 * Esta funcion se encarga de borrar el apunte con el identificador
 * correspondiente.
 ***********************************************************************/
int postgresiface2::borrarcuenta(int idcuenta) {
    QString query="";
    query.sprintf("DELETE FROM cuenta WHERE idcuenta=%d",idcuenta);
    return(ejecuta(query));
}// end borrarcuenta

/***********************************************************************
 * Esta funcion se encarga de abrir el asiento pasado como parametro
 * correspondiente.
 ***********************************************************************/
int postgresiface2::abreasiento(int idasiento) {
    QString query="";
    query.sprintf("SELECT abreasiento(%d)",idasiento);
    return(ejecuta(query));
}// end abreasiento


int postgresiface2::modificacuenta(int idcuenta, QString desccuenta, QString codigo, bool cimputacion, bool cbloqueada, int idgrupo, bool cactivo, QString nombreent, QString cifent, QString dir, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
   QString cadena;
   cadena.sprintf("%d",idcuenta);
    QString query="";
    QString bloqueada = cbloqueada ? (char *) "TRUE" : (char *) "FALSE";
    QString activo = cactivo ?  "TRUE" :  "FALSE";
    QString imputacion = cimputacion ?  "TRUE" :  "FALSE";
	 QString nodebe = cnodebe ? "TRUE" : "FALSE";
	 QString nohaber = cnohaber ? "TRUE" : "FALSE";
    query.sprintf("UPDATE cuenta SET descripcion='%s', codigo='%s', imputacion=%s, bloqueada=%s, idgrupo=%d, activo=%s, nombreent_cuenta='%s', cifent_cuenta='%s', dirent_cuenta='%s', telent_cuenta='%s', coment_cuenta='%s', bancoent_cuenta='%s', emailent_cuenta='%s', webent_cuenta='%s', tipocuenta=%d, nodebe=%s, nohaber = %s WHERE idcuenta=%d\n",desccuenta.latin1(),codigo.latin1(),imputacion.latin1(),bloqueada.latin1(),idgrupo,activo.latin1(),nombreent.latin1(), cifent.latin1(),dir.latin1(), tel.latin1(), comm.latin1(),banco.latin1(), email.latin1(), web.latin1(),tipocuenta,nodebe.latin1(), nohaber.latin1(), idcuenta);
    fprintf(stderr,"%s\n",query.latin1());
    return(ejecuta(query));  
}// end modificacuenta


int postgresiface2::nuevacuenta(QString desccuenta, QString codigo, int padre, int idgrupo, QString nombreent, QString cifent, QString dir, QString tel, QString comm, QString banco, QString email, QString web, int tipocuenta, bool cnodebe, bool cnohaber) {
    QString query="";
    QString tpadre;
    if (padre == 0) {
       tpadre="NULL";
    }  else  {
       tpadre.sprintf("'%d'",padre);
    }// end if
	 	QString nodebe = cnodebe ? "TRUE" : "FALSE";
		QString nohaber = cnohaber ? "TRUE" : "FALSE";

    query.sprintf("INSERT INTO cuenta (descripcion, padre,codigo, idgrupo, nombreent_cuenta, cifent_cuenta, dirent_cuenta, telent_cuenta, coment_cuenta, bancoent_cuenta, emailent_cuenta, webent_cuenta, tipocuenta, nodebe, nohaber) VALUES('%s',%s,'%s',%d, '%s','%s', '%s', '%s', '%s', '%s', '%s', '%s', %d, %s, %s)",desccuenta.latin1(),tpadre.latin1(), codigo.latin1(), idgrupo, nombreent.latin1(), cifent.latin1(), dir.latin1(), tel.latin1() ,comm.latin1(), banco.latin1(), email.latin1(), web.latin1(), tipocuenta, nodebe.latin1(), nohaber.latin1() );
    fprintf(stderr,"%s\n",query.latin1());
    return(ejecuta(query));
}// end nuevacuenta

int postgresiface2::nuevoasiento(QString nombre, QString fecha, int numasiento) {
    QString query="";
    int val;
    int ordenasiento;
    if (numasiento == 0) {
      query="SELECT max(idasiento) FROM asiento";
      cursor2 *cur=cargacursor(query,"cargaasientoseq");
      val = atoi(cur->valor(0).latin1());
      val++;
      delete cur;
    } else {
      val = numasiento;
    }// end if
    query="SELECT max(ordenasiento) FROM asiento";
    cursor2 *cur=cargacursor(query,"cargaasientos");
    ordenasiento = atoi(cur->valor(0).latin1());
    ordenasiento++;
    delete cur;
    query.sprintf("INSERT INTO asiento (idasiento,descripcion, fecha, ordenasiento) VALUES (%d,'%s','%s', %d)",val,nombre.latin1(), fecha.latin1(), ordenasiento);
    ejecuta(query);
    return(val);
}// end nuevoasiento


cursor2 *postgresiface2::cargaempresas() {
   QString query;
   query= "SELECT * FROM empresa";
   cursor2 *cur=cargacursor(query,"cargaempresas");
   return(cur);
}// end cargaempresas


int postgresiface2::cargaempresa(QString nomempresa, QString login, QString password) {
    fprintf(stderr,"postgresiface2::cargaempresa\n");
    QString query="";
    PGresult   *res;
 /* make a connection to the database */
    QString conexion;

    if (confpr->valor(CONF_SERVIDOR) == "localhost") {
       conexion.sprintf( "dbname = %s", (char *)confpr->valor(CONF_METABASE).c_str());
    } else {
       conexion.sprintf("hostaddr=%s port=%s dbname=%s", (char *) confpr->valor(CONF_SERVIDOR).c_str(), (char *) confpr->valor(CONF_PUERTO).c_str(), (char *) confpr->valor(CONF_METABASE).c_str());
    }// end if

    conn = PQconnectdb(conexion);

    if (PQstatus(conn) == CONNECTION_BAD) {
        fprintf(stderr, "Connection to database '%s' failed.\n", dbName.latin1());
        fprintf(stderr, "%s", PQerrorMessage(conn));
    }// end if
    begin();

    /*
     * fetch rows from the pg_database, the system catalog of
     * databases
     */
    query.sprintf("DECLARE mycursor CURSOR FOR SELECT * FROM empresa, usuario, usuario_empresa where usuario.idusuario=usuario_empresa.idusuario AND empresa.idempresa=usuario_empresa.idempresa AND usuario.login='%s' AND empresa.nombredb='%s' AND usuario.password='%s'",login.latin1(), nomempresa.latin1(), password.latin1());
    res = PQexec(conn, query.latin1());
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

