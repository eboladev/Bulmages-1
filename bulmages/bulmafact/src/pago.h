/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef PAGO_H
#define PAGO_H

#include <QString>

#include "company.h"
#include "dbrecord.h"
#include "fichabf.h"

/// Almacena la informaci&oacute;n de un pago.
/** */
class Pago : public FichaBf {
public:
    Pago(company *, QWidget *);
    virtual ~Pago();
    virtual int cargar(QString);
    virtual void pintar();
    virtual int guardar();
    virtual int borrar();
    void vaciar();
    virtual void pintaidpago(QString) {}
    ;
    virtual void pintaidproveedor(QString) {}
    ;
    virtual void pintafechapago(QString) {}
    ;
    virtual void pintacantpago(QString) {}
    ;
    virtual void pintarefpago(QString) {}
    ;
    virtual void pintaprevisionpago(QString) {}
    ;
    virtual void pintacomentpago(QString) {}
    ;
    virtual void pintaidbanco(QString) {};
    void setidpago(QString val);
    void setidproveedor(QString val);
    void setfechapago(QString val);
    void setcantpago(QString val);
    void setrefpago(QString val);
    void setprevisionpago(QString val);
    void setcomentpago(QString val);
    void setidbanco(QString val);
};

#endif

