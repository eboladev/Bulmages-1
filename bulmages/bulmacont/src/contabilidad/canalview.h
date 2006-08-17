/***************************************************************************
                          canalview.h  -  description
                             -------------------
    begin                : lun mar 24 2003
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

#ifndef CANALVIEW_H
#define CANALVIEW_H

#include "ui_canalbase.h"
#include <qwidget.h>
#include <qcombobox.h>
#include <qlineedit.h>
#include <q3textedit.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

/** *@author Tomeu Borr� Riera */

class empresa;

class canalview : public QWidget, public Ui_canaldlg, public dialogChanges {
   Q_OBJECT
public:
  postgresiface2 *conexionbase;
  int canales[100];
  int idcanal;    // Indica cual es el canal que se esta visualizando
                    // Si su valor es 0 entonces es que no se est�visualizando ningn centro de coste.
  empresa *empresaactual;
public: 
	canalview(empresa *, QWidget *parent=0);
	~canalview();
  void mostrarplantilla();
  void pintar();

public slots:
  virtual void on_mui_idcanal_valueChanged(QString);
  virtual void on_mui_guardar_clicked();
  virtual void on_mui_crear_clicked();
  virtual void on_mui_borrar_clicked();  
  virtual void closeEvent(QCloseEvent *);
};

#endif
