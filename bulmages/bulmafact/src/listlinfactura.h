//
// C++ Interface: ListLinFactura
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINFACTURA_H
#define LISTLINFACTURA_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linfactura.h"

class ListLinFactura {
public:
    company *companyact;
    QString mdb_idfactura;
    QPtrList<LinFactura> m_lista;
public:
   
    ListLinFactura(company *comp);
    ListLinFactura();
    void setcompany(company *c) {
       fprintf(stderr,"ListLinFactura setCompany\n");
       companyact=c;
       fprintf(stderr,"ListLinFactura  fin de setCompany\n");
    };
    virtual ~ListLinFactura();
    void guardaListLinFactura();
    void vaciar();
    virtual void pintaListLinFactura() {
        fprintf(stderr,"La funci� pintaListLinFactura aun no ha sido implementada\n");
    };
    void cargaListLinFactura(QString);
    void borrar();
    void nuevalinea(QString desclfactura, QString cantlfactura, QString pvplfactura, QString descuentolfactura, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString ivalfactura);
    LinFactura *linpos(int);
    float calculabase();
    float calculaiva();
    void borraLinFactura(int);
};

#endif
