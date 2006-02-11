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

#include <QObject>
#include <Q3PtrList>
#include "funcaux.h"

/**
@author Tomeu Borras
*/

#include "company.h"
#include "linfactura.h"

class ListLinFactura {
public:
    company *companyact;
    QString mdb_idfactura;
    Q3PtrList<LinFactura> m_lista;
public:
   
    ListLinFactura(company *comp);
    ListLinFactura();
    void setidfactura(QString id) {
        mdb_idfactura=id;
        LinFactura *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidfactura(mdb_idfactura);
            i++;
        }// end for
    }; 

    void setcompany(company *c) {
       companyact=c;
    };
    virtual ~ListLinFactura();
    void guardaListLinFactura();
    void vaciar();
    virtual void pintaListLinFactura() {
        _depura("La funci� pintaListLinFactura aun no ha sido implementada\n",2);
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
