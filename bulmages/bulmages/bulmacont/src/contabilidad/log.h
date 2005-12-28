/***************************************************************************
 *Copyright (C) 2003 by Fco. Javier MC, Jean René Mérou, Víctor G. Marimón *
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
#ifndef LOG_H
#define LOG_H

#include <qstring.h>
#include <iostream>
#include <fstream>
using namespace std;

#include "empresa.h"

// Estos tipos son utilizados para poder hacer los logs. Se determinan en la función add.
#define LOG_SEG 1 // seguridad
#define LOG_SIS 2 // sistema
#define LOG_DBD 4 // de base de datos
#define LOG_TRA 8 // trazado de usuarios
// ... 8 bits -> ocho tipos de logs que se pueden complementar


/**
We want to log everything
@authors:
- Fco. Javier MC
- René Mérou, http://h.says.it  ,   h@myjabber.net
- Víctor G. Marimón
*/



class bitacora {
private:
    ofstream *archivolog;
    empresa *empresaactual;
    
public:
    bitacora();
    void add(int , int, QString ,QString);
    void setempresa(empresa *emp) {empresaactual=emp;}
    ~bitacora();
};

extern bitacora *ctllog;

#endif
