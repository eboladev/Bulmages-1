/***************************************************************************
 *   Copyright (C) 2004 by Josep Burcion                                   *
 *   josep@burcion.com                                                     *
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
 ***************************************************************************/

#include "funcaux.h"


/************************************************************************************/
/* Esta función convierte un numero con decimales a un entero. Usando la regla      */
/* que si el el primer decimal es mayor o igual  a 5 se devuelve el entero superior.*/
/***********************************************************************************/
int roundI(double valor) {
    int retorno;
    double mayor=floor(valor);
    if ((mayor-valor) >= 0.5)  retorno=(int)mayor-1;
    else retorno= (int)mayor;
    return retorno;
}

