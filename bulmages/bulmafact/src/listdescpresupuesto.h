//
// C++ Interface: ListDescuentoPresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCPRESUPUESTO_H
#define LISTDESCPRESUPUESTO_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descpresupuesto.h"

class ListDescuentoPresupuesto {
public:
    company *companyact;
    QString mdb_idpresupuesto;
    Q3PtrList<DescuentoPresupuesto> m_lista;
public:

    ListDescuentoPresupuesto(company *comp);
    ListDescuentoPresupuesto();
    void setcompany(company *c) {
        fprintf(stderr,"ListDescuentoPresupuesto setCompany\n");
        companyact=c;
        fprintf(stderr,"ListDescuentoPresupuesto  fin de setCompany\n");
    };
    virtual ~ListDescuentoPresupuesto();
    void guardaListDescuentoPresupuesto();
    void vaciar();
    virtual void pintaListDescuentoPresupuesto() {
        fprintf(stderr,"La funci� pintaListDescuentoPresupuesto aun no ha sido implementada\n");
    };
    int cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoPresupuesto *linpos(int);
    void borraDescuentoPresupuesto(int);
    void setidpresupuesto(QString id) {
        mdb_idpresupuesto=id;
        DescuentoPresupuesto *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidpresupuesto(mdb_idpresupuesto);
            i++;
        }// end for
    };
};

#endif
