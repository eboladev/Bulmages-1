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
//#include "estadisticasview.h"
//#include "resmensualview.h"
#include "mpatrimonialesview.h"
#include "canalview.h"
#include "ccosteview.h"
#include "ainteligentesview.h"
#include "propiedadesempresa.h"
#include "cambiactaview.h"
#include "amortizacionesview.h"
//#include "adocumental.h"
#include "cobropagoview.h"
#include "tipoivaview.h"
#include "fpagoview.h"

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

/** \brief inicializa la clase con el nombre de la base de datos y con el workspace.
  */
int empresa::inicializa1(QString nombreDB, QWorkspace *space) {
   fprintf(stderr,"EMPRESA::inicializa1()\n");
   pWorkspace = space;
   nombre = confpr->valor(CONF_LOGIN_USER);
   contrasenya = confpr->valor(CONF_PASSWORD_USER);
   fprintf(stderr,"conexionbase->inicializa\n");
   conexionbase2->inicializa(nombreDB, nombre, contrasenya);
   fprintf(stderr,"fin de conexionbase->inicializa\n");

  
  // Calculamos el número de dígitos que tiene el nivel último de la empresa. 
   conexionbase2->begin();
   QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursor2 *cursoraux1 = conexionbase2->cargacursor(query,"codcuenta");
   conexionbase2->commit();
   numdigitos=cursoraux1->valor(2).length();
   delete cursoraux1;       

   if (extracto != NULL) {
         delete extracto;
         delete diario;
         delete balance;
         delete balance1;
         delete introapunts1;
         delete selccostes;
         delete selcanales;
   }// end if  

  /// Inicializamos los selectores de centros de coste y canales
  selccostes=new selectccosteview(this,0,"selccostes");   
  selcanales=new selectcanalview(this,0,"selcanales");
  
  /// Inicializamos las ventanas de uso generalizado
  extracto = new extractoview1(this, pWorkspace,"extracto");
  diario = new diarioview1(this,pWorkspace,"diario");
  balance = new balanceview(this, pWorkspace,"balance");
  balance1 = new balance1view(this, pWorkspace,"balance2");
  introapunts1 = new intapunts3view(this, pWorkspace,"introapunts2");
  
  /// Pasamos parametros a las ventanas para que puedan coordinarse entre si.\todo El paso de mensajes debería pasar por la clase \ref empresa siempre.
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

/** \brief Se utiliza para mostrar un selector de empresas \ref abreempresaview
  * Al usuario debe seleccionar una empresa y el sistema empieza la inicialización de clases a partir de dicha inicialización.
  */
QString empresa::searchCompany() {
//El cambio de empresa se realiza desde el selector.
  fprintf(stderr,"empresa::searchCompany vamos a mostrar el abreempresaview\n");
  abreempresaview *nuevae = new abreempresaview(0,"BulmaCont" );
  nuevae->exec();
   fprintf(stderr,"Vamos a cambiar la empresa \n");
   QString bd= nuevae->nomDB();
   fprintf(stderr,"Empresa cambiada a %s\n", bd.ascii());
   delete nuevae;
  return(bd);
}// end searchCompany


/** 
  * Esta funcion abre la ventana que presenta las cuentas contables
  */
int empresa::muestracuentas() 	{
  listcuentasview1 *nuevae = new listcuentasview1(this, 0,"mostracuentas",true);
  nuevae->inicializa();
  nuevae->exec();
  delete nuevae;
  return(0);
}// end muestracuentas


/**
  * Esta funcion presenta los centros de coste. Luego recarga los
  * comboboxes de todo el resto de pantallas para que se actualizen
  */
int empresa::ccostes() {
  ccosteview *ccoste = new ccosteview(this, 0,"ccostes", true);
  ccoste->exec();
  delete ccoste;
  balance->cargacostes();
  return(0);
}// end ccostes


/**
  * Esta funcion presenta la pantalla de edicion de canales. Luego
  * recarga los comboboxes de todo el resto de pantallas para que se
  * actualizen.
  */
int empresa::canales() {
  canalview *canal = new canalview(this,0,"canales",true);
  canal->exec();
  delete canal;
  return(0);
}// end ccostes


/** Crea la ventana de tipos de IVA y la presenta en modo modal
  * Es invocada desde la clase principal
  */
int empresa::tiposIVA() {
   tipoivaview *tip = new tipoivaview(this, 0, "tipos Iva");
   tip->exec();
   delete tip;
   return(0);
}// end tiposiva


/** Crea la ventana de Formas de Pago y la presenta en modo modal
  * Es invocada desde la clase principal
  */
int empresa::fPago() {
   fpagoview *tip = new fpagoview(this, 0, "Formas de Pago");
   tip->exec();
   delete tip;
   return(0);
}// end tiposiva


int empresa::cambioejercicio() {
//El ejercicio ha cambiado y recargamos el cursor de asientos del nuevo ejercicio
  introapunts1->cargarcursor();
  introapunts1->boton_fin();
  return(0);
}// end cambioejercicio


int empresa::nuevaempresa() {
//La creación de una nueva empresa ha pasado al selector.
  return(0);
}// end nuevaempresa





int empresa::nuevacuenta() {
   cuentaview * nuevae =new cuentaview(this,0,"nuevacuenta",true);
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
   nuevae->inicializa(conexionbase2);
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
  } else if (widget == extracto) {
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
  } else if (widget == extracto){
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
  } else if (widget == extracto) {
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
  } else if (widget == extracto) {
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
   introapunts1->cargarcursor();
   introapunts1->boton_fin();
   introapunts1->show();
}// end Abrirasientos


void empresa::Ordenarasientos() {

   QString query= "SELECT reordenaasientosall()";
   conexionbase2->begin();
   cursor2 * cur = conexionbase2->cargacursor(query, "hola");
   conexionbase2->commit();
   delete cur;
   introapunts1->cargarcursor();
   introapunts1->boton_fin();
   introapunts1->show();
}// end Abrirasientos


int empresa::registroiva() {
//   listivaview *perd = new listivaview(this, EjercicioActual.ascii());
   listivaview *perd = new listivaview(this, "0");
   perd->inicializa(introapunts1);
   perd->exec();
   delete perd;
   return(0);
}// end registroiva

int empresa::modelo347() {
//   BModelo347 *dlg347 = new BModelo347(conexionbase2,EjercicioActual.ascii());
   BModelo347 *dlg347 = new BModelo347(conexionbase2,"0");
   //dlg347->inicializa(conexionbase2, introapunts1);
   dlg347->exec();
   delete dlg347;
   return(0);
}// end modelo347

/*************************************************************
 * Esta funcion borra la empresa actual
 *************************************************************/
 
int empresa::borrarempresa() {
//El proceso de borrado se realiza desde las herramientas el selector.
  return(0);
}// end borrarrempresa


// Esta función es la que se encarga de cerrar al empresa
// Y de crear un nuevo ejercicio para dicha empresa
void empresa::nuevoejercicio() {
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


// Esta función recalcula los saldos parciales del plan contable.
void empresa::recalculasaldos() {
   conexionbase2->begin();
   conexionbase2->ejecuta("SELECT recalculasaldos()");
   conexionbase2->commit();
}// end recalculasaldos





/** Esta función se dispara para poner en marcha la gestión de cobros y pagos
  */
void empresa::cobPag() {
   cobropagoview *adoc= new cobropagoview(this,0,"hola");
   adoc->exec();
   delete adoc;
}// cobPag
