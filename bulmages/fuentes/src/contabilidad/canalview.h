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
/***@author Tomeu Borrás Riera */

class canalview : public canaldlg  {
   Q_OBJECT
public:
  postgresiface2 *conexionbase;
  int canales[100];
  int idcanal;    // Indica cual es el canal que se esta visualizando
                    // Si su valor es 0 entonces es que no se está visualizando ningun centro de coste.

public: 
	canalview(QWidget *parent=0, const char *name=0,  bool modal=true);
	~canalview();
  void inicializa(postgresiface2 *);
  void mostrarplantilla();

public slots:
  void cambiacombo(int);
  void boton_guardar();
  void boton_nuevo();
  void boton_borrar();  
};

#endif
