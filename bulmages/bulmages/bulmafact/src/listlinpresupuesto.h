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
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linpresupuesto.h"

class listlinpresupuesto {
public:
    company *companyact;
    QString mdb_idpresupuesto;
    Q3PtrList<linpresupuesto> m_lista;
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
        fprintf(stderr,"La función pintalistlinpresupuesto aun no ha sido implementada\n");
    };
    void chargeBudgetLines(QString);
    void borrar();
    void nuevalinea(QString, QString, QString, QString, QString, QString, QString, QString);
    linpresupuesto *linpos(int);
    float calculabase();
    float calculaiva();
    void borralinpresupuesto(int);
    void setidpresupuesto(QString id) {
        mdb_idpresupuesto=id;
        linpresupuesto *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidpresupuesto(mdb_idpresupuesto);
            i++;
        }// end for
    };    
};

#endif
