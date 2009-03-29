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

#include <QTextCodec>
#include <QLocale>
#include <QDir>
#include <QtDBus/QtDBus>
#include <QtDBus/QDBusConnection>

#include "blapplication.h"
#include "blconfiguration.h"
#include "blplugins.h"
#include "bulmafact.h"
#include "blsplashscreen.h"
#include "bldblogindialog.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "blplugins.h"
#include "blconfig.h"
#include "bli18n.h"

//#include "dbusobject.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages\\bulmafact_"
#else
#define CONFGLOBAL CONFIG_DIR_CONFIG + QString("bulmafact_")
#endif




/// Inicio de ejecucion del programa.
/// NOTA: En el main no se puede utilizar _depura ya que puede que no este bien inicializado confpr.
int main ( int argc, char **argv )
{
    /// Puntero a Bulmafact
    Bulmafact *bges;
    try {
        fprintf ( stderr, "--> MAIN::Iniciando el programa. <--\n" );
        Q_INIT_RESOURCE ( bulmages );

        /// Leemos la configuracion que luego podremos usar siempre.
        confpr = new BlConfiguration ( "bulmafact" );

	/// Inicializa el sistema de traducciones 'gettext'.
	setlocale(LC_ALL, "");
	bindtextdomain ("bulmafact", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
	textdomain ("bulmafact");

        /// Iniciamos la clase QApplication para el uso de las Qt.
        g_theApp = new BlApplication ( argc, argv );

        /// Preparamos el sistema de plugins.
        g_plugins = new BlPlugins();

        /// Definimos la codificacion a Unicode.
        QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
        QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

        g_theApp->setFont ( QFont ( confpr->valor ( CONF_FONTFAMILY_BULMAGES ).toAscii().constData(), atoi ( confpr->valor ( CONF_FONTSIZE_BULMAGES ).toAscii().constData() ) ) );

        /// Cargamos el BlSplashScreen.
        BlSplashScreen *splashScr = new BlSplashScreen ( confpr->valor ( CONF_SPLASH_BULMAFACT ), "Iglues/BulmaFact", CONFIG_VERSION );
        splashScr->mensaje ( _( "Iniciando clases" ) );
        splashScr->setBarraProgreso ( 1 );

        /// Leemos los argumentos pasados por la linea de comandos.
        QString db = "";
        QString us = "";
        QString pass = "";

        /// Dependiendo de los argumentos pasados se lanza con unas opciones u otras para la
        /// conexion con la base de datos.
        if ( argc == 5 ) {
            /// Leemos los argumentos pasados por la linea de comandos.
            db = argv[2];
            us = argv[3];
            pass = argv[4];
            confpr->setValor ( CONF_LOGIN_USER, us );
            confpr->setValor ( CONF_PASSWORD_USER, pass );
            bges = new Bulmafact ( db );
            bges->hide();
        } else if ( argc == 3 ) {
            /// Leemos los argumentos pasados por la linea de comandos.
            db = argv[2];
            bges = new Bulmafact ( db );
            bges->hide();
        } else {
            BlDbLoginDialog *login1 = new BlDbLoginDialog ( 0, "" );
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
        splashScr->mensaje ( _( "Leyendo configuracion" ) );
        splashScr->setBarraProgreso ( 2 );

        /// Leemos la configuracion especifica de la base de datos que se ha abierto.
        QString confEsp = CONFGLOBAL + bges->getcompany() ->dbName() + ".conf";
        QDir archivoConf;
        if ( !archivoConf.exists ( confEsp ) ) {
            QString mensaje = "--> El archivo '" + confEsp + "' no existe. <--\n";
            fprintf ( stderr,"%s", mensaje.toAscii().constData() );
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
	  g_theApp->setStyleSheet(style);
	} // end if

        splashScr->mensaje ( _( "Cargando plugins" ) );
        splashScr->setBarraProgreso ( 10 );

        /// Hacemos la carga de las librerias que contienen los plugins.
        g_plugins->cargaLibs ( confpr->valor ( CONF_PLUGINS_BULMAFACT ) );

        splashScr->mensaje ( _( "Lanzando plugins" ) );
        splashScr->setBarraProgreso ( 20 );

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "entryPoint", bges );

        splashScr->mensaje ( _( "Inicializando componentes" ) );
        splashScr->setBarraProgreso ( 30 );

        /// Lanzamos la creacion de las ventanas principales.
        bges->createMainWindows ( splashScr );

        splashScr->mensaje ( _( "Terminado" ) );
        splashScr->setBarraProgreso ( 100 );

        delete splashScr;
        bges->show();

        g_theApp->exec();

        /// Disparamos los plugins con entryPoint.
        g_plugins->lanza ( "exitPoint", bges );
    } catch ( ... ) {
        mensajeInfo ( _( "Error inesperado en BulmaFact. El programa se cerrara." ) );
    } // end try

    fprintf ( stderr, "--> MAIN::Cerrando el programa. <--\n" );
    /// Liberamos memoria.
    delete bges;
    delete g_theApp;
    delete confpr;

    return 0;
}

