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

#include "dbrecord.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de facturap de la base de datos y el programa.
*/


class FacturaProveedor : public DBRecord {
protected:
    ListLinFacturaProveedor *listalineas;
    ListDescuentoFacturaProv *listadescuentos;
    company *companyact;
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

    void setidproveedor(QString val) { setDBvalue("idproveedor",val);};
    void setreffacturap(QString val) {setDBvalue("reffacturap",val);};
    void setnumfacturap(QString val) { setDBvalue("numfacturap",val);};
    void setfechafacturap(QString val) { setDBvalue("ffacturap",val);};
    void setdescfacturap(QString val) {setDBvalue("descfacturap",val);};
    void setcomentfacturap(QString val) { setDBvalue("comentfacturap",val);};

    void setidfacturap(QString val) {setDBvalue("idfacturap",val);listalineas->setidfacturap(val);listadescuentos->setidfacturap(val);};
    void setIdUsuari(QString val) {setDBvalue("idusuari",val);};
    void setidforma_pago(QString val) {setDBvalue("idforma_pago",val);};
    void setprocesadafacturap(QString val) {setDBvalue("procesadafacturap",val);};
    void vaciaFacturaProveedor();

    virtual void cargaFacturaProveedorDescuentas(QString) {};
    virtual void calculateImports() {};
    virtual void inicialize() {};
    virtual QString calculateValues() {return "";};
};
#endif
