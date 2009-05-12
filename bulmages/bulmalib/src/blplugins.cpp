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

#include "blplugins.h"
#include "blconfiguration.h"


typedef int ( *MyPrototype ) ( void * );
MyPrototype myFunction;

typedef int ( *MyPrototype1 ) ( void *, void ** );
MyPrototype1 myFunction1;

BlPlugins BL_EXPORT *g_plugins = NULL;
void BL_EXPORT *g_plugParams = NULL;


void initPlugins()
{
    g_plugins = new BlPlugins();
}

///
/**
**/
BlPlugins::BlPlugins()
{
    _depura ( "BlPlugins::BlPlugins", 0 );
    _depura ( "END BlPlugins::BlPlugins", 0 );
}


///
/**
**/
BlPlugins::~BlPlugins()
{
    _depura ( "BlPlugins::~BlPlugins", 0 );
    _depura ( "END BlPlugins::~BlPlugins", 0 );
}


///
/**
\param libs
**/
void BlPlugins::cargaLibs ( const QString &libs )
{
    _depura ( "BlPlugins::cargaLibs", 0, libs );

    bool cargado = FALSE;
    /// Hacemos la carga de los plugins.
    QString cad = libs;

    if ( cad == "" ) {
        _depura ( "END BlPlugins::cargaLibs", 0, cad );
        return;
    } // end if

    QStringList dirs = g_confpr->valor ( CONF_DIR_PLUGINS ).split ( ";" );

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
            mensajeInfo ( _ ("No se ha podido cargar la libreria: " ) + *it );
        } // end if
    } // end for
    _depura ( "END BlPlugins::cargaLibs", 0 );
}

///
/**
\param func
\param clase
\return
**/
int BlPlugins::lanza ( const char *func, void *clase )
{
    _depura ( "BlPlugins::lanza", 0, func );
    int a = 0;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        myFunction = ( MyPrototype ) m_lista.at ( i ) ->resolve ( func );
        if ( myFunction && a == 0 ) {
            a = myFunction ( clase );
        } // end if
    } // end for
    _depura ( "END BlPlugins::lanza", 0 );
    return a;
}


///
/**
\return
**/
int BlPlugins::lanza ( const char *func, void *clase, void **ret )
{
    _depura ( "BlPlugins::lanza", 0, func );
    int a = 0;
    for ( int i = 0; i < m_lista.size(); ++i ) {
        myFunction1 = ( MyPrototype1 ) m_lista.at ( i ) ->resolve ( func );
        if ( myFunction1 && a == 0 ) {
            _depura ( "Plugins_lanza ", 0, "Encontrada una funcion correspondiente con el prototipo" );
            a = myFunction1 ( clase, ret );
        } else {
            _depura ( "Plugins_lanza ", 0, "No hay funcion correspondiente con el prototipo" );
        } // end if
    } // end for
    _depura ( "END BlPlugins::lanza", 0 );
    return a;
}
