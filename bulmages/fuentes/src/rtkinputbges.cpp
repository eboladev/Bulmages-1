/*<<<<<COPYRIGHT*/
/* GestiONG Gestión para ONGs y Asociaciones sin ánimo de lucro
 * Copyright (C) 2003 Francisco Santiago Capel Torres
 * 
 * rtkinputsql.cpp RTK standard sql input object
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
/*>>>>>COPYRIGHT*/
#ifdef REPORTS

//#include <stdio.h>

#include <qdatetime.h>
#include "rtkinputbges.h"

using namespace RTK;

bool InputBGes::init() {
   consulta->primerregistro();         
   inicializado = 1;
   return true;
}

InputBGes::~InputBGes()
{
}


bool InputBGes::next() {
        if (!inicializado) {
           // Ya se ha hecho un next, por tanto hacemos lo que toca.
           consulta->siguienteregistro();
 //          fprintf(stderr," InputBGes::Next : %s\n",consulta->valor("idasiento").ascii());
        } else {
           // El primer next que se hace debe saltarse.
           inicializado=0;
           return (consulta->eof());
        }
   return(!consulta->eof());
}


Variant InputBGes::getValue(uint i) const {
   fprintf(stderr,"InputBGes::getValue()\n");
   fprintf(stderr,"%s\n", consulta->valor((int) i).ascii());
        if( i<(uint)consulta->numcampos() )
                return Variant(consulta->valor((int) i));
        else
                return Variant("");
}


Variant InputBGes::getValue(char *s) const {
   fprintf(stderr,"InputBGes::getValue()\n");
   fprintf(stderr,"$s -- > %s\n",s , consulta->valor(s).ascii());
   return Variant(consulta->valor(s));
}// end getValue


#endif

