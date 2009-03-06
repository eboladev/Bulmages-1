/********************************************************************************
 *   Copyright (C) 2006 by IGLUES                                               *
 *   info@bulmages.org                                                          *
 *   Created by Pablo Alvarez de Sotomayor Posadillo <i02sopop@gmail.com>       *
 *                                                                              *
 *   This program is free software; you can redistribute it and/or modify       *
 *   it under the terms of the GNU General Public License as published by       *
 *   the Free Software Foundation; either version 2 of the License, or          *
 *   (at your option) any later version.                                        *
 *                                                                              *
 *   This program is distributed in the hope that it will be useful,            *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the              *
 *   GNU General Public License for more details.                               *
 *                                                                              *
 *   You should have received a copy of the GNU General Public License          *
 *   along with this program; if not, write to the                              *
 *   Free Software Foundation, Inc.,                                            *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.                  *
 *******************************************************************************/

#include <QTextCodec>
#include <QLocale>
#include <QDir>

#include "blapplication.h"
#include "blconfiguration.h"
#include "plugins.h"
#include "bulmatpv.h"
#include "blsplashscreen.h"
#include "logpass.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "plugins.h"
#include "config.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages\\bulmatpv_"
#else
#define CONFGLOBAL CONFIG_DIR_CONFIG + QString("bulmatpv_")
#endif


BlApplication* theApp;
BulmaTPV* bges;

/// Inicio de ejecucion del programa.
/// NOTA: En el main no se puede utilizar _depura ya que puede que no este bien inicializado confpr.
int main ( int argc, char **argv )
{
    try {
        fprintf ( stderr, "--> MAIN::Iniciando el programa. <--\n" );
        Q_INIT_RESOURCE ( bulmages );

        /// Leemos la configuracion que luego podremos usar siempre.
        confpr = new BlConfiguration ( "bulmatpv" );
        /// Preparamos el sistema de plugins.
        g_plugins = new Plugins();
        /// Iniciamos la clase QApplication para el uso de las Qt.
        theApp = new BlApplication ( argc, argv );

        /// Definimos la codificacion a Unicode.
        QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
        QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

        theApp->setFont ( QFont ( confpr->valor ( CONF_FONTFAMILY_BULMAGES ).toAscii().constData(), atoi ( confpr->valor ( CONF_FONTSIZE_BULMAGES ).toAscii().constData() ) ) );

        /// Cargamos el BlSplashScreen.
        BlSplashScreen* splashScr = new BlSplashScreen ( confpr->valor ( CONF_SPLASH_BULMATPV ), "Iglues/BulmaTPV", CONFIG_VERSION );
        splashScr->mensaje ( _("Iniciando clases" ) );
        splashScr->setBarraProgreso ( 1 );

        /// Leemos los argumentos pasados por la linea de comandos.
        QString db = "";
        QString us = "";
        QString pass = "";

        /// Dependiendo de los argumentos pasados se lanza con unas opciones u otras para la
        /// conexion con la base de datos.
        if ( argc == 5 ) {
            db = argv[2];
            us = argv[3];
            pass = argv[4];
            confpr->setValor ( CONF_LOGIN_USER, us );
            confpr->setValor ( CONF_PASSWORD_USER, pass );
            bges = new BulmaTPV ( db );
            bges->hide();
        } else if ( argc == 3 ) {
            db = argv[2];
            bges = new BulmaTPV ( db );
            bges->hide();
        } else {
            logpass* login1 = new logpass ( 0, "" );
            if ( !login1->authOK() ) {
                login1->exec();
            } // end if
            /// Si la autentificacion falla una segunda vez abortamos el programa.
            if ( !login1->authOK() ) {
                exit ( 1 );
            } // end if
            delete login1;
            bges = new BulmaTPV ( "" );
            bges->hide();
        } // end if

        g_main = bges;

        splashScr->show();
        splashScr->mensaje ( _( "Leyendo configuracion" ) );
        splashScr->setBarraProgreso ( 2 );

        /// Leemos la configuracion especifica de la base de datos que se ha abierto.
        QString confEsp = CONFGLOBAL + bges->empresaTPV()->dbName() + ".conf";
        QDir archivoConf;
        if (!archivoConf.exists(confEsp)) {
            QString mensaje = "--> El archivo '" + confEsp + "' no existe. <--\n";
            fprintf(stderr, mensaje.toAscii().constData());
        } else {
            confpr->leeconfig(confEsp);
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

        splashScr->mensaje ( _( "Cargando plugins" ) );
        splashScr->setBarraProgreso ( 4 );

        /// Hacemos la carga de las librerias que contienen los plugins.
        g_plugins->cargaLibs ( confpr->valor ( CONF_PLUGINS_BULMATPV ) );

        splashScr->mensaje ( _( "Lanzando plugins" ) );
        splashScr->setBarraProgreso ( 5 );

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "entryPoint", bges );

        splashScr->mensaje ( _( "Inicializando componentes" ) );
        splashScr->setBarraProgreso ( 6 );

        /// Lanzamos la creacion de las ventanas principales.
        bges->createMainWindows ( splashScr );

        splashScr->mensaje ( _( "Terminado" ) );
        splashScr->setBarraProgreso ( 100 );

        delete splashScr;
	if (confpr->valor(CONF_TPV_FULLSCREEN) == "TRUE") {
	        bges->showFullScreen();
	} else {
		bges->showMaximized();
	} // end if

        theApp->exec();

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "exitPoint", bges );
    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado en BulmaTPV. El programa se cerrara." ) );
    } // end try

    fprintf ( stderr, "--> MAIN::Cerrando el programa. <--\n" );
    /// Liberamos memoria.
    delete bges;
    delete theApp;
    delete confpr;

    return 0;
}

