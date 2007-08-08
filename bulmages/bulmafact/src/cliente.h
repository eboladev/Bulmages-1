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

#ifndef CLIENTE_H
#define CLIENTE_H

#include <QString>

#include "company.h"
#include "fichabf.h"


/// Administra los datos de un cliente.
/** Esta clase se encarga del trabajo entre un cliente y la base de datos.
    ESta hecha para trabajar conjuntamente con una clase de visualizacion que muestre
    los datos del cliente.

    Tiene dos tipos de metodos especiales:
    setXXX son utlizados para alterar valores de un cliente.
    pintaXXX son utilizados para invocar a al repintado de determinado elemento. */
class Cliente : public FichaBf {
public:
    Cliente(company *, QWidget *);
    virtual ~Cliente();
    virtual void pintar();
    virtual int guardar();
    virtual int borrar();
    virtual int cargar(QString);
    virtual void pintaidcliente(QString);
    virtual void pintanomcliente(QString);
    virtual void pintanomaltcliente(QString);
    virtual void pintacifcliente(QString);
    virtual void pintabancocliente(QString);
    virtual void pintadircliente(QString);
    virtual void pintapoblcliente(QString);
    virtual void pintacpcliente(QString);
    virtual void pintatelcliente(QString);
    virtual void pintateltrabcliente(QString);
    virtual void pintamovilcliente(QString);
    virtual void pintafaxcliente(QString);
    virtual void pintamailcliente(QString);
    virtual void pintaurlcliente(QString);
    virtual void pintafaltacliente(QString);
    virtual void pintafbajacliente(QString);
    virtual void pintacomentcliente(QString);
    virtual void pintainactivocliente(QString);
    virtual void pintaprovcliente(QString);
    virtual void pintacodcliente(QString);
    virtual void pintacorpcliente(QString);
    virtual void pintaidforma_pago(QString);
    virtual void pintarecargoeqcliente(QString);
    virtual void pintaregimenfiscalcliente(QString);
};

#endif

