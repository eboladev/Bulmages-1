/***************************************************************************
                          empresa.h  -  description
                             -------------------
    begin                : Wed Nov 27 2002
    copyright            : (C) 2002 by Tomeu Borrás
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

#ifndef EMPRESA_H
#define EMPRESA_H

#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qstring.h>
#include <qsqldatabase.h>
#include <qworkspace.h>
#include <qwidget.h>
#include <qsizepolicy.h>


#include "intapunts3view.h"
#include "balanceview.h"
#include "balance1view.h"
#include "diarioview1.h"
#include "extractoview1.h"
#include "bmodelo347.h"
#include "postgresiface2.h"
#include "log.h"
#include "selectccosteview.h"
#include "selectcanalview.h"


/**
 * @author Tomeu Borrás Riera
 * \class empresa empresa.h
 * \brief Soporte principal para cada empresa contable
 *  
 * Clase que soporta todas las funciones de la empresa contable. Supuestamente el programa podrá tener múltiples empresas abiertas al mismo tiempo.
 * Esta clase es el corazón del programa donde acaban confluyendo todos los demás objetos.
 * Se encarga de instanciar los objetos necesarios, inicializarlos e invocarlos cuando es preciso.
 * También debe servir como punto de comunicación entre los diferentes objetos de modo que éstos sólo tengan referencias a esta clase y haciendo el flujo del código más fácil de entender.
 * \todo proporcionar la funcionalidad de comunicación entre las otras clases.
 */
class empresa {
private:
  QString nombre;
  int ano;
  QString contrasenya;
  QWorkspace *pWorkspace;
  postgresiface2 *conexionbase2;  // Usaremos la nueva versión de postgresiface, durante un tiempo deberá convivir con la version anterior.
  postgresiface2 *conexionanterior2;
  
  balance1view *balance1;  
  diarioview1 *diario;
  extractoview1 *extracto;
  balanceview *balance;
  intapunts3view *introapunts1; // introapunts.  
  int numdigitos;  // El numero de digitos que tienen las cuentas.
  
  // Hay unos selectores generales, que van a ser estos y que indican con que se trabaja, luego
  // Estos selectores generales se podran invocar desde distintas partes.
  selectccosteview *selccostes;
  selectcanalview *selcanales;
public:
  QString nombreDB;
  
public:
  empresa();
  ~empresa();
  /// Returns a pointer to the database class
  postgresiface2 *bdempresa() {return (conexionbase2);}
  intapunts3view *intapuntsempresa() {return(introapunts1);}
  int numdigitosempresa() { return(numdigitos);}
  QString nomuserempresa() { return(nombre);}
  QString nombreempresa() {return(nombre);}
  
  int muestracuentas();
  int nuevacuenta();
  int muestraapuntes();
  int muestraapuntes1();
  int muestraasientos();
  int propiedadempresa();
  int nuevaempresa();
  int borrarempresa();
  /// This function search for a comapany
  QString searchCompany();
  int libromayor();
  int librodiario();
  int librobalance();
  int librobalancetree();
  int registroiva();
  int modelo347();
  int inicializa1(QString, QWorkspace *);
  int boton_siguiente();
  int boton_anterior();
  int boton_primero();
  int boton_ultimo();
  int boton_guardar();
  int boton_imprimir();
  int boton_reload();
  int ainteligentes();
  int ccostes();
  int canales();
  int tiposIVA();
  int fPago();
  void cierraempresa();
  void abreempresa();
  void regularizaempresa();
  void maximiza();
  void Abrirasientos();
  void Ordenarasientos();
  void Filtro();
  void nuevoejercicio();
  int mpatrimoniales();
  int compbalance();
  int amortizaciones();
  void reemplazacuentaenasientos();
  int cambioejercicio();
  void centrocostedefecto(){   selccostes->exec();}
  void canaldefecto() {   selcanales->exec();}
  selectccosteview * getselccostes() {return selccostes;}
  selectcanalview *getselcanales() {return selcanales;}
  void recalculasaldos();
  void cobPag();  // Gestion de cobros y pagos
};
#endif
