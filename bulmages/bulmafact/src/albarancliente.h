//
// C++ Interface: albarancliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ALBARANCLIENTE_H
#define ALBARANCLIENTE_H

#include <qstring.h>
#include <qtable.h>
#include <qptrlist.h>

#include "company.h"
#include "listlinalbarancliente.h"
/**
@author Tomeu Borras
*/
class AlbaranCliente{
protected:
    ListLinAlbaranCliente *listalineas;

    company *companyact;

    QString mdb_idalbaran;
    QString mdb_numalbaran;
    QString mdb_fechaalbaran;
    QString mdb_idusuario;
    QString mdb_comentalbaran;
    QString mdb_idcliente;
    QString mdb_idforma_pago;
    QString mdb_idfactura;
    QString mdb_idalmacen;
    
    QString mdb_codigoalmacen;
    QString mdb_nomalmacen;
    QString mdb_cifcliente;
    QString mdb_nomcliente;
    QString mdb_numfactura;

public:
    AlbaranCliente(company *);
    virtual ~AlbaranCliente();
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinAlbaranCliente ( ListLinAlbaranCliente *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };    
    void cargaAlbaranCliente(QString);
    void pintaAlbaranCliente();

    virtual void pintaIdAlbaran(QString) {};
    virtual void pintaNumAlbaran(QString) {};
    virtual void pintaFechaAlbaran(QString) {};
    virtual void pintaIdUsuario(QString) {};
    virtual void pintaComentAlbaran(QString) {};
    virtual void pintaIdCliente(QString ) {};
    virtual void pintaIdForma_Pago(QString) {};
    virtual void pintaIdFactura(QString){};
    virtual void pintaIdAlmacen(QString){};
    virtual void pintaCodigoAlmacen(QString) {};
    virtual void pintaNomAlmacen(QString) {};
    virtual void pintaCifCliente(QString) {};
    virtual void pintaNomCliente(QString) {};
    virtual void pintaNumFactura(QString) {};
    virtual void pintatotales(float, float) {};

    void setIdAlbaran(QString val) { mdb_idalbaran=val;};
    void setNumAlbaran(QString val) { mdb_numalbaran=val;};
    void setFechaAlbaran(QString val) { mdb_fechaalbaran=val;};
    void setIdUsuario(QString val) { mdb_idusuario=val;};
    void setComentAlbaran(QString val) { mdb_comentalbaran=val;};
    void setIdCliente(QString val) { mdb_idcliente=val;};
    void setIdForma_Pago(QString val) { mdb_idforma_pago=val;};
    void setIdFactura(QString val) { mdb_idfactura=val;};
    void setIdAlmacen(QString val) {mdb_idalmacen=val;};
    void setCodigoAlmacen(QString val) {mdb_codigoalmacen=val;};
    void setNomAlmacen(QString val) { mdb_nomalmacen=val;};
    void setCifCliente(QString val) {mdb_cifcliente=val;};
    void setNomCliente(QString val) {mdb_nomcliente=val;};
    void setNumFactura(QString val) {mdb_numfactura=val;};
    
    void vaciaAlbaranCliente();    
};

#endif
