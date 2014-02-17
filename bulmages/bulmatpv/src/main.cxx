/********************************************************************************
 *   Copyright (C) 2006 by IGLUES                                               *
 *   info@bulmages.org                                                          *
 *   Created by Pablo Alvarez de Sotomayor Posadillo <i02sopop@gmail.com>       *
 *                                                                              *
 *   Copyright (C) 2009 by Aron Galdon                                          *
 *   auryn@wanadoo.es                                                           *
 *   http://www.iglues.org                                                      *
 *                                                                              *
 *   Copyright (C) 2009 by Arturo Martin Llado                                  *
 *   auryn@wanadoo.es                                                           *
 *   http://www.iglues.org                                                      *
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
 ********************************************************************************/

/// Faltan el sistema de log ctlog.

/// Los datos de ejecuci&oacute;n del programa son sencillos.
/** La ejecuci&oacute;n primero crea e inicializa los objetos configuraci&oacute;n,
    idioma, blsplashscreen, etc. Interpreta posibles argumentos de l&iacute;nea de comandos.
    Luego intenta entrar en el sistema de base de datos.
    Y por &uacute;ltimo crea el objeto del tipo \ref BtBulmaTpv que es la aplicaci&oacute;n
    de ventanas. */

#include <QtCore/QDir>
#include <QtCore/QString>
#include <QtCore/QTextCodec>

#include "btbulmatpv.h"
#include "blapplication.h"
#include "blargparser.h"
#include "blconfiguration.h"
#include "bldblogindialog.h"
#include "blplugins.h"
#include "blsplashscreen.h"

#define CONFGLOBAL CONFIG_DIR_CONFIG + QString("bulmatpv_")

