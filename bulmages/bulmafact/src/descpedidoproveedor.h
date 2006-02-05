//
// C++ Interface: DescuentoPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCPEDIDOPROVEEDOR_H
#define DESCPEDIDOPROVEEDOR_H


/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"

class DescuentoPedidoProveedor : public DBRecord {
private:

    company *companyact;
    void definetabla();
public:
    DescuentoPedidoProveedor(company *);
    DescuentoPedidoProveedor(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoPedidoProveedor(company *, QString , QString , QString , QString);
    virtual ~DescuentoPedidoProveedor();
    virtual void pintaDescuentoPedidoProveedor() {}
    ;
    void guardaDescuentoPedidoProveedor();
    void vaciaDescuentoPedidoProveedor();


    inline QString iddpedidoproveedor() {
        return DBvalue("iddpedidoproveedor");
    };
    inline QString conceptdpedidoproveedor() {
        return DBvalue("conceptdpedidoproveedor");
    };
    inline QString proporciondpedidoproveedor() {
        return DBvalue("proporciondpedidoproveedor");
    };
    inline QString idpedidoproveedor()  {
        return DBvalue("idpedidoproveedor");
    };

    inline void setiddpedidoproveedor(QString val) {
        setDBvalue("iddpedidoproveedor",val);
    };
    inline void setconceptdpedidoproveedor(QString val) {
        setDBvalue("conceptdpedidoproveedor",val);
    };
    inline void setproporciondpedidoproveedor(QString val) {
        setDBvalue("proporciondpedidoproveedor",val);
    };
    inline void setidpedidoproveedor(QString val)  {
        setDBvalue("idpedidoproveedor",val);
    };

    void borrar();
};

#endif
