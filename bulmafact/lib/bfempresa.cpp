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
#include "bclientes.h"
#define __VERSION_ACTUAL__ "bulmafact 0.0.1"

/*******************************************************************************/
/* Clase BfEmpresa                                                             */
/*******************************************************************************/

BfEmpresa::BfEmpresa() {
    nombreDB = new QString(""); //Inicializa el nombre de la base de datos con un valor vacio.
}


BfEmpresa::~BfEmpresa() {
}

/************************************************************************************/
/* Esta función crea un nuevo cliente en la base de datos.                          */
/************************************************************************************/
int BfEmpresa::nuevoCliente(QString* codigo) {
    QString query;
    pgIface* connexionDB;
    BfCursor* recordset;
    int codi=0;
    QString fecha;
    fecha = this->fechaPresente();
    connexionDB = new pgIface(*nombreDB);
    connexionDB->begin();
    if (codigo==NULL) { //Asignamos al nuevo cliente el siguiente codigo al último utilizado.
        recordset = connexionDB->cargaCursor("SELECT max(idcliente) AS ultimCodi FROM cliente","ultimoCli");
        if ( recordset->eof() ) { codi=1; }
        else { codi = recordset->valor(0).toInt() + 1; }
        query.sprintf("INSERT INTO cliente (idcliente, faltacliente, idrecargo) VALUES ('%i','%s','1')",codi,fecha.ascii());    
        connexionDB->ejecuta(query);
        connexionDB->commit();
    } else { //El usuario ha escojido un id para el nuevo cliente
        query.sprintf("SELECT idcliente FROM cliente WHERE idcliente='%s'", codigo->ascii());    
        recordset = connexionDB->cargaCursor(query,"escojeCli");
        if ( recordset->eof() ) { 
            codi=codigo->toInt();
            query.sprintf("INSERT INTO cliente (idcliente, faltacliente, idrecargo) VALUES ('%i','%s','1')",codi,fecha.ascii());    
            connexionDB->ejecuta(query);
            connexionDB->commit();
        } 
    }
    delete connexionDB;
    return codi;    
}

/************************************************************************************/
/* Esta función elimina al cliente de la base de datos (PELIGRO!!!)                 */
/************************************************************************************/
int BfEmpresa::eliminarCliente(QString* codigo) {
    QString query;
    pgIface * connexionDB = new pgIface(*nombreDB);
    connexionDB->begin();
    query.sprintf("DELETE FROM cliente WHERE idcliente='%s'",codigo->ascii());    
    connexionDB->ejecuta(query);
    connexionDB->commit();
    delete connexionDB;
    return codigo->toInt();

}

/************************************************************************************/
/* Esta función salva los datos del formulario clientes en la base de datos.       */
/************************************************************************************/
int BfEmpresa::salvaCliente(BClientes* datos) {
    //Leemos todos los campos de la pantalla cliente.
    //Otra opción seria passar a esta función todos los campos como paràmetros.
    QString idcliente = datos->codigo_cli->text();
    QString nomcliente = datos->nombre1_cli->text();
    QString nomaltcliente = datos->nombre2_cli->text();
    QString cifcliente = datos->CIF_cli->text();
    QString bancocliente = datos->ccc_cli->text();
    QString dircliente = datos->direccion_cli->text();
    QString poblcliente = datos->poblacion_cli->text();
    QString cpcliente = datos->CP_cli->text();
    QString telcliente = datos->telf_cli->text();
    QString faxcliente = datos->fax_cli->text();
    QString mailcliente = datos->mail_cli->text();
    QString urlcliente = datos->WEB_cli->text();
    QString faltacliente = datos->fecha_alta_cli->text();
    QString fbajacliente = "01/01/2000"; //datos->fecha_baja_cli->text();
    QString comentcliente = datos->comentarios_cli->text();
    //QString idrecargo = datos->codigo_cli->text();
    //Fin lectura campos en pantalla
    QString query;
    pgIface * connexionDB = new pgIface(*nombreDB);
    connexionDB->begin();
    query.sprintf("UPDATE cliente SET nomcliente='%s',nomaltcliente='%s', cifcliente='%s', bancocliente='%s', dircliente='%s', poblcliente='%s', cpcliente='%s', \
                   telcliente='%s', faxcliente='%s', mailcliente='%s', urlcliente='%s', faltacliente='%s', fbajacliente='%s', comentcliente='%s' \
                   WHERE idcliente='%s'"
                   ,nomcliente.ascii(), nomaltcliente.ascii(), cifcliente.ascii(), bancocliente.ascii(), dircliente.ascii(), poblcliente.ascii(), cpcliente.ascii(), 
                   telcliente.ascii(), faxcliente.ascii(), mailcliente.ascii(), urlcliente.ascii(), faltacliente.ascii(), fbajacliente.ascii(), comentcliente.ascii(),
                   idcliente.ascii());    
    connexionDB->ejecuta(query);
    connexionDB->commit();
    delete connexionDB;
    return datos->codigo_cli->text().toInt();
}

