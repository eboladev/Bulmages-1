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

#ifndef PEDIDOCLIENTE_H
#define PEDIDOCLIENTE_H

#include <QString>

#include "listlinpedidoclienteview.h"
#include "listdescpedidoclienteview.h"
#include "fixed.h"
#include "company.h"
#include "dbrecord.h"


class PedidoCliente : public DBRecord {
protected:
    ListLinPedidoClienteView *listalineas;
    ListDescuentoPedidoClienteView *listadescuentos;
    company *companyact;

public:
    /// Establece cual es la lista subformulario del presupuesto.
    /// Normalmente para apuntar listlinpresupuestoview.
    void setListLinPedidoCliente(ListLinPedidoClienteView *a) {
        listalineas = a;
        listalineas->setcompany(companyact);
    };
    void setListDescuentoPedidoCliente(ListDescuentoPedidoClienteView *a) {
        listadescuentos = a;
        listadescuentos->setcompany(companyact);
    };
    PedidoCliente(company *);
    virtual ~PedidoCliente();
    virtual int cargar(QString);
    virtual void pintar();
    virtual int guardar();
    virtual int borrar();
    virtual void imprimirPedidoCliente();
    virtual void calculaypintatotales();
    ListLinPedidoClienteView* getlistalineas() {
        return listalineas;
    };
    ListDescuentoPedidoClienteView* getlistadescuentos() {
        return listadescuentos;
    };
    virtual void pintaidcliente(QString) {}
    ;
    virtual void pintaidalmacen(QString) {}
    ;
    virtual void pintaidpedidocliente(QString) {}
    ;
    virtual void pintanumpedidocliente(QString) {}
    ;
    virtual void pintafechapedidocliente(QString) {}
    ;
    virtual void pintadescpedidocliente(QString) {}
    ;
    virtual void pintanomalmacen(QString) {}
    ;
    virtual void pintaidforma_pago(QString) {}
    ;
    virtual void pintacodigoalmacen(QString) {}
    ;
    virtual void pintacomentpedidocliente(QString) {}
    ;
    virtual void pintaidpresupuesto(QString) {}
    ;
    virtual void pintadescpresupuesto(QString) {}
    ;
    virtual void pintarefpresupuesto(QString) {}
    ;
    virtual void pintarefpedidocliente(QString) {}
    ;
    virtual void pintaprocesadopedidocliente(QString) {}
    ;
    virtual void pintacontactpedidocliente(QString) {}
    ;
    virtual void pintatelpedidocliente(QString) {}
    ;
    virtual void pintaidtrabajador(QString) {}
    ;
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {}
    ;
    void setidcliente(QString val) {
        setDBvalue("idcliente", val);
    };
    void setidalmacen(QString val) {
        setDBvalue("idalmacen", val);
    };
    void setidpedidocliente(QString val) {
        setDBvalue("idpedidocliente", val);
        listalineas->setColumnValue( "idpedidocliente",val);
        listadescuentos->setColumnValue( "idpedidocliente",val);
    };
    void setnumpedidocliente(QString val) {
        setDBvalue("numpedidocliente", val);
    };
    void setfechapedidocliente(QString val) {
        setDBvalue("fechapedidocliente", val);
    };
    void setdescpedidocliente(QString val) {
        setDBvalue("descpedidocliente", val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago", val);
    };
    void setcomentpedidocliente(QString val) {
        setDBvalue("comentpedidocliente", val);
    };
    void setrefpedidocliente(QString val) {
        setDBvalue("refpedidocliente", val);
    };
    void setprocesadopedidocliente(QString val) {
        setDBvalue("procesadopedidocliente", val);
    };
    void setcontactpedidocliente(QString val) {
        setDBvalue("contactpedidocliente", val);
    };
    void settelpedidocliente(QString val) {
        setDBvalue("telpedidocliente", val);
    };
    void setidtrabajador(QString val) {
        setDBvalue("idtrabajador", val);
    };
    void vaciaPedidoCliente();
};

#endif

