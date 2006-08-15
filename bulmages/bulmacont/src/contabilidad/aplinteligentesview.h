/***************************************************************************
                          aplinteligentesview.h  -  description
                             -------------------
    begin                : Mon Feb 10 2003
    copyright            : (C) 2003 by Tomeu Borr� Riera
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

#include "ui_aplinteligentesbase.h"

#include <qwidget.h>
#include <stdlib.h>
#include <stdio.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <q3table.h>
#include <q3groupbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qdatetime.h>
#include <qpixmap.h>
#include "listcuentasview1.h"
#include "funcaux.h"
#include "postgresiface2.h"

#include <QDomDocument>
#include "busquedafecha.h"
#include "busquedacuenta.h"

class empresa;
/** 
  * @author Tomeu Borr� Riera 
  * \class aplinteligentesview aplinteligentesview.h
  * \brief Formulari�para la introducci� de asientos mediante las plantillas.
  * Clase que sirve para introducir asientos en el sistema mediante el uso de plantillas predefinidas, o asientos plantilla (\ref ainteligentesview).
  
 A partir de la plantilla se genera el formulario de forma din�ica. Y una vez introducidos los datos se hacen calculos en un bucle resolvedor de incognitas recursivas. Si no hay bucles infinitos al final se obtienen todos los campos necesarios para hacer la introducci� de asientos y esta se efectua.
 
 	La clase tiene dos modos de actuar dependiendo de si estamos trabajando con un asiento abierto o con uno cerrado. En el primer caso el comportamiento es de todo lo introducido al asiento abierto.
	En el segundo caso se genera un asiento nuevo por cada introducci� de plantilla.
  */

class aplinteligentesview : public QDialog,  public Ui_AplInteligentesBase  {
   Q_OBJECT
private:
/// LA empresa del programa.
  empresa *companyact;

 QDomDocument m_doc;


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
  
  int modo; // Si modo==0 entonces es modo normal. Si modo==1 entonces es modo 1 ejecuci�. 
           // El modo provoca que la plantilla de asiento inteligente se cierre tras haber pulsado
           // El bot� de aceptar y asi permite recuperar el control a otros estamentos.
           // Por defecto el modo es 0.

  QLabel    *labelcta[100];
  QLabel    *labelfecha[100];
  QLabel    *labelnumero[100];
  QLabel    *labeltexto[100];
  BusquedaCuenta *varcta[100];
  BusquedaFecha *varfecha[100];
  QLineEdit *varnumero[100];
  QLineEdit *vartexto[100];
/*  QToolButton *boton_cta[100];*/
  
  int numainteligente; // Esta variable indica el idainteligente que estamos usando como plantilla.
  int numasiento;       // Esta variable indica en que asiento se va a introducir el asiento inteligente.
  QString listasientos[100];  // Esta lista se usa en conjuncion con el combo-box para saber los incices de las plantillas de asientos inteligentes.
  int idainteligente;
  unsigned int numdigitos;
public: 
  aplinteligentesview(empresa *, QWidget *parent=0, const char *name=0);
  ~aplinteligentesview();
  void inicializa(int);
  void mostrarplantilla();
  void recogevalores();
  void creaasiento();
  void recogevariables(QString, int);
  QString aplicavariable(QString);
  void borrawidgets();
  void selectsiguiente(QObject *);
  void inicializavariables();
//  void inicializavariablesapunte(int);
  void cifcuenta(int);
  void selectfirst();
  void muestraplantilla(QString);
  void setvalores(QString, QString);
  void setfechaasiento(QString fecha) {fechaasiento->setText(fecha);}
  
  // La funcion sirve para especificar el modo de funcionamiento de los asientos intelgientes.
  // Consultar la variable m�odo para m� detalles.
  void setmodo(int i) { modo=i;}
  
public slots:
  virtual void on_mui_aceptar_clicked();
  virtual void return_numero();
  virtual void return_texto();
  virtual void on_mui_comboainteligentes_activated(int index);
};

#endif
