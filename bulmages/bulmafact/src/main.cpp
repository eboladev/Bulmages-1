/***************************************************************************
 *   Copyright (C) 2004 by Tomeu Borras Riera                              *
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
#include <QDir>

#include "qapplication2.h"
#include "configuracion.h"
#include "plugins.h"
#include "bulmafact.h"
#include "splashscreen.h"
#include "logpass.h"
#include "funcaux.h"
#include "fixed.h"
#include "plugins.h"
#include "config.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages\\bulmafact_"
#else
#define CONFGLOBAL CONFIG_DIR_CONFIG + QString("bulmafact_")
#endif


QApplication2 *theApp;
Bulmafact *bges;
QTranslator *traductor;

/// Inicio de ejecucion del programa.
/// NOTA: En el main no se puede utilizar _depura ya que puede que no este bien inicializado confpr.
int main ( int argc, char **argv )
{
    try {
        fprintf ( stderr, "--> MAIN::Iniciando el programa. <--\n" );
        Q_INIT_RESOURCE ( bulmages );

        /// Leemos la configuracion que luego podremos usar siempre.
        confpr = new configuracion ( "bulmafact" );
        /// Preparamos el sistema de plugins.
        g_plugins = new Plugins();
        /// Iniciamos la clase QApplication para el uso de las Qt.
        theApp = new QApplication2 ( argc, argv );

        /// Definimos la codificacion a Unicode.
        QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
        QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

        theApp->setFont ( QFont ( confpr->valor ( CONF_FONTFAMILY_BULMAGES ).toAscii().constData(), atoi ( confpr->valor ( CONF_FONTSIZE_BULMAGES ).toAscii().constData() ) ) );

        /// Cargamos el sistema de traducciones.
        traductor = new QTranslator ( 0 );

        /// La funcion QLocale::system().name() devuelve el codigo en formato
        /// 'es_ES' de idioma y pais.
        /// El orden de busqueda del archivo de traduccion es:
        /// 1) bulmalib_es_ES.qm
        /// 2) bulmalib_es_ES
        /// 3) bulmalib_es.qm
        /// 4) bulmalib_es
        /// 5) bulmalib_.qm
        /// 6) bulmalib_
        /// Cogemos traducciones de bulmalib
        if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
            traductor->load ( QString ( "bulmalib_" ) + QLocale::system().name(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } else {
            QString archivo = "bulmalib_" + confpr->valor ( CONF_TRADUCCION );
            traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } // end if
        theApp->installTranslator ( traductor );

        /// Cogemos traducciones de bulmafact
        traductor = new QTranslator ( 0 );
        if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
            traductor->load ( QString ( "bulmafact_" ) + QLocale::system().name(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } else {
            QString archivo = "bulmafact_" + confpr->valor ( CONF_TRADUCCION );
            traductor->load ( archivo.toAscii().constData(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } // end if
        theApp->installTranslator ( traductor );

        /// Cargamos el splashScreen.
        Splash *splashScr = new Splash ( confpr->valor ( CONF_SPLASH_BULMAFACT ), "BulmaFact", CONFIG_VERSION );
        splashScr->mensaje ( QApplication::translate ( "main", "Iniciando clases" ) );
        splashScr->setBarraProgreso ( 1 );

        /// Leemos los argumentos pasados por la linea de comandos.
        QString db = argv[2];
        QString us = argv[3];
        QString pass = argv[4];

        /// Dependiendo de los argumentos pasados se lanza con unas opciones u otras para la
        /// conexion con la base de datos.
        if ( argc == 5 ) {
            confpr->setValor ( CONF_LOGIN_USER, us );
            confpr->setValor ( CONF_PASSWORD_USER, pass );
            bges = new Bulmafact ( db );
            bges->hide();
        } else if ( argc == 3 ) {
            bges = new Bulmafact ( db );
            bges->hide();
        } else {
            logpass *login1 = new logpass ( 0, "" );
            if ( !login1->authOK() ) {
                login1->exec();
            } // end if
            /// Si la autentificacion falla una segunda vez abortamos el programa.
            if ( !login1->authOK() ) {
                exit ( 1 );
            } // end if
            delete login1;
            bges = new Bulmafact ( "" );
            bges->hide();
        } // end if
        g_main = bges;

        splashScr->show();
        splashScr->mensaje ( QApplication::translate ( "main", "Leyendo configuracion" ) );
        splashScr->setBarraProgreso ( 2 );

        /// Leemos la configuracion especifica de la base de datos que se ha abierto.
        QString confEsp = CONFGLOBAL + bges->getcompany() ->nameDB() + ".conf";
        QDir archivoConf;
        if ( !archivoConf.exists ( confEsp ) ) {
            QString mensaje = "--> El archivo '" + confEsp + "' no existe. <--\n";
            fprintf ( stderr, mensaje.toAscii().constData() );
        } else {
            confpr->leeconfig ( confEsp );
        } // end if

        splashScr->mensaje ( QApplication::translate ( "main", "Cargando traducciones" ) );
        splashScr->setBarraProgreso ( 3 );

        /// Cargamos el sistema de traducciones una vez pasado por las configuraciones generales
        traductor = new QTranslator ( 0 );
        if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
            traductor->load ( QString ( "bulmalib_" ) + QLocale::system().name(),
                              confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } else {
            QString archivo = "bulmalib_" + confpr->valor ( CONF_TRADUCCION );
            traductor->load ( archivo, confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } // end if
        theApp->installTranslator ( traductor );

        traductor = new QTranslator ( 0 );
        if ( confpr->valor ( CONF_TRADUCCION ) == "locales" ) {
            traductor->load ( QString ( "bulmafact_" ) + QLocale::system().name(),
                              confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } else {
            QString archivo = "bulmafact_" + confpr->valor ( CONF_TRADUCCION );
            traductor->load ( archivo.toAscii().constData(), confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );
        } // end if
        theApp->installTranslator ( traductor );

        splashScr->mensaje ( QApplication::translate ( "main", "Cargando plugins" ) );
        splashScr->setBarraProgreso ( 4 );

        /// Hacemos la carga de las librerias que contienen los plugins.
        g_plugins->cargaLibs ( confpr->valor ( CONF_PLUGINS_BULMAFACT ) );

        splashScr->mensaje ( QApplication::translate ( "main", "Lanzando plugins" ) );
        splashScr->setBarraProgreso ( 5 );

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "entryPoint", bges );

        splashScr->mensaje ( QApplication::translate ( "main", "Inicializando componentes" ) );
        splashScr->setBarraProgreso ( 6 );

        /// Lanzamos la creacion de las ventanas principales.
        bges->createMainWindows ( splashScr );



        splashScr->mensaje ( QApplication::translate ( "main", "Terminado" ) );
        splashScr->setBarraProgreso ( 100 );

        delete splashScr;
        bges->show();

        theApp->exec();

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "exitPoint", bges );
    } catch ( ... ) {
        mensajeInfo ( QApplication::translate ( "main", "Error inesperado en BulmaFact. El programa se cerrara." ) );
    } // end try

    fprintf ( stderr, "--> MAIN::Cerrando el programa. <--\n" );
    /// Liberamos memoria.
    delete bges;
    delete theApp;
    delete traductor;
    delete confpr;

    return 0;
}

