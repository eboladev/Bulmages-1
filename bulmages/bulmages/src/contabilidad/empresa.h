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
#include "postgresiface2.h"


/** * @author Tomeu Borrás Riera */

class empresa {
private:
  char *nombre;
  char nombredb[20];
  char *contrasenya;
  int ano;
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

public:
  empresa();
  ~empresa();
  postgresiface2 *bdempresa() {return (conexionbase2);}
  intapunts3view *intapuntsempresa() {return(introapunts1);}
  int numdigitosempresa() { return(numdigitos);}
  int inicializa(QString * DB=0, QString * User=0, QString * Passwd=0);
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
  void Ordenarasientos();
  void SegCuentas();
  void BalanceGrafico();
  void Filtro();
  void nuevoejercicio();
  int mpatrimoniales();
  int compbalance();
  int amortizaciones();
  void reemplazacuentaenasientos();

};
#endif
