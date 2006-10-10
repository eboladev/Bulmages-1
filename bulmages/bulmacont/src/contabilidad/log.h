/***************************************************************************
 *   Copyright (C) 2003 by Fco. Javier M.C. &                              *
 *                         Jean Rene Merou (http://h.says.it) &            *
 *                         Victor G. Marimon.                              *
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

#include <iostream>
#include <fstream>

#include <QString>

using namespace std;

#include "empresa.h"

/// Estos tipos son utilizados para poder hacer los logs. Se determinan en la funci&oacute;n add.
#define LOG_SEG 1 // seguridad
#define LOG_SIS 2 // sistema
#define LOG_DBD 4 // de base de datos
#define LOG_TRA 8 // trazado de usuarios
/// ... 8 bits -> ocho tipos de logs que se pueden complementar.


class bitacora {
private:
    ofstream *archivolog;
    empresa *empresaactual;

public:
    bitacora();
    void add(int, int, QString, QString);
    void setempresa(empresa *emp) {
        empresaactual = emp;
    }
    ~bitacora();
};


extern bitacora *ctllog;

#endif

