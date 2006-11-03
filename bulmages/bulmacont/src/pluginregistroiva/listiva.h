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

#ifndef LISTIVA_H
#define LISTIVA_H

#include <QObject>
#include <Q3PtrList>

#include "empresa.h"
#include "iva.h"
#include "funcaux.h"
#include "fixed.h"


class ListIva {
public:
    empresa *companyact;
    QString mdb_idregistroiva;
    Q3PtrList<Iva> m_lista;

public:
    ListIva(empresa *comp);
    ListIva();
    void setcompany(empresa *c) {
        companyact=c;
    };
    virtual ~ListIva();
    void guardaListIva();
    void vaciar();
    virtual void pintaListIva() {
        _depura("Esta funcion aun no ha sido implementada\n", 2);
    };
    int cargaListIva(QString);
    void borrar();
    Iva *linpos(int);
    void append(Iva *iva) {
        m_lista.append(iva);
    };
    Fixed calculaiva();
    Fixed calculabase();
    void borraIva(int);
    void setidregistroiva(QString id) {
        mdb_idregistroiva = id;
        Iva *linea;
        uint i = 0;
        for (linea = m_lista.first(); linea; linea = m_lista.next()) {
            linea->setidregistroiva(mdb_idregistroiva);
            i++;
        } // end for
    };
};

#endif

