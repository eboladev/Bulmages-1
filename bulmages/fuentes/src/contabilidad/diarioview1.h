/***************************************************************************
                          diarioview1.h  -  description
                             -------------------
    begin                : Thu Jan 9 2003
    copyright            : (C) 2003 by Tomeu Borrï¿½ Riera
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

#ifndef DIARIOVIEW1_H
#define DIARIOVIEW1_H

#include "diariodlg1.h"

#include <string>
using std::string;

#include "qlineedit1.h"
#include "qtable1.h"
#include "postgresiface2.h"

#include "filtrardiarioview.h"
#include "intapunts3view.h"
#include "extractoview1.h"
#include "balanceview.h"

class empresa;
class intapunts3view;
class extractoview1;
class balanceview;

/** 
  * @author Tomeu Borrás Riera
*/

class diarioview1 : public diariodlg1  {
   Q_OBJECT
public:
   intapunts3view *introapunts;
   extractoview1 *extracto;
   balanceview *balance;
   QTable1 *listado;
   QLineEdit1 *finicial1;
// Esta variable nos sirve para hacer las pruebas iniciales con las fechas.
   postgresiface2 *conexionbase;
   int ccostes[200];
   filtrardiarioview *filt;
   empresa *empresaactual;
public:
   diarioview1(empresa *, QWidget *parent=0, const char *name=0, int flags=0);
   ~diarioview1();
   void accept();
   void inicializa1(QString, QString, int);
   void inicializa2(intapunts3view *, extractoview1 *, balanceview *);
   void presentar();
   void boton_filtrar();
   
public slots:
   virtual void boton_imprimir();
   virtual void boton_guardar();
	virtual void boton_extracto1(int);
	virtual void boton_balance1(int);
   virtual void boton_asiento();
   virtual void return_fechainicial();
   virtual void return_fechafinal();
   virtual void contextmenu(int , int , const QPoint &);   
   virtual void textChanged(const QString &);
   virtual void boton_fechainicial();
   virtual void boton_fechafinal();
};
#endif
