//
// C++ Interface: albaranpproveedor
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ALBARANPROVEEDOR_H
#define ALBARANPROVEEDOR_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "company.h"
#include "dbrecord.h"

#include "listlinalbaranproveedor.h"
#include "listdescalbaranprov.h"
/**
@author Tomeu Borras
*/
class AlbaranProveedor : public DBRecord {
protected:
    ListLinAlbaranProveedor *listalineas;
    ListDescuentoAlbaranProv *listadescuentos;

    company *companyact;

public:
    AlbaranProveedor(company *);
    virtual ~AlbaranProveedor();
    /// Establece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinAlbaranProveedor ( ListLinAlbaranProveedor *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    void setListDescuentoAlbaranProveedor ( ListDescuentoAlbaranProv *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    };



    virtual int cargaAlbaranProveedor(QString);
    void pintaAlbaranProveedor();
    void guardaAlbaranProveedor();
    void borraAlbaranProveedor();
    ListLinAlbaranProveedor* getlistalineas() {return listalineas;};
    ListDescuentoAlbaranProv* getlistadescuentos() {return listadescuentos;};

    virtual void pintaidalbaranp(QString) {};
    virtual void pintanumalbaranp(QString) {};
    virtual void pintafechaalbaranp(QString) {};
    virtual void pintacomentalbaranp(QString) {};
    virtual void pintaidproveedor(QString ) {};
    virtual void pintaidforma_pago(QString) {};
    virtual void pintaidalmacen(QString){};
    virtual void pintadescalbaranp(QString) {};
    virtual void pintarefalbaranp(QString) {};
    virtual void pintatotales(float, float) {};

    void setidalbaranp(QString val) { setDBvalue("idalbaranp",val);listalineas->setidalbaranp(val);listadescuentos->setidalbaranp(val);};
    void setnumalbaranp(QString val) { setDBvalue("numalbaranp",val);};
    void setfechaalbaranp(QString val) { setDBvalue("fechaalbaranp",val);};
    void setloginusuario(QString val) { setDBvalue("loginusuario",val);};
    void setcomentalbaranp(QString val) { setDBvalue("comentalbaranp",val);};
    void setidproveedor(QString val) { setDBvalue("idproveedor",val);};
    void setidforma_pago(QString val) { setDBvalue("idforma_pago",val);};
    void setidalmacen(QString val) {setDBvalue("idalmacen",val);};
    void setrefalbaranp(QString val) {setDBvalue("refalbaranp",val);};
    void setdescalbaranp(QString val) {setDBvalue("descalbaranp",val);};  
    void imprimirAlbaranProveedor();
    void vaciaAlbaranProveedor();    
};

#endif
