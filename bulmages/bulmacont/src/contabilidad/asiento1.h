//
// C++ Interface: albarancliente
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ASIENTO1_H
#define ASIENTO1_H

#include <qstring.h>
#include <q3table.h>
#include <q3ptrlist.h>

#include "fixed.h"
#include "dbrecord.h"
#include "listlinasiento1view.h"

class empresa;

/**
@author Tomeu Borras
*/
class Asiento1 : public DBRecord {
public:
    enum estadoasiento {ASVacio=0, ASAbierto=1, ASCerrado=2};
protected:
    ListLinAsiento1View *listalineas;
    empresa *m_companyact;
public:
    empresa *companyact() {
        return m_companyact;
    };
    Asiento1(empresa *);
    virtual ~Asiento1();
    int cargar(QString );
    Fixed totaldebe() {
        return listalineas->totaldebe();
    };
    Fixed totalhaber() {
        return listalineas->totalhaber();
    };
    void vaciaAsiento1();
    void abreAsiento1();
    void cierraAsiento1();
    estadoasiento  estadoAsiento1();
    int guardar();
    void setidasiento(QString val) {
        setDBvalue("idasiento",val);
	listalineas->setColumnValue("idasiento",val);
    };
    QString idasiento() {
        return DBvalue("idasiento");
    };

    /// Establece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.
    void setListLinAsiento1 ( ListLinAsiento1View *a) {
        listalineas =a;
    };
    void borraAsiento1();

    virtual void pintaidasiento(QString) {
        _depura("funcion no implementada pintaidasiento");
    };
    virtual void pintadescripcion(QString) {
        _depura("funcion no implementada pintadescripcion");
    };
    virtual void pintafecha(QString) {
        _depura("funcion no implementada pintafecha");
    };
    virtual void pintacomentariosasiento(QString) {
        _depura("funcion no implementada pintacomentariosasiento");
    };
    virtual void pintaordenasiento(QString) {
        _depura("funcion no implementada pintaordenasiento");
    };
    virtual void pintaclase(QString) {
        _depura("funcion no implementada pintaclase");
    };


    virtual void calculaypintatotales() {
        _depura("funcion no implementada calculaypintatotales");
    };
    virtual void pintaAsiento1();
    virtual void trataestadoAsiento1() {
        _depura("funcion no implementada trataestadoAsiento1");
    };
    virtual void asiento_cierre() {
        _depura("Funcion no implementada");
    };
    virtual void asiento_apertura() {
        _depura("Funcion no implementada");
    };
    virtual void asiento_regularizacion() {
        _depura("Funcion no implementada");
    };

};

#endif
