/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef INVENTARIO_H
#define INVENTARIO_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "listcontrolstockview.h"
#include "company.h"
#include "funcaux.h"
#include "dbrecord.h"


class Inventario : public DBRecord {

protected:
    ListControlStockView *listalineas;
    company *companyact;

public:
    Inventario(company *);
    virtual ~Inventario();
    /// Establece cual es la lista subformulario del presupuesto. Normalmente
    /// para apuntar listlinpresupuestoview.
    void setListControlStock(ListControlStockView *a) {
        _depura("Inventario::setListControlStock", 0);
        listalineas = a;
        listalineas->setcompany(companyact);
    };
    ListControlStockView* getlistalineas() {
        return listalineas;
    };
    virtual int cargar(QString);
    void pintaInventario();
    virtual int guardar();
    virtual int borrar();
    void imprimirInventario();
    virtual void pintaidinventario(QString) {}
    ;
    virtual void pintafechainventario(QString) {}
    ;
    virtual void pintanominventario(QString) {}
    ;
    virtual void pregenerar();
    void setidinventario(QString val) {
        setDBvalue("idinventario", val);
        listalineas->setColumnValue( "idinventario", val);
    };
    void setfechainventario(QString val) {
        setDBvalue("fechainventario", val);
    };
    void setnominventario(QString val) {
        setDBvalue("nominventario", val);
    };
    void vaciaInventario();
};

#endif

