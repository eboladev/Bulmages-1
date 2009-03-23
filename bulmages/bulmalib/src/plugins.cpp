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

#include "plugins.h"
#include "blconfiguration.h"


typedef int ( *MyPrototype ) ( void * );
MyPrototype myFunction;

typedef int ( *MyPrototype1 ) ( void *, void ** );
MyPrototype1 myFunction1;

Plugins *g_plugins;
void *g_plugParams;


///
/**
**/
Plugins::Plugins()
{
    _depura ( "Plugins::Plugins", 0 );
    _depura ( "END Plugins::Plugins", 0 );
}


///
/**
**/
Plugins::~Plugins()
{
    _depura ( "Plugins::~Plugins", 0 );
    _depura ( "END Plugins::~Plugins", 0 );
}


///
/**
\param libs
**/
void Plugins::cargaLibs ( const QString &libs )
{
    _depura ( "Plugins::cargaLibs", 0, libs );

    bool cargado = FALSE;
    /// Hacemos la carga de los plugins.
    QString cad = libs;

    if ( cad == "" ) {
        _depura ( "END Plugins::cargaLibs", 0, cad );
        return;
    } // end if

    QStringList dirs = confpr->valor ( CONF_DIR_PLUGINS ).split ( ";" );

    QStringList plugins = cad.split ( ";" );
    for ( QStringList::Iterator it = plugins.begin(); it != plugins.end(); ++it ) {
        cargado = FALSE;
        for ( QStringList::Iterator ot = dirs.begin(); ot != dirs.end(); ++ot ) {
            QString file = *ot + *it;
            QLibrary *lib = new QLibrary ( file );
            lib->load();
            if ( lib->isLoaded() ) {
                cargado = TRUE;
                m_lista.append ( lib );
            } // end if
        } // end for
        if ( ! cargado ) {
            mensajeInfo ( "No se ha podido cargar la libreria: " + *it );
        } // end if
    } // end for
    _depura ( "END Plugins::cargaLibs", 0 );
}

///
/**
\param func
\param clase
\return
**/
int Plugins::lanza ( const char *func, void *clase )
{
    _depura ( "Plugins::lanza", 0, func );
    int a = 0;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        myFunction = ( MyPrototype ) m_lista.at ( i ) ->resolve ( func );
        if ( myFunction && a == 0) {
            a = myFunction ( clase );
        } // end if
    } // end for
    _depura ( "END Plugins::lanza", 0 );
    return a;
}


///
/**
\return
**/
int Plugins::lanza ( const char *func, void *clase, void **ret )
{
    _depura ( "Plugins::lanza", 0, func );
    int a = 0;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        myFunction1 = ( MyPrototype1 ) m_lista.at ( i ) ->resolve ( func );
        if ( myFunction1 && a == 0) {
            _depura ( "Plugins_lanza ", 0, "Encontrada una funcion correspondiente con el prototipo" );
            a = myFunction1 ( clase, ret );
        } else {
            _depura ( "Plugins_lanza ", 0, "No ha entrado la libreria" );
        } // end if
    } // end for
    _depura ( "END Plugins::lanza", 0 );
    return a;
}
