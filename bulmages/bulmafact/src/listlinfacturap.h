//
// C++ Interface: ListLinFacturaProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINFACTURAP_H
#define LISTLINFACTURAP_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linfacturap.h"

class ListLinFacturaProveedor {
public:
    company *companyact;
    QString mdb_idfacturap;
    QPtrList<LinFacturaProveedor> m_lista;
public:
   
    ListLinFacturaProveedor(company *comp);
    ListLinFacturaProveedor();
    void setidfacturap(QString id) {
        mdb_idfacturap=id;
        LinFacturaProveedor *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidfacturap(mdb_idfacturap);
            i++;
        }// end for
    }; 
    
    void setcompany(company *c) {
       fprintf(stderr,"ListLinFacturaProveedor setCompany\n");
       companyact=c;
       fprintf(stderr,"ListLinFacturaProveedor  fin de setCompany\n");
    };
    virtual ~ListLinFacturaProveedor();
    void guardaListLinFacturaProveedor();
    void vaciar();
    virtual void pintaListLinFacturaProveedor() {
        fprintf(stderr,"La función pintaListLinFacturaProveedor aun no ha sido implementada\n");
    };
    void cargaListLinFacturaProveedor(QString);
    void borrar();
    void nuevalinea(QString desclfacturap, QString cantlfacturap, QString pvplfacturap, QString descuentolfacturap, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfacturap);
    LinFacturaProveedor *linpos(int);
    float calculabase();
    float calculaiva();
    void borraLinFacturaProveedor(int);
};

#endif
