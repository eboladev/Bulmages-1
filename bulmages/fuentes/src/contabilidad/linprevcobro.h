//
// C++ Interface: linprevcobro
//
// Description:
//
//
// Author: Tomeu Borras <tborras@conetxia.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LINPREVCOBRO_H
#define LINPREVCOBRO_H

/**
@author Tomeu Borras
*/
#include "empresa.h"
#include "postgresiface2.h"

class linprevcobro {
private:
    QString mdb_idprevcobro;
    QString mdb_fprevistaprevcobro;
    QString mdb_fcobroprevcobro;
    QString mdb_idfpago;
    QString mdb_idcuenta;
    QString mdb_idasiento;
    QString mdb_cantidadprevistaprevcobro;
    QString mdb_cantidadprevcobro;
    QString mdb_idregistroiva;
    QString mdb_tipoprevcobro;
    QString mdb_docprevcobro;
    QString mdb_idctacliente;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_codigocuenta;
    QString mdb_codigoctacliente;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido pq se pinta en muchas partes.
    QString mdb_nomcuenta;
    QString mdb_nomctacliente;
    empresa *empresaactual;
    postgresiface2 *conexionbase;
public:

    linprevcobro(empresa *);
    linprevcobro(empresa *, QString );
    /// La carga r�ida tiene un comportamiento poco restrictivo para aumnetar la eficiencia.
    linprevcobro(empresa *, QString , QString , QString , QString , QString , QString , QString, QString, QString, QString, QString, QString, QString, QString, QString , QString );
    virtual ~linprevcobro();
    virtual void pintalinprevcobro() {};
    void guardalinprevcobro();
    void vacialinprevcobro();
    int creaPago();


    inline QString idprevcobro() {
        return mdb_idprevcobro;
    };
    inline QString fprevistaprevcobro() {
        return mdb_fprevistaprevcobro;
    };
    inline QString fcobroprevcobro() {
        return mdb_fcobroprevcobro;
    };
    inline QString idfpago()  {
        return mdb_idfpago;
    };
    inline QString idcuenta() {
        return mdb_idcuenta;
    };
    inline QString idctacliente() {
	return mdb_idctacliente;
    };
    inline QString idasiento() {
        return mdb_idasiento;
    };
    inline QString cantidadprevistaprevcobro() {
        return mdb_cantidadprevistaprevcobro;
    };
    inline QString cantidadprevcobro() {
        return mdb_cantidadprevcobro;
    };
    inline QString idregistroiva() {
        return mdb_idregistroiva;
    };
    inline QString tipoprevcobro() {
        return mdb_tipoprevcobro;
    };
    inline QString docprevcobro() {
        return mdb_docprevcobro;
    };
    inline QString codigocuenta() {
        return mdb_codigocuenta;
    };

    inline QString codigoctacliente() {
        return mdb_codigoctacliente;
    };

    inline QString nomcuenta() {
        return mdb_nomcuenta;
    };
    inline QString nomctacliente() {
        return mdb_nomctacliente;
    };

    inline void setidprevcobro(QString val) {
        mdb_idprevcobro=val;
    };
    inline void setfprevistaprevcobro(QString val) {
        mdb_fprevistaprevcobro=val;
    };
    inline void setfcobroprevcobro(QString val) {
        mdb_fcobroprevcobro=val;
    };
    inline void setidfpago(QString val)  {
        mdb_idfpago=val;
    };

    inline void setidasiento(QString val) {
        mdb_idasiento=val;
    };
    inline void setcantidadprevistaprevcobro(QString val) {
        mdb_cantidadprevistaprevcobro=val;
    };
    inline void setcantidadprevcobro(QString val) {
        mdb_cantidadprevcobro=val;
    };
    inline void setidregistroiva(QString val) {
        mdb_idregistroiva=val;
    };
    inline void settipoprevcobro(QString val) {
        mdb_tipoprevcobro=val;
    };
    inline void setdocprevcobro(QString val) {
        mdb_docprevcobro=val;
    };

    void setcodigocuenta(QString);
    void setidcuenta(QString);
    inline void setnomcuenta(QString val) {
        mdb_nomcuenta=val;
    };

    void setcodigoctacliente(QString);
    void setidctacliente(QString);
    inline void setnomctacliente(QString val) {
        mdb_nomctacliente=val;
    };

    void borrar();
};

#endif
