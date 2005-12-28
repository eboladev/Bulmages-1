//
// C++ Interface: ListDescuentoFacturaProv
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCFACTURAPROV_H
#define LISTDESCFACTURAPROV_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descfacturaprov.h"
#include <funcaux.h>

class ListDescuentoFacturaProv {
public:
    company *companyact;
    QString mdb_idfacturap;
    Q3PtrList<DescuentoFacturaProv> m_lista;
public:
   
    ListDescuentoFacturaProv(company *comp);
    ListDescuentoFacturaProv();
    void setcompany(company *c) {
       _depura("ListDescuentoFacturaProv setCompany\n");
       companyact=c;
       _depura("ListDescuentoFacturaProv  fin de setCompany\n");
    };
    virtual ~ListDescuentoFacturaProv();
    void guardaListDescuentoFacturaProv();
    void vaciar();
    virtual void pintaListDescuentoFacturaProv() {
        _depura("La funcion pintaListDescuentoFacturaProv aun no ha sido implementada\n");
    };
    void cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoFacturaProv *linpos(int);
    void borraDescuentoFacturaProv(int);
    void setidfacturap(QString id) {
        mdb_idfacturap=id;
        DescuentoFacturaProv *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidfacturap(mdb_idfacturap);
            i++;
        }// end for
    };    
};

#endif
