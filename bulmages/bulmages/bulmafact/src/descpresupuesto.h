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

class DescuentoPresupuesto {
private:
    QString mdb_iddpresupuesto;
    QString mdb_conceptdpresupuesto;
    QString mdb_proporciondpresupuesto;
    QString mdb_idpresupuesto;  
    company *companyact;
public:
    DescuentoPresupuesto(company *);
    DescuentoPresupuesto(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoPresupuesto(company *, QString , QString , QString , QString);
    virtual ~DescuentoPresupuesto();
    virtual void pintaDescuentoPresupuesto() {};
    void guardaDescuentoPresupuesto();
    void vaciaDescuentoPresupuesto();


    inline QString iddpresupuesto() {return mdb_iddpresupuesto;};
    inline QString conceptdpresupuesto() {return mdb_conceptdpresupuesto;};
    inline QString proporciondpresupuesto() {return mdb_proporciondpresupuesto;};
    inline QString idpresupuesto()  {return mdb_idpresupuesto;};


    
    inline void setiddpresupuesto(QString val) {mdb_iddpresupuesto=val;};
    inline void setconceptdpresupuesto(QString val) {mdb_conceptdpresupuesto=val;};
    inline void setproporciondpresupuesto(QString val) {mdb_proporciondpresupuesto=val;};
    inline void setidpresupuesto(QString val)  {mdb_idpresupuesto=val;};

    void borrar();  
        
};

#endif
