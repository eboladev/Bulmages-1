/***************************************************************************
                          propiedadesempresa.h  -  description
                             -------------------
    begin                : Fri Nov 29 2002
    copyright            : (C) 2002 by Tomeu Borrás Riera
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

#ifndef PROPIEDADESEMPRESA_H
#define PROPIEDADESEMPRESA_H

#include <qwidget.h>
#include <propiedemp.h>
#include <qlineedit.h>
#include <qlistview.h>
#include <qtable.h>
#include <stdlib.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

/** *@author Tomeu Borrás Riera */

class propiedadesempresa : public propiedemp , dialogChanges {
   Q_OBJECT
public:
   QString empresadb;
   postgresiface2 *conexionbase;
   
public: 
  propiedadesempresa(QWidget *parent=0, const char *name=0,bool modal=true);
  ~propiedadesempresa();
  int inicializa(postgresiface2 *);
//  void modificacodcuenta(char *);
//  void accept();
public slots:
  virtual void s_saveConfig();
  virtual void close();
  
  private:
  void update_value(postgresiface2 *,QString ,QString);
};
#endif
