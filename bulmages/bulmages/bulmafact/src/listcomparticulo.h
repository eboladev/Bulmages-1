//
// C++ Interface: ListCompArticulo
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTCOMPARTICULO_H
#define LISTCOMPARTICULO_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "comparticulo.h"

class ListCompArticulo {
public:
    company *companyact;
    QString mdb_idarticulo;
    Q3PtrList<CompArticulo> m_lista;
public:
   
    ListCompArticulo(company *comp);
    ListCompArticulo();
    void setcompany(company *c) {
       fprintf(stderr,"ListCompArticulo setCompany\n");
       companyact=c;
       fprintf(stderr,"ListCompArticulo  fin de setCompany\n");
    };
    virtual ~ListCompArticulo();
    void guardaListCompArticulo();
    void vaciar();
    virtual void pintaListCompArticulo() {
        fprintf(stderr,"La función pintaListCompArticulo aun no ha sido implementada\n");
    };
    void cargaListCompArticulo(QString);
    void borrar();
    void nuevalinea(QString, QString, QString, QString);
    CompArticulo *linpos(int);
    void borraCompArticulo(int);
    void setidarticulo(QString id) {
        mdb_idarticulo=id;
        CompArticulo *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidarticulo(mdb_idarticulo);
            i++;
        }// end for
    };    
};

#endif
