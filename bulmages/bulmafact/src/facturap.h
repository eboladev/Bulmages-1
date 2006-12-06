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


/** Hace de intermediario entre la base de datos y la pantalla de Factura Proveedor.
*/
class FacturaProveedor : public DBRecord {
protected:
    /// Puntero a la lista de lineas de factura de proveedor. 
    ListLinFacturaProveedorView *listalineas;
    /// Puntero a la lista de descuentos de factura de proveedor.
    ListDescuentoFacturaProvView *listadescuentos;
    /// Puntero a la clase company para trabajar con la Base de Datos y para el paso de mensajes.
    company *companyact;

public:
    FacturaProveedor(company *);
    virtual ~FacturaProveedor();
    
    ListLinFacturaProveedorView* getlistalineas();
    ListDescuentoFacturaProvView* getlistadescuentos();
    
    virtual int borrar();
    
    /// Establece cu&aacute;l es la lista subformulario del presupuesto. Normalmente para
    /// apuntar listlinpresupuestoview.
    void setListLinFacturaProveedor(ListLinFacturaProveedorView *a);
    void setListDescuentoFacturaProv(ListDescuentoFacturaProvView *a);
    void setidproveedor(QString val);
    void setreffacturap(QString val);
    void setnumfacturap(QString val);
    void setfechafacturap(QString val);
    void setdescfacturap(QString val);
    void setcomentfacturap(QString val);
    void setidfacturap(QString val);
    void setidforma_pago(QString val);
    void setprocesadafacturap(QString val);
    
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
    virtual void calculaypintatotales();
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {}
    ;
};

#endif

