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


/** * @author Tomeu Borrás Riera */

class empresa {
private:
  QString nombre;
  QString EjercicioActual;     // Indica cual es el ejercicio actual de la base de datos.
  int ano;
  QString contrasenya;
  QWorkspace *pWorkspace;
//  postgresiface1 *conexionbase; // LA conexion con la base de datos
  postgresiface2 *conexionbase2;  // Usaremos la nueva versión de postgresiface, durante un tiempo deberá convivir con la version anterior.
  
//  postgresiface1 *conexionanterior;       // La base de datos del ejercicio anterior de la empresa
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

  int m_idcCosteDef;  // El centro de coste por defecto.
  int m_idCanalDef; // El canal por defecto.

public:
  empresa();
  ~empresa();
  postgresiface2 *bdempresa() {return (conexionbase2);}
  intapunts3view *intapuntsempresa() {return(introapunts1);}
  int numdigitosempresa() { return(numdigitos);}
  QString nomuserempresa() { return(nombre);}
  QString nombreempresa() {return(nombre);}
  QString ejercicioactual() {return(EjercicioActual);}
  void setejactual(QString ej) {EjercicioActual=ej;}
  int inicializa(QString * DB=0, QString * User=0, QString * Passwd=0);
  int idcCosteDef() const {return m_idcCosteDef;}
  int idCanalDef() const {return m_idCanalDef;}
  void setidcCosteDef(int a) { m_idcCosteDef=a;}
  void setidCanalDef(int a) {m_idCanalDef=a;}
  
  int muestracuentas();
  int nuevacuenta();
  int muestraapuntes();
  int muestraapuntes1();
  int muestraasientos();
  int propiedadempresa();
  int guardarempresa();
  int nuevaempresa();
  int cargarempresa();
  int borrarempresa();
  int cambiarempresa();
  int libromayor();
  int librodiario();
  int librobalance();
  int librobalancetree();
  int registroiva();
  int modelo347();
  int inicializa1(QWorkspace *);
  int boton_siguiente();
  int boton_anterior();
  int boton_primero();
  int boton_ultimo();
  int boton_guardar();
  int boton_imprimir();
  int boton_reload();
  int ainteligentes();
  void testnewdb();
  int ccostes();
  int canales();
  void cierraempresa();
  void abreempresa();
  void regularizaempresa();
  void maximiza();
  void Abrirasientos();
  void Ordenarasientos(int);
  void SegCuentas();
  void BalanceGrafico();
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

  QString nombreDB;
};
#endif
