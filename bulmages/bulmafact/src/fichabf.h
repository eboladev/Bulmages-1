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

#ifndef FICHA1_H
#define FICHA1_H

#include "ficha.h"
#include "subform2bf.h"
#include "company.h"


/// Una factura puede tener multiples bases imponibles. Por eso definimos el tipo base
/// como un QMap.
typedef QMap<QString, Fixed> base;

class FichaBf : public Ficha {
    Q_OBJECT

public:
    SubForm2Bf *m_listalineas;
    SubForm2Bf *m_listadescuentos;

public:
    Company *empresaBase();
    FichaBf(Company *comp, QWidget *parent = 0, Qt::WFlags f = 0, edmode modo = EditMode);
    virtual ~FichaBf();
    void setListaLineas(SubForm2Bf *form);
    void setListaDescuentos(SubForm2Bf *form);
    SubForm2Bf* getlistalineas();
    SubForm2Bf* getlistadescuentos();
    virtual void imprimir();
    virtual void generaRML();
    virtual void calculaypintatotales();
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed, Fixed, Fixed) {};
};

#endif

