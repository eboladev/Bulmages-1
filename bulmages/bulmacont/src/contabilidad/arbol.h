//
// C++ Interface: arbol
//
// Description: 
//
//
// Author: Ricardo DÃ­az <richard@galdi.es>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef ARBOL_H
#define ARBOL_H

#include <qstring.h>
#include "postgresiface2.h"

/**
 @author Ricardo Di­az
 * Fichero de definicion de las clase \ref arbol que crea una estructura tipo arbol para el almacenaje
 * en memoria dinamica de todas cuentas del plan contable de la base de datos que calcula sus saldos cuando se genera.
 * La potencia de esta estructura radica en la actualizacion automática de todas los estados de cada cuenta (las hojas)
 * cada vez que se modifica los valores de alguna de ellas.
*/

class Arbol{
private:
    typedef struct tipohoja;
    typedef struct tiporama {
	tipohoja* hoja;	// puntero a la hoja donde se almacena los datos de la cuenta
	tiporama* sgte; // puntero a una rama que apunta a una hoja hermana (del mismo nivel)
    };
    typedef struct tipohoja {
	int idcuenta;	// idcuenta de la BD
	QString codigo;	// codigo de la cuenta
	QString descripcion; // su descripcion contable
	double saldoant, debe, haber, saldo, debeej, haberej, saldoej; // los valores a calcular
	int numapuntes; // num. de apuntes que modifican los valores a calcular
	tiporama* ramas; // puntero a mas ramas con hojas
    };
    tipohoja** raiz;	// Raiz del arbol que contiene todas las cuentas de nivel 2
    tipohoja* hoja;	// Hoja que contiene los datos de una cuenta
    tiporama* rama;	// Rama que contiene una hoja y mas ramas o no
    postgresiface2 *conexionbase;  // Para el acceso a la BD con la que estamos trabajando
    QString hojaactiva; // Codigo de la ultima hoja visitada (nos servira de indice)
    
private:
    void SintetizarRamas(cursor2**, tiporama**);
    void ActualizarHoja(tiporama**, cursor2*, bool*);
    void Deshojar(tiporama*, unsigned int, bool, bool*);
    //void ImprimirRama(tiporama*);
    
public:    
    Arbol();
    ~Arbol();
    /// \brief Añade una nueva rama al arbol con su hoja
    void nuevarama(cursor2*);
    /// \brief Constituye el arbol inicializando los valores que corresponden a cada cuenta (hoja)
    void inicializa(cursor2*);
    /// \brief Actualiza los valores de las hojas en el arbol
    void actualizahojas(cursor2*);
    /// \brief Inicializa el acceso al arbol cambiando el valor de la variable "visitada"
    void inicia();
    /// \brief Mueve el puntero que indexa una hoja del arbol, segun condicionen los parametros, para que se acceda a la informacion que contiene
    bool deshoja(unsigned int, bool);
    /// \brief Devuelve el contenido de valor solicitado como parametro
    QString hojaactual(QString);
    /// Se recorre el arbol e imprime sus valores (util para mostrar por salida estandar los valores de las hojas)
    //void imprimir();
};

#endif
