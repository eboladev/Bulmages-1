//
// C++ Interface: linpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IVA_H
#define IVA_H

/**
@author Tomeu Borras
*/

#include "dbrecord.h"

class empresa;


class Iva : public DBRecord {
private:
    empresa *m_companyact;
    void definetabla();

public:
    Iva(empresa *);
    Iva(empresa *, QString );

    Iva(empresa *, cursor2 *);
    virtual ~Iva();
    virtual void pintaIva() {};
    void guardaIva();
    void vaciaIva();


    inline QString idiva() {
        return DBvalue("idiva");
    };
    inline QString idtipoiva() {
        return DBvalue("idtipoiva");
    };
    inline QString idregistroiva() {
        return DBvalue("idregistroiva");
    };
    inline QString idcuenta() {
        return DBvalue("idcuenta");
    };
    inline QString codigo() {
        return DBvalue("codigo");
    };
    inline QString nombretipoiva() {
        return DBvalue("nombretipoiva");
    };
    inline QString baseiva()  {
        return DBvalue("baseiva");
    };
    inline QString ivaiva() {
        return DBvalue("ivaiva");
    };



    inline void setidiva(QString val) {
        setDBvalue("idiva",val);
    };
    void setidtipoiva(const QString &val);
    inline void setidregistroiva(QString val) {
        setDBvalue("idregistroiva",val);
    };
    inline void setbaseiva(QString val)  {
        setDBvalue("baseiva",val);
    };
    inline void setivaiva(QString val) {
        setDBvalue("ivaiva",val);
    };

    int borrar();

};

#endif
