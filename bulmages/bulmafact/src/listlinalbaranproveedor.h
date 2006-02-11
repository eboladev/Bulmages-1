//
// C++ Interface: ListLinAlbaranProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINALBARANPROVEEDOR_H
#define LISTLINALBARANPROVEEDOR_H

#include <QObject>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linalbaranproveedor.h"

class ListLinAlbaranProveedor {
public:
    company *companyact;
    QString mdb_idalbaranp;
    Q3PtrList<LinAlbaranProveedor> m_lista;
public:
   
    ListLinAlbaranProveedor(company *comp);
    ListLinAlbaranProveedor();
    void setcompany(company *c) {
       fprintf(stderr,"ListLinAlbaranProveedor setCompany\n");
       companyact=c;
       fprintf(stderr,"ListLinAlbaranProveedor  fin de setCompany\n");
    };
    virtual ~ListLinAlbaranProveedor();
    void guardaListLinAlbaranProveedor();
    void vaciar();
    virtual void pintaListLinAlbaranProveedor() {
        fprintf(stderr,"La función pintaListLinAlbaranProveedor aun no ha sido implementada\n");
    };
    void setidalbaranp(QString id) {
        mdb_idalbaranp=id;
        LinAlbaranProveedor *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidalbaranp(mdb_idalbaranp);
            i++;
        }// end for
    };    
    
    
    void cargaListLinAlbaranProveedor(QString);
    void borrar();
    void nuevalinea(QString desclalbaranp, QString cantlalbaranp, QString pvplalbaranp, QString descontlalbaranp, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalalbaranp);
    LinAlbaranProveedor *linpos(int);
    float calculabase();
    float calculaiva();
    void borraLinAlbaranProveedor(int);
};

#endif
