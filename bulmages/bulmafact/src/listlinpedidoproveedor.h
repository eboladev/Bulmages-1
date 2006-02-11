//
// C++ Interface: ListLinPedidoProveedor
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPEDIDOPROVEEDOR_H
#define LISTLINPEDIDOPROVEEDOR_H

#include <QObject>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linpedidoproveedor.h"

class ListLinPedidoProveedor {
public:
    company *companyact;
    QString mdb_idpedidoproveedor;
    Q3PtrList<LinPedidoProveedor> m_lista;
public:

    ListLinPedidoProveedor(company *comp);
    ListLinPedidoProveedor();
    void setcompany(company *c) {
        fprintf(stderr,"ListLinPedidoProveedor setCompany\n");
        companyact=c;
        fprintf(stderr,"ListLinPedidoProveedor  fin de setCompany\n");
    };
    virtual ~ListLinPedidoProveedor();
    void guardaListLinPedidoProveedor();
    void vaciar();
    void setidpedidoproveedor(QString id) {
        mdb_idpedidoproveedor=id;
        LinPedidoProveedor *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidpedidoproveedor(mdb_idpedidoproveedor);
            i++;
        }// end for
    };
    virtual void pintaListLinPedidoProveedor() {
        fprintf(stderr,"La funci� pintaListLinPedidoProveedor aun no ha sido implementada\n");
    };
    void cargaListLinPedidoProveedor(QString);
    void borrar();
    void nuevalinea(QString desclpedidoproveedor, QString cantlpedidoproveedor, QString pvplpedidoproveedor, QString prevlpedidoproveedor, QString ivalpedidoproveedor, QString descuentolpedidoproveedor, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString puntlpedidoproveedor);
    LinPedidoProveedor *linpos(int);
    float calculabase();
    float calculaiva();
    void borraLinPedidoProveedor(int);
};

#endif
