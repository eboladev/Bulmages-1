/***************************************************************************
                          usuariosview.h  -  description
                             -------------------
    begin                : vie abr 4 2003
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

#ifndef USUARIOSVIEW_H
#define USUARIOSVIEW_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qtimer.h>
#include <qstring.h>
#include <stdio.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include "postgresiface2.h"
#include <usuariosdlg.h>
#include "configuracion.h"

/**  *@author Tomeu Borrás Riera */

class usuariosview : public usuariosdlg  {
  Q_OBJECT
public:
  int semaforo;
public: 
  usuariosview(QWidget *parent=0, const char *name=0);
  ~usuariosview();
  void inicializa();
public slots:
  void listusuarios_currentChanged(QListViewItem *);
  void datos_textChanged(const QString &);
  void accept();
  void nuevo_usuario();
  void borrar_usuario(); 
};

#endif
