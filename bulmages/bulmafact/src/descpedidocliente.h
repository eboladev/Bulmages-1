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
#include "dbrecord.h"

class DescuentoPedidoCliente : public DBRecord {
private:

    company *companyact;
    void definetabla();
public:
    DescuentoPedidoCliente(company *);
    DescuentoPedidoCliente(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoPedidoCliente(company *, QString , QString , QString , QString);
    virtual ~DescuentoPedidoCliente();
    virtual void pintaDescuentoPedidoCliente() {}
    ;
    void guardaDescuentoPedidoCliente();
    void vaciaDescuentoPedidoCliente();


    inline QString iddpedidocliente() {
        return DBvalue("iddpedidocliente");
    };
    inline QString conceptdpedidocliente() {
        return DBvalue("conceptdpedidocliente");
    };
    inline QString proporciondpedidocliente() {
        return DBvalue("proporciondpedidocliente");
    };
    inline QString idpedidocliente()  {
        return DBvalue("idpedidocliente");
    };



    inline void setiddpedidocliente(QString val) {
        setDBvalue("iddpedidocliente",val);
    };
    inline void setconceptdpedidocliente(QString val) {
        setDBvalue("conceptdpedidocliente",val);
    };
    inline void setproporciondpedidocliente(QString val) {
        setDBvalue("proporciondpedidocliente",val);
    };
    inline void setidpedidocliente(QString val)  {
        setDBvalue("idpedidocliente",val);
    };

    void borrar();

};

#endif
