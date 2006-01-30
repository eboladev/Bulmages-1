//
// C++ Interface: presupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FACTURAP_H
#define FACTURAP_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "listlinfacturap.h"
#include "company.h"
#include "linfacturap.h"
#include "listdescfacturaprov.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de facturap de la base de datos y el programa.
*/


class FacturaProveedor {
protected:
    ListLinFacturaProveedor *listalineas;
    ListDescuentoFacturaProv *listadescuentos;

    company *companyact;

    QString mdb_idproveedor;
    QString mdb_numfacturap;
    QString mdb_ffacturap;
    QString mdb_procesadafacturap;
    
    
    QString mdb_comentfacturap;

    QString mdb_reffacturap;
    QString mdb_descfacturap;

    QString mdb_idfacturap;
    QString mdb_idusuari;
    QString mdb_idforma_pago;

public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinFacturaProveedor ( ListLinFacturaProveedor *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };
    void setListDescuentoFacturaProv ( ListDescuentoFacturaProv *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    }; 

    ListLinFacturaProveedor* getlistalineas() {return listalineas;};    
    ListDescuentoFacturaProv* getlistadescuentos() {return listadescuentos;};

    FacturaProveedor(company *);
    virtual ~FacturaProveedor();
    virtual int cargaFacturaProveedor(QString );
    void pintaFacturaProveedor();
    void guardaFacturaProveedor();
    void borraFacturaProveedor();
    void imprimirFacturaProveedor();
    
    virtual void pintaidproveedor(QString ) {};

    virtual void pintareffacturap(QString) {};
    virtual void pintanumfacturap(QString ) {};
    virtual void pintafechafacturap(QString ) {};
    virtual void pintadescfacturap(QString) {};
    virtual void pintacomentfacturap(QString ) {};
    virtual void pintaidforma_pago(QString) {};
    virtual void pintaprocesadafacturap(QString) {};
    virtual void pintatotales(float, float) {};

    void setidproveedor(QString val) { mdb_idproveedor=val;};
    void setreffacturap(QString val) {mdb_reffacturap=val;};
    void setnumfacturap(QString val) { mdb_numfacturap=val;};
    void setfechafacturap(QString val) { mdb_ffacturap=val;};
    void setdescfacturap(QString val) {mdb_descfacturap=val;};
    void setcomentfacturap(QString val) { mdb_comentfacturap=val;};

    void setidfacturap(QString val) {mdb_idfacturap=val;listalineas->setidfacturap(val);listadescuentos->setidfacturap(val);};
    void setIdUsuari(QString val) {mdb_idusuari=val;};
    void setidforma_pago(QString val) {mdb_idforma_pago=val;};
    void setprocesadafacturap(QString val) {mdb_procesadafacturap=val;};
    void vaciaFacturaProveedor();

    virtual void cargaFacturaProveedorDescuentas(QString) {};
    virtual void calculateImports() {};
    virtual void inicialize() {};
    virtual QString calculateValues() {return "";};
};
#endif
