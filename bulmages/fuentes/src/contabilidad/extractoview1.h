/***************************************************************************
                          extractoview1.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
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

#ifndef EXTRACTOVIEW1_H
#define EXTRACTOVIEW1_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qdatetimeedit.h>
#include <qfiledialog.h>
#include <qwidget.h>
#include <qtable.h>
#include <qtoolbutton.h>
#include <qlayout.h>
#include <qpopupmenu.h>

#include "extractodlg1.h"
#include "listcuentasview1.h"
#include "diarioview1.h"
#include "qtable1.h"
#include "postgresiface2.h"
#include "intapunts3view.h"
#include "balanceview.h"
#include "filtrarextractosview.h"

class intapunts3view;
class diarioview1;
class balanceview;
class empresa;
/** *@author Tomeu Borrás Riera */

class extractoview1 : public extractodlg1  {
   Q_OBJECT
public:
  int numregistro;
  int numregistros;
  diarioview1 *diario;
  intapunts3view *introapunts;
  balanceview *balance;
  QTable1 *listado;
  postgresiface2* conexionbase;
  cursor2 *cursorcta;
  int numdigitos;
  int ccostes[200];
  filtrarextractosview *filt;
  empresa *empresaactual;    
public:
	extractoview1(empresa *,QWidget *parent=0, const char *name=0, int flags=0);
	~extractoview1();
   void inicializa1(QString, QString, QString, QString, int);
   void inicializa2(intapunts3view *, diarioview1 *, balanceview *);
   void vaciar();
   void presentar();
   void ajustes();
   void cargacostes();
   void boton_filtrar();

public slots:
   virtual void accept();
   virtual void boton_siguiente();
   virtual void boton_anterior();
   virtual void boton_fin();
   virtual void boton_inicio();
   virtual void boton_buscacuentainicial();
   virtual void boton_buscacuentafinal();
   virtual void boton_borrapunteo();
   virtual void boton_imprimir();
   virtual void boton_guardar();
   virtual void boton_asiento();
   virtual void boton_diario1(int);
   virtual void boton_fechainicial();
   virtual void boton_fechafinal();
   virtual void boton_balance1(int);
   virtual void return_codigoinicial();
   virtual void return_codigofinal();
   virtual void return_fechainicial();
   virtual void return_fechafinal();
   virtual void contextmenu(int , int , const QPoint &);
   virtual void apuntecambiadogrid (int, int);
   virtual void boton_casacion();
   virtual void boton_guardarpunteo();
   virtual void boton_cargarpunteos();
   virtual void codigo_textChanged(const QString &);
   virtual void fecha_textChanged(const QString &);
};

#endif
