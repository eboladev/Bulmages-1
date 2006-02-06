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
#include "mpatrimonialesview.h"
#include "canalview.h"
#include "ccosteview.h"
#include "ainteligentesview.h"
#include "propiedadesempresa.h"
#include "cambiactaview.h"
#include "amortizacionesview.h"
#include "cobropagoview.h"
#include "tipoivaview.h"
#include "fpagoview.h"
#include "balance1view.h"
#include "selectccosteview.h"
#include "selectcanalview.h"

#include <qobject.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#endif


#include "asiento1view.h"

// #define __DEBUG__
void empresa::s_asiento1(){
	_depura("empresa::s_asiento1",0);
	Asiento1View * as = new Asiento1View(this);
	pWorkspace->addWindow(as);
	as->show();
}


empresa::empresa(){
   diario = NULL;
   extracto = NULL;
   balance1 = NULL;
   balance = NULL; 
   introapunts1 = NULL;
   selccostes  = NULL;
   selcanales = NULL;
   nombre = "";
}// end empresa


empresa::~empresa(){
  if (diario) delete diario;
  if (extracto) delete extracto;
  if (balance1) delete balance1;
  if (balance) delete balance;
  if (introapunts1) delete introapunts1;
  if (selccostes) delete selccostes;
  if (selcanales) delete selcanales;
}// end ~empresa

/** \brief inicializa la clase con el nombre de la base de datos y con el workspace.
  */
int empresa::inicializa1() {
  
  // Calculamos el numero de digitos que tiene el nivel ultimo de la empresa. 
   QString query = "SELECT * FROM configuracion WHERE nombre= 'CodCuenta'";
   cursor2 *cursoraux1 = cargacursor(query,"codcuenta");
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
  extracto = new extractoview1(this, 0,"extracto");
  pWorkspace->addWindow(extracto);
  diario = new diarioview1(this,0,"diario");
  pWorkspace->addWindow(diario);
  balance = new balanceview(this, 0,"balance");
  pWorkspace->addWindow(balance);
  balance1 = new balance1view(this, 0,"balance2");
  pWorkspace->addWindow(balance1);
  introapunts1 = new intapunts3view(this, 0,"introapunts2");
  pWorkspace->addWindow(introapunts1);
  
  /// Pasamos parametros a las ventanas para que puedan coordinarse entre si.\todo El paso de mensajes deber� pasar por la clase \ref empresa siempre.
  introapunts1->inicializa1(extracto, diario, balance);
  extracto->inicializa2(introapunts1, diario, balance);
  diario->inicializa2(introapunts1,extracto, balance);
  balance->inicializa2(introapunts1, diario, extracto);
  balance1->inicializa2(introapunts1, diario, extracto);  
  
  return(0);
}// end inicializa1

/** \brief Maximiza todas las ventanas que est� en el workspace de la aplicaci�
  * 
  * Envia un mensaje showMaximized a todos los objetos que deben ser maximizados
  */
void empresa::maximiza() {
  diario->showMaximized();
  extracto->showMaximized();
  balance1->showMaximized();
  balance->showMaximized();
  introapunts1->showMaximized();
}// end maximiza


void empresa::init(QString bd) {
    if (bd == "")
        bd = searchCompany();
    inicializa(bd);
}// end init


/** \brief Se utiliza para mostrar un selector de empresas \ref abreempresaview
  * Al usuario debe seleccionar una empresa y el sistema empieza la inicializacion de clases a partir de dicha inicializaci�.
  */
QString empresa::searchCompany() {
    //El cambio de empresa se realiza desde el selector.
    _depura("empresa::searchCompany vamos a mostrar el abreempresaview\n",0);
    abreempresaview *nuevae = new abreempresaview(0,"BulmaCont" );
    nuevae->exec();
    _depura("Vamos a cambiar la empresa \n",0);
    QString bd= nuevae->nomDB();
    _depura("Empresa cambiada a"+bd,0);
    delete nuevae;
    /// Si no se ha seleccionado ninguna base de datos entonces abortamos.
    if (bd == "")
        exit(1);
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
//La creaci� de una nueva empresa ha pasado al selector.
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
  nuevae->inicializa(this, introapunts1);
  nuevae->exec();
  delete nuevae;
  return(0);
}// end muestraapuntes


int empresa::propiedadempresa() {
   propiedadesempresa * nuevae = new propiedadesempresa(0,"",true);
   nuevae->inicializa(this);
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
   nuevae->inicializa(this);
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
   // Esto es lo m� correcto, lo anterior se llamar�desde esta nueva ventana.
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
   cursor2 *cur = cargacursor("SELECT abreasientos()");
   delete cur;   
   introapunts1->cargarcursor();
   introapunts1->boton_fin();
   introapunts1->show();
}// end Abrirasientos


void empresa::Ordenarasientos() {

   QString query= "SELECT reordenaasientosall()";
   cursor2 * cur = cargacursor(query, "hola");
   delete cur;
   introapunts1->cargarcursor();
   introapunts1->boton_fin();
   introapunts1->show();
}// end Abrirasientos


int empresa::registroiva() {
   listivaview *perd = new listivaview(this, "0");
   perd->inicializa(introapunts1);
   perd->exec();
   delete perd;
   return(0);
}// end registroiva

int empresa::modelo347() {
   BModelo347 *dlg347 = new BModelo347(this,"0");
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


// Esta funci� es la que se encarga de cerrar al empresa
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

// Esta funci� reemplaza en los apuntes todas las ocurrencias de una determinada cuenta por
// Otra cuenta.
void empresa::reemplazacuentaenasientos() {
    cambiactaview *ctac = new  cambiactaview(this,0,0, false);
    ctac->exec();
}// end reemplazacuentasenasientos


// Esta funci� recalcula los saldos parciales del plan contable.
void empresa::recalculasaldos() {
   ejecuta("SELECT recalculasaldos()");
}// end recalculasaldos





/** Esta funci� se dispara para poner en marcha la gesti� de cobros y pagos
  */
void empresa::cobPag() {
   cobropagoview *adoc= new cobropagoview(this,0,"hola");
   adoc->exec();
   delete adoc;
}// cobPag

selectccosteview *empresa::getselccostes() {return selccostes;}
selectcanalview *empresa::getselcanales() {return selcanales;}
void empresa::centrocostedefecto() {selccostes->exec();}
void empresa::canaldefecto() {selcanales->exec();}


