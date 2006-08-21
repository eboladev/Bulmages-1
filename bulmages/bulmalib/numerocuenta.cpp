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

#include <QString>

#include "numerocuenta.h"

/// Constructor con los 4 parametros
/// \param bancopar Codigo banco (4 digitos)
/// \param entidadpar Codigo entidad (4 digitos)
/// \param dcpar Digitos de Control (2 digitos)
/// \param numcuentapar Codigo de cuenta (10 digitos)
numerocuenta::numerocuenta(QString bancopar, QString entidadpar, QString dcpar, QString numcuentapar) {
    setbanco(bancopar);
    setentidad(entidadpar);
    setnumcuenta(numcuentapar);
    dc = dcpar;
}


/// Overload. En este caso los digitos de control se calculan automaticamente.
numerocuenta::numerocuenta(QString bancopar, QString entidadpar, QString numcuentapar) {
    setbanco(bancopar);
    setentidad(entidadpar);
    setnumcuenta(numcuentapar);
    dc = getdc();
}


/// Overload. Si la cuenta se presenta como una unica cadena, extrae los 4 elementos de ella.
numerocuenta::numerocuenta(QString cuenpar) {
    QString tem = cuenpar;
    tem.remove("-"); /// TODO: Que elimine tambien otro tipo de caracteres como espacios, etc.
    banco = tem.mid(0, 4);
    entidad = tem.mid(4, 4);
    dc = tem.mid(8, 2);
    numcuenta = tem.mid(10, 10);
}


numerocuenta::~numerocuenta() {}


/// Devuelve los digitos de control del numero de cuenta.
QString numerocuenta::getdc() {
    QString resul;
    int i;
    int sum = 0;

    /// Unidades =  6
    /// Decenas  =  3
    /// Centenas =  7
    /// Millares =  9
    /// DiezMil. = 10
    /// CienMil. =  5
    /// Millones =  8
    /// DiezMill.=  4
    /// CienMill.=  2
    /// MilMill. =  1

    const int we[] = {
                         1, 2, 4, 8, 5, 10, 9, 7, 3, 6
                     };

    for (i = 0; i < 4; i++)
        sum += QString(banco[i]).toInt() * we[i + 2];

    for (i = 0; i < 4; i++)
        sum += QString(entidad[i]).toInt() * we[i + 6];

    sum = (sum % 11);
    sum = 11 - sum;

    if (sum == 11)
        sum = 0;
    if (sum == 10)
        sum = 1;

    int sum2 = 0;
    for (i = 0; i < 10; i++)
        sum2 += QString(numcuenta[i]).toInt() * we[i];

    sum2 = (sum2 % 11);
    sum2 = 11 - sum2;

    if (sum2 == 11)
        sum2 = 0;
    if (sum2 == 10)
        sum2 = 1;

    resul.sprintf("%d%d", sum, sum2);
    return resul;
}

