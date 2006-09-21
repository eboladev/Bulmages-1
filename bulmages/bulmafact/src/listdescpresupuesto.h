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

#ifndef LISTDESCPRESUPUESTO_H
#define LISTDESCPRESUPUESTO_H

#include <QObject>
#include <QList>

#include "company.h"
#include "descpresupuesto.h"
#include "funcaux.h"


/// Muestra y administra la lista de descuentos por presupuesto.
/** */
class ListDescuentoPresupuesto {
public:
    company *companyact;
    QString mdb_idpresupuesto;
    QList<DescuentoPresupuesto *> m_lista;

public:
    ListDescuentoPresupuesto(company *comp);
    ListDescuentoPresupuesto();
    virtual ~ListDescuentoPresupuesto();
    void setcompany(company *c) {
        _depura("ListDescuentoPresupuesto setCompany.", 0);
        companyact = c;
        _depura("ListDescuentoPresupuesto fin de setCompany.", 0);
    };
    void guardaListDescuentoPresupuesto();
    void vaciar();
    virtual void pintaListDescuentoPresupuesto() {
        _depura("La funcion pintaListDescuentoPresupuesto aun \
                no ha sido implementada.", 0);
    };
    int cargaDescuentos(QString);
    int borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoPresupuesto *linpos(int);
    int borraDescuentoPresupuesto(int);
    void setidpresupuesto(QString id) {
        mdb_idpresupuesto = id;
        for (int i = 0; i < m_lista.size(); ++i) {
            m_lista.at(i)->setidpresupuesto(mdb_idpresupuesto);
        } // end for
    };
};

#endif

