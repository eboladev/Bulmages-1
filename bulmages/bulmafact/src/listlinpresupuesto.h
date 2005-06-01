//
// C++ Interface: listlinpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPRESUPUESTO_H
#define LISTLINPRESUPUESTO_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
class linpresupuesto;

class listlinpresupuesto {
private:
    QString mdb_idpresupuesto;
public:
    QPtrList<linpresupuesto> m_lista;
    company *companyact;
    listlinpresupuesto(company *comp);
    virtual ~listlinpresupuesto();
    void guardalistlinpresupuesto();
    void vaciar();
    virtual void pintalistlinpresupuesto() {
        fprintf(stderr,"La funci� pintalistlinpresupuesto aun no ha sido implementada\n");
    };
    void chargeBudgetLines(QString);
    void borrar();
    void nuevalinea(QString, QString, QString, QString, QString, QString, QString);
    linpresupuesto *linpos(int);
};

#endif
