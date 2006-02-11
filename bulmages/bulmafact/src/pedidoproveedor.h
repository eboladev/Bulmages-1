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
#ifndef PEDIDOPROVEEDOR_H
#define PEDIDOPROVEEDOR_H

/*
CREATE TABLE pedidoproveedor (
   idpedidoproveedor serial PRIMARY KEY,
   numpedidoproveedor character varying(60),
   fechapedidoproveedor date,
   descpedidoproveedor character varying(500),
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
   idalmacen integer NOT NULL REFERENCES almacen(idalmacen)
);
*/

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "listlinpedidoproveedor.h"
#include "listdescpedidoproveedor.h"
#include "fixed.h"

#include "company.h"
#include "linpedidoproveedor.h"
#include "dbrecord.h"
/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de factura de la base de datos y el programa.
*/


class PedidoProveedor : public DBRecord {
protected:
    ListLinPedidoProveedor *listalineas;
    ListDescuentoPedidoProveedor *listadescuentos;
    company *companyact;

    
public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinPedidoProveedor ( ListLinPedidoProveedor *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    void setListDescuentoPedidoProveedor ( ListDescuentoPedidoProveedor *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    }; 
        
    PedidoProveedor(company *);
    virtual ~PedidoProveedor();
    virtual int cargaPedidoProveedor(QString);
    void pintaPedidoProveedor();
    void guardaPedidoProveedor();
    void borraPedidoProveedor();
    void imprimirPedidoProveedor();
    void calculaypintatotales();   
     
    ListLinPedidoProveedor* getlistalineas() {return listalineas;};
    ListDescuentoPedidoProveedor* getlistadescuentos() {return listadescuentos;};
    
    virtual void pintaidproveedor(QString ) {};
    virtual void pintaidalmacen(QString ) {};
    virtual void pintaidpedidoproveedor(QString ) {};
    virtual void pintanumpedidoproveedor(QString ) {};
    virtual void pintafechapedidoproveedor(QString ) {};
    virtual void pintadescpedidoproveedor(QString ) {};
    virtual void pintanomalmacen(QString ) {};
    virtual void pintaidforma_pago(QString ) {};
    virtual void pintacodigoalmacen(QString ) {};
    virtual void pintacomentpedidoproveedor(QString) {};
    virtual void pintaidpresupuesto(QString) {};
    virtual void pintadescpresupuesto(QString) {};
    virtual void pintarefpresupuesto(QString) {};
    virtual void pintarefpedidoproveedor(QString) {};
    virtual void pintaprocesadopedidoproveedor(QString) {};
    virtual void pintacontactpedidoproveedor(QString) {};
    virtual void pintatelpedidoproveedor(QString) {};
    virtual void pintaidtrabajador(QString) {};
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {};


    void setidproveedor(QString val) { setDBvalue("idproveedor",val);};
    void setidalmacen(QString val) { setDBvalue("idalmacen",val);};
    void setidpedidoproveedor(QString val) { setDBvalue("idpedidoproveedor",val);listalineas->setidpedidoproveedor(val);listadescuentos->setidpedidoproveedor(val);};
    void setnumpedidoproveedor(QString val) { setDBvalue("numpedidoproveedor",val);};
    void setfechapedidoproveedor(QString val) { setDBvalue("fechapedidoproveedor",val);};
    void setdescpedidoproveedor(QString val) { setDBvalue("descpedidoproveedor",val);};
    void setidforma_pago(QString val) {setDBvalue("idforma_pago",val);};
    void setcomentpedidoproveedor(QString val) {setDBvalue("comentpedidoproveedor",val);};
    void setrefpedidoproveedor(QString val) {setDBvalue("refpedidoproveedor",val);};
    void setprocesadopedidoproveedor(QString val) {setDBvalue("procesadopedidoproveedor",val);};
    void setcontactpedidoproveedor(QString val) {setDBvalue("contactpedidoproveedor",val);};
    void settelpedidoproveedor(QString val) {setDBvalue("telpedidoproveedor",val);};
    void setidtrabajador(QString val) {setDBvalue("idtrabajador",val);};
    
    void vaciaPedidoProveedor();
};
#endif
