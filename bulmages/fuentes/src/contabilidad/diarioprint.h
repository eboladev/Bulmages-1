/***************************************************************************
                          diarioprint.h  -  description
                             -------------------
    begin                : Wed Jan 8 2003
    copyright            : (C) 2003 by Tomeu Borrás Riera
    email                : tborras@conetxia.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef DIARIOPRINT_H
#define DIARIOPRINT_H
#include <qwidget.h>
#ifndef WIN32
#include <unistd.h>
#include <sys/wait.h>
#endif
#include <errno.h>
#include <sys/types.h>
#include <string>
using std::string;
#include "postgresiface2.h"

/***@author Tomeu Borrás Riera */

class diarioprint {
public:
   char *finicial;
   char *ffinal;
   char *fichero;
   postgresiface2 *conexionbase;  
public: 
   diarioprint();
   ~diarioprint();
   int inicializa(postgresiface2 *);
   void inicializa1(char *, char *);
   void inicializa2(char *);
   void accept();
   
};

#endif
