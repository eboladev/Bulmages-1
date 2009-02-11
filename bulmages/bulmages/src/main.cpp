/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include <QTranslator>
#include <QTextCodec>
#include <QLocale>

#include "bselector.h"
#include "blapplication.h"
#include "blconfiguration.h"
#include "logpass.h"
#include "blpostgresqlclient.h"
#include "funcaux.h"
#include "plugins.h"


/// Instancia de la aplicacion. Usada en algunos casos para acceder a determinadas
/// funcionalidades como la traduccion.
BlApplication *theApp;
/// Instancia de la traduccion que se carga en tiempo de ejecucion y que se usa
/// de forma global.
QTranslator *traductor;


/// Crea el objeto base y lo lanza, tambien comprueba si se ha lanzado la
/// aplicacion con parametros o sin ellos.
/**
\param argc
\param argv
\return
**/
int main ( int argc, char **argv )
{
    confpr = new BlConfiguration ( "bulmages" );
    BlApplication a ( argc, argv );
    Q_INIT_RESOURCE ( bulmages );
    theApp = &a;

    /// Preparamos el sistema de plugins.
    g_plugins = new Plugins();

    /// Definimos la codificaci&oacute;n a Unicode.
    QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
    QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "CP1252" ) );

    theApp->setFont ( QFont ( confpr->valor ( CONF_FONTFAMILY_BULMAGES ).toAscii().constData(), atoi ( confpr->valor ( CONF_FONTSIZE_BULMAGES ).toAscii().constData() ) ) );

    /// Set the location where your .qm files are in load() below as the last parameter
    /// instead of "." for development, use "/" to use the english original as
    /// .qm files are stored in the base project directory.
    traductor = new QTranslator ( 0 );
    if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
        traductor->load ( QString ( "bulmalib_" ) + QLocale::system().name(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = "bulmalib_" + confpr->valor ( CONF_TRADUCCION );
        traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } // end if
    a.installTranslator ( traductor );

    traductor = new QTranslator ( 0 );
    if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
        traductor->load ( QString ( "bulmages_" ) + QLocale::system().name(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } else {
        QString archivo = "bulmages_" + confpr->valor ( CONF_TRADUCCION );
        traductor->load ( archivo.toAscii().constData(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
    } // end if
    a.installTranslator ( traductor );

    /// Hacemos la carga de las librerias que contienen los plugins.
    g_plugins->cargaLibs ( confpr->valor ( CONF_PLUGINS_BULMAGES ) );

    /// Disparamos los plugins con entryPoint.
    g_plugins->lanza ( "entryPoint", theApp );


    BSelector *bw = new BSelector();
    bw->setWindowTitle ( _( "Selector de BulmaGes" ) );

    /// Lo primero que hacemos es comprobar el sistema de autentificacion de
    /// Postgres para pedir un 'login' y un 'password' en caso de que sea necesario para
    /// entrar en el sistema.
    logpass *login1 = new logpass ( 0, "" );
    if ( !login1->authOK() ) {
        login1->exec();
    } // end if
    if ( !login1->authOK() ) {
        exit ( 1 );
    } // end if
    delete login1;

    /// Una vez que hemos pasado por el sistema de 'login' y 'password' y ya somos
    /// autenticos podemos mostrar el selector de acciones.
    bw->show();
    return a.exec();
}

