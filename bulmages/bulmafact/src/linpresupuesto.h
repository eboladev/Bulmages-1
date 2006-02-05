//
// C++ Interface: linpresupuesto
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINPRESUPUESTO_H
#define LINPRESUPUESTO_H

/**
@author Tomeu Borras
*/
#include "company.h"
#include "dbrecord.h"

class linpresupuesto : public DBRecord {
private:

    company *companyact;

    void definetabla();
public:
    linpresupuesto(company *);
    linpresupuesto(company *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.

    linpresupuesto(company *, QString , QString , QString , QString , QString , QString , QString, QString, QString, QString );

    linpresupuesto(company *, cursor2 *);
    virtual ~linpresupuesto();
    virtual void pintalinpresupuesto() {}
    ;
    void guardalinpresupuesto();
    void vacialinpresupuesto();


    inline QString idlpresupuesto() {
        return DBvalue("idlpresupuesto");
    };
    inline QString desclpresupuesto() {
        return DBvalue("desclpresupuesto");
    };
    inline QString cantlpresupuesto() {
        return DBvalue("cantlpresupuesto");
    };
    inline QString pvplpresupuesto()  {
        return DBvalue("pvplpresupuesto");
    };
    inline QString descuentolpresupuesto() {
        return DBvalue("descuentolpresupuesto");
    };
    inline QString idpresupuesto() {
        return DBvalue("idpresupuesto");
    };
    inline QString idarticulo() {
        return DBvalue("idarticulo");
    };
    inline QString codigocompletoarticulo() {
        return DBvalue("codigocompletoarticulo");
    };
    inline QString nomarticulo() {
        return DBvalue("nomarticulo");
    };
    inline QString ivalpresupuesto() {
        return DBvalue("ivalpresupuesto");
    };


    inline void setidlpresupuesto(QString val) {
        setDBvalue("idlpresupuesto",val);
    };
    inline void setdesclpresupuesto(QString val) {
        setDBvalue("desclpresupuesto",val);
    };
    inline void setcantlpresupuesto(QString val) {
        setDBvalue("cantlpresupuesto",val);
    };
    inline void setpvplpresupuesto(QString val)  {
        setDBvalue("pvplpresupuesto",val);
    };
    inline void setdescuentolpresupuesto(QString val) {
        setDBvalue("descuentolpresupuesto",val);
    };
    inline void setidpresupuesto(QString val) {
        setDBvalue("idpresupuesto",val);
    };
    inline void setivalpresupuesto(QString val) {
        setDBvalue("ivalpresupuesto",val);
    };
    void setcodigocompletoarticulo(QString);


    /// EStas funciones no estan como debe, el uso de cada una de ellas debe hacer cambios sobre la base de datos.
    void setidarticulo(QString);
    inline void setnomarticulo(QString val) {
        setDBvalue("nomarticulo",val);
    };
    /// Hace el calculo de la base imponible de la l�ea
    float calculabase();
    /// Hace el calculo del IVA de la l�ea.
    float calculaiva();
    void borrar();

};

#endif
