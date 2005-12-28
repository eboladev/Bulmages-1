/***************************************************************************
                          ccosteview.h  -  description
                             -------------------
    begin                : sáb mar 22 2003
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

#ifndef CCOSTEVIEW_H
#define CCOSTEVIEW_H

#include <qwidget.h>
#include <ccostedlg.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <q3textedit.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

/** *@author Tomeu Borrás Riera */
class empresa;

class ccosteview : public ccostedlg, dialogChanges  {
  Q_OBJECT
public:
  postgresiface2 *conexionbase;
  int ccostes[100];
  int idc_coste;    // Indica cual es el centro de coste que se esta visualizando
                    // Si su valor es 0 entonces es que no se está visualizando ningun centro de coste.
  empresa *empresaactual;
  
  int col_idc_coste, col_desc_coste, col_nom_coste; // Los indices de las columnas de los items.
public:
  ccosteview(empresa *, QWidget *parent=0, const char *name=0, bool modal=true);
  ~ccosteview();
  void pintar();
  void mostrarplantilla();
  
public slots:
//  virtual void cambiacombo(int);
  virtual void boton_guardar();
  virtual void boton_nuevo();
  virtual void boton_borrar();
  virtual void seleccionado(Q3ListViewItem *);
  virtual void close();
};

#endif
