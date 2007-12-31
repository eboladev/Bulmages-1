/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   http://www.iglues.org                                                 *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef BLWIDGET_H
#define BLWIDGET_H

#include <QWidget>
#include <QCloseEvent>

#include "dialogchanges.h"
#include "funcaux.h"
#include "empresabase.h"


class PEmpresaBase {
private:
  /// Indica si es modo consulta o modo edicion. (altera el comportamiento del
  /// doble click sobre la lista)
  EmpresaBase *m_empresaBase;

public:
  PEmpresaBase(EmpresaBase *);
  PEmpresaBase();
  virtual ~PEmpresaBase();
  virtual void setEmpresaBase(EmpresaBase *);
  virtual EmpresaBase *empresaBase();
  virtual QString valorCampo();
  virtual void setValorCampo(QString );
};


class BLWidget : public QWidget, public PEmpresaBase {
  Q_OBJECT

public:
  BLWidget(QWidget *parent = 0, Qt::WFlags f = 0);
  BLWidget(EmpresaBase *, QWidget *parent = 0, Qt::WFlags f = 0);
  virtual ~BLWidget();
};

#endif

