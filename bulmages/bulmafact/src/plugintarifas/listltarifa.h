//
// C++ Interface: ListLTarifa
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTLTARIFA_H
#define LISTLTARIFA_H

#include <QObject>
//Added by qt3to4:
#include <Q3PtrList>

/**
@author Tomeu Borras
*/

#include "company.h"
#include "ltarifa.h"
#include "funcaux.h"
#include "fixed.h"

class ListLTarifa {
private:

    Q3PtrList<LTarifa> m_lista;

protected:
    company *m_companyact;
    QString mdb_idtarifa;
    QString mdb_idarticulo;
    QString mdb_idalmacen;
public:
	company *companyact() {return m_companyact;};
       Q3PtrList<LTarifa>* lista() {return &m_lista;};
    ListLTarifa(company *comp);
    ListLTarifa();
    void setcompany(company *c) {
       m_companyact=c;
    };
    virtual ~ListLTarifa();
    void guardar();
    void vaciar();
    virtual void pintar() {
        _depura("Esta funcion aun no ha sido implementada\n",2);
    };
    int cargarParaArticulo(QString);
//    void borrar();
    LTarifa *linpos(int);
    void append(LTarifa *ltarifa) {m_lista.append(ltarifa);};


    void borrar(int);
    void setidtarifa(QString id) {
        mdb_idtarifa=id;
        LTarifa *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidtarifa(mdb_idtarifa);
            i++;
        }// end for
    };    
};

#endif
