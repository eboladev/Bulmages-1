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

#ifndef COBRO_H
#define COBRO_H

#include <QString>
#include <QObject>
#include <QWidget>

#include "company.h"
#include "dbrecord.h"
#include "fichabf.h"


/** Clase que interactua entre la pantalla de Cobro y la base de datos.
    Incorpora los metodos de guardado de datos y abstraye metodos de visualizacion. */
class Cobro : public FichaBf {
public:
    Cobro(company *, QWidget *);
    virtual ~Cobro();
    virtual int guardar();
    virtual void pintar();
    void setidcobro(QString val);
    void setidcliente(QString val);
    void setfechacobro(QString val);
    void setfechavenccobro(QString val);
    void setcantcobro(QString val);
    void setrefcobro(QString val);
    void setprevisioncobro(QString val);
    void setcomentcobro(QString val);
    void setidbanco(QString val);
    virtual void pintaidcobro(QString);
    virtual void pintaidcliente(QString);
    virtual void pintafechacobro(QString);
    virtual void pintafechavenccobro(QString);
    virtual void pintacantcobro(QString);
    virtual void pintarefcobro(QString);
    virtual void pintaprevisioncobro(QString);
    virtual void pintacomentcobro(QString);
    virtual void pintaidbanco(QString);
};

#endif

