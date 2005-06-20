//
// C++ Interface: DescuentoFactura
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESCFACTURA_H
#define DESCFACTURA_H



/**
@author Tomeu Borras
*/
#include "company.h"

class DescuentoFactura {
private:
    QString mdb_iddfactura;
    QString mdb_conceptdfactura;
    QString mdb_proporciondfactura;
    QString mdb_idfactura;  
    company *companyact;
public:
    DescuentoFactura(company *);
    DescuentoFactura(company *, QString );
    /// La carga rápida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoFactura(company *, QString , QString , QString , QString);
    virtual ~DescuentoFactura();
    virtual void pintaDescuentoFactura() {};
    void guardaDescuentoFactura();
    void vaciaDescuentoFactura();


    inline QString iddfactura() {return mdb_iddfactura;};
    inline QString conceptdfactura() {return mdb_conceptdfactura;};
    inline QString proporciondfactura() {return mdb_proporciondfactura;};
    inline QString idfactura()  {return mdb_idfactura;};


    
    inline void setiddfactura(QString val) {mdb_iddfactura=val;};
    inline void setconceptdfactura(QString val) {mdb_conceptdfactura=val;};
    inline void setproporciondfactura(QString val) {mdb_proporciondfactura=val;};
    inline void setidfactura(QString val)  {mdb_idfactura=val;};

    void borrar();  
        
};

#endif
