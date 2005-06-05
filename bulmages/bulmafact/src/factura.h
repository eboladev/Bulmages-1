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
#include "company.h"
#include "linfactura.h"
/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de factura de la base de datos y el programa.
*/


class Factura {
protected:
    ListLinFactura *listalineas;

    company *companyact;
    QString m_idfactura;
    QString m_idclient;
    QString m_idalmacen;
    QString m_initialValues;


    QString mdb_idcliente;
    QString mdb_idalmacen;
    QString mdb_numfactura;
    QString mdb_ffactura;
    QString mdb_contactfactura;
    QString mdb_telfactura;
    QString mdb_comentfactura;
    QString mdb_nomcliente;
    QString mdb_cifcliente;
    QString mdb_codigoalmacen;
    QString mdb_nomalmacen;
    QString mdb_idfactura;
    QString mdb_idusuari;
    QString mdb_idforma_pago;

public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinFactura ( ListLinFactura *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    
    Factura(company *);
    virtual ~Factura();
    void cargaFactura(QString );
    void pintaFactura();
    void guardaFactura();
    void borraFactura();
    
    virtual void pintaIdClient(QString ) {};
    virtual void pintaIdAlmacen(QString ) {};
    virtual void pintaNumFactura(QString ) {};
    virtual void pintaFFactura(QString ) {};
    virtual void pintaContractFactura(QString ) {};
    virtual void pintaTelFactura(QString ) {};
    virtual void pintaComentFactura(QString ) {};
    virtual void pintaNomClient(QString ) {};
    virtual void pintaCifClient(QString ) {};
    virtual void pintaCodigoAlmacen(QString ) {};
    virtual void pintaNomAlmacen(QString ) {};
    virtual void pintaFormaPago(QString) {};
    virtual void pintatotales(float, float) {};

    void setIdClient(QString val) { mdb_idcliente=val;};
    void setIdAlmacen(QString val) { mdb_idalmacen=val;};
    void setNumFactura(QString val) { mdb_numfactura=val;};
    void setFFactura(QString val) { mdb_ffactura=val;};
    void setContactFactura(QString val) { mdb_contactfactura=val;};
    void setTelFactura(QString val) { mdb_telfactura=val;};
    void setComentFactura(QString val) { mdb_comentfactura=val;};
    void setNomClient(QString val) { mdb_nomcliente=val;};
    void setCifClient(QString);
    void setCodigoAlmacen(QString);
    void setNomAlmacen(QString val) { mdb_nomalmacen=val;};
    void setIdFactura(QString val) {mdb_idfactura=val;};
    void setIdUsuari(QString val) {mdb_idusuari=val;};
    void setFormaPago(QString val) {mdb_idforma_pago=val;};
    void vaciaFactura();

    virtual void cargaFacturaDescuentas(QString) {};
    virtual void calculateImports() {};
    virtual void inicialize() {};
    virtual QString calculateValues() {return "";};
};
#endif
