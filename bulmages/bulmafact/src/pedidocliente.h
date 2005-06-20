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
#include "listdescpedidocliente.h"
#include "fixed.h"

#include "company.h"
#include "linpedidocliente.h"
/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de factura de la base de datos y el programa.
*/


class PedidoCliente {
protected:
    ListLinPedidoCliente *listalineas;
    ListDescuentoPedidoCliente *listadescuentos;
    company *companyact;
    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_idpedidocliente;
    QString mdb_numpedidocliente;
    QString mdb_fechapedidocliente;
    QString mdb_descpedidocliente;
    QString mdb_idforma_pago;
    QString mdb_comentpedidocliente;
    QString mdb_refpedidocliente;
    QString mdb_procesadopedidocliente;
    
    QString mdb_contactpedidocliente;
    QString mdb_telpedidocliente;
    
public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinPedidoCliente ( ListLinPedidoCliente *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    void setListDescuentoPedidoCliente ( ListDescuentoPedidoCliente *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    }; 
        
    PedidoCliente(company *);
    virtual ~PedidoCliente();
    virtual void cargaPedidoCliente(QString);
    void pintaPedidoCliente();
    void guardaPedidoCliente();
    void borraPedidoCliente();
    void imprimirPedidoCliente();
    void calculaypintatotales();   
     
    ListLinPedidoCliente* getlistalineas() {return listalineas;};
    ListDescuentoPedidoCliente* getlistadescuentos() {return listadescuentos;};
    
    virtual void pintaidcliente(QString ) {};
    virtual void pintaidalmacen(QString ) {};
    virtual void pintaidpedidocliente(QString ) {};
    virtual void pintanumpedidocliente(QString ) {};
    virtual void pintafechapedidocliente(QString ) {};
    virtual void pintadescpedidocliente(QString ) {};
    virtual void pintanomalmacen(QString ) {};
    virtual void pintaidforma_pago(QString ) {};
    virtual void pintacodigoalmacen(QString ) {};
    virtual void pintacomentpedidocliente(QString) {};
    virtual void pintaidpresupuesto(QString) {};
    virtual void pintadescpresupuesto(QString) {};
    virtual void pintarefpresupuesto(QString) {};
    virtual void pintarefpedidocliente(QString) {};
    virtual void pintaprocesadopedidocliente(QString) {};
    virtual void pintacontactpedidocliente(QString) {};
    virtual void pintatelpedidocliente(QString) {};
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {};


    void setidcliente(QString val) { mdb_idcliente=val;};
    void setidalmacen(QString val) { mdb_idalmacen=val;};
    void setidpedidocliente(QString val) { mdb_idpedidocliente=val;listalineas->setidpedidocliente(val);listadescuentos->setidpedidocliente(val);};
    void setnumpedidocliente(QString val) { mdb_numpedidocliente=val;};
    void setfechapedidocliente(QString val) { mdb_fechapedidocliente=val;};
    void setdescpedidocliente(QString val) { mdb_descpedidocliente=val;};
    void setidforma_pago(QString val) {mdb_idforma_pago=val;};
    void setcomentpedidocliente(QString val) {mdb_comentpedidocliente=val;};
    void setrefpedidocliente(QString val) {mdb_refpedidocliente=val;};
    void setprocesadopedidocliente(QString val) {mdb_procesadopedidocliente=val;};
    void setcontactpedidocliente(QString val) {mdb_contactpedidocliente=val;};
    void settelpedidocliente(QString val) {mdb_telpedidocliente=val;};
    
    void vaciaPedidoCliente();
};
#endif
