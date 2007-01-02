/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
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

#ifndef PRESUPUESTO_H
#define PRESUPUESTO_H

#include <QString>

#include "fixed.h"
#include "listlinpresupuestoview.h"
#include "listdescpresupuestoview.h"
#include "company.h"
#include "dbrecord.h"


/// Administra la informaci&oacute;n de un Presupuesto.
/** */
class Presupuesto : public DBRecord {
protected:
    ListLinPresupuestoView *listalineas;
    ListDescuentoPresupuestoView *listadescuentos;
    company *companyact;

public:
    Presupuesto(company *);
    void setlislinpresupuesto(ListLinPresupuestoView *a) {
        listalineas = a;
        listalineas->setcompany(companyact);
    };
    void setlisdescPresupuesto(ListDescuentoPresupuestoView *a) {
        listadescuentos = a;
        listadescuentos->setcompany(companyact);
    };
    ListLinPresupuestoView* _listalineas() {
        return listalineas;
    };
    ListDescuentoPresupuestoView *_listadescuentos() {
        return listadescuentos;
    };
    company *_company() {
        return companyact;
    };
    QString idcliente() {
        return DBvalue("idcliente");
    };
    QString idalmacen() {
        return DBvalue("idalmacen");
    };
    QString numPresupuesto() {
        return DBvalue("numPresupuesto");
    };
    QString fPresupuesto() {
        return DBvalue("fPresupuesto");
    };
    QString vencPresupuesto() {
        return DBvalue("vencPresupuesto");
    };
    QString contactPresupuesto() {
        return DBvalue("contactPresupuesto");
    };
    QString telPresupuesto() {
        return DBvalue("telPresupuesto");
    };
    QString comentPresupuesto() {
        return DBvalue("comentPresupuesto");
    };
    QString idPresupuesto() {
        return DBvalue("idPresupuesto");
    };
    QString procesadoPresupuesto() {
        return DBvalue("procesadoPresupuesto");
    };
    QString descPresupuesto() {
        return DBvalue("descPresupuesto");
    };
    QString refPresupuesto() {
        return DBvalue("refPresupuesto");
    };
    QString idforma_pago() {
        return DBvalue("idforma_pago");
    };
    QString idtrabajador() {
        return DBvalue("idtrabajador");
    };
    virtual ~Presupuesto();
    virtual int cargar(QString);
    virtual void pintaPresupuesto();
    virtual int guardar();
    virtual int borrar();
    virtual void imprimirPresupuesto();
    virtual void calculaypintatotales();
    virtual QString detalleArticulos();
    virtual void pintaidcliente(QString) {}
    ;
    virtual void pintaIdAlmacen(QString) {}
    ;
    virtual void pintaNumPresupuesto(QString) {}
    ;
    virtual void pintaFPresupuesto(QString) {}
    ;
    virtual void pintaVencPresupuesto(QString) {}
    ;
    virtual void pintaContractPresupuesto(QString) {}
    ;
    virtual void pintaTelPresupuesto(QString) {}
    ;
    virtual void pintaComentPresupuesto(QString) {}
    ;
    virtual void pintaidalmacen(QString) {}
    ;
    virtual void pintaCodigoAlmacen(QString) {}
    ;
    virtual void pintaNomAlmacen(QString) {}
    ;
    virtual void pintaprocesadoPresupuesto(QString) {}
    ;
    virtual void pintadescPresupuesto(QString) {}
    ;
    virtual void pintarefPresupuesto(QString) {}
    ;
    virtual void pintaidforma_pago(QString) {}
    ;
    virtual void pintaidtrabajador(QString) {}
    ;
    virtual void pintatotales(Fixed, Fixed, Fixed, Fixed) {}
    ;
    void setidcliente(QString val) {
        setDBvalue("idcliente", val);
    };
    void setidalmacen(QString val) {
        setDBvalue("idalmacen", val);
    };
    void setidtrabajador(QString val) {
        setDBvalue("idtrabajador", val);
    };
    void setnumPresupuesto(QString val) {
        setDBvalue("numPresupuesto", val);
    };
    void setfPresupuesto(QString val) {
        setDBvalue("fPresupuesto", val);
    };
    void setvencPresupuesto(QString val) {
        setDBvalue("vencPresupuesto", val);
    };
    void setcontactPresupuesto(QString val) {
        setDBvalue("contactPresupuesto", val);
    };
    void settelPresupuesto(QString val) {
        setDBvalue("telPresupuesto", val);
    };
    void setcomentPresupuesto(QString val) {
        setDBvalue("comentPresupuesto", val);
    };
    void setidPresupuesto(QString val) {
        setDBvalue("idPresupuesto", val);
        listalineas->setColumnValue("idPresupuesto", val);
        listadescuentos->setColumnValue("idPresupuesto", val);
    };
    void setprocesadoPresupuesto(QString val) {
        setDBvalue("procesadoPresupuesto", val);
    };
    void setdescPresupuesto(QString val) {
        setDBvalue("descPresupuesto", val);
    };
    void setrefPresupuesto(QString val) {
        setDBvalue("refPresupuesto", val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago", val);
    };
    void vaciaPresupuesto();
};

#endif

