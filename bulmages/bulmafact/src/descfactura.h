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
#include "dbrecord.h"

class DescuentoFactura : public DBRecord {
private:
    company *companyact;
	void definetabla();
public:
    DescuentoFactura(company *);
    DescuentoFactura(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    DescuentoFactura(company *, QString , QString , QString , QString);
    virtual ~DescuentoFactura();
    virtual void pintaDescuentoFactura() {};
    void guardaDescuentoFactura();
    void vaciaDescuentoFactura();

    inline QString iddfactura() {return DBvalue("iddfactura");};
    inline QString conceptdfactura() {return DBvalue("conceptdfactura");};
    inline QString proporciondfactura() {return DBvalue("proporciondfactura");};
    inline QString idfactura()  {return DBvalue("idfactura");};

    inline void setiddfactura(QString val) {setDBvalue("iddfactura",val);};
    inline void setconceptdfactura(QString val) {setDBvalue("conceptdfactura",val);};
    inline void setproporciondfactura(QString val) {setDBvalue("proporciondfactura",val);};
    inline void setidfactura(QString val)  {setDBvalue("idfactura",val);};

    void borrar();  
};

#endif
