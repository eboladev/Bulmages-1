/***************************************************************************
                          empresa.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borras
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

#include "empresa.h"
#include "abreempresaview.h"
#include "listcuentasview1.h"
#include "cuentaview.h"
#include "listivaview.h"
#include "asientosview.h"
#include "balancesview.h"
#include "estadisticasview.h"
#include "resmensualview.h"
#include "mpatrimonialesview.h"
#include "canalview.h"
#include "ccosteview.h"
#include "ainteligentesview.h"
#include "propiedadesempresa.h"
#include "cambiactaview.h"
#include "amortizacionesview.h"


#include <qobject.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif

// #define __DEBUG__


empresa::empresa(){
   diario = NULL;
   extracto = NULL;
   balance1 = NULL;
   balance = NULL; 
   introapunts1 = NULL;
   selccostes  = NULL;
   selcanales = NULL;
   nombre = "";
   conexionbase2 = new postgresiface2();
   conexionanterior2 = NULL;
}// end empresa


empresa::~empresa(){
  if (diario) delete diario;
  if (extracto) delete extracto;
  if (balance1) delete balance1;
  if (balance) delete balance;
  if (introapunts1) delete introapunts1;
  if (conexionbase2) delete conexionbase2;
  if (conexionanterior2) delete conexionanterior2;
  if (selccostes) delete selccostes;
  if (selcanales) delete selcanales;
}// end ~empresa



/***********************************************************************
 *  Esta funcion es una prueba de la nueva conexion a la base de datos *
 *  En estos momentos esta en desuso                                   *
 ***********************************************************************/
void empresa::testnewdb() {
}// end testnewdb


int empresa::inicializa1(QWorkspace *space) {
   fprintf(stderr,"EMPRESA::inicializa1()\n");
   pWorkspace = space;
   fprintf(stderr,"conexionbase->inicializa\n");
   conexionbase2->inicializa(nombreDB.ascii());
   fprintf(stderr,"fin de conexionbase->inicializa\n");

  
  // Calculamos el número de dígitos que tiene el nivel último de la empresa. 
   conexionbase2->begin();
   QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursor2 *cursoraux1 = conexionbase2->cargacursor(query,"codcuenta");
   conexionbase2->commit();
   numdigitos=cursoraux1->valor(2).length();
   delete cursoraux1;  
    
    
    //Buscamos el último ejercicio en la tabla "ejercicios"
   query="SELECT MAX(ejercicio) AS ejercicio FROM ejercicios WHERE periodo=0";
   conexionbase2->begin();
   cursor2 *recordSet = conexionbase2->cargacursor(query,"recordSet");
   conexionbase2->commit();
   if (!recordSet->eof()) EjercicioActual=recordSet->valor("ejercicio");
   else EjercicioActual="";
   delete recordSet;    
   if (extracto != NULL) {
         delete extracto;
         delete diario;
         delete balance;
         delete balance1;
         delete introapunts1;
         delete selccostes;
         delete selcanales;
   }// end if  

  // Inicializamos los selectores de centros de coste y canales
  selccostes=new selectccosteview(this,0,"selccostes");   
  selcanales=new selectcanalview(this,0,"selcanales");

/* 
  extracto = new extractoview1(this, pWorkspace,"extracto");
  diario = new diarioview1(this,pWorkspace,"diario");
  balance = new balanceview(this, pWorkspace,"balance");
  balance1 = new balance1view(this, pWorkspace,"balance2");
  introapunts1 = new intapunts3view(this, pWorkspace,"introapunts2");
*/
  extracto = new extractoview1(this, pWorkspace,"extracto");
  diario = new diarioview1(this,pWorkspace,"diario");
  balance = new balanceview(this, pWorkspace,"balance");
  balance1 = new balance1view(this, pWorkspace,"balance2");
  introapunts1 = new intapunts3view(this, pWorkspace,"introapunts2");
  
  introapunts1->inicializa(conexionbase2);
  
  // Pasamos parametros a las ventanas para que puedan coordinarse entre si.
  introapunts1->inicializa1(extracto, diario, balance);
  extracto->inicializa2(introapunts1, diario, balance);
  diario->inicializa2(introapunts1,extracto, balance);
  balance->inicializa2(introapunts1, diario, extracto);
  balance1->inicializa2(introapunts1, diario, extracto);  
  
  return(0);
}// end inicializa1

