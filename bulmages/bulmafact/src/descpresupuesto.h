//
// C++ Interface: DescuentoPresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCPRESUPUESTO_H
#define DESCPRESUPUESTO_H

/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"

class DescuentoPresupuesto : DBRecord {
private:
    company *companyact;
    void definetabla();
public:
    DescuentoPresupuesto(company *);
    DescuentoPresupuesto(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoPresupuesto(company *, QString , QString , QString , QString);
    virtual ~DescuentoPresupuesto();
    virtual void pintaDescuentoPresupuesto() {}
    ;
    void guardaDescuentoPresupuesto();
    void vaciaDescuentoPresupuesto();

    inline QString iddpresupuesto() {
        return DBvalue("iddpresupuesto");
    };
    inline QString conceptdpresupuesto() {
        return DBvalue("conceptdpresupuesto");
    };
    inline QString proporciondpresupuesto() {
        return DBvalue("proporciondpresupuesto");
    };
    inline QString idpresupuesto()  {
        return DBvalue("idpresupuesto");
    };

    inline void setiddpresupuesto(QString val) {
        setDBvalue("iddpresupuesto",val);
    };
    inline void setconceptdpresupuesto(QString val) {
        setDBvalue("conceptdpresupuesto",val);
    };
    inline void setproporciondpresupuesto(QString val) {
        setDBvalue("proporciondpresupuesto",val);
    };
    inline void setidpresupuesto(QString val)  {
        setDBvalue("idpresupuesto",val);
    };

    void borrar();
};

#endif
