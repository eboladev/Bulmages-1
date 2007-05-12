/***************************************************************************
 *   Copyright (C) 2005 by Ricardo Diaz                                    *
 *   richard@galdi.es                                                      *
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

#ifndef ARBOL_H
#define ARBOL_H

#include <QString>

#include "postgresiface2.h"
#include "funcaux.h"
#include "fixed.h"


/// Clase que crea una estructura en forma de arbol para almacenar el plan contable.
/** Fichero de definicion de las clase arbol. Crea una estructura tipo arbol para
    almacenar en memoria dinamica todas las cuentas del plan contable de
    la base de datos y calcula sus saldos cuando se genera.
    La potencia de esta estructura radica en la actualizacion automatica de
    todos los estados de cada cuenta (las hojas) cada vez que se modifican los valores
    de alguna de ellas.
*/
class Arbol {
private:
    typedef struct tipohoja;
    typedef struct tiporama {
        tipohoja* hoja; /// Puntero a la hoja donde se almacena los datos de la cuenta.
        tiporama* sgte; /// Puntero a una rama que apunta a una hoja hermana (del mismo nivel).
    };
    typedef struct tipohoja {
        int idcuenta; /// Identificador de la cuenta de la base de datos.
        QString codigo; /// C&oacute;digo de la cuenta.
        QString descripcion; /// Su descripci&oacute;n contable.
        /// Los valores a calcular.
        Fixed saldoant, debe, haber, saldo, debeej, haberej, saldoej;
        int numapuntes; /// N&uacute;mero de apuntes que modifican los valores a calcular.
        tiporama* ramas; /// Puntero a m&aacute;s ramas con hojas.
    };
    tipohoja** raiz; /// Raiz del arbol que contiene todas las cuentas de nivel 2.
    tipohoja* hoja; /// Hoja que contiene los datos de una cuenta.
    tiporama* rama; /// Rama que contiene una hoja y m&aacute;s ramas o no.
    postgresiface2 *conexionbase; /// Nos da acceso a la base de datos con la que estamos trabajando.
    QString hojaactiva; /// C&oacute;digo de la &uacute;ltima hoja visitada (nos servir&aacute; de &iacute;ndice).

private:
    void SintetizarRamas(cursor2**, tiporama**);
    void ActualizarHoja(tiporama**, cursor2*, bool*);
    void Deshojar(tiporama*, unsigned int, bool, bool*);

public:
    Arbol();
    ~Arbol();
    /// A&ntilde;ade una nueva rama al arbol con su hoja.
    void nuevarama(cursor2*);
    /// Constituye el arbol inicializando los valores que corresponden a cada cuenta (hoja).
    void inicializa(cursor2*);
    /// Actualiza los valores de las hojas en el arbol.
    void actualizahojas(cursor2*);
    /// Inicializa el acceso al arbol cambiando el valor de la variable "visitada".
    void inicia();
    /// Mueve el puntero que indexa una hoja del arbol: primer parametro indica el nivel en el que queremos deshojar; el segundo si deshoja o no otros niveles
    bool deshoja(unsigned int, bool);
    /// Devuelve el valor solicitado: codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej, etc.
    QString hojaactual(QString);
    /// Se posiciona el puntero en la hoja cuyo codigo corresponde al pasado por parametro e informa del exito o no
    /// El segundo parametro indica a que nivel de profundidad buscar las cuentas; por defecto a 4, el m�imo exigido en un balance abreviado
    bool irHoja(QString, unsigned int nivel=4);
};

#endif

