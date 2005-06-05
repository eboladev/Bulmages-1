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
#include "linpresupuesto.h"

class listlinpresupuesto {
public:
    company *companyact;
    QString mdb_idpresupuesto;
    QPtrList<linpresupuesto> m_lista;
public:
   
    listlinpresupuesto(company *comp);
    listlinpresupuesto();
    void setcompany(company *c) {
       fprintf(stderr,"listlinpresupuesto setCompany\n");
       companyact=c;
       fprintf(stderr,"listlinpresupuesto  fin de setCompany\n");
    };
    virtual ~listlinpresupuesto();
    void guardalistlinpresupuesto();
    void vaciar();
    virtual void pintalistlinpresupuesto() {
        fprintf(stderr,"La funci� pintalistlinpresupuesto aun no ha sido implementada\n");
    };
    void chargeBudgetLines(QString);
    void borrar();
    void nuevalinea(QString, QString, QString, QString, QString, QString, QString, QString);
    linpresupuesto *linpos(int);
    float calculabase();
    float calculaiva();
    void borralinpresupuesto(int);
};

#endif
