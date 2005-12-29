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

class DescuentoAlbaranProv {
private:
    QString mdb_iddalbaranp;
    QString mdb_conceptdalbaranp;
    QString mdb_proporciondalbaranp;
    QString mdb_idalbaranp;  
    company *companyact;
public:
    DescuentoAlbaranProv(company *);
    DescuentoAlbaranProv(company *, QString );
    /// La carga rapida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoAlbaranProv(company *, QString , QString , QString , QString);
    virtual ~DescuentoAlbaranProv();
    virtual void pintaDescuentoAlbaranProv() {};
    void guardaDescuentoAlbaranProv();
    void vaciaDescuentoAlbaranProv();

    inline QString iddalbaranp() {return mdb_iddalbaranp;};
    inline QString conceptdalbaranp() {return mdb_conceptdalbaranp;};
    inline QString proporciondalbaranp() {return mdb_proporciondalbaranp;};
    inline QString idalbaranp()  {return mdb_idalbaranp;};
    inline void setiddalbaranp(QString val) {mdb_iddalbaranp=val;};
    inline void setconceptdalbaranp(QString val) {mdb_conceptdalbaranp=val;};
    inline void setproporciondalbaranp(QString val) {mdb_proporciondalbaranp=val;};
    inline void setidalbaranp(QString val)  {mdb_idalbaranp=val;};
    void borrar();  
};

#endif