void empresa::maximiza() {
  
  diario->showMaximized();
  extracto->showMaximized();
  balance1->showMaximized();
  balance->showMaximized();
  introapunts1->showMaximized();
}// end maximiza

/************************************************************************
 * Esta funcion hace el inicio de la empresa, muestra el dialogo de abrir
 * y hace los pasos necesarios.
************************************************************************/
int empresa::inicializa(QString * DB, QString * User, QString * Passwd) {
  if (nombre == NULL || 1) {
    //salto el dialeg de login. Ja tinc el nom d'usuari, password i el nom de la base de dades.
    nombre = *User;
    contrasenya = *Passwd;
    nombreDB = *DB;
    //fi del salt
    
    // Buscamos la empresa anterior y hacemos lo que corresponda.
    // ----------------------------------------------------------
    char query[300];
    postgresiface2 *metabase;
    metabase = new postgresiface2();
    fprintf(stderr,"Acabamos de crear la metabase\n");
    metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
    fprintf(stderr,"acabamos de inicializar la metabase\n");
    metabase->begin();
    fprintf(stderr,"Acabamos de hacer un begin\n");
    sprintf(query,"SELECT * FROM EMPRESA WHERE nombredb='%s'\n",nombreDB.ascii());
    fprintf(stderr,"%s\n",query);
    cursor2 *cursoraux = metabase->cargacursor(query,"cursorempresa");
    sprintf(query,"SELECT * FROM EMPRESA WHERE nombre='%s' AND ano<%s ORDER BY ano DESC\n",cursoraux->valor("nombre").ascii(), cursoraux->valor("ano").ascii());
    cursor2 *cursoraux2 = metabase->cargacursor(query,"cursorempresa1");
    if (!cursoraux2->eof()) {
       conexionanterior2 = new postgresiface2();
       conexionanterior2->inicializa( cursoraux2->valor("nombredb"));
       fprintf(stderr,"Hemos encontrado el año anterior\n");
    }// end if
    metabase->commit();
    delete cursoraux;
    delete cursoraux2;
    delete metabase;  
  }// end if
  return(0);
}// end inicializa


/******************************************************************
  Esta funcion abre la ventana que presenta las cuentas contables
*******************************************************************/
int empresa::muestracuentas() 	{
  listcuentasview1 *nuevae = new listcuentasview1(0,"mostracuentas",true);
  nuevae->inicializa(conexionbase2);
  nuevae->exec();
  delete nuevae;
  return(0);
}// end muestracuentas


void empresa::SegCuentas() {
// Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
   resmensualview *rmen = new resmensualview(0,0);
   rmen->inicializa(conexionbase2);
   rmen->exec();
   delete rmen;   
}// end SegCuentas

void empresa::BalanceGrafico() {
   // Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
   estadisticasview *est = new estadisticasview(0,0);
   est->inicializa(conexionbase2);
   est->exec();
   delete est;   
}// end BalanceGrafico

/*****************************************************************
  Esta funcion presenta los centros de coste. Luego recarga los
  comboboxes de todo el resto de pantallas para que se actualizen
  ****************************************************************/
int empresa::ccostes() {
  ccosteview *ccoste = new ccosteview(this, 0,"ccostes", true);
  ccoste->exec();
  delete ccoste;
  balance->cargacostes();
  return(0);
}// end ccostes


/*******************************************************************
  Esta funcion presenta la pantalla de edicion de canales. Luego
  recarga los comboboxes de todo el resto de pantallas para que se
  actualizen.
  ******************************************************************/
int empresa::canales() {
  canalview *canal = new canalview(this,0,"canales",true);
  canal->exec();
  delete canal;
  return(0);
}// end ccostes

int empresa::cambioejercicio() {
//El ejercicio ha cambiado y recargamos el cursor de asientos del nuevo ejercicio
  introapunts1->cargarcursor(-1);
  introapunts1->boton_fin();
  return(0);
}// end cambioejercicio


int empresa::nuevaempresa() {
//La creación de una nueva empresa ha pasado al selector.
/*  
  nuevaempresaview * nuevae = new nuevaempresaview(0,"nevaempresa");
  nuevae->exec();
  nombre=NULL;
  if (nuevae->empresacreada == 1) {
    inicializa();
    inicializa1(pWorkspace);
  }// end if
  delete nuevae; 
*/  
  return(0);
}// end nuevaempresa


