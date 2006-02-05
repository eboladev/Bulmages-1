//
// C++ Interface: DescuentoAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCALBARANCLIENTE_H
#define DESCALBARANCLIENTE_H

/*
-- Descuento de albaran.
-- Numero
--Concepte: Descripci�del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dalbaran (
   iddalbaran serial PRIMARY KEY,
   conceptdalbaran character varying(2000),
   proporciondalbaran numeric(5,2),
   idalbaran integer REFERENCES albaran(idalbaran)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o despu� de �ta.
);
*/

/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"

class DescuentoAlbaranCliente : public DBRecord {
private:
    company *companyact;
    void definetabla();
public:
    DescuentoAlbaranCliente(company *);
    DescuentoAlbaranCliente(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoAlbaranCliente(company *, QString , QString , QString , QString);
    virtual ~DescuentoAlbaranCliente();
    virtual void pintaDescuentoAlbaranCliente() {};
    void guardaDescuentoAlbaranCliente();
    void vaciaDescuentoAlbaranCliente();

    inline QString iddalbaran() {
        return DBvalue("iddalbaran");
    };
    inline QString conceptdalbaran() {
        return DBvalue("conceptdalbaran");
    };
    inline QString proporciondalbaran() {
        return DBvalue("proporciondalbaran");
    };
    inline QString idalbaran()  {
        return DBvalue("idalbaran");
    };

    inline void setiddalbaran(QString val) {
        setDBvalue("iddalbaran",val);
    };
    inline void setconceptdalbaran(QString val) {
        setDBvalue("conceptdalbaran",val);
    };
    inline void setproporciondalbaran(QString val) {
        setDBvalue("proporciondalbaran",val);
    };
    inline void setidalbaran(QString val)  {
        setDBvalue("idalbaran",val);
    };
    void borrar();
};

#endif
