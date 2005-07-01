//
// C++ Interface: ListDescuentoAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCALBARANCLIENTE_H
#define LISTDESCALBARANCLIENTE_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descalbarancliente.h"

class ListDescuentoAlbaranCliente {
public:
    company *companyact;
    QString mdb_idalbaran;
    QPtrList<DescuentoAlbaranCliente> m_lista;
public:
   
    ListDescuentoAlbaranCliente(company *comp);
    ListDescuentoAlbaranCliente();
    void setcompany(company *c) {
       fprintf(stderr,"ListDescuentoAlbaranCliente setCompany\n");
       companyact=c;
       fprintf(stderr,"ListDescuentoAlbaranCliente  fin de setCompany\n");
    };
    virtual ~ListDescuentoAlbaranCliente();
    void guardaListDescuentoAlbaranCliente();
    void vaciar();
    virtual void pintaListDescuentoAlbaranCliente() {
        fprintf(stderr,"La función pintaListDescuentoAlbaranCliente aun no ha sido implementada\n");
    };
    void cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoAlbaranCliente *linpos(int);
    void borraDescuentoAlbaranCliente(int);
    void setidalbaran(QString id) {
        mdb_idalbaran=id;
        DescuentoAlbaranCliente *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidalbaran(mdb_idalbaran);
            i++;
        }// end for
    };    
};

#endif