/// Inicio de ejecucion del programa.
/// NOTA: En el main no se puede utilizar blDebug ya que puede que no este bien inicializado g_confpr.
int main ( int argc, char **argv )
{
    bool valorSalida = 0;

    try {
        fprintf ( stderr, "--> MAIN::Iniciando el programa. <--\n" );
        Q_INIT_RESOURCE ( bulmages );
        /// Leemos la configuracion que luego podremos usar siempre.
        initConfiguration ("bulmatpv");

        /// Inicializa el sistema de traducciones 'gettext'.
        setlocale(LC_ALL, "");
        blBindTextDomain ("bulmatpv", g_confpr->value(CONF_DIR_TRADUCCION).toLatin1().constData());
        blTextDomain ("bulmatpv");

	if (g_confpr->value(CONF_TPV_TEXTOMESA).isEmpty()) g_confpr->setValue(CONF_TPV_TEXTOMESA, _("MESA "));

        /// Iniciamos la clase QApplication para el uso de las Qt.
        g_theApp = new BlApplication ( argc, argv );

        /// Preparamos el sistema de plugins.
        initPlugins();

        /// Definimos la codificaci&oacute;n a Unicode.
        //QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
        QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

        g_theApp->setFont ( QFont ( g_confpr->value( CONF_FONTFAMILY_BULMAGES ).toLatin1().constData(), atoi ( g_confpr->value( CONF_FONTSIZE_BULMAGES ).toLatin1().constData() ) ) );

        /// Interpretar tomar los valores pasados por l&iacute;nea de comandos.
        BlArgParser* argParser = new BlArgParser( g_theApp->arguments().size(), argv );

	g_confpr->setValue( CONF_REPLACE_STRING, argParser->confReplaceString() );

        if ( ! argParser->host().isEmpty() ) {
            g_confpr->setValue( CONF_SERVIDOR, argParser->host() );
        } // end if
        
        if ( ! argParser->port().isEmpty() ) {
            g_confpr->setValue( CONF_PUERTO, argParser->port() );
        } // end if

        /// Salir ordenadamente del programa si s&oacute;lo se ha pedido ver la ayuda o la versi&oacute;n.
        if ( argParser->showHelp() | argParser->showVersion() ) {
            delete argParser;
            delete g_theApp;
            delete g_confpr;
            return 0;
        } // end if

        /// Cargamos el BlSplashScreen.
        BlSplashScreen* splashScr = new BlSplashScreen ( g_confpr->value( CONF_SPLASH_BULMATPV ), "Iglues/BulmaTpv", CONFIG_VERSION );
        splashScr->setMessage ( _( "Iniciando clases" ) );
        splashScr->setProgressBar ( 1 );

        /// Preguntar el nombre de usuario y/o contrase&ntilde;a en caso necesario.
        BlDbLoginDialog* login1 = new BlDbLoginDialog ( 0, "" );
        if ( !login1->authOK() || argParser->askPassword() ) {
            if ( !argParser->userName().isEmpty() ) {
                login1->m_login->setText( argParser->userName() );
                login1->m_password->setFocus();
            } // end if
            login1->exec();
        } // end if
        
        /// Si la autentificacion falla una segunda vez abortamos el programa.
        if ( !login1->authOK() ) {
            exit ( 1 );
        } // end if
        
        delete login1;

        BtBulmaTPV* bges = new BtBulmaTPV ( argParser->dbName() );
        bges->hide();
        g_main = bges;

        /// No se va a usar m&aacute;s el gestor de argumentos.
        delete argParser;

        /// Verifica la version de la base de datos para funcionar adecuadamente.
        bges->company()->dbVersionCheck("DBRev-BulmaTPV", "1.6.0-0001");

        splashScr->show();
        splashScr->setMessage ( _( "Leyendo configuracion" ) );
        splashScr->setProgressBar ( 2 );

        /// Leemos la configuracion especifica de la base de datos que se ha abierto.
        QString confGlobalEsp = CONFGLOBAL + bges->company()->dbName() + ".conf";
        QDir archivoConf;
        if ( !archivoConf.exists ( confGlobalEsp ) ) {
            QString mensaje = "-->" + _("El archivo '") + confGlobalEsp + _("' no existe. <--\n");
            fprintf ( stderr, "%s", mensaje.toLatin1().constData() );
        } else {
            g_confpr->readConfig ( confGlobalEsp );
        } // end if
     
      QString confLocalEsp = g_confpr->getLocalDir() + QString("bulmatpv_") + bges->company()->dbName() + ".conf";
      if ( !archivoConf.exists ( confLocalEsp ) ) {
          QString mensaje = "-->" + _("El archivo '") + confLocalEsp + _("' no existe. <--\n");
          fprintf ( stderr, "%s", mensaje.toLatin1().constData() );
      } else {
          g_confpr->readConfig ( confLocalEsp );
      } // end if

        // Pone el color de fondo del workspace si esta definido y es un color valido.
        if ( QColor(g_confpr->value( CONF_BACKGROUND_COLOR )).isValid() ) {
            bges->workspace()->setBackground(QBrush(QColor( g_confpr->value( CONF_BACKGROUND_COLOR ) )));
        } // end if

        // Pone la imagen de fondo del workspace si esta definido y es una imagen valida.
        if ( !QPixmap(g_confpr->value( CONF_BACKGROUND_IMAGE )).isNull() ) {
            bges->workspace()->setBackground(QBrush( QPixmap(g_confpr->value( CONF_BACKGROUND_IMAGE )) ));
        } // end if

        /// Hacemos la carga de las hojas de estilo.
        QFile arch(g_confpr->value(CONF_STYLESHEET));
        if (arch.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString style = arch.readAll();
            g_theApp->setStyleSheet(style);
        } // end if

        splashScr->setMessage ( _( "Cargando plugins" ) );
        splashScr->setProgressBar ( 10 );

        /// Hacemos la carga de las librerias que contienen los plugins.
        g_plugins->loadLibs ( g_confpr->value( CONF_PLUGINS_BULMATPV ) );

        splashScr->setMessage ( _( "Lanzando plugins" ) );
        splashScr->setProgressBar ( 20 );

        /// Disparamos los plugins con entryPoint.
        g_plugins->run ( "entryPoint", bges );

        splashScr->setMessage ( _( "Inicializando componentes" ) );
        splashScr->setProgressBar ( 30 );

        /// Lanzamos la creacion de las ventanas principales.
        bges->createMainWindows ( splashScr );

        splashScr->setMessage ( _( "Terminado" ) );
        splashScr->setProgressBar ( 100 );
        
        delete splashScr;
        
        

	
        // Ponemos el TPV en modo a pantalla completa si asi se nos pide
        if (g_confpr->value(CONF_TPV_FULLSCREEN) == "TRUE") {
            bges->showFullScreen();
        } else {
           bges->showMaximized();
        } // end if

	int height = bges->height();
	
        // Bloqueamos el esquema de ventanas si asi se nos pide
        if (g_confpr->value(CONF_BLOCK_WINDOWS) == "TRUE") {
        
            QList<QDockWidget *> dockedList = bges->findChildren<QDockWidget *>();
            int listItems = dockedList.count();
            
            QDockWidget *dock = NULL;
            
            for (int i = 0; i < listItems; i++) {
            
                dock = dockedList.value(i);
                
                dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

            } // end for
        
        } // end if
        
        // Mostramos la aplicacion
        bges->show();

        // Entramos en el bucle de operacion que durara mientras el programa este en marcha
        valorSalida = g_theApp->exec();

        /// Disparamos los plugins con entryPoint.
        g_plugins->run ( "exitPoint", bges );

	/// Liberamos memoria.
	delete bges;
	delete g_theApp;
	delete g_confpr;
	
   } catch ( ... ) {
        blMsgInfo ( _( "Error inesperado en BulmaTPV. El programa se cerrara." ) );
   } // end try


   fprintf ( stderr, "--> MAIN::Cerrando el programa. <--\n" );
   
   return valorSalida;
}
