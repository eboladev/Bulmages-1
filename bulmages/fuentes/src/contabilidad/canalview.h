/***************************************************************************
                          canalview.h  -  description
                             -------------------
    begin                : lun mar 24 2003
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

#ifndef CANALVIEW_H
#define CANALVIEW_H

#include <canaldlg.h>
#include <qwidget.h>
#include <ccostedlg.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

/** *@author Tomeu Borrás Riera */

class empresa;

class canalview : public canaldlg , dialogChanges {
   Q_OBJECT
public:
  postgresiface2 *conexionbase;
  int canales[100];
  int idcanal;    // Indica cual es el canal que se esta visualizando
                    // Si su valor es 0 entonces es que no se está visualizando ningun centro de coste.
  empresa *empresaactual;
public: 
	canalview(empresa *, QWidget *parent=0, const char *name=0,  bool modal=true);
	~canalview();
  void mostrarplantilla();
  void pintar();

public slots:
  virtual void cambiacombo(int);
  virtual void boton_guardar();
  virtual void boton_nuevo();
  virtual void boton_borrar();  
  virtual void close();
};

#endif
