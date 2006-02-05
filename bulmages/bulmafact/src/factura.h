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
#ifndef FACTURA_H
#define FACTURA_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "listlinfactura.h"
#include "listdescfactura.h"
#include "fixed.h"
#include "company.h"
#include "linfactura.h"
#include "dbrecord.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de factura de la base de datos y el programa.
*/


class Factura : public DBRecord {
protected:
    ListLinFactura *listalineas;
    ListDescuentoFactura *listadescuentos;

    company *companyact;

public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinFactura ( ListLinFactura *a) {
        listalineas =a;
        listalineas->setcompany(companyact);
    };

    void setListDescuentoFactura ( ListDescuentoFactura *a) {
        listadescuentos =a;
        listadescuentos->setcompany(companyact);
    };
/*
    ListDescuentoFactura *listadescuentos() {
        return listadescuentos;
    };
*/
    company *_company() {
        return companyact;
    };

    QString codigoserie_factura() {
        return DBvalue("codigoserie_factura");
    };
    QString idcliente() {
        return DBvalue("idcliente");
    };
    QString idalmacen() {
        return DBvalue("idalmacen");
    };
    QString numfactura() {
        return DBvalue("numfactura");
    };
    QString ffactura() {
        return DBvalue("ffactura");
    };
    QString procesadafactura() {
        return DBvalue("procesadafactura");
    };
    QString comentfactura() {
        return DBvalue("comentfactura");
    };
    QString reffactura() {
        return DBvalue("reffactura");
    };
    QString descfactura() {
        return DBvalue("descfactura");
    };
    QString idfactura() {
        return DBvalue("idfactura");
    };
    QString idusuari() {
        return DBvalue("idusuari");
    };
    QString idforma_pago() {
        return DBvalue("idforma_pago");
    };


    ListLinFactura* getlistalineas() {
        return listalineas;
    };
    ListDescuentoFactura* getlistadescuentos() {
        return listadescuentos;
    };

    Factura(company *);
    virtual ~Factura();
    virtual int cargaFactura(QString );
    void pintaFactura();
    void guardaFactura();
    void borraFactura();
    void imprimirFactura();
    void calculaypintatotales();

    virtual void pintaidcliente(QString ) {}
    ;

    virtual void pintareffactura(QString) {}
    ;
    virtual void pintaNumFactura(QString ) {}
    ;
    virtual void pintafechafactura(QString ) {}
    ;
    virtual void pintadescfactura(QString) {}
    ;

    virtual void pintacodigoserie_factura(QString) {}
    ;
    virtual void pintaComentFactura(QString ) {}
    ;

    virtual void pintaidalmacen(QString ) {}
    ;

    virtual void pintaidforma_pago(QString) {}
    ;
    virtual void pintaprocesadafactura(QString) {}
    ;
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {}
    ;

    void setidcliente(QString val) {
        setDBvalue("idcliente",val);
    };
    void setidalmacen(QString val) {
        setDBvalue("idalmacen",val);
    };
    void setreffactura(QString val) {
        setDBvalue("reffactura",val);
    };
    void setNumFactura(QString val) {
        setDBvalue("numfactura",val);
    };
    void setfechafactura(QString val) {
        setDBvalue("ffactura",val);
    };
    void setdescfactura(QString val) {
        setDBvalue("descfactura",val);
    };

    void setcodigoserie_factura(QString val) {
        setDBvalue("codigoserie_factura",val);
    };
    void setcomentfactura(QString val) {
        setDBvalue("comentfactura",val);
    };

    void setidfactura(QString val) {
        setDBvalue("idfactura",val);
        listalineas->setidfactura(val);
        listadescuentos->setidfactura(val);
    };
    void setIdUsuari(QString val) {
        setDBvalue("idusuari",val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago",val);
    };
    void setprocesadafactura(QString val) {
        setDBvalue("procesadafactura",val);
    };
    void vaciaFactura();

    virtual void cargaFacturaDescuentas(QString) {}
    ;
    virtual void calculateImports() {}
    ;
    virtual void inicialize() {}
    ;
    virtual QString calculateValues() {
        return "";
    };
};
#endif
