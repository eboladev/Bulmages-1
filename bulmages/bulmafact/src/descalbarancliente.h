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
--Concepte: Descripció del motiu de descompte.
--Proporcio: Percentatge a descomptar.
-- Descompte de pressupost a clients.
CREATE TABLE dalbaran (
   iddalbaran serial PRIMARY KEY,
   conceptdalbaran character varying(2000),
   proporciondalbaran numeric(5,2),
   idalbaran integer REFERENCES albaran(idalbaran)
   -- Falta poner el lugar donde se aplica el descuento, antes de la factura o después de ésta.
);
*/

/**
@author Tomeu Borras
*/
#include "company.h"

class DescuentoAlbaranCliente {
private:
    QString mdb_iddalbaran;
    QString mdb_conceptdalbaran;
    QString mdb_proporciondalbaran;
    QString mdb_idalbaran;  
    company *companyact;
public:
    DescuentoAlbaranCliente(company *);
    DescuentoAlbaranCliente(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoAlbaranCliente(company *, QString , QString , QString , QString);
    virtual ~DescuentoAlbaranCliente();
    virtual void pintaDescuentoAlbaranCliente() {};
    void guardaDescuentoAlbaranCliente();
    void vaciaDescuentoAlbaranCliente();


    inline QString iddalbaran() {return mdb_iddalbaran;};
    inline QString conceptdalbaran() {return mdb_conceptdalbaran;};
    inline QString proporciondalbaran() {return mdb_proporciondalbaran;};
    inline QString idalbaran()  {return mdb_idalbaran;};


    
    inline void setiddalbaran(QString val) {mdb_iddalbaran=val;};
    inline void setconceptdalbaran(QString val) {mdb_conceptdalbaran=val;};
    inline void setproporciondalbaran(QString val) {mdb_proporciondalbaran=val;};
    inline void setidalbaran(QString val)  {mdb_idalbaran=val;};

    void borrar();  
        
};

#endif
