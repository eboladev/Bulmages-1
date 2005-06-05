//
// C++ Interface: ListLinAlbaranCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINALBARANCLIENTE_H
#define LISTLINALBARANCLIENTE_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linalbarancliente.h"

class ListLinAlbaranCliente {
public:
    company *companyact;
    QString mdb_idalbaran;
    QPtrList<LinAlbaranCliente> m_lista;
public:
   
    ListLinAlbaranCliente(company *comp);
    ListLinAlbaranCliente();
    void setcompany(company *c) {
       fprintf(stderr,"ListLinAlbaranCliente setCompany\n");
       companyact=c;
       fprintf(stderr,"ListLinAlbaranCliente  fin de setCompany\n");
    };
    virtual ~ListLinAlbaranCliente();
    void guardaListLinAlbaranCliente();
    void vaciar();
    virtual void pintaListLinAlbaranCliente() {
        fprintf(stderr,"La funci� pintaListLinAlbaranCliente aun no ha sido implementada\n");
    };
    void cargaListLinAlbaranCliente(QString);
    void borrar();
    void nuevalinea(QString desclalbaran, QString cantlalbaran, QString pvplalbaran, QString descontlalbaran, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo);
    LinAlbaranCliente *linpos(int);
    float calculabase();
    float calculaiva();
    void borraLinAlbaranCliente(int);
};

#endif
