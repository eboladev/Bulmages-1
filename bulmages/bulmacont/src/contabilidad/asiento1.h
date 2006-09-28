/***************************************************************************
 *   Copyright (C) 2006 by Tomeu Borras Riera                              *
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

#ifndef ASIENTO1_H
#define ASIENTO1_H

#include <Q3Table>
#include <Q3PtrList>

#include <QString>

#include "fixed.h"
#include "dbrecord.h"
#include "listlinasiento1view.h"

class empresa;


/// Clase Asiento1.
/** */
class Asiento1 : public DBRecord {
public:
    enum estadoasiento {ASVacio = 0, ASAbierto = 1, ASCerrado = 2};

protected:
    ListLinAsiento1View *listalineas;
    empresa *m_companyact;

public:
    empresa *companyact() {
        return m_companyact;
    };
    Asiento1(empresa *);
    virtual ~Asiento1();
    int cargar(QString);
    Fixed totaldebe(QString);
    Fixed totalhaber(QString);
    void vaciaAsiento1();
    void abreAsiento1();
    void cierraAsiento1();
    estadoasiento estadoAsiento1();
    int guardar();
    void setidasiento(QString val) {
        setDBvalue("idasiento", val);
        listalineas->setColumnValue("idasiento", val);
    };
    QString idasiento() {
        return DBvalue("idasiento");
    };
    /// Establece cual es la lista subformulario del presupuesto.
    void setListLinAsiento1(ListLinAsiento1View *a) {
        listalineas = a;
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
    virtual void calculaypintatotales(QString) {
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

