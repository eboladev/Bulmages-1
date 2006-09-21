/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef PAGO_H
#define PAGO_H

#include <QString>

#include "company.h"
#include "dbrecord.h"


/// Almacena la informaci&oacute;n de un pago.
/** */
class Pago : public DBRecord {
protected:
    company *companyact;

public:
    Pago(company *);
    virtual ~Pago();
    virtual int cargar(QString);
    virtual void pintar();
    void guardaPago();
    void borraPago();
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
    void setidpago(QString val) {
        setDBvalue("idpago", val);
    };
    void setidproveedor(QString val) {
        setDBvalue("idproveedor", val);
    };
    void setfechapago(QString val) {
        setDBvalue("fechapago", val);
    };
    void setcantpago(QString val) {
        setDBvalue("cantpago", val);
    };
    void setrefpago(QString val) {
        setDBvalue("refpago", val);
    };
    void setprevisionpago(QString val) {
        setDBvalue("previsionpago", val);
    };
    void setcomentpago(QString val) {
        setDBvalue("comentpago", val);
    };
};

#endif

