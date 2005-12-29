//
// C++ Interface: ListDescuentoPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTDESCPEDIDOPROVEEDOR_H
#define LISTDESCPEDIDOPROVEEDOR_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "descpedidoproveedor.h"

class ListDescuentoPedidoProveedor {
public:
    company *companyact;
    QString mdb_idpedidoproveedor;
    Q3PtrList<DescuentoPedidoProveedor> m_lista;
public:
   
    ListDescuentoPedidoProveedor(company *comp);
    ListDescuentoPedidoProveedor();
    void setcompany(company *c) {
       fprintf(stderr,"ListDescuentoPedidoProveedor setCompany\n");
       companyact=c;
       fprintf(stderr,"ListDescuentoPedidoProveedor  fin de setCompany\n");
    };
    virtual ~ListDescuentoPedidoProveedor();
    void guardaListDescuentoPedidoProveedor();
    void vaciar();
    virtual void pintaListDescuentoPedidoProveedor() {
        fprintf(stderr,"La función pintaListDescuentoPedidoProveedor aun no ha sido implementada\n");
    };
    void cargaDescuentos(QString);
    void borrar();
    void nuevalinea(QString concept, QString propor);
    DescuentoPedidoProveedor *linpos(int);
    void borraDescuentoPedidoProveedor(int);
    void setidpedidoproveedor(QString id) {
        mdb_idpedidoproveedor=id;
        DescuentoPedidoProveedor *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidpedidoproveedor(mdb_idpedidoproveedor);
            i++;
        }// end for
    };    
};

#endif
