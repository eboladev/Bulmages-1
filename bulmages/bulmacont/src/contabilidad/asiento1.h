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
#ifndef ASIENTO1_H
#define ASIENTO1_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "empresa.h"

#include "fixed.h"
#include "dbrecord.h"
#include "listlinasiento1.h"

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

/*
    /// Funciones de retorno de valores
    QString idalbaran() {return DBvalue("idalbaran");};
    QString numalbaran() {return DBvalue("numalbaran");};
    QString fechaalbaran() {return DBvalue("fechaalbaran");};
    QString comentalbaran() {return DBvalue("comentalbaran");};
    QString comentprivalbaran() {return DBvalue("comentprivalbaran");};
    QString idcliente() {return DBvalue("idcliente");};
    QString idforma_pago() {return DBvalue("idforma_pago");};
    QString idalmacen() {return DBvalue("idalmacen");};
    QString refalbaran() {return DBvalue("refalbaran");};
    QString descalbaran() {return DBvalue("descalbaran");};
    QString numfactura() {return DBvalue("numfactura");};
    QString idtrabajador() {return DBvalue("idtrabajador");};
    QString procesadoalbaran() {return DBvalue("procesadoalbaran");};
    QString contactalbaran() {return DBvalue("contactalbaran");};
    QString telalbaran() {return DBvalue("telalbaran");};

*/
    /// Establece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinAsiento1 ( ListLinAsiento1 *a) {
        listalineas =a;
	listalineas->setcompany(companyact);
    };    

/*

    virtual int cargaAlbaranCliente(QString);
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

    void setidalbaran(QString val) { setDBvalue("idalbaran",val);listalineas->setidalbaran(val);listadescuentos->setidalbaran(val);};
    void setNumAlbaran(QString val) { setDBvalue("numalbaran",val);};
    void setfechaalbaran(QString val) { setDBvalue("fechaalbaran",val);};
    void setcomentalbaran(QString val) { setDBvalue("comentalbaran",val);};
    void setcomentprivalbaran(QString val) {setDBvalue("comentprivalbaran",val);};
    void setidcliente(QString val) { setDBvalue("idcliente",val);};
    void setidforma_pago(QString val) { setDBvalue("idforma_pago",val);};
    void setidtrabajador(QString val) {setDBvalue("idtrabajador",val);};
    void setidalmacen(QString val) {setDBvalue("idalmacen",val);};
    void setNumFactura(QString val) {setDBvalue("numfactura",val);};
    void setrefalbaran(QString val) {setDBvalue("refalbaran",val);};
    void setdescalbaran(QString val) {setDBvalue("descalbaran",val);};
    void setcontactalbaran(QString val) {setDBvalue("contactalbaran",val);};
    void settelalbaran(QString val) {setDBvalue("telalbaran",val);};
    void setprocesadoalbaran(QString val) {setDBvalue("procesadoalbaran",val);};
    void imprimirAlbaranCliente();
    void vaciaAlbaranCliente();
*/
};

#endif
