//
// C++ Interface: Cobro
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COBRO_H
#define COBRO_H

#include <qstring.h>

/** @author Tomeu Borras
  * \brief Clase que hace de intermediaria entre la tabla de Cobro de la base de datos y el programa.
*/
#include "company.h"
#include "dbrecord.h"

class Cobro : public DBRecord {
protected:
    company *companyact;
public:
    Cobro(company *);
    virtual ~Cobro();
    virtual void cargaCobro(QString );
    void pintaCobro();
    void guardaCobro();
    void borraCobro();
     void vaciaCobro();   
    
    virtual void pintaidcobro(QString) {};
    virtual void pintaidcliente(QString) {};
    virtual void pintafechacobro(QString) {};
    virtual void pintacantcobro(QString) {};
    virtual void pintarefcobro(QString) {};
    virtual void pintaprevisioncobro(QString) {};
    virtual void pintacomentcobro(QString) {};

    void setidcobro(QString val) { setDBvalue("idcobro",val);};
    void setidcliente(QString val) {setDBvalue("idcliente",val);};
    void setfechacobro(QString val) { setDBvalue("fechacobro",val);};
    void setcantcobro(QString val) { setDBvalue("cantcobro",val);};
    void setrefcobro(QString val) { setDBvalue("refcobro",val);};
    void setprevisioncobro(QString val) { setDBvalue("previsioncobro",val);};
    void setcomentcobro(QString val) { setDBvalue("comentcobro",val);};
};
#endif
