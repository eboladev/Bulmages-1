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

#ifndef FACTURAP_H
#define FACTURAP_H

#include <QString>

#include "listlinfacturapview.h"
#include "company.h"
#include "listdescfacturaprovview.h"
#include "dbrecord.h"


class FacturaProveedor : public DBRecord {

protected:
    ListLinFacturaProveedorView *listalineas;
    ListDescuentoFacturaProvView *listadescuentos;
    company *companyact;

public:
    FacturaProveedor(company *);
    virtual ~FacturaProveedor();
    ListLinFacturaProveedorView* getlistalineas() {
        return listalineas;
    };
    ListDescuentoFacturaProvView* getlistadescuentos() {
        return listadescuentos;
    };
    virtual int borrar();
    /// Establece cual es la lista subformulario del presupuesto. Normalmente para
    /// apuntar listlinpresupuestoview.
    void setListLinFacturaProveedor(ListLinFacturaProveedorView *a) {
        listalineas = a;
        listalineas->setcompany(companyact);
    };
    void setListDescuentoFacturaProv(ListDescuentoFacturaProvView *a) {
        listadescuentos = a;
        listadescuentos->setcompany(companyact);
    };
    void setidproveedor(QString val) {
        setDBvalue("idproveedor", val);
    };
    void setreffacturap(QString val) {
        setDBvalue("reffacturap", val);
    };
    void setnumfacturap(QString val) {
        setDBvalue("numfacturap", val);
    };
    void setfechafacturap(QString val) {
        setDBvalue("ffacturap", val);
    };
    void setdescfacturap(QString val) {
        setDBvalue("descfacturap", val);
    };
    void setcomentfacturap(QString val) {
        setDBvalue("comentfacturap", val);
    };
    void setidfacturap(QString val) {
        setDBvalue("idfacturap", val);
        listalineas->setColumnValue("idfacturap",val);
        listadescuentos->setColumnValue( "idfacturap",val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago", val);
    };
    void setprocesadafacturap(QString val) {
        setDBvalue("procesadafacturap", val);
    };
    virtual void imprimirFacturaProveedor();
    virtual int cargar(QString);
    virtual void pintar();
    virtual int guardar();
    virtual void pintaidproveedor(QString) {}
    ;
    virtual void pintareffacturap(QString) {}
    ;
    virtual void pintanumfacturap(QString) {}
    ;
    virtual void pintafechafacturap(QString) {}
    ;
    virtual void pintadescfacturap(QString) {}
    ;
    virtual void pintacomentfacturap(QString) {}
    ;
    virtual void pintaidforma_pago(QString) {}
    ;
    virtual void pintaprocesadafacturap(QString) {}
    ;
    virtual void pintatotales(Fixed, Fixed) {}
    ;
    virtual void cargaFacturaProveedorDescuentas(QString) {}
    ;
    virtual void calculateImports() {}
    ;
    virtual void inicialize() {}
    ;
    virtual QString calculateValues() {
        return "";
    };
};

#endif

