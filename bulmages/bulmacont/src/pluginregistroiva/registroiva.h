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
#ifndef REGISTROIVA_H
#define REGISTROIVA_H

#include <QString>
#include <Q3Table>
#include <Q3PtrList>

#include "fixed.h"

#include "listiva.h"
#include "listlinprevcobro.h"

//#include "listdescpresupuesto.h"

/** @author Tomeu Borras & Alvaro de Miguel
  * \brief Clase que hace de intermediaria entre la tabla de presupuesto de la base de datos y el programa.
*/
#include "empresa.h"
#include "dbrecord.h"

/*
CREATE TABLE registroiva (
    idregistroiva serial PRIMARY KEY,
    contrapartida integer REFERENCES cuenta(idcuenta),
    baseimp numeric(12,2),
    iva numeric(12,2),		-- De solo lectura
    ffactura date,
    factura character varying(70),
    idborrador integer NOT NULL REFERENCES borrador(idborrador),
    incregistro boolean,
    regularizacion boolean,
    plan349 boolean,
    numorden character varying(50),
    cif character varying(25),
    idfpago integer REFERENCES fpago(idfpago),
    factemitida boolean NOT NULL,
    rectificaaregistroiva integer REFERENCES registroiva(idregistroiva)
);
*/




class RegistroIva : public DBRecord {
protected:
    ListIva *m_listalineas;
    ListLinPrevCobro *m_listadescuentos;
    empresa *m_companyact;
public:
    /// Esta blece cual es la lista subformulario del presupuesto. Normalmente para apuntar listlinpresupuestoview.

    void setListIva ( ListIva *a) {
        m_listalineas =a;
        m_listalineas->setcompany(m_companyact);
    };

    void setListLinPrevCobro ( ListLinPrevCobro *a) {
        m_listadescuentos =a;
        m_listadescuentos->setcompany(m_companyact);
    };

    RegistroIva(empresa *);

    ListIva* listalineas() {
        return m_listalineas;
    };

    ListLinPrevCobro* mlistadescuentos() {
        return m_listadescuentos;
    };


    empresa * _company() {
        return m_companyact;
    };

    QString idregistroiva() {
        return DBvalue("idregistroiva");
    };
    QString contrapartida() {
        return DBvalue("contrapartida");
    };
    QString baseimp() {
        return DBvalue("baseimp");
    };
    QString iva() {
        return DBvalue("iva");
    };
    QString ffactura() {
        return DBvalue("ffactura");
    };
    QString femisionregistroiva() {
        return DBvalue("femisionregistroiva");
    };
    QString factura() {
        return DBvalue("factura");
    };
    QString idborrador() {
        return DBvalue("idborrador");
    };
    QString incregistro() {
        return DBvalue("incregistro");
    };
    QString regularizacion() {
        return DBvalue("regularizacion");
    };
    QString plan349() {
        return DBvalue("plan349");
    };
    QString numorden() {
        return DBvalue("numorden");
    };
    QString cif() {
        return DBvalue("cif");
    };
    QString idfpago() {
        return DBvalue("idfpago");
    };
    QString factemitida() {
        return DBvalue("factemitida");
    };
    QString rectificaaregistroiva() {
        return DBvalue("rectificaaregistroiva");
    };
    QString serieregistroiva() {
        return DBvalue("serieregistroiva");
    };

    void setidregistroiva(const QString &val) {
        setDBvalue("idregistroiva",val);
    };
    void setcontrapartida(const QString &val) {
        setDBvalue("contrapartida",val);
    };
    void setbaseimp(const QString &val) {
        setDBvalue("baseimp",val);
    };
    void setiva(const QString &val) {
        setDBvalue("iva",val);
    };
    void setffactura(const QString &val) {
        setDBvalue("ffactura",val);
    };
    void setfactura(const QString &val) {
        setDBvalue("factura",val);
    };
    void setidborrador(const QString &val) {
        setDBvalue("idborrador",val);
    };
    void setincregistro(const QString &val) {
        setDBvalue("incregistro",val);
    };
    void setregularizacion(const QString &val) {
        setDBvalue("regularizacion",val);
    };
    void setplan349(const QString &val) {
        setDBvalue("plan349",val);
    };
    void setnumorden(const QString &val) {
        setDBvalue("numorden",val);
    };
    void setcif(const QString &val) {
        setDBvalue("cif",val);
    };
    void setidfpago(const QString &val) {
        setDBvalue("idfpago",val);
    };
    void setfactemitida(const QString &val) {
        setDBvalue("factemitida",val);
    };
    void setrectificaaregistroiva(const QString &val) {
        setDBvalue("rectificaaregistroiva",val);
    };
    void setfemisionregistroiva(const QString &val) {
        setDBvalue("femisionregistroiva",val);
    };
    void setserieregistroiva(const QString &val) {
        setDBvalue("serieregistroiva",val);
    };


    virtual void pintaidregistroiva(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintacontrapartida(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintabaseimp(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaiva(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaffactura(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintafactura(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaidborrador(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaincregistro(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaregularizacion(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaplan349(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintanumorden(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintacif(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaidfpago(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintafactemitida(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintarectificaaregistroiva(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintafemisionregistroiva(const QString &) {
        _depura("funcion no implementada",2);
    };
    virtual void pintaserieregistroiva(const QString &) {
        _depura("funcion no implementada",2);
    };



    virtual ~RegistroIva();
    virtual int cargaRegistroIva(QString );
    virtual void pintaRegistroIva();
    virtual int guardaRegistroIva();
    virtual void borraRegistroIva();
    virtual void calculaypintatotales() {};
    void vaciaRegistroIva();
    int buscaborradorservicio(int);
    int buscaborradorcliente(int);
    int buscaborradoriva(int);
    void inicializa1(int);
    void buscafecha(int);
    void buscaNumFactura(int);
};
#endif
