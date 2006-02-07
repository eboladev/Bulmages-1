//
// C++ Interface: albarancliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ASIENTO1_H
#define ASIENTO1_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "fixed.h"
#include "dbrecord.h"
#include "listlinasiento1.h"

class empresa;

/**
@author Tomeu Borras
*/
class Asiento1 : public DBRecord {
public:
protected:
    ListLinAsiento1 *listalineas;
    empresa *companyact;
public:
    Asiento1(empresa *);
    virtual ~Asiento1();
    int cargaAsiento1(QString );
    Fixed totaldebe() {return listalineas->totaldebe();};
    Fixed totalhaber() {return listalineas->totalhaber();};
    void vaciaAsiento1();
	void abreAsiento1();
	void cierraAsiento1();
	int  estadoAsiento1();
	void guardaAsiento1();
	void setidasiento(QString val) {setDBvalue("idasiento",val);};

    /// Establece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinAsiento1 ( ListLinAsiento1 *a) {
        listalineas =a;
        listalineas->setcompany(companyact);
    };

    virtual void pintaidasiento(QString){_depura("funcion no implementada");};
    virtual void pintadescripcion(QString){_depura("funcion no implementada");};
    virtual void pintafecha(QString){_depura("funcion no implementada");};
    virtual void pintacomentariosasiento(QString){_depura("funcion no implementada");};
    virtual void pintaordenasiento(QString){_depura("funcion no implementada");};
    virtual void pintaclase(QString){_depura("funcion no implementada");};


    virtual void calculaypintatotales() {_depura("funcion no implementada");};
    virtual void pintaAsiento1();

};

#endif
