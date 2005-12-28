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

class DescuentoPedidoProveedor {
private:
    QString mdb_iddpedidoproveedor;
    QString mdb_conceptdpedidoproveedor;
    QString mdb_proporciondpedidoproveedor;
    QString mdb_idpedidoproveedor;  
    company *companyact;
public:
    DescuentoPedidoProveedor(company *);
    DescuentoPedidoProveedor(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoPedidoProveedor(company *, QString , QString , QString , QString);
    virtual ~DescuentoPedidoProveedor();
    virtual void pintaDescuentoPedidoProveedor() {};
    void guardaDescuentoPedidoProveedor();
    void vaciaDescuentoPedidoProveedor();


    inline QString iddpedidoproveedor() {return mdb_iddpedidoproveedor;};
    inline QString conceptdpedidoproveedor() {return mdb_conceptdpedidoproveedor;};
    inline QString proporciondpedidoproveedor() {return mdb_proporciondpedidoproveedor;};
    inline QString idpedidoproveedor()  {return mdb_idpedidoproveedor;};


    
    inline void setiddpedidoproveedor(QString val) {mdb_iddpedidoproveedor=val;};
    inline void setconceptdpedidoproveedor(QString val) {mdb_conceptdpedidoproveedor=val;};
    inline void setproporciondpedidoproveedor(QString val) {mdb_proporciondpedidoproveedor=val;};
    inline void setidpedidoproveedor(QString val)  {mdb_idpedidoproveedor=val;};

    void borrar();  
        
};

#endif
