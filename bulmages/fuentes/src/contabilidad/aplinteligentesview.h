/***************************************************************************
                          aplinteligentesview.h  -  description
                             -------------------
    begin                : Mon Feb 10 2003
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

#ifndef APLINTELIGENTESVIEW_H
#define APLINTELIGENTESVIEW_H

#include <qwidget.h>
#include <aplinteligentesdlg.h>
#include <stdlib.h>
#include <stdio.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qtable.h>
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include "listcuentasview1.h"
#include "funcaux.h"
#include "postgresiface2.h"
#include "intapunts3view.h"

class empresa;
/** *@author Tomeu Borrás Riera */

class aplinteligentesview : public aplinteligentesdlg  {
   Q_OBJECT
private:
  empresa *empresaactual;
  postgresiface2 *conexionbase;
  QString variablescta[100][3];
  QString variablesfecha[100][3];
  QString variablesnumero[100][3];
  QString variablestexto[100][3];
  QString variablespredefinidas[100][3];
  QString variablesapunte[100][3];		// Las variables de una punte

  int indvariablescta;
  int indvariablesfecha;
  int indvariablesnumero;
  int indvariablestexto;
  int indvariablespredefinidas;
  int indvariablesapunte;					// El indice a las variables de apunte
  intapunts3view *intapunts;
  
  int modo; // Si modo==0 entonces es modo normal. Si modo==1 entonces es modo 1 ejecución. 
           // El modo provoca que la plantilla de asiento inteligente se cierre tras haber pulsado
           // El botón de aceptar y asi permite recuperar el control a otros estamentos.
           // Por defecto el modo es 0.

  QLabel    *labelcta[100];
  QLabel    *nomcta[100];
  QLabel    *labelfecha[100];
  QLabel    *labelnumero[100];
  QLabel    *labeltexto[100];
  QLineEdit *varcta[100];
  QLineEdit *varfecha[100];
  QLineEdit *varnumero[100];
  QLineEdit *vartexto[100];
  QToolButton *boton_cta[100];
  
  int numainteligente; // Esta variable indica el idainteligente que estamos usando como plantilla.
  int numasiento;       // Esta variable indica en que asiento se va a introducir el asiento inteligente.
  int listasientos[100];  // Esta lista se usa en conjuncion con el combo-box para saber los incices de las plantillas de asientos inteligentes.
  int idainteligente;
  unsigned int numdigitos;
public: 
  aplinteligentesview(empresa *, QWidget *parent=0, const char *name=0);
  ~aplinteligentesview();
  void inicializa(int, intapunts3view *);
  void mostrarplantilla();
  void recogevalores();
  void creaasiento();
  void recogevariables(QString, int);
  QString aplicavariable(QString);
  void borrawidgets();
  void selectsiguiente(QLineEdit *);
  void inicializavariables();
//  void inicializavariablesapunte(int);
  void cifcuenta(int);
  void selectfirst();
  void muestraplantilla(int);
  void setvalores(QString, QString);
  void setfechaasiento(QString fecha) {fechaasiento->setText(fecha);}
  
  // La funcion sirve para especificar el modo de funcionamiento de los asientos intelgientes.
  // Consultar la variable modo para más detalles.
  void setmodo(int i) { modo=i;}
  
public slots:
  virtual void boton_crear();
  virtual void cambiada_plantilla(int);
  virtual void return_cta();
  virtual void return_fecha();
  virtual void return_numero();
  virtual void return_texto();
  virtual void boton_buscacuenta();  
  virtual void fecha_textChanged(const QString &);
  virtual void codigo_textChanged(const QString &);
};

#endif
