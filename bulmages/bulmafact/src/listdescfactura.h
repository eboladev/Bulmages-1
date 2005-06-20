//
// C++ Interface: ListDescuentoFactura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCFACTURA_H
#define LISTDESCFACTURA_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descfactura.h"

class ListDescuentoFactura {
public:
    company *companyact;
    QString mdb_idfactura;
    QPtrList<DescuentoFactura> m_lista;
public:
   
    ListDescuentoFactura(company *comp);
    ListDescuentoFactura();
    void setcompany(company *c) {
       fprintf(stderr,"ListDescuentoFactura setCompany\n");
       companyact=c;
       fprintf(stderr,"ListDescuentoFactura  fin de setCompany\n");
    };
    virtual ~ListDescuentoFactura();
    void guardaListDescuentoFactura();
    void vaciar();
    virtual void pintaListDescuentoFactura() {
        fprintf(stderr,"La funci� pintaListDescuentoFactura aun no ha sido implementada\n");
    };
    void cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoFactura *linpos(int);
    void borraDescuentoFactura(int);
    void setidfactura(QString id) {
        mdb_idfactura=id;
        DescuentoFactura *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidfactura(mdb_idfactura);
            i++;
        }// end for
    };    
};

#endif
