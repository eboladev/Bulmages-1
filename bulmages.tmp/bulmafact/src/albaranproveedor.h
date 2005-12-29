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
#include "listlinalbaranproveedor.h"
#include "listdescalbaranprov.h"
/**
@author Tomeu Borras
*/
class AlbaranProveedor{
protected:
    ListLinAlbaranProveedor *listalineas;
    ListDescuentoAlbaranProv *listadescuentos;

    company *companyact;

    QString mdb_idalbaranp;
    QString mdb_numalbaranp;
    QString mdb_fechaalbaranp;
    QString mdb_loginusuario;
    QString mdb_comentalbaranp;
    QString mdb_idproveedor;
    QString mdb_idforma_pago;
    QString mdb_idalmacen;
    QString mdb_refalbaranp;
    QString mdb_descalbaranp;
    QString mdb_numfactura;

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



    virtual void cargaAlbaranProveedor(QString);
    void pintaAlbaranProveedor();
    void guardaAlbaranProveedor();
    void borraAlbaranProveedor();
    ListLinAlbaranProveedor* getlistalineas() {return listalineas;};
    ListDescuentoAlbaranProv* getlistadescuentos() {return listadescuentos;};

    virtual void pintaidalbaranp(QString) {};
    virtual void pintanumalbaranp(QString) {};
    virtual void pintafechaalbaranp(QString) {};
    virtual void pintaloginusuario(QString) {};
    virtual void pintacomentalbaranp(QString) {};
    virtual void pintaidproveedor(QString ) {};
    virtual void pintaidforma_pago(QString) {};
    virtual void pintaidalmacen(QString){};
    virtual void pintadescalbaranp(QString) {};
    virtual void pintarefalbaranp(QString) {};
    virtual void pintatotales(float, float) {};

    void setidalbaranp(QString val) { mdb_idalbaranp=val;listalineas->setidalbaranp(val);listadescuentos->setidalbaranp(val);};
    void setnumalbaranp(QString val) { mdb_numalbaranp=val;};
    void setfechaalbaranp(QString val) { mdb_fechaalbaranp=val;};
    void setloginusuario(QString val) { mdb_loginusuario=val;};
    void setcomentalbaranp(QString val) { mdb_comentalbaranp=val;};
    void setidproveedor(QString val) { mdb_idproveedor=val;};
    void setidforma_pago(QString val) { mdb_idforma_pago=val;};
    void setidalmacen(QString val) {mdb_idalmacen=val;};
    void setrefalbaranp(QString val) {mdb_refalbaranp=val;};
    void setdescalbaranp(QString val) {mdb_descalbaranp=val;};  
    void imprimirAlbaranProveedor();
    void vaciaAlbaranProveedor();    
};

#endif