/************************************************************************************/
/* Esta función carga los datos en la pantalla cliente.                             */
/************************************************************************************/
int BfEmpresa::cargaCliente(BClientes* datos) {
    QString query;
    BfCursor* recordset;
    pgIface * connexionDB = new pgIface(*nombreDB);
    connexionDB->begin();
    query.sprintf("SELECT * FROM cliente WHERE idcliente='%s'",datos->codigo_cli->text().ascii());    
    recordset = connexionDB->cargaCursor(query,"cargaCli");
    connexionDB->commit();
    if (! recordset->eof() ) { 
        //datos->codigo_cli->setText();
        datos->nombre0_cli->setText(recordset->valor("nomcliente"));
        datos->nombre1_cli->setText(recordset->valor("nomcliente"));
        datos->nombre2_cli->setText(recordset->valor("nomaltcliente"));
        datos->CIF_cli->setText(recordset->valor("cifcliente"));
        datos->ccc_cli->setText(recordset->valor("bancocliente"));
        datos->direccion_cli->setText(recordset->valor("dircliente"));
        datos->poblacion_cli->setText(recordset->valor("poblcliente"));
        datos->CP_cli->setText(recordset->valor("cpcliente"));
        datos->telf_cli->setText(recordset->valor("telcliente"));
        datos->fax_cli->setText(recordset->valor("faxcliente"));
        datos->mail_cli->setText(recordset->valor("mailcliente"));
        datos->WEB_cli->setText(recordset->valor("urlcliente"));
        datos->fecha_alta_cli->setText(recordset->valor("faltacliente"));
        datos->fecha_baja_cli->setText(recordset->valor("fbajacliente"));
        datos->comentarios_cli->setText(recordset->valor("comentcliente"));
    }
    delete connexionDB;
    return datos->codigo_cli->text().toInt();
}

/************************************************************************************/
/* Esta función retorna un cursor con una lista de todas las bases de datos que     */
/* se han encontrado en el servidor y que sean compatibles con la versión actual    */
/* de bulmaFACT.                                                                    */
/***********************************************************************************/
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


/************************************************************************************/
/* Busca en la tabla clientes nombres parecidos A ...                               */
/************************************************************************************/
BfCursor* BfEmpresa::buscarParecidos(QString tabla, QString argumentoBusqueda, QString opcionesBusqueda) {
    QString query("");
    BfCursor* recordset=NULL;
    pgIface * connexionDB = new pgIface(*nombreDB);
    connexionDB->begin();
    if (tabla=="cliente") query.sprintf("SELECT * FROM cliente WHERE nomcliente %s%s%%' ORDER BY nomcliente",opcionesBusqueda.ascii(), argumentoBusqueda.ascii()) ;    
    if (tabla=="proveedor") query.sprintf("SELECT * FROM proveedor WHERE nomproveedor %s%s%%' ORDER BY nomproveedor",opcionesBusqueda.ascii(),argumentoBusqueda.ascii());    
    if (tabla=="articulo") query.sprintf("SELECT * FROM articulo WHERE nomarticulo %s%s%%' ORDER BY nomarticulo",opcionesBusqueda.ascii(),argumentoBusqueda.ascii());    
    fprintf(stderr, "Debug: %s\n",tabla.ascii());
    recordset = connexionDB->cargaCursor(query,"cargaClientes");
    connexionDB->commit();
    delete connexionDB;
    return recordset;
}

/************************************************************************************/
/* Inicializa la empresa. Para empezar cargamos el nombre de la base de datos.      */
/************************************************************************************/
void BfEmpresa::cargaEmpresa(QString *NombreBaseDatos) {
nombreDB=NombreBaseDatos;
}

/************************************************************************************/
/* Devuelve la fecha presente del sistema                                           */
/************************************************************************************/
QString BfEmpresa::fechaPresente(){
    return QDate::currentDate().toString("dd/MM/yyyy");
}

/************************************************************************************/
/* Esta función convierte un numero con decimales a un entero. Usando la regla      */
/* que si el el primer decimal es mayor o igual  a 5 se devuelve el entero superior.*/
/***********************************************************************************/
int BfEmpresa::roundI(double valor) {
    int retorno;
    double mayor=floor(valor);
    if ((mayor-valor) >= 0.5)  retorno=(int)mayor-1;
    else retorno= (int)mayor;
    return retorno;
}

/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!!!!!!!!!  FIN  DE LA CLASE BfEmpresa    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
/* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */

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

