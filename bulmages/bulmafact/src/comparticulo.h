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

#ifndef COMPARTICULO_H
#define COMPARTICULO_H

#include "company.h"
#include "dbrecord.h"


class CompArticulo : public DBRecord {

private:
    company *companyact;
    void definetabla();

public:
    CompArticulo(company *);
    CompArticulo(company *, QString, QString);

    /// La carga rapida tiene un comportamiento poco restrictivo para
    /// aumentar la eficiencia.
    CompArticulo(company *, QString , QString , QString , QString , QString);
    virtual ~CompArticulo();
    virtual void pintaCompArticulo() {}
    ;
    void guardaCompArticulo();
    void vaciaCompArticulo();
    inline QString idarticulo() {
        return DBvalue("idarticulo");
    };
    inline QString idcomponente() {
        return DBvalue("idcomponente");
    };
    inline QString cantcomparticulo() {
        return DBvalue("cantcomparticulo");
    };
    inline QString codigocompletocomponente() {
        return DBvalue("codigocompletocomponente");
    };
    inline QString nomcomponente() {
        return DBvalue("nomcomponente");
    };
    inline void setidarticulo(QString val) {
        setDBvalue("idarticulo", val);
    };
    inline void setcantcomparticulo(QString val) {
        setDBvalue("cantcomparticulo", val);
    };
    void setcodigocompletocomponente(QString);
    void setidcomponente(QString);
};

#endif

