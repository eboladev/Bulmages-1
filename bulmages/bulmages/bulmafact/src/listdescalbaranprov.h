//
// C++ Interface: ListDescuentoAlbaranProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCALBARANPROV_H
#define LISTDESCALBARANPROV_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descalbaranprov.h"
#include "funcaux.h"

class ListDescuentoAlbaranProv {
public:
    company *companyact;
    QString mdb_idalbaranp;
    Q3PtrList<DescuentoAlbaranProv> m_lista;
public:
   
    ListDescuentoAlbaranProv(company *comp);
    ListDescuentoAlbaranProv();
    void setcompany(company *c) {
       _depura("ListDescuentoAlbaranProv setCompany\n");
       companyact=c;
       _depura("ListDescuentoAlbaranProv  fin de setCompany\n");
    };
    virtual ~ListDescuentoAlbaranProv();
    void guardaListDescuentoAlbaranProv();
    void vaciar();
    virtual void pintaListDescuentoAlbaranProv() {
        _depura("La funcion pintaListDescuentoAlbaranProv aun no ha sido implementada\n");
    };
    void cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoAlbaranProv *linpos(int);
    void borraDescuentoAlbaranProv(int);
    void setidalbaranp(QString id) {
        mdb_idalbaranp=id;
        DescuentoAlbaranProv *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidalbaranp(mdb_idalbaranp);
            i++;
        }// end for
    };    
};

#endif
