/***************************************************************************
 *   Copyright (C) 2002 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2002 by Josep Burcion                                   *
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
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <cstdlib>

#include <QApplication>
#include <QFont>
#include <QString>
#include <QTextCodec>
#include <QLibrary>
#include <QLocale>

#ifndef WIN32
#include <unistd.h>
#endif

#include "bulmacont.h"
#include "blsplashscreen.h"
#include "blconfiguration.h"
#include "qtextcodec.h"
#include "logpass.h"
#include "blplugins.h"
#include "blapplication.h"
#include "config.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages\\bulmacont_"
#else
#define CONFGLOBAL CONFIG_DIR_CONFIG + QString("bulmacont_")
#endif

#include <iostream>


/// Estas son las variables globales de la aplicaci&oacute;n.
/// El puntero de la aplicaci&oacute;n.
BlApplication *theApp;

/// Faltan el configurador de par&aacute;metros confpr y el sistema de log ctlog.

/// Los datos de ejecuci&oacute;n del programa son sencillos.
/** La ejecuci&oacute;n primero crea e inicializa los objetos configuraci&oacute;n,
    idioma, blsplashscreen, etc.
    Luego intenta entrar en el sistema de base de datos.
    Y por &uacute;ltimo crea el objeto del tipo \ref Bulmacont que es la aplicaci&oacute;n
    de ventanas. */

int main ( int argc, char **argv )
{
    /// Leemos la configuraci&oacute;n que luego podremos usar siempre.
    confpr = new BlConfiguration ( "bulmacont" );
    int valorsalida = 0;
    bool success = false;
    QString db = "";
    QString us = "";
    QString pass = "";
    Bulmacont *bges;

    try {
        /// Inicializa el sistema de traducciones 'gettext'.
        setlocale(LC_ALL, "");
        bindtextdomain ("bulmacont", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
        textdomain ("bulmacont");

        /// Inicializamos el objeto global para uso de plugins.
        g_plugins = new BlPlugins();

        /// Creamos la aplicaci&oacute;n principal.
        theApp = new BlApplication ( argc, argv );
        theApp->setFont ( QFont ( confpr->valor ( CONF_FONTFAMILY_BULMAGES ).toAscii(), atoi ( confpr->valor ( CONF_FONTSIZE_BULMAGES ).toAscii() ) ) );

        /// Definimos la codificaci&oacute;n a Unicode.
        QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
        QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

        /// Cargamos el BlSplashScreen.
        BlSplashScreen *splashScr = new BlSplashScreen ( confpr->valor ( CONF_SPLASH_BULMACONT ), "Iglues/BulmaCont", CONFIG_VERSION );
        splashScr->mensaje ( _( "Iniciando clases" ) );
        splashScr->setBarraProgreso ( 1 );

        /// Miramos en los par&aacute;metros pasados al programa por si ya viene
        /// indicada la empresa y no hay que mostrar selector.
        if ( argc == 5 ) {
	    db = argv[2];
	    us = argv[3];
	    pass = argv[4];
            confpr->setValor ( CONF_LOGIN_USER, us );
            confpr->setValor ( CONF_PASSWORD_USER, pass );
            bges = new Bulmacont ( NULL, 0, db );
            bges->hide();
        } else if ( argc == 3 ) {
            db = argv[2];
            bges = new Bulmacont ( NULL, 0, db );
            bges->hide();
        } else {
            logpass *login1 = new logpass ( 0, "" );
            if ( !login1->authOK() )
                login1->exec();
            if ( !login1->authOK() )
                exit ( 1 );
            delete login1;
            bges = new Bulmacont ( NULL, 0, "" );
            bges->hide();
        } // end if
        g_main = bges;

        splashScr->show();
        splashScr->mensaje ( _( "Leyendo configuracion" ) );
        splashScr->setBarraProgreso ( 2 );

        /// Leemos la configuracion especifica de la base de datos que se ha abierto.
        QString confEsp = CONFGLOBAL + bges->empresaactual() ->dbName() + ".conf";
        QDir archivoConf;
        if ( !archivoConf.exists ( confEsp ) ) {
            QString mensaje = "--> El archivo '" + confEsp + "' no existe. <--\n";
            fprintf ( stderr, "%s", mensaje.toAscii().constData() );
        } else {
            confpr->leeconfig ( confEsp );
        } // end if


	// Pone el color de fondo del workspace si esta definido y es un color valido.
	if ( QColor(confpr->valor ( CONF_BACKGROUND_COLOR )).isValid() ) {
	    bges->workspace()->setBackground(QBrush(QColor( confpr->valor ( CONF_BACKGROUND_COLOR ) )));
	} // end if

	// Pone la imagen de fondo del workspace si esta definido y es una imagen valida.
	if ( !QImage(confpr->valor ( CONF_BACKGROUND_IMAGE )).isNull() ) {
	    bges->workspace()->setBackground(QBrush( QImage(confpr->valor ( CONF_BACKGROUND_IMAGE )) ));
	} // end if

	/// Hacemos la carga de las hojas de estilo.
	QFile arch(confpr->valor(CONF_STYLESHEET));
	if (arch.open(QIODevice::ReadOnly | QIODevice::Text)) {
	  QString style = arch.readAll();
	  theApp->setStyleSheet(style);
	} // end if

        /// Cargamos las librerias de g_plugins.
        g_plugins->cargaLibs ( confpr->valor ( CONF_PLUGINS_BULMACONT ) );

        splashScr->mensaje ( _( "Lanzando plugins" ) );
        splashScr->setBarraProgreso ( 5 );

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "entryPoint", bges );

        splashScr->mensaje ( _( "Inicializando componentes" ) );
        splashScr->setBarraProgreso ( 6 );

        bges->empresaactual() ->createMainWindows ( splashScr );

        bges->show();
        delete splashScr;

        valorsalida = theApp->exec();

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "exitPoint", bges );

    } catch ( ... ) {
        mensajeInfo ( "Error inesperado en BulmaCont, el programa se cerrara." );
    } // end try

    /// Liberamos memoria.
    delete bges;
    delete theApp;
    delete confpr;

    fprintf ( stderr, "--> MAIN::Cerrando el programa. <--\n" );
    return valorsalida;
}

