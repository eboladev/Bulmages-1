/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2005 by Alvaro de Miguel                                *
 *   alvaro.demiguel@gmail.com                                             *
 *   http://www.iglues.org                                                 *
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
#include "fichabf.h"


/// Administra la informaci&oacute;n de un Presupuesto.
/** */
class Presupuesto : public FichaBf {

public:
    Presupuesto(Company *, QWidget *);

    QString idcliente() {
        return DBvalue("idcliente");
    };
    QString idalmacen() {
        return DBvalue("idalmacen");
    };
    QString numPresupuesto() {
        return DBvalue("numpresupuesto");
    };
    QString fPresupuesto() {
        return DBvalue("fpresupuesto");
    };
    QString vencPresupuesto() {
        return DBvalue("vencpresupuesto");
    };
    QString contactPresupuesto() {
        return DBvalue("contactpresupuesto");
    };
    QString telPresupuesto() {
        return DBvalue("telpresupuesto");
    };
    QString comentPresupuesto() {
        return DBvalue("comentpresupuesto");
    };
    QString idPresupuesto() {
        return DBvalue("idpresupuesto");
    };
    QString procesadoPresupuesto() {
        return DBvalue("procesadopresupuesto");
    };
    QString descPresupuesto() {
        return DBvalue("descpresupuesto");
    };
    QString refPresupuesto() {
        return DBvalue("refpresupuesto");
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
        setDBvalue("numpresupuesto", val);
    };
    void setfPresupuesto(QString val) {
        setDBvalue("fpresupuesto", val);
    };
    void setvencPresupuesto(QString val) {
        setDBvalue("vencpresupuesto", val);
    };
    void setcontactPresupuesto(QString val) {
        setDBvalue("contactpresupuesto", val);
    };
    void settelPresupuesto(QString val) {
        setDBvalue("telpresupuesto", val);
    };
    void setcomentPresupuesto(QString val) {
        setDBvalue("comentpresupuesto", val);
    };
    void setidPresupuesto(QString val) {
        setDBvalue("idpresupuesto", val);
        m_listalineas->setColumnValue("idpresupuesto", val);
        m_listadescuentos->setColumnValue("idpresupuesto", val);
    };
    void setprocesadoPresupuesto(QString val) {
        setDBvalue("procesadopresupuesto", val);
    };
    void setdescPresupuesto(QString val) {
        setDBvalue("descpresupuesto", val);
    };
    void setrefPresupuesto(QString val) {
        setDBvalue("refpresupuesto", val);
    };
    void setidforma_pago(QString val) {
        setDBvalue("idforma_pago", val);
    };
    void vaciaPresupuesto();
};

#endif

