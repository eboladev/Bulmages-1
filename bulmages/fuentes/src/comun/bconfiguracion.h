/***************************************************************************
 *   Copyright (C) 2003 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 ***************************************************************************/
#ifndef BCONFIGURACION_H
#define BCONFIGURACION_H

#include "uiconfiguracion.h"
#include "postgresiface2.h"
#include <qlistview.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qtextedit.h>
#include <qpopupmenu.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qfontdatabase.h>

/**
@author Josep Burcion
*/

class BConfiguracion : public UIconfiguracion
{

public:
    BConfiguracion(QWidget * parent = 0, const char * name = 0, WFlags f = WType_TopLevel);
    ~BConfiguracion();

private:
    void cargarFichaUtilidades();
    void cargarFichaUsuarios();
    
private slots:
  virtual void cerrar();
  virtual void listView1_currentChanged(QListViewItem *);
  virtual void listView2_clickBotonDerecho(QListViewItem*,const QPoint&,int);
  virtual void FontChanged(const QString &);
  virtual void FontSizeChanged(int);
  virtual void BotonA_10aceptar();
  virtual void BotonA_11rechazar();
};

#endif
