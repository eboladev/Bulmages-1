//
// C++ Interface: ListIva
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTIVA_H
#define LISTIVA_H

#include <QObject>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "empresa.h"
#include "iva.h"
#include "funcaux.h"

class ListIva {
public:
    empresa *companyact;
    QString mdb_idregistroiva;
    Q3PtrList<Iva> m_lista;
public:
   
    ListIva(empresa *comp);
    ListIva();
    void setcompany(empresa *c) {
       companyact=c;
    };
    virtual ~ListIva();
    void guardaListIva();
    void vaciar();
    virtual void pintaListIva() {
        _depura("Esta funcion aun no ha sido implementada\n",2);
    };
    int cargaListIva(QString);
    void borrar();
    Iva *linpos(int);
    void append(Iva *iva) {m_lista.append(iva);};


    Fixed calculaiva();
    Fixed calculabase();

    void borraIva(int);
    void setidregistroiva(QString id) {
        mdb_idregistroiva=id;
        Iva *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidregistroiva(mdb_idregistroiva);
            i++;
        }// end for
    };    
};

#endif
