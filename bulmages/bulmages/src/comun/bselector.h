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

#include "selectorbase.h"

#include <qevent.h>
#include "abreempresaview.h"
#include "bconfiguracion.h"

/** @author Josep Burcion 
  * @brief Muestra un selector de aplicaciones y configuraci�
  */

class BSelector : public selectorbase {
Q_OBJECT

private:
/// Nombre de la base de datos que representa a la empresa.
   QString m_empresabd;
/// Indica si va a ser contabilidad o facturaci�   
   QString m_tipoempresa;        
 
public:
    BSelector(QWidget * parent=0,const char * name=0);
    ~BSelector();
    QString empresaDB() {return m_empresabd;};
    void setEmpresaBD(QString emp) {m_empresabd=emp;};
    QString tipoEmpresa() {return m_tipoempresa;};
    void setTipoEmpresa(QString tip) {m_tipoempresa = tip;};

public slots:
  virtual void seleccionaempresa_clicked();
  
private slots:
  virtual void salir_clicked();
  virtual void configura_clicked();
  virtual void m_iglues_clicked();
  virtual void m_bulmatpv_clicked();
  virtual void contabilidad_clicked();
  virtual void produccion_clicked();
  virtual void m_bcontaweb_clicked();
  virtual void m_bulmafact_clicked();
};

#endif
