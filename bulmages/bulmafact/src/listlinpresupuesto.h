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

#ifndef LISTLINPRESUPUESTO_H
#define LISTLINPRESUPUESTO_H

#include <QList>
#include <QObject>

#include "company.h"
#include "linpresupuesto.h"
#include "funcaux.h"


/// Administra las l&iacute;neas de detalle de un presupuesto.
/** */
class listlinpresupuesto {
public:
    company *companyact;
    QString mdb_idpresupuesto;
    QList<linpresupuesto *> m_lista;

public:
    listlinpresupuesto(company *comp);
    listlinpresupuesto();
    virtual ~listlinpresupuesto();
    void setcompany(company *c) {
        _depura("listlinpresupuesto setCompany.", 0);
        companyact = c;
        _depura("listlinpresupuesto fin de setCompany.", 0);
    };
    void guardalistlinpresupuesto();
    void vaciar();
    virtual void pintalistlinpresupuesto() {
        _depura("Esta funcion aun no ha sido implementada\n", 2);
    };
    int chargeBudgetLines(QString);
    void borrar();
    void nuevalinea(QString, QString, QString, QString, QString, QString, QString, QString);
    linpresupuesto *linpos(int);
    float calculabase();
    float calculaiva();
    void borralinpresupuesto(int);
    void setidpresupuesto(QString id) {
        mdb_idpresupuesto = id;
        for (int i = 0; i < m_lista.size(); ++i) {
            m_lista.at(i)->setidpresupuesto(mdb_idpresupuesto);
        } // end for
    };
};

#endif

