//
// C++ Interface: DescuentoPedidoCliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCPEDIDOCLIENTE_H
#define DESCPEDIDOCLIENTE_H


/**
@author Tomeu Borras
*/
#include "company.h"

class DescuentoPedidoCliente {
private:
    QString mdb_iddpedidocliente;
    QString mdb_conceptdpedidocliente;
    QString mdb_proporciondpedidocliente;
    QString mdb_idpedidocliente;  
    company *companyact;
public:
    DescuentoPedidoCliente(company *);
    DescuentoPedidoCliente(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoPedidoCliente(company *, QString , QString , QString , QString);
    virtual ~DescuentoPedidoCliente();
    virtual void pintaDescuentoPedidoCliente() {};
    void guardaDescuentoPedidoCliente();
    void vaciaDescuentoPedidoCliente();


    inline QString iddpedidocliente() {return mdb_iddpedidocliente;};
    inline QString conceptdpedidocliente() {return mdb_conceptdpedidocliente;};
    inline QString proporciondpedidocliente() {return mdb_proporciondpedidocliente;};
    inline QString idpedidocliente()  {return mdb_idpedidocliente;};


    
    inline void setiddpedidocliente(QString val) {mdb_iddpedidocliente=val;};
    inline void setconceptdpedidocliente(QString val) {mdb_conceptdpedidocliente=val;};
    inline void setproporciondpedidocliente(QString val) {mdb_proporciondpedidocliente=val;};
    inline void setidpedidocliente(QString val)  {mdb_idpedidocliente=val;};

    void borrar();  
        
};

#endif
