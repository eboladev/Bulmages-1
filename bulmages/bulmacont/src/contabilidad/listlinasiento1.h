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
#ifndef LISTLINASIENTO1_H
#define LISTLINASIENTO1_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "empresa.h"
#include "linasiento1.h"

class ListLinAsiento1 {
public:
    empresa *companyact;
    QString mdb_idasiento;
    Q3PtrList<LinAsiento1> m_lista;
public:

    ListLinAsiento1(empresa *comp);
    ListLinAsiento1();
    void setidasiento(QString id) {
        mdb_idasiento=id;
        LinAsiento1 *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidasiento(mdb_idasiento);
            i++;
        }// end for
    };

    void setcompany(empresa *c) {
        companyact=c;
    };
    virtual ~ListLinAsiento1();
    virtual int guardaListLinAsiento1();
    void vaciar();
    virtual void pintaListLinAsiento1() {
        _depura("La funcion pintaListLinFactura aun no ha sido implementada\n",2);
    };
    void cargaListLinAsiento1(QString);
    void borrar();
    LinAsiento1 *linpos(int);
    void borraLinAsiento1(int);
    Fixed totaldebe();
    Fixed totalhaber();

};

#endif
