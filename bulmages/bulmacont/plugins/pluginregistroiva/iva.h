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

#ifndef IVA_H
#define IVA_H

#include "bldb.h"


class BcCompany ;


class Iva : public BlDbRecord
{
private:
    BcCompany *m_companyact;
    void definetabla();

public:
    Iva ( BcCompany * );
    Iva ( BcCompany *, QString );
    Iva ( BcCompany *, BlDbRecordSet * );
    virtual ~Iva();
    virtual void pintaIva() {}
    ;
    void guardaIva();
    void vaciaIva();
    inline QString idiva() {
        return dbValue ( "idiva" );
    };
    inline QString idtipoiva() {
        return dbValue ( "idtipoiva" );
    };
    inline QString idregistroiva() {
        return dbValue ( "idregistroiva" );
    };
    inline QString idcuenta() {
        return dbValue ( "idcuenta" );
    };
    inline QString codigo() {
        return dbValue ( "codigo" );
    };
    inline QString nombretipoiva() {
        return dbValue ( "nombretipoiva" );
    };
    inline QString baseiva() {
        return dbValue ( "baseiva" );
    };
    inline QString ivaiva() {
        return dbValue ( "ivaiva" );
    };
    inline void setidiva ( QString val ) {
        setDbValue ( "idiva", val );
    };
    void setidtipoiva ( const QString &val );
    inline void setidregistroiva ( QString val ) {
        setDbValue ( "idregistroiva", val );
    };
    inline void setbaseiva ( QString val ) {
        setDbValue ( "baseiva", val );
    };
    inline void setivaiva ( QString val ) {
        setDbValue ( "ivaiva", val );
    };
    int borrar();
};

#endif

