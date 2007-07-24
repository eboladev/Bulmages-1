/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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

#include "blwidget.h"

PEmpresaBase::PEmpresaBase(EmpresaBase *emp) {
  m_empresaBase = emp;
}

PEmpresaBase::PEmpresaBase() {
  m_empresaBase = NULL;
}

PEmpresaBase::~PEmpresaBase() {
  if(m_empresaBase != NULL)
    delete m_empresaBase;
}

void PEmpresaBase::setEmpresaBase(EmpresaBase *emp) {
  m_empresaBase = emp;
}

EmpresaBase *PEmpresaBase::empresaBase() {
  return m_empresaBase;
}

BLWidget::BLWidget(QWidget *parent, Qt::WFlags f) 
  :QWidget(parent, f), PEmpresaBase() {
  _depura("BLWidget::BLWidget", 0);
  _depura("END BLWidget::BLWidget", 0);
}

BLWidget::BLWidget(EmpresaBase *emp, QWidget *parent, Qt::WFlags f) 
  :QWidget(parent, f), PEmpresaBase(emp) {
  _depura("BLWidget::BLWidget", 0);
  _depura("END BLWidget::BLWidget", 0);
}

BLWidget::~BLWidget() {
  _depura("BLWidget::~BLWidget", 0);
  _depura("END BLWidget::~BLWidget", 0);
}
