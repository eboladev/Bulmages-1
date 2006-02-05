//
// C++ Interface: CompArticulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPARTICULO_H
#define COMPARTICULO_H

/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"

class CompArticulo : public DBRecord {
private:

    company *companyact;
    void definetabla();
public:
    CompArticulo(company *);
    CompArticulo(company *, QString, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    CompArticulo(company *, QString , QString , QString , QString , QString );
    virtual ~CompArticulo();
    virtual void pintaCompArticulo() {}
    ;
    void guardaCompArticulo();
    void vaciaCompArticulo();

    inline QString idarticulo() {
        return DBvalue("idarticulo");
    };
    inline QString idcomponente() {
        return DBvalue("idcomponente");
    };
    inline QString cantcomparticulo() {
        return DBvalue("cantcomparticulo");
    };
    inline QString codigocompletocomponente()  {
        return DBvalue("codigocompletocomponente");
    };
    inline QString nomcomponente() {
        return DBvalue("nomcomponente");
    };
    inline void setidarticulo(QString val) {
        setDBvalue("idarticulo",val);
    };
    inline void setcantcomparticulo(QString val) {
        setDBvalue("cantcomparticulo",val);
    };
    void setcodigocompletocomponente(QString);
    void setidcomponente(QString);
    void borrar();
};

#endif
