/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#ifndef FACTURASLIST_H
#define FACTURASLIST_H

#include "busquedacliente.h"
#include "busquedaarticulo.h"
#include "company.h"
#include "funcaux.h"
#include "subform2bf.h"
#include "listado.h"


/// Administra el detalle del listado de facturas a clientes.
/** */
class FacturasListSubform : public SubForm2Bf {
    Q_OBJECT

public:
    FacturasListSubform(QWidget *parent = 0, const char *name = 0);
    ~FacturasListSubform();

public slots:
    virtual void cargar();
    virtual void cargar(QString query);
};


#include "ui_facturaslistbase.h"


/// Administra el listado de facturas a clientes.
/** */
class FacturasList : public Listado, public Ui_FacturasListBase {
    Q_OBJECT

private:
    QString mdb_idfactura;

public:
    FacturasList(QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    FacturasList(company *,QWidget *parent = 0, Qt::WFlags flag = 0, edmode editmodo = EditMode);
    ~FacturasList();
    void iniciaForm();
    void setEmpresaBase (company *comp);
    QString idfactura();
    void setidcliente(QString val);
    void setidarticulo(QString val);
    QString generaFiltro();
    void presentar();
    void editar(int);
    void borrar();
    void crear();
    void imprimir();

signals:
    void selected(QString);
};

#endif

