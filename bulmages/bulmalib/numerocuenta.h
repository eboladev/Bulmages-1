/***************************************************************************
 *   Copyright (C) 2004 by David Gutierrez Rubio                           *
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

#ifndef NUMEROCUENTA_H
#define NUMEROCUENTA_H

#include <QString>

/// Esta clase implementa el Codigo Cuenta Cliente (CCC) de un banco.
/// Su funcion mas importante es cuentaesvalida.
class numerocuenta {
public:
    numerocuenta(QString, QString, QString, QString); ///Banco, entidad, dc y numcuenta.
    numerocuenta(QString, QString, QString); ///Banco, entidad y numcuenta. El dc lo calcula automaticamente.
    numerocuenta(QString); ///Banco, entidad, dc y numcuenta todo seguido, con guiones opcionales.
    ~numerocuenta();
    QString getdc(); ///Calcula y devuelve los digitos de control del ccc
    /// Comprueba si es un numero de cuenta valido.
    /// Devuelve: true Si es un codigo CCC correcto. false En caso contrario.
    bool cuentaesvalida() {
        return((getdc() == dc));
    };
    /// Devuelve el CCC separando sus miembros por la cadena \c sep.
    QString getcodigo(QString sep = "") {
        return(banco + sep + entidad + sep + dc + sep + numcuenta);
    };
    /// Devuelve el numero de cuenta.
    QString getnumcuenta() {
        return(numcuenta);
    };
    /// Devuelve el numero de banco.
    QString getbanco() {
        return(banco);
    };
    /// Devuelve el nmero de entidad.
    QString getentidad() {
        return(entidad);
    };

    /// Fija el numero de cuenta.
    /// Si el argumento tiene menos de 10 digitos, se rellena con ceros a la izquierda.
    void setnumcuenta(QString cad) {
        numcuenta = cad.rightJustify(10, '0');
    };

    /// Fija el numero de banco.
    /// Si el argumento tiene menos de 4 digitos, se rellena con ceros a la izquierda.
    void setbanco(QString cad) {
        banco = cad.rightJustify(4, '0');
    };

    /// Fija el numero de entidad.
    /// Si el argumento tiene menos de 4 digitos, se rellena con ceros a la izquierda.
    void setentidad(QString cad) {
        entidad = cad.rightJustify(4, '0');
    };

private:
    QString banco;
    QString entidad;
    QString dc;
    QString numcuenta;

};

#endif

