/***************************************************************************
                          ainteligentesview.h  -  description
                             -------------------
    begin                : Thu Feb 6 2003
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

#ifndef AINTELIGENTESVIEW_H
#define AINTELIGENTESVIEW_H

#include <qwidget.h>
#include "ainteligentesdlg.h"

#include "postgresiface2.h"


/*** @author Tomeu Borrás Riera */
class empresa;


class ainteligentesview : public ainteligentesdlg  {
   Q_OBJECT
private:
   cursor2 *cainteligentes;
   empresa *empresaactual;
   int numdigitos;
   
private:
  postgresiface2 *conexionbase;
  int idasientointeligente;
  int ccostes[100];
  int ccanales[100];
  QComboBox *combocoste, *combocanal;
  int oldrow,oldcol;
  
public: 
  ainteligentesview(empresa *, QWidget *parent=0, const char *name=0, bool modal=true);
  ~ainteligentesview();
  void repinta();
  void return_asiento();
  void cargacanales();
  void cargacostes();
  void cargacombo();
  
public slots:
  virtual void boton_nuevo();
  virtual void boton_save();
  virtual void boton_borrar();
  virtual void boton_inicio();
  virtual void boton_fin();
  virtual void boton_siguiente();
  virtual void boton_anterior();
  virtual void boton_exportar();
  virtual void boton_importar();
  virtual void currentChanged(int, int);
  virtual void selectionChanged();
  virtual void boton_igualant();
  virtual void boton_cuentas();
  virtual void accept();
  virtual void comboActivated (const QString &);
};

#endif
