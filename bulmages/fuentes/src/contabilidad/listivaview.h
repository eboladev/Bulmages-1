/***************************************************************************
                          listivaview.h  -  description
                             -------------------
    begin                : Thu Jan 30 2003
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
/***************************************************************************
      LISTA DE BUGS CONOCIDOS
      
      - Las cuentas del estilo 47200477 o 47700472 deformaran los resultados
       ya que la busqueda esta hecha con un LIKE que no contempla estos casos.

      - Las fechas incorrectas producen mal funcionamiento pej: 31/06/2003 no
      dara ningun registro pq dia 31 no existe.
 ***************************************************************************/



 
#ifndef LISTIVAVIEW_H
#define LISTIVAVIEW_H

#include <qwidget.h>
#include <listivadlg.h>
#include <qlineedit.h>
#include <qtable.h>
#include <qpopupmenu.h>



#include "postgresiface2.h"
#include "intapunts3view.h"

/** *@author Tomeu Borrás Riera */

class empresa;

class listivaview : public listivadlg  {
   Q_OBJECT
public:
  empresa *empresaactual;
  postgresiface2 *conexionbase;
  intapunts3view *introapunts;

public: 
	listivaview(empresa *, QString ejerActual="",QWidget *parent=0, const char *name=0);
	~listivaview();
  void inicializa(intapunts3view *);
public slots:
  virtual void doble_click_soportado(int,int,int, const QPoint &);
  virtual void doble_click_repercutido(int,int,int, const QPoint &);
  virtual void boton_reload();
  virtual void boton_print();
  virtual void menu_contextual(int, int, const QPoint &);
  virtual void menu_contextual1(int, int, const QPoint &);
  virtual void boton_finicial();
  virtual void boton_ffinal();
  virtual void finicial_textChanged(const QString &);
  virtual void ffinal_textChanged(const QString &);
  virtual void finicial_lostFocus();
  virtual void ffinal_lostFocus();
};

#endif