// Esta función se usa sólo si se llama a la anterior sin argumentos.
// Ya no es una reinicialización. Es una inicialización.
int empresa::cambiarempresa() {
//El cambio de empresa se realiza desde el selector.

//  int retorno;
  fprintf(stderr,"empresa::cambiarempresa\n");
  abreempresaview *nuevae = new abreempresaview("Abrir Empresa",true );
  nuevae->exec();
  fprintf(stderr,"fin de la ejecución del formulario de selección de empresa \n");
//  retorno = nuevae->result();
//  fprintf(stderr,"cambiarempresa result:%d",retorno);
//  if (retorno) {
   fprintf(stderr,"Vamos a cambiar la empresa \n");
   QString bd= nuevae->empresabd;
   QString us= nuevae->nombre;
   QString pas= nuevae->contrasena;
   fprintf(stderr,"%s %s %s", bd.ascii(), us.ascii(), pas.ascii());
   inicializa(&bd, &us, &pas);
   setejactual(nuevae->ejercicioMetaDB);
//    inicializa1(pWorkspace);
//  }// end if
  delete nuevae;
  return(0);
}// end cambiarempresa


int empresa::nuevacuenta() {
   cuentaview * nuevae =new cuentaview(0,"nuevacuenta",true);
   nuevae->inicializa(conexionbase2);
   // Al ser una insercion no es preciso cargar ninguna cuenta previa.
   //   nuevae->cargacuenta(24);
   nuevae->exec();
   delete nuevae;
   return(0);
}// end nuevacuenta


int empresa::muestraapuntes() {
  return(0);
}// end muestraapuntes


int empresa::muestraapuntes1() {
  introapunts1->setWindowState(introapunts1->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
  return(0);
}// end muestraapuntes1


int empresa::muestraasientos() {
  asientosview *nuevae = new asientosview(this, 0,"",true);
  nuevae->inicializa(conexionbase2, introapunts1);
  nuevae->exec();
  delete nuevae;
  return(0);
}// end muestraapuntes


int empresa::propiedadempresa() {
   propiedadesempresa * nuevae = new propiedadesempresa(0,"",true);
   nuevae->inicializa(conexionbase2, nombreDB.ascii());
   nuevae->exec();
   delete nuevae;
   return(0);
}// end propiedadempresa


int empresa::amortizaciones(){
    amortizacionesview * amors = new amortizacionesview(this,0,"",true);
    amors->exec();
    delete amors;
    return(0);
}// end amortizaciones


/*******************************************************************
 * Esta funcion llama a la pantalla de creacion, modificacion      *
 * de asientos inteligentes.                                       *
 *******************************************************************/
int empresa::ainteligentes() {
   ainteligentesview * nuevae = new ainteligentesview(this, 0,"ainteligentes",true);
   nuevae->exec();
   delete nuevae;
  return(0);
}// end ainteligentes



/*******************************************************************
 * Esta funcion llama a la pantalla de creacion, modificacion      *
 * de masas patrimoniales.                                         *
 *******************************************************************/
int empresa::mpatrimoniales() {
   mpatrimonialesview * nuevae = new mpatrimonialesview(0,"mpatrimoniales",true);
   nuevae->inicializa(conexionbase2);
//   nuevae->setmodoselector();
   nuevae->exec();
   delete nuevae;
  return(0);
}// end mpatrimoniales


/*******************************************************************
 * Esta funcion llama a la pantalla de creacion, modificacion      *
 * de masas patrimoniales.                                         *
 *******************************************************************/
int empresa::compbalance() {
   // Esto es lo más correcto, lo anterior se llamará desde esta nueva ventana.
   balancesview * nueva = new balancesview(this,0,"balances");
   nueva->exec();
   delete nueva;   
   return(0);

}// end mpatrimoniales



int empresa::libromayor() {
  extracto->setWindowState(extracto->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
  return(0);
}//end libromayor


int empresa::boton_siguiente() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
    introapunts1->boton_siguiente();
  } else if (widget == diario){
//    diario->boton_siguiente();
  } else {
    extracto->boton_siguiente();
  }// end if
  return(0);  
}// end boton_siguiente


int empresa::boton_anterior() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
    introapunts1->boton_anterior();
  } else if (widget == diario){
// diario->boton_siguiente
//  } else if (widget == introapunts1) {
//      introapunts1->boton_anterior();
  } else {
    extracto->boton_anterior();
  }// end if
  return(0);
}// end boton_siguiente


