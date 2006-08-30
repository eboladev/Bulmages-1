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
#ifndef COBROPAGOVIEW_H
#define COBROPAGOVIEW_H

#include "ui_cobropagobase.h"

/**
  * @author Tomeu Borrás Riera 
  * \class cobropagoview cobropagoview.h
  * \brief Listado de gestion de cobros y pagos.
  */

#include "empresa.h"
class postgresiface2;

class cobropagoview : public QDialog, public Ui_CobroPagoBase {
    Q_OBJECT
public:
    postgresiface2 *conexionbase;
    empresa *empresaactual;
    int numdigitos;
public:
    cobropagoview(empresa *, QWidget *parent = 0, const char *name = 0);
    ~cobropagoview();
public slots:
    virtual void s_actualizar();
    virtual void s_guardar();
    virtual void s_recalculaSaldo();
};

#endif
