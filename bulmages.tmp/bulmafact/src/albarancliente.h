//
// C++ Interface: albarancliente
//
// Description: 
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ALBARANCLIENTE_H
#define ALBARANCLIENTE_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "company.h"
#include "listlinalbarancliente.h"
#include "listdescalbarancliente.h"
#include "fixed.h"

/**
@author Tomeu Borras
*/
class AlbaranCliente{
protected:
    ListLinAlbaranCliente *listalineas;
    ListDescuentoAlbaranCliente *listadescuentos;

    company *companyact;

    QString mdb_idalbaran;
    QString mdb_numalbaran;
    QString mdb_fechaalbaran;
    QString mdb_comentalbaran;
    QString mdb_comentprivalbaran;
    QString mdb_idcliente;
    QString mdb_idforma_pago;
    QString mdb_idalmacen;
    QString mdb_refalbaran;
    QString mdb_descalbaran;
    QString mdb_numfactura;
    QString mdb_idtrabajador;
    QString mdb_procesadoalbaran;
    QString mdb_contactalbaran;
    QString mdb_telalbaran;

public:
    AlbaranCliente(company *);
    virtual ~AlbaranCliente();
    /// Establece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinAlbaranCliente ( ListLinAlbaranCliente *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };    
    void setListDescuentoAlbaranCliente ( ListDescuentoAlbaranCliente *a) {
        listadescuentos =a;
	listadescuentos->setcompany(companyact);
    };
    virtual void cargaAlbaranCliente(QString);
    void pintaAlbaranCliente();
    void guardaAlbaranCliente();
    void borraAlbaranCliente();
    ListLinAlbaranCliente* getlistalineas() {return listalineas;};
    ListDescuentoAlbaranCliente* getlistadescuentos() {return listadescuentos;};
    void calculaypintatotales();
    
    virtual void pintaIdAlbaran(QString) {};
    virtual void pintaNumAlbaran(QString) {};
    virtual void pintafechaalbaran(QString) {};
    virtual void pintaComentAlbaran(QString) {};
    virtual void pintaComentPrivAlbaran(QString) {};
    virtual void pintaidcliente(QString ) {};
    virtual void pintaidforma_pago(QString) {};
    virtual void pintaIdFactura(QString){};
    virtual void pintaidalmacen(QString){};
    virtual void pintaNumFactura(QString) {};
    virtual void pintadescalbaran(QString) {};
    virtual void pintarefalbaran(QString) {};
    virtual void pintaidtrabajador(QString) {};
    virtual void pintacontactalbaran(QString) {};
    virtual void pintatelalbaran(QString) {};
    virtual void pintaprocesadoalbaran(QString) {};
    
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {};

    void setidalbaran(QString val) { mdb_idalbaran=val;listalineas->setidalbaran(val);listadescuentos->setidalbaran(val);};
    void setNumAlbaran(QString val) { mdb_numalbaran=val;};
    void setfechaalbaran(QString val) { mdb_fechaalbaran=val;};
    void setcomentalbaran(QString val) { mdb_comentalbaran=val;};
    void setcomentprivalbaran(QString val) {mdb_comentprivalbaran=val;};
    void setidcliente(QString val) { mdb_idcliente=val;};
    void setidforma_pago(QString val) { mdb_idforma_pago=val;};
    void setidtrabajador(QString val) {mdb_idtrabajador=val;};
    void setidalmacen(QString val) {mdb_idalmacen=val;};
    void setNumFactura(QString val) {mdb_numfactura=val;};
    void setrefalbaran(QString val) {mdb_refalbaran=val;};
    void setdescalbaran(QString val) {mdb_descalbaran=val;};
    void setcontactalbaran(QString val) {mdb_contactalbaran=val;};
    void settelalbaran(QString val) {mdb_telalbaran=val;};
    void setprocesadoalbaran(QString val) {mdb_procesadoalbaran=val;};
    void imprimirAlbaranCliente();
    void vaciaAlbaranCliente();    
};

#endif
