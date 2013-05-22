/***************************************************************************
 *   Copyright (C) 2005 by Ricardo Diaz                                    *
 *   richard@galdi.es                                                      *
 *   http://www.iglues.org                                                 *
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

#ifndef BCPLANCONTABLEARBOL_H
#define BCPLANCONTABLEARBOL_H

#include <QString>

#include "blpostgresqlclient.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "pdefs_pluginbf_cuenta.h"


/// Clase que crea una estructura en forma de arbol para almacenar el plan contable.
/** Fichero de definicion de las clase arbol. Crea una estructura tipo arbol para
    almacenar en memoria dinamica todas las cuentas del plan contable de
    la base de datos y calcula sus saldos cuando se genera.
    La potencia de esta estructura radica en la actualizacion automatica de
    todos los estados de cada cuenta (las hojas) cada vez que se modifican los valores
    de alguna de ellas.
*/
class PLUGINBF_CUENTA_EXPORT BcPlanContableArbol
{
private:
    struct tipohoja;
    struct tiporama {
        tipohoja* hoja; /// Puntero a la hoja donde se almacena los datos de la cuenta.
        tiporama* sgte; /// Puntero a una rama que apunta a una hoja hermana (del mismo nivel).
    };
    struct tipohoja {
        int idcuenta; /// Identificador de la cuenta de la base de datos.
        QString codigo; /// C&oacute;digo de la cuenta.
        QString descripcion; /// Su descripci&oacute;n contable.
        /// Los valores a calcular.
        BlFixed saldoant, debe, haber, saldo, debeej, haberej, saldoej;
        int numapuntes; /// N&uacute;mero de apuntes que modifican los valores a calcular.
        tiporama* ramas; /// Puntero a m&aacute;s ramas con hojas.
    };
    //tipohoja** raiz; /// Raiz del arbol que contiene todas las cuentas de nivel 2.
    QList <tipohoja*> raiz; /// Raiz del arbol que contiene todas las cuentas de nivel 2.
    tipohoja* hoja; /// Hoja que contiene los datos de una cuenta.
    tiporama* rama; /// Rama que contiene una hoja y m&aacute;s ramas o no.
    BlPostgreSqlClient* dbConnection; /// Nos da acceso a la base de datos con la que estamos trabajando.
    /*QString hojaactiva;*/ /// Código de la última hoja visitada (nos servirá de índice para recorred el árbol).
    tipohoja* hojaactiva;

private:
    void SintetizarRamas ( BlDbRecordSet**, tiporama** );
    void ActualizarHoja ( tiporama**, BlDbRecordSet*, bool* );
    void Deshojar ( tiporama*, unsigned int, bool, bool* );

public:
    BcPlanContableArbol();
    ~BcPlanContableArbol();

    /// A&ntilde;ade una nueva rama al arbol con su hoja.
    void nuevaRama ( BlDbRecordSet* );

    /// Constituye el arbol inicializando los valores que corresponden a cada cuenta (hoja).
    void inicializa ( BlDbRecordSet* );

    /// Actualiza los valores de las hojas en el arbol.
    void actualizaHojas ( BlDbRecordSet* );

    /// Inicializa el acceso al arbol. Para ello, cambia el valor de la variable índice "hojaactiva" a la raíz.
    void inicia();

    /// Mueve el puntero que indexa una hoja del arbol: primer parametro indica el nivel en el que queremos deshojar; el segundo si deshoja o no otros niveles
    bool deshoja ( unsigned int, bool );

    /// Devuelve el valor solicitado: codigo, saldoant, debe, haber, saldo, debeej, haberej, saldoej, etc.
    QString hojaActual ( QString );

    /// Se posiciona el puntero en la hoja cuyo codigo corresponde al pasado por parametro e informa del exito o no
    /// El segundo parametro indica a que nivel de profundidad buscar las cuentas; por defecto a 4, el minimo exigido en un balance abreviado
    bool irHoja ( QString, unsigned int nivel = 4 );

    /// Devuelvo el codigo de la cuenta mayor que hay en el BcPlanContableArbol. A no ser que se indique otro nivel,
    /// se obtiene el máximo de nivel 4.
    QString codigoCuentaMayor ( unsigned int nivel = 4 );

    /// Devuelvo el codigo cuenta del hijo que hay en el BcPlanContableArbol para esa cuenta. El primer parametro indica el codigo de
    /// cuenta que representa la hoja padre. El segundo parametro indica que se busque hasta ese nivel del BcPlanContableArbol.
    QString hijoMayor ( QString, unsigned int nivel = 4 );
};

#endif

