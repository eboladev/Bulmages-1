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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef BSELECTOR_H
#define BSELECTOR_H

#include <qevent.h>

#include "abreempresaview.h"
#include "uiselector.h"
#include "bconfiguracion.h"

/** @author Josep Burcion */

class BSelector : public UIselector {
 Q_OBJECT

public:
   QString empresabd;
   QString loggin;
   QString password;
   QString ejercicio;   // Indica el ejercicio si hace falta
   QString tipo;        // Indica si va a ser contabilidad o facturación
 
public:
    BSelector(QWidget * parent=0,const char * name=0);
    ~BSelector();

public slots:
  virtual void seleccionaempresa_clicked();
  
private slots:
  virtual void salir_clicked();
  virtual void configura_clicked();
  virtual void m_iglues_clicked();
  virtual void m_bulmatpv_clicked();
  virtual void contabilidad_clicked();
  virtual void produccion_clicked();
  virtual void almacen_clicked();
  virtual void m_bulmafact_clicked();
};

#endif
