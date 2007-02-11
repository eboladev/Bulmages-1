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

#ifndef FICHA1_H
#define FICHA1_H

#include "ficha.h"
#include "subform2bf.h"
#include "dbrecord.h"
#include "company.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base como un QMap.
typedef QMap<QString, Fixed> base;

class FichaBf : public Ficha, public DBRecord {
    Q_OBJECT
public:
    SubForm2Bf *m_listalineas;
    SubForm2Bf *m_listadescuentos;

    /// Puntero a la clase company para poder trabajar con la base de datos y hacer traspaso de mensajes.
    company *m_companyact;

public:
    FichaBf(company *comp, QWidget *parent = 0, Qt::WFlags f = 0);
    ~FichaBf();
    void setListaLineas(SubForm2Bf * form) {
        m_listalineas = form;
    };
    void setListaDescuentos(SubForm2Bf *form) {
        m_listadescuentos = form;
    };
    SubForm2Bf* getlistalineas() {
        return m_listalineas;
    };
    SubForm2Bf* getlistadescuentos() {
        return m_listadescuentos;
    };
    void imprimir();
    virtual void calculaypintatotales();
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed, Fixed, Fixed) {};
    virtual int sacaWindow();
};

#endif

