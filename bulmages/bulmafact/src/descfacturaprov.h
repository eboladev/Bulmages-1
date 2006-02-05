//
// C++ Interface: DescuentoFacturaProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCFACTURAPROV_H
#define DESCFACTURAPROV_H


/*
-- Descuento de facturap proveedor
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dfacturap (
   iddfacturap serial PRIMARY KEY,
   conceptdfacturap character varying(2000),
   proporciondfacturap numeric(12,2),
   idfacturapp integer NOT NULL REFERENCES facturap(idfacturap)
   -- Falta poner el lugar donde se aplica el descuento, antes de la facturap o despu� de �ta.
); 
*/



/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"


class DescuentoFacturaProv : public DBRecord {
private:

    company *companyact;
    void definetabla();
public:
    DescuentoFacturaProv(company *);
    DescuentoFacturaProv(company *, QString );
    /// La carga rapida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoFacturaProv(company *, QString , QString , QString , QString);
    virtual ~DescuentoFacturaProv();
    virtual void pintaDescuentoFacturaProv() {}
    ;
    void guardaDescuentoFacturaProv();
    void vaciaDescuentoFacturaProv();

    inline QString iddfacturap() {
        return DBvalue("iddfacturap");
    };
    inline QString conceptdfacturap() {
        return DBvalue("conceptdfacturap");
    };
    inline QString proporciondfacturap() {
        return DBvalue("proporciondfacturap");
    };
    inline QString idfacturap()  {
        return DBvalue("idfacturap");
    };

    inline void setiddfacturap(QString val) {
        setDBvalue("iddfacturap",val);
    };
    inline void setconceptdfacturap(QString val) {
        setDBvalue("conceptdfacturap",val);
    };
    inline void setproporciondfacturap(QString val) {
        setDBvalue("proporciondfacturap",val);
    };
    inline void setidfacturap(QString val)  {
        setDBvalue("idfacturap",val);
    };
    void borrar();
};

#endif