int empresa::boton_guardar() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
  } else if (widget == diario){
    diario->boton_guardar();
  } else {
    extracto->boton_guardar();
  }// end if
  return(0);
}// end boton_siguiente

int empresa::boton_imprimir() {
  QWidget *widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
  } else if (widget == diario){
    diario->boton_imprimir();
  } else if(widget == balance1){
     balance1->boton_imprimir();
	} else if (widget == balance) {
		balance->boton_imprimir();
  } else if (widget == extracto) {
    extracto->boton_imprimir();
  }// end if
  return(0);
}// end boton_siguiente

int empresa::boton_reload() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
  } else if (widget == diario){
    diario->accept();
  } else if(widget == extracto){
    extracto->accept();
  } else if(widget== balance){
    balance->accept();
  } else if (widget == balance1) {
     balance1->accept();
  }// end if
  return(0);
}// end boton_reload

int empresa::boton_primero() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
    introapunts1->boton_inicio();
  } else if (widget == diario){
  } else {
    extracto->boton_inicio();
  }// end if
  return(0);
}// end boton_primero

int empresa::boton_ultimo() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
    introapunts1->boton_fin();
  } else if (widget == diario){
  } else {
    extracto->boton_fin();
  }// end if
  return(0);
}// end boton_ultimo


