//
// C++ Interface: ListDescuentoPedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCPEDIDOCLIENTE_H
#define LISTDESCPEDIDOCLIENTE_H

#include <qobject.h>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descpedidocliente.h"

class ListDescuentoPedidoCliente {
public:
    company *companyact;
    QString mdb_idpedidocliente;
    QPtrList<DescuentoPedidoCliente> m_lista;
public:
   
    ListDescuentoPedidoCliente(company *comp);
    ListDescuentoPedidoCliente();
    void setcompany(company *c) {
       fprintf(stderr,"ListDescuentoPedidoCliente setCompany\n");
       companyact=c;
       fprintf(stderr,"ListDescuentoPedidoCliente  fin de setCompany\n");
    };
    virtual ~ListDescuentoPedidoCliente();
    void guardaListDescuentoPedidoCliente();
    void vaciar();
    virtual void pintaListDescuentoPedidoCliente() {
        fprintf(stderr,"La función pintaListDescuentoPedidoCliente aun no ha sido implementada\n");
    };
    void cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoPedidoCliente *linpos(int);
    void borraDescuentoPedidoCliente(int);
    void setidpedidocliente(QString id) {
        mdb_idpedidocliente=id;
        DescuentoPedidoCliente *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidpedidocliente(mdb_idpedidocliente);
            i++;
        }// end for
    };    
};

#endif
