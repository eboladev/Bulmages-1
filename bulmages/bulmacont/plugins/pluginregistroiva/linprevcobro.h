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

#ifndef LINPREVCOBRO_H
#define LINPREVCOBRO_H

#include "bccompany.h"
#include "blpostgresqlclient.h"


class linprevcobro
{
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
    /// Aunque no es un elemento de la tabla, si es importante que este incluido porque se
    /// pinta en muchas partes.
    QString mdb_codigocuenta;
    QString mdb_codigoctacliente;
    /// Aunque no es un elemento de la tabla, si es importante que este incluido porque se
    /// pinta en muchas partes.
    QString mdb_nomcuenta;
    QString mdb_nomctacliente;
    BcCompany *empresaactual;
    BlPostgreSqlClient *conexionbase;

public:
    linprevcobro ( BcCompany * );
    linprevcobro ( BcCompany *, QString );
    /// La carga r&aacute;pida tiene un comportamiento poco restrictivo para aumentar la
    /// eficiencia.
    linprevcobro ( BcCompany *, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString, QString );
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
    inline QString idfpago() {
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
    inline void setidprevcobro ( QString val ) {
        mdb_idprevcobro = val;
    };
    inline void setfprevistaprevcobro ( QString val ) {
        mdb_fprevistaprevcobro = val;
    };
    inline void setfcobroprevcobro ( QString val ) {
        mdb_fcobroprevcobro = val;
    };
    inline void setidfpago ( QString val ) {
        mdb_idfpago = val;
    };
    inline void setidasiento ( QString val ) {
        mdb_idasiento = val;
    };
    inline void setcantidadprevistaprevcobro ( QString val ) {
        mdb_cantidadprevistaprevcobro = val;
    };
    inline void setcantidadprevcobro ( QString val ) {
        mdb_cantidadprevcobro = val;
    };
    inline void setidregistroiva ( QString val ) {
        mdb_idregistroiva = val;
    };
    inline void settipoprevcobro ( QString val ) {
        mdb_tipoprevcobro = val;
    };
    inline void setdocprevcobro ( QString val ) {
        mdb_docprevcobro = val;
    };
    void setcodigocuenta ( QString );
    void setidcuenta ( QString );
    inline void setnomcuenta ( QString val ) {
        mdb_nomcuenta = val;
    };
    void setcodigoctacliente ( QString );
    void setidctacliente ( QString );
    inline void setnomctacliente ( QString val ) {
        mdb_nomctacliente = val;
    };
    void borrar();
};

#endif