int empresa::librodiario() {
  diario->setWindowState(diario->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
  return(0);
}//end librodiario


int empresa::librobalance() {
   balance->setWindowState(balance->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
   return(0);
}//end librobalance


int empresa::librobalancetree() {
   balance1->setWindowState(balance1->windowState() &  ~Qt::WindowMinimized | Qt::WindowActive);
   return(0);
}// end librobalancetree


void empresa::Abrirasientos() {
   conexionbase2->begin();
   conexionbase2->ejecuta("SELECT abreasientos()");
   conexionbase2->commit();
   introapunts1->cargarcursor(0);
   introapunts1->boton_fin();
   introapunts1->show();
}// end Abrirasientos


void empresa::Ordenarasientos(int ejercicio) {
   conexionbase2->begin();
   QString query;
   query.sprintf("SELECT reordenaasientos(%i)",ejercicio);
   conexionbase2->ejecuta(query);
   conexionbase2->commit();
   introapunts1->cargarcursor(0);
   introapunts1->boton_fin();
   introapunts1->show();
}// end Abrirasientos

int empresa::registroiva() {
   listivaview *perd = new listivaview(EjercicioActual.ascii());
   perd->inicializa(conexionbase2, introapunts1);
   perd->exec();
   delete perd;
   return(0);
}// end registroiva

int empresa::modelo347() {
   BModelo347 *dlg347 = new BModelo347(conexionbase2,EjercicioActual.ascii());
   //dlg347->inicializa(conexionbase2, introapunts1);
   dlg347->exec();
   delete dlg347;
   return(0);
}// end modelo347

/*
int empresa::guardarempresa() {
  char *args[4];
  int pid;
  int error;
  QString fn = QFileDialog::getSaveFileName(0, "Empresas (*.pgdump)", 0,"Guardar Empresa","Elige el nombre de empresa con el que guardar");
  if (!fn.isEmpty()) {
     fprintf(stderr,"Vamos a guardar la empresa en el fichero %s\n",fn.ascii());
     args[0]=(char *) nombreDB.ascii();
     args[1]=(char *) nombreDB.ascii();
     args[2]=(char *) fn.ascii();
     args[3]=NULL;
#ifndef WIN32
     if ((pid=fork()) < 0) {
       perror ("Fork failed");
       exit(errno);
     }// end if
     if (!pid) {
        string argumentos = confpr->valor(CONF_EJECUTABLES) + "guardaemp";
        error = execvp(argumentos.c_str(),args);
     }// end if
     if (pid) {
        waitpid (pid, NULL, 0);
     }// end if
#endif
  }// end if
  return(0);  
}// end guardarempresa
*/

/*************************************************************
 * Esta funcion carga una empresa previamente guardada
 *************************************************************/
 /*
int empresa::cargarempresa() {
  char *args[4];
  int pid;
  int error;
  QString fn = QFileDialog::getOpenFileName(0, theApp->translate("empresa","Empresas (*.pgdump)",""), 0,theApp->translate("empresa","Cargar Empresa",""),theApp->translate("emrpesa","Elige el fichero a cargar.",""));
  if (!fn.isEmpty()) {
     args[0]=(char *) nombreDB.ascii();
     args[1]=(char *) nombreDB.ascii();
     args[2]=(char *) fn.ascii();
     args[3]=NULL;
//     defaultDB->close();
     // Para cargar la empresa debe estar sin ningun usuario dentro de ella.
     delete conexionbase2;
     conexionbase2 = new postgresiface2();
#ifndef WIN32
     if ((pid=fork()) < 0) {
       perror ("Fork failed");
       exit(errno);
     }// end if
     if (!pid) {
        string argumentos = confpr->valor(CONF_EJECUTABLES) + "cargaemp";
        fprintf(stderr,"Ejecutamos el cargaemp\n");
        error = execvp(argumentos.c_str(),args);
        fprintf(stderr,"Fin del cargaemp\n");
     }// end if
     if (pid) {
        waitpid (pid, NULL, 0);
     }// end if
     inicializa();
     inicializa1(pWorkspace);
     maximiza();
#endif
  }// end if
  return(0);
}// end cargarempresa
*/

/*************************************************************
 * Esta funcion borra la empresa actual
 *************************************************************/
 
int empresa::borrarempresa() {
//El proceso de borrado se realiza desde las herramientas el selector.
/*  
  char *args[3];
  args[0]="borraremp";
  args[1]= nombredb;
  args[2]=NULL;
  int pid, error;
  fprintf(stderr,"Borrado de la empresa %s\n",nombredb);
  // Aqui vamos a borrar la columna correspondiente en la metabd
  char query[400];
  cursor2 *cursoraux;
  postgresiface2 *metabase;
  metabase = new postgresiface2();
  metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
  metabase->begin();
  sprintf(query,"SELECT * FROM EMPRESA WHERE nombredb='%s'\n",nombredb);
  cursoraux = metabase->cargacursor(query,"cursorempresa");
  sprintf(query,"DELETE FROM usuario_empresa WHERE idempresa=%s\n",cursoraux->valor(0).ascii());
  metabase->ejecuta(query);
  sprintf(query,"DELETE FROM  empresa WHERE nombredb='%s'\n",nombredb);
  metabase->ejecuta(query);
  metabase->commit();
  delete cursoraux;
  delete metabase;
  nombre=NULL;
#ifndef WIN32
  if ((pid=fork()) < 0) {
        perror ("Fork failed");
        exit(errno);
  }// end if
  if (pid==0) {
     string argumentos = confpr->valor(CONF_EJECUTABLES) + "borraremp";
     error = execvp(argumentos.c_str(),args);
  } else {
        waitpid (pid, NULL, 0);
  }// end if
#endif
  cambiarempresa();
*/  
  return(0);
}// end borrarrempresa


// Esta función es la que se encarga de cerrar al empresa
// Y de crear un nuevo ejercicio para dicha empresa
void empresa::nuevoejercicio() {
//Para crear un nuevo ejercicio se tiene que hacer desde el selector.
/*  
  char *args[4];
  int pid;
  int error;
   QString query;
    postgresiface2 *metabase;
    metabase = new postgresiface2();
    metabase->inicializa(confpr->valor(CONF_METABASE).c_str());
    metabase->begin();
    query.sprintf("SELECT * FROM empresa WHERE nombredb='%s'",nombredb);
    fprintf(stderr,"%s\n",query.ascii());
    cursor2 *cur = metabase->cargacursor(query,"empresa");
    if (!cur->eof()) {
       QString nnombre = cur->valor("nombre");
       QString nnombredb = cur->valor("nombredb");
       QString nanodb = cur->valor("ano");
       nnombredb = nnombredb.left(nnombredb.length()-4);
       bool ok;
       int inanodb = nanodb.toInt(&ok,10);
       inanodb++;
       QString nanodb2;
       nanodb2.setNum(inanodb,10);
       nnombredb += nanodb2;
       query.sprintf("INSERT INTO EMPRESA ( nombre, nombredb, ano) VALUES ('%s','%s',%d)", nnombre.ascii(), nnombredb.ascii(), inanodb);
       fprintf(stderr,"%s\n",query.ascii());
       metabase->ejecuta(query);

       query.sprintf("SELECT last_value AS idempresa FROM empresa_idempresa_seq");
       cursor2 *cur2=metabase->cargacursor(query,"idempresa");
              
       query.sprintf("SELECT * FROM usuario_empresa WHERE idempresa = %s", cur->valor("idempresa").ascii());
       cursor2 *cur1=metabase->cargacursor(query,"us_emp");
       while (!cur1->eof()) {
          query.sprintf("INSERT INTO usuario_empresa (idusuario, idempresa, permisos) VALUES (%s,%s,%s)", cur1->valor("idusuario").ascii(), cur2->valor("idempresa").ascii(), cur1->valor("permisos").ascii());
          metabase->ejecuta(query);
          cur1->siguienteregistro();
       }// end whiel
       delete cur1;
       delete cur2;

       // Guardamos la Empresa
       args[0]=nombredb;
       args[1]=nombredb;
       args[2]="/tmp/bulmacop.pgdump";
       args[3]=NULL;
#ifndef WIN32
       if ((pid=fork()) < 0) {
         perror ("Fork failed");
         exit(errno);
       }// end if
       if (!pid) {
          string argumentos = confpr->valor(CONF_EJECUTABLES) + "guardaemp";
          error = execvp(argumentos.c_str(),args);
       }// end if
       if (pid) {
          waitpid (pid, NULL, 0);
       }// end if

       // Cargamos la empresa guardada
       args[0]=(char *)nnombredb.ascii();
       args[1]=(char *)nnombredb.ascii();
       args[2]="/tmp/bulmacop.pgdump";
       args[3]=NULL;
  //     defaultDB->close();
       if ((pid=fork()) < 0) {
         perror ("Fork failed");
         exit(errno);
       }// end if
       if (!pid) {
          string argumentos = confpr->valor(CONF_EJECUTABLES) + "cargaemp";
          error = execvp(argumentos.c_str(),args);
       }// end if
       if (pid) {
          waitpid (pid, NULL, 0);
       }// end if
#endif
       strcpy(nombredb,nnombredb.ascii());
    }// end if
    delete cur;
    metabase->commit();
    delete metabase;
    inicializa();
    inicializa1(pWorkspace);
    abreempresa();

// Borramos los asientos anteriores
   char query1[500];
   int valor;
   int resultado;
   valor = QMessageBox::warning( 0, theApp->translate("empresa","Borrar Asiento",""), theApp->translate("empresa","Se procedera a borrar el asiento.",""), QMessageBox::Yes, QMessageBox::No);
   if (valor ==  QMessageBox::Yes) {
       conexionbase2->begin();
       sprintf(query1,"DELETE FROM apunte where idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
       resultado = conexionbase2->ejecuta(query1);
       sprintf(query1,"DELETE FROM borrador where idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
       resultado += conexionbase2->ejecuta(query1);
       sprintf(query1,"DELETE FROM asiento WHERE idasiento NOT IN (SELECT max(idasiento) FROM asiento)");
       resultado += conexionbase2->ejecuta(query1);
       if (resultado != 0) {
           conexionbase2->rollback();
       } else {
           conexionbase2->commit();
       }// end if
       Ordenarasientos();
//       introapunts1->repinta();
   }// end if
*/
}// end nuevoejercicio

void empresa::cierraempresa() {
  introapunts1->show();
  introapunts1->asiento_cierre();
}// end cierraempresa


void empresa::abreempresa() {
  introapunts1->show();
  introapunts1->asiento_apertura();
}// end abreempresa


void empresa::regularizaempresa() {
  introapunts1->show();
  introapunts1->asiento_regularizacion();
}// end regularizaempresa


void empresa::Filtro() {
  QWidget *widget;
  widget = pWorkspace->activeWindow ();
  if (widget == introapunts1) {
    introapunts1->boton_filtrar();
  } else if (widget == extracto){
     extracto->boton_filtrar();
  } else if (widget == diario) {
     diario->boton_filtrar();
  } else {
//    extracto->boton_filtrar();
  }// end if
}// end Filtro

// Esta función reemplaza en los apuntes todas las ocurrencias de una determinada cuenta por
// Otra cuenta.
void empresa::reemplazacuentaenasientos() {
    cambiactaview *ctac = new  cambiactaview(this,0,0, false);
    ctac->exec();
}// end reemplazacuentasenasientos

