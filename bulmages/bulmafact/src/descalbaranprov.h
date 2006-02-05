//
// C++ Interface: DescuentoAlbaranProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCALBARANPROV_H
#define DESCALBARANPROV_H

/*
-- Descuento albaranp proveedor
-- Numero
-- Concepte: Descripci�del motiu de descompte.
-- Proporcio: Percentatge a descomptar.
-- Descompte d'albar�a clients.
CREATE TABLE dalbaranpp (
   iddalbaranpp serial PRIMARY KEY,
   conceptdalbaranpp character varying(500),
   proporciondalbaranpp numeric(12,2),
   idalbaranpp integer NOT NULL REFERENCES albaranp(idalbaranp)
);
*/

/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"

class DescuentoAlbaranProv : public DBRecord {
private:
    company *companyact;
    void definetabla();
public:
    DescuentoAlbaranProv(company *);
    DescuentoAlbaranProv(company *, QString );
    /// La carga rapida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoAlbaranProv(company *, QString , QString , QString , QString);
    virtual ~DescuentoAlbaranProv();
    virtual void pintaDescuentoAlbaranProv() {}
    ;
    void guardaDescuentoAlbaranProv();
    void vaciaDescuentoAlbaranProv();

    inline QString iddalbaranp() {
        return DBvalue("iddalbaranp");
    };
    inline QString conceptdalbaranp() {
        return DBvalue("conceptdalbaranp");
    };
    inline QString proporciondalbaranp() {
        return DBvalue("proporciondalbaranp");
    };
    inline QString idalbaranp()  {
        return DBvalue("idalbaranp");
    };
    inline void setiddalbaranp(QString val) {
        setDBvalue("iddalbaranp",val);
    };
    inline void setconceptdalbaranp(QString val) {
        setDBvalue("conceptdalbaranp",val);
    };
    inline void setproporciondalbaranp(QString val) {
        setDBvalue("proporciondalbaranp",val);
    };
    inline void setidalbaranp(QString val)  {
        setDBvalue("idalbaranp",val);
    };
    void borrar();
};

#endif
