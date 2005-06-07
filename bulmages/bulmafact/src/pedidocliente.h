//
// C++ Interface: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PEDIDOCLIENTE_H
#define PEDIDOCLIENTE_H

/*
CREATE TABLE pedidocliente (
   idpedidocliente serial PRIMARY KEY,
   numpedidocliente character varying(60),
   fechapedidocliente date,
   descpedidocliente character varying(500),
   idcliente integer NOT NULL REFERENCES cliente(idcliente),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen)
);
*/

#include <qstring.h>
#include <qtable.h>
#include <qptrlist.h>

#include "listlinpedidocliente.h"
#include "company.h"
#include "linpedidocliente.h"
/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de factura de la base de datos y el programa.
*/


class PedidoCliente {
protected:
    ListLinPedidoCliente *listalineas;

    company *companyact;
    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_idpedidocliente;
    QString mdb_numpedidocliente;
    QString mdb_fechapedidocliente;
    QString mdb_descpedidocliente;
    QString mdb_nomcliente;
    QString mdb_nomalmacen;
    QString mdb_codigoalmacen;
    QString mdb_cifcliente;
    QString mdb_idforma_pago;
    QString mdb_comentpedidocliente;
public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinPedidoCliente ( ListLinPedidoCliente *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    
    PedidoCliente(company *);
    virtual ~PedidoCliente();
    void cargaPedidoCliente(QString);
    void pintaPedidoCliente();
    void guardaPedidoCliente();
    void borraPedidoCliente();
    
    virtual void pintaidcliente(QString ) {};
    virtual void pintaidalmacen(QString ) {};
    virtual void pintaidpedidocliente(QString ) {};
    virtual void pintanumpedidocliente(QString ) {};
    virtual void pintafechapedidocliente(QString ) {};
    virtual void pintadescpedidocliente(QString ) {};
    virtual void pintanomcliente(QString ) {};
    virtual void pintanomalmacen(QString ) {};
    virtual void pintaidforma_pago(QString ) {};
    virtual void pintacodigoalmacen(QString ) {};
    virtual void pintacifcliente(QString ) {};
    virtual void pintacomentpedidocliente(QString) {};
    virtual void pintatotales(float, float) {};


    void setidcliente(QString val) { mdb_idcliente=val;};
    void setidalmacen(QString val) { mdb_idalmacen=val;};
    void setidpedidocliente(QString val) { mdb_idpedidocliente=val;};
    void setnumpedidocliente(QString val) { mdb_numpedidocliente=val;};
    void setfechapedidocliente(QString val) { mdb_fechapedidocliente=val;};
    void setdescpedidocliente(QString val) { mdb_descpedidocliente=val;};
    void setnomcliente(QString val) { mdb_nomcliente=val;};
    void setnomalmacen(QString val) {mdb_nomalmacen=val;};
    void setidforma_pago(QString val) {mdb_idforma_pago=val;};
    void setcomentpedidocliente(QString val) {mdb_comentpedidocliente=val;};
    void setcodigoalmacen(QString val);
    void setcifcliente(QString val);
    
    void vaciaPedidoCliente();

    virtual void cargaPedidoClienteDescuentas(QString) {};
    virtual void calculateImports() {};
    virtual QString calculateValues() {return "";};
};
#endif
