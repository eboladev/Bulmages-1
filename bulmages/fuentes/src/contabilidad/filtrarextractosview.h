/***************************************************************************
                          filtrarextractosview.h  -  description
                             -------------------
    begin                : dom jul 27 2003
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
#ifndef FILTRAREXTRACTOSVIEW_H
#define FILTRAREXTRACTOSVIEW_H

#include <qwidget.h>
#include <qpopupmenu.h>
#include <qcombobox.h>

#include "filtrarextractosdlg.h"
#include "postgresiface2.h"
#include "selectccosteview.h"
#include "selectcanalview.h"

class empresa;

/***@ author Tomeu Borrás Riera  */

class filtrarextractosview : public filtrarextractosdlg  {
Q_OBJECT
private:
  postgresiface2* conexionbase;
  int numdigitos;
  empresa *empresaactual;   
   int m_colNomCoste;
   int m_colDescCoste;
   int m_colIdCoste;
   int m_colStatusCoste;  
public:
  filtrarextractosview(empresa *, QWidget *parent=0, const char *name=0);
  ~filtrarextractosview();

public slots:
  virtual void codigo_textChanged(const QString &);
  virtual void boton_buscacontrapartida();
  virtual void return_codigo();
  virtual void boton_ccostes();
  virtual void boton_canales();
};

#endif

