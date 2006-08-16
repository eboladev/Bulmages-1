/***************************************************************************
                          ccosteview.h  -  description
                             -------------------
    begin                : s� mar 22 2003
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

#ifndef CCOSTEVIEW_H
#define CCOSTEVIEW_H

#include <qwidget.h>
#include "ui_ccostebase.h"
#include <qcombobox.h>
#include <qlineedit.h>
#include <q3textedit.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

/** *@author Tomeu Borras Riera */
class empresa;

class ccosteview : public QWidget, public Ui_ccostedlg, public dialogChanges  {
  Q_OBJECT
public:
  postgresiface2 *conexionbase;
  int ccostes[100];
  int idc_coste;    // Indica cual es el centro de coste que se esta visualizando
                    // Si su valor es 0 entonces es que no se est�visualizando ningun centro de coste.
  empresa *empresaactual;
public:
  ccosteview(empresa *, QWidget *parent=0);
  ~ccosteview();
  void pintar();
  void mostrarplantilla();
  
public slots:
//  virtual void cambiacombo(int);
  virtual void on_mui_guardar_clicked();
  virtual void on_mui_crear_clicked();
  virtual void on_mui_borrar_clicked();
  virtual void on_mui_list_itemClicked(QTreeWidgetItem *, int);
  virtual void closeEvent(QCloseEvent *);
};

#endif
