/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef FACTURA_H
#define FACTURA_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "listlinfacturaview.h"
#include "listdescfacturaview.h"
#include "fixed.h"
#include "company.h"
#include "dbrecord.h"


class Factura : public DBRecord {

protected:
    ListLinFacturaView *listalineas;
    ListDescuentoFacturaView *listadescuentos;
    company *companyact;

public:
    Factura(company *);
    virtual ~Factura();
    /// Establece cual es la lista subformulario del presupuesto. Normalmente para
    /// apuntar listlinpresupuestoview.
    void setListLinFactura ( ListLinFacturaView *a) {
        listalineas = a;
        listalineas->setcompany(companyact);
    };
    void setListDescuentoFactura ( ListDescuentoFacturaView *a) {
        listadescuentos = a;
        listadescuentos->setcompany(companyact);
    };
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
    QString idforma_pago() {
        return DBvalue("idforma_pago");
    };
    ListLinFacturaView* getlistalineas() {
        return listalineas;
    };
    ListDescuentoFacturaView* getlistadescuentos() {
        return listadescuentos;
    };
    virtual int cargar(QString);
    void pintaFactura();
    virtual int guardar();
    virtual int  borrar();
    void setidcliente(QString val) {
        setDBvalue("idcliente", val);
    };
    void setidalmacen(QString val) {
        setDBvalue("idalmacen", val);
    };
    void setreffactura(QString val) {
        setDBvalue("reffactura", val);
    };
    void setNumFactura(QString val) {
        setDBvalue("numfactura", val);
    };
    void setfechafactura(QString val) {
        setDBvalue("ffactura", val);
    };
    void setdescfactura(QString val) {
        setDBvalue("descfactura", val);
    };
    void setcodigoserie_factura(QString val) {
        setDBvalue("codigoserie_factura", val);
    };
    void setcomentfactura(QString val) {
        setDBvalue("comentfactura", val);
    };
    void setidfactura(QString val) {
        setDBvalue("idfactura", val);
        listalineas->setColumnValue( "idfactura", val);
        listadescuentos->setColumnValue( "idfactura", val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago", val);
    };
    void setprocesadafactura(QString val) {
        setDBvalue("procesadafactura", val);
    };
    void vaciaFactura();
    virtual void imprimirFactura();
    virtual void calculaypintatotales();
    virtual void pintaidcliente(QString) {}
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
    virtual void pintaComentFactura(QString) {}
    ;
    virtual void pintaidalmacen(QString ) {}
    ;
    virtual void pintaidforma_pago(QString) {}
    ;
    virtual void pintaprocesadafactura(QString) {}
    ;
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {}
    ;
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

