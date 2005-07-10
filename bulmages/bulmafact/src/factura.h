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
#ifndef FACTURA_H
#define FACTURA_H

#include <qstring.h>
#include <qtable.h>
#include <qptrlist.h>

#include "listlinfactura.h"
#include "listdescfactura.h"
#include "fixed.h"
#include "company.h"
#include "linfactura.h"
/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de factura de la base de datos y el programa.
*/


class Factura {
protected:
    ListLinFactura *listalineas;
    ListDescuentoFactura *listadescuentos;

    company *companyact;

    QString mdb_codigoserie_factura;

    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_numfactura;
    QString mdb_ffactura;
    QString mdb_procesadafactura;
    
    
    QString mdb_comentfactura;

    QString mdb_reffactura;
    QString mdb_descfactura;

    QString mdb_idfactura;
    QString mdb_idusuari;
    QString mdb_idforma_pago;

public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinFactura ( ListLinFactura *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };

    void setListDescuentoFactura ( ListDescuentoFactura *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    }; 
            
    ListLinFactura* getlistalineas() {return listalineas;};    
    ListDescuentoFactura* getlistadescuentos() {return listadescuentos;};
    
    Factura(company *);
    virtual ~Factura();
    virtual void cargaFactura(QString );
    void pintaFactura();
    void guardaFactura();
    void borraFactura();
    void imprimirFactura();
    void calculaypintatotales(); 
    
    virtual void pintaidcliente(QString ) {};

    virtual void pintareffactura(QString) {};
    virtual void pintaNumFactura(QString ) {};
    virtual void pintafechafactura(QString ) {};
    virtual void pintadescfactura(QString) {};

    virtual void pintacodigoserie_factura(QString) {};
    virtual void pintaComentFactura(QString ) {};

    virtual void pintaidalmacen(QString ) {};

    virtual void pintaidforma_pago(QString) {};
    virtual void pintaprocesadafactura(QString) {};
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {};

    void setidcliente(QString val) { mdb_idcliente=val;};
    void setidalmacen(QString val) { mdb_idalmacen=val;};
    void setreffactura(QString val) {mdb_reffactura=val;};
    void setNumFactura(QString val) { mdb_numfactura=val;};
    void setfechafactura(QString val) { mdb_ffactura=val;};
    void setdescfactura(QString val) {mdb_descfactura=val;};

    void setcodigoserie_factura(QString val) {mdb_codigoserie_factura=val;};
    void setcomentfactura(QString val) { mdb_comentfactura=val;};

    void setidfactura(QString val) {mdb_idfactura=val;listalineas->setidfactura(val);listadescuentos->setidfactura(val);};
    void setIdUsuari(QString val) {mdb_idusuari=val;};
    void setidforma_pago(QString val) {mdb_idforma_pago=val;};
    void setprocesadafactura(QString val) {mdb_procesadafactura=val;};
    void vaciaFactura();

    virtual void cargaFacturaDescuentas(QString) {};
    virtual void calculateImports() {};
    virtual void inicialize() {};
    virtual QString calculateValues() {return "";};
};
#endif
