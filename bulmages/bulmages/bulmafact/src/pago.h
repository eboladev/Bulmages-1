//
// C++ Interface: Pago
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PAGO_H
#define PAGO_H

/*
CREATE TABLE pago (
   idpago serial PRIMARY KEY,
   idproveedor integer NOT NULL REFERENCES proveedor(idproveedor),
   fechapago date DEFAULT NOW(),
   cantpago numeric(12,2) DEFAULT 0,
   refpago character varying(12) NOT NULL,
   previsionpago boolean DEFAULT FALSE,
   comentpago character varying(500),
   idtrabajador integer REFERENCES trabajador(idtrabajador)   
);
*/


#include <qstring.h>

/** @author Tomeu Borras
  * \brief Clase que hace de intermediaria entre la tabla de Pago de la base de datos y el programa.
*/
#include "company.h"



class Pago {
protected:
    company *companyact;
    QString mdb_idpago;
    QString mdb_idproveedor;
    QString mdb_fechapago;
    QString mdb_cantpago;
    QString mdb_refpago;
    QString mdb_previsionpago;
    QString mdb_comentpago;

public:
    Pago(company *);
    virtual ~Pago();
    virtual void cargaPago(QString );
    void pintaPago();
    void guardaPago();
    void borraPago();
     void vaciaPago();   
    
    virtual void pintaidpago(QString) {};
    virtual void pintaidproveedor(QString) {};
    virtual void pintafechapago(QString) {};
    virtual void pintacantpago(QString) {};
    virtual void pintarefpago(QString) {};
    virtual void pintaprevisionpago(QString) {};
    virtual void pintacomentpago(QString) {};

    void setidpago(QString val) { mdb_idpago=val;};
    void setidproveedor(QString val) {mdb_idproveedor=val;};
    void setfechapago(QString val) { mdb_fechapago=val;};
    void setcantpago(QString val) { mdb_cantpago=val;};
    void setrefpago(QString val) { mdb_refpago=val;};
    void setprevisionpago(QString val) { mdb_previsionpago=val;};
    void setcomentpago(QString val) { mdb_comentpago=val;};
};
#endif
