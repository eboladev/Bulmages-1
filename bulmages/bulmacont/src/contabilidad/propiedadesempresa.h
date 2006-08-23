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

#include "ui_propiedadesempresabase.h"
#include <qlineedit.h>
#include <q3listview.h>
#include <q3table.h>
#include <stdlib.h>
#include "postgresiface2.h"
#include "dialogchanges.h"

/** *@author Tomeu Borrás Riera */

class empresa;

class propiedadesempresa : public QWidget, Ui_PropiedadesEmpresaBase , dialogChanges {
   Q_OBJECT
public:
   QString empresadb;
   empresa *m_companyact;
public: 
  propiedadesempresa(empresa *emp, QWidget *parent=0);
  ~propiedadesempresa();
  int inicializa();

public slots:
  virtual void s_saveConfig();
  virtual bool close(bool);
  virtual void extiendeCuentas();
  private:
  void update_value(QString ,QString);
};
#endif
