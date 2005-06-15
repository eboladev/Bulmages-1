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


class Cobro {
protected:
    company *companyact;
    QString mdb_idcobro;
    QString mdb_idcliente;
    QString mdb_fechacobro;
    QString mdb_cantcobro;
    QString mdb_refcobro;
    QString mdb_previsioncobro;
    QString mdb_comentcobro;

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

    void setidcobro(QString val) { mdb_idcobro=val;};
    void setidcliente(QString val) {mdb_idcliente=val;};
    void setfechacobro(QString val) { mdb_fechacobro=val;};
    void setcantcobro(QString val) { mdb_cantcobro=val;};
    void setrefcobro(QString val) { mdb_refcobro=val;};
    void setprevisioncobro(QString val) { mdb_previsioncobro=val;};
    void setcomentcobro(QString val) { mdb_comentcobro=val;};
};
#endif
