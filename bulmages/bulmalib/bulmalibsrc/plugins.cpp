/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
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
#include <QLibrary>
#include <QStringList>

#include "funcaux.h"
#include "plugins.h"


typedef int (*MyPrototype)(void *);
MyPrototype myFunction;

typedef int (*MyPrototype1)(void *, void **);
MyPrototype1 myFunction1;

Plugins *g_plugins;


Plugins::Plugins() {}


Plugins::~Plugins() {}


void Plugins::cargaLibs(const QString libs) {
    /// Hacemos la carga de los plugins.
    QString cad = libs;
    _depura("Plugins::cargaLibs", 0, cad);
    if (cad == "") {
        _depura("END Plugins::cargaLibs", 0, cad);
        return;
    } // end if
    QStringList plugins = cad.split(";");
    for (QStringList::Iterator it = plugins.begin(); it != plugins.end(); ++it) {
        QLibrary *lib = new QLibrary(*it);
        lib->load();
        if (!lib->isLoaded()) {
            mensajeInfo("No se ha podido cargar la libreria: " + *it + "\nERROR: " + lib->errorString());
        } else {
            m_lista.append(lib);
        } // end if
    } // end for
    _depura("END Plugins::cargaLibs", 0);
}


int Plugins::lanza(const char *func, void *clase) {
    _depura("Plugins::lanza", 0, func);
    int a = 0;
    for (int i = 0; i < m_lista.size(); ++i) {
        myFunction = (MyPrototype) m_lista.at(i)->resolve(func);
        if (myFunction) {
            _depura("Plugins_lanza", 0, "Encontrada una funcion correspondiente con el prototipo");
            a = myFunction(clase);
        } else {
            _depura("Plugins_lanza", 0, "No ha entrado la libreria");
        } // end if
    } // end for
    _depura("END Plugins::lanza", 0);
    return a;
}


int Plugins::lanza(const char *func, void *clase, void **ret) {
    _depura("Plugins::lanza", 0, func);
    int a = 0;
    for (int i = 0; i < m_lista.size(); ++i) {
        myFunction1 = (MyPrototype1) m_lista.at(i)->resolve(func);
        if (myFunction1) {
            _depura("Plugins_lanza", 0, "Encontrada una funcion correspondiente con el prototipo");
            a = myFunction1(clase, ret);
        } else {
            _depura("Plugins_lanza", 0, "No ha entrado la libreria");
        } // end if
    } // end for
    _depura("END Plugins::lanza", 0);
    return a;
}

