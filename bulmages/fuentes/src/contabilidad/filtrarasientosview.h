/***************************************************************************
                          filtrarasientosview.h  -  description
                             -------------------
    begin                : Sat Jan 4 2003
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

#ifndef FILTRARASIENTOSVIEW_H
#define FILTRARASIENTOSVIEW_H

#include <qwidget.h>
#include <qlineedit.h>
#include <filtrarasientosdlg.h>
#include <qcombobox.h>

#include "postgresiface2.h"

class empresa;

/***@author Tomeu Borrás Riera */

class filtrarasientosview : public filtrarasientosdlg  {
Q_OBJECT
private:
  postgresiface2* conexionbase;
  int numdigitos;
  empresa *empresaactual;  
public: 
	filtrarasientosview(empresa *,QWidget *parent=0, const char *name=0);
	~filtrarasientosview();
	QString ejercicio() {return m_ejercicio->text(m_ejercicio->currentItem());};
public slots:
  virtual void boton_reset();
  virtual void boton_ccostes();
  virtual void boton_canales();
};

#endif
