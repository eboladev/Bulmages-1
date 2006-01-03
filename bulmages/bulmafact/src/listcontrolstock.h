//
// C++ Interface: ListControlStock
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LISTCONTROLSTOCK_H
#define LISTCONTROLSTOCK_H

#include <qobject.h>
//Added by qt3to4:
#include <Q3PtrList>
#include "funcaux.h"

/**
@author Tomeu Borras
*/

#include "company.h"
#include "controlstock.h"

class ListControlStock {
public:
    company *companyact;
    QString mdb_idinventario;
    Q3PtrList<ControlStock> m_lista;
public:
    ListControlStock(company *comp);
    ListControlStock();
    void setidinventario(QString id) {
        mdb_idinventario=id;
        ControlStock *linea;
        uint i = 0;
        for ( linea = m_lista.first(); linea; linea = m_lista.next() ) {
            linea->setidinventario(mdb_idinventario);
            i++;
        }// end for
    }; 
    
    void setcompany(company *c) {
       fprintf(stderr,"ListControlStock setCompany\n");
       companyact=c;
       fprintf(stderr,"ListControlStock  fin de setCompany\n");
    };
    virtual ~ListControlStock();
    void guardaListControlStock();
    void vaciar();
    virtual void pintaListControlStock() {
        _depura("La funci� pintaListControlStock aun no ha sido implementada\n",2);
    };
    void cargaListControlStock(QString);
    virtual void borrarListControlStock();
    virtual void borrarControlStock(int);

    void nuevalinea(QString idalmacen, QString idarticulo, QString stockantcontrolstock, QString stocknewcontrolstock, QString punteocontrolstock, QString codigocompletoarticulo, QString nomarticulo, QString nomalmacen, QString codigoalmacen);

    ControlStock *linpos(int);
    virtual void pregenerar();
};

#endif
