//
// C++ Interface: ListLinPedidoCliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLINPEDIDOCLIENTE_H
#define LISTLINPEDIDOCLIENTE_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linpedidocliente.h"
#include "funcaux.h"

class ListLinPedidoCliente {
public:
    company *companyact;
    QString mdb_idpedidocliente;
    Q3PtrList<LinPedidoCliente> m_lista;
public:

    ListLinPedidoCliente(company *comp);
    ListLinPedidoCliente();
    void setcompany(company *c) {
        companyact=c;
    };
    virtual ~ListLinPedidoCliente();
    void guardaListLinPedidoCliente();
    void vaciar();
    void setidpedidocliente(QString id) {
        mdb_idpedidocliente=id;
        LinPedidoCliente *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidpedidocliente(mdb_idpedidocliente);
            i++;
        }// end for
    };
    virtual void pintaListLinPedidoCliente() {
        _depura("La funcion pintaListLinPedidoCliente aun no ha sido implementada\n",2);
    };
    void cargaListLinPedidoCliente(QString);
    void borrar();
    void nuevalinea(QString desclpedidocliente, QString cantlpedidocliente, QString pvplpedidocliente, QString prevlpedidocliente, QString ivalpedidocliente, QString descuentolpedidocliente, QString idarticulo, QString codigocompletoarticulo, QString nomarticulo, QString puntlpedidocliente);
    LinPedidoCliente *linpos(int);
    float calculabase();
    float calculaiva();
    void borraLinPedidoCliente(int);
};

#endif
