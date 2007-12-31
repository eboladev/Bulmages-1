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

#include "dbrecord.h"


class Empresa;


class Iva : public DBRecord
{
private:
    Empresa *m_companyact;
    void definetabla();

public:
    Iva ( Empresa * );
    Iva ( Empresa *, QString );
    Iva ( Empresa *, cursor2 * );
    virtual ~Iva();
    virtual void pintaIva()
    {}
    ;
    void guardaIva();
    void vaciaIva();
    inline QString idiva()
    {
        return DBvalue ( "idiva" );
    };
    inline QString idtipoiva()
    {
        return DBvalue ( "idtipoiva" );
    };
    inline QString idregistroiva()
    {
        return DBvalue ( "idregistroiva" );
    };
    inline QString idcuenta()
    {
        return DBvalue ( "idcuenta" );
    };
    inline QString codigo()
    {
        return DBvalue ( "codigo" );
    };
    inline QString nombretipoiva()
    {
        return DBvalue ( "nombretipoiva" );
    };
    inline QString baseiva()
    {
        return DBvalue ( "baseiva" );
    };
    inline QString ivaiva()
    {
        return DBvalue ( "ivaiva" );
    };
    inline void setidiva ( QString val )
    {
        setDBvalue ( "idiva", val );
    };
    void setidtipoiva ( const QString &val );
    inline void setidregistroiva ( QString val )
    {
        setDBvalue ( "idregistroiva", val );
    };
    inline void setbaseiva ( QString val )
    {
        setDBvalue ( "baseiva", val );
    };
    inline void setivaiva ( QString val )
    {
        setDBvalue ( "ivaiva", val );
    };
    int borrar();
};

#endif

