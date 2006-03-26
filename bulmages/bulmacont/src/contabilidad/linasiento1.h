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
#ifndef LINASIENTO1_H
#define LINASIENTO1_H

/**
@author Tomeu Borras
*/
#include "empresa.h"
#include "dbrecord.h"
#include "fixed.h"

class LinAsiento1 : public DBRecord {
private:

    empresa *companyact;
    void definetabla();
public:
    LinAsiento1(empresa *);
    LinAsiento1(empresa *, QString );
/*
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.

    linpresupuesto(company *, QString , QString , QString , QString , QString , QString , QString, QString, QString, QString );
*/

    LinAsiento1(empresa *, cursor2 *);
    virtual ~LinAsiento1();
    virtual void pintaLinAsiento1() {}
    ;
    int guardaLinAsiento1();
    void vaciaLinAsiento1();
	Fixed calculadebe();
	Fixed calculahaber();


    inline QString idborrador() {
        return DBvalue("idborrador");
    };
    inline QString codigoborrador() {
        return DBvalue("codigoborrador");
    };
    inline QString idasiento() {
        return DBvalue("idasiento");
    };
    inline QString iddiario()  {
        return DBvalue("iddiario");
    };
    inline QString fecha() {
        return DBvalue("fecha").left(10);
    };
    inline QString conceptocontable() {
        return DBvalue("conceptocontable");
    };
    inline QString idcuenta() {
        return DBvalue("idcuenta");
    };
    inline QString descripcion() {
        return DBvalue("descripcion");
    };
    inline QString debe() {
        return DBvalue("debe");
    };
    inline QString haber() {
        return DBvalue("haber");
    };
    inline QString contrapartida() {
        return DBvalue("contrapartida");
    };
    inline QString comentario() {
        return DBvalue("comentario");
    };
    inline QString idcanal() {
        return DBvalue("idcanal");
    };
    inline QString marcaconciliacion() {
        return DBvalue("marcaconciliacion");
    };
    inline QString idc_coste() {
        return DBvalue("idc_coste");
    };
    inline QString idapunte() {
        return DBvalue("idapunte");
    };
    inline QString idtipoiva() {
        return DBvalue("idtipoiva");
    };
    inline QString orden() {
        return DBvalue("orden");
    };
    inline QString codigo() {
        return DBvalue("codigo");
    };
    inline QString descripcioncuenta() {
        return DBvalue("descripcioncuenta");
    };
    inline QString nombrecanal() { return DBvalue("nombrecanal");};
    inline QString descripcioncanal() { return DBvalue("descripcioncanal");};
    inline QString nombrec_coste() { return DBvalue("nombrec_coste");};
    inline QString descripcionc_coste() { return DBvalue("descripcionc_coste");};

    inline QString idregistroiva() { return DBvalue("idregistroiva");};
    inline QString factura() { return DBvalue("factura");};
    inline QString ffactura() { return DBvalue("ffactura");};



    inline void setidborrador(QString val) {
        setDBvalue("idborrador",val);
    };
    inline void setcodigoborrador(QString val) {
        setDBvalue("codigoborrador",val);
    };
    inline void setidasiento(QString val) {
        setDBvalue("idasiento",val);
    };
    inline void setiddiario(QString val)  {
        setDBvalue("iddiario",val);
    };
    inline void setfecha(QString val) {
        setDBvalue("fecha",val);
    };
    inline void setconceptocontable(QString val) {
        setDBvalue("conceptocontable",val);
    };
    inline void setdescripcion(QString val) {
        setDBvalue("descripcion",val);
    };
    inline void setdebe(QString val) {
        setDBvalue("debe",val);
    };
    inline void sethaber(QString val) {
        setDBvalue("haber",val);
    };
    inline void setcontrapartida(QString val) {
        setDBvalue("contrapartida",val);
    };
    inline void setcomentario(QString val) {
        setDBvalue("comentario",val);
    };
    inline void setmarcaconciliacion(QString val) {
        setDBvalue("marcaconciliacion",val);
    };
    inline void setidapunte(QString val) {
        setDBvalue("idapunte",val);
    };
    inline void setidtipoiva(QString val) {
        setDBvalue("idtipoiva",val);
    };
    inline void setorden(QString val) {
        setDBvalue("orden",val);
    };
    void setcodigo(QString val);
    void setnombrecanal(QString);
    void setnombrec_coste(QString );
    void setidcuenta(QString );
    void setidcanal(QString );
    void setidc_coste(QString );


    int borrar();

};

#endif
