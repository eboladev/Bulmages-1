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
#include "bfbulmafact.h"
#include "blsplashscreen.h"
#include "bldblogindialog.h"
#include "blfunctions.h"
#include "blfixed.h"
#include "blargparser.h"
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
/// NOTA: En el main no se puede utilizar _depura ya que puede que no este bien inicializado g_confpr.
int main ( int argc, char **argv )
{
   /// Puntero a BfBulmaFact
   BfBulmaFact *bges;
   try {
      fprintf ( stderr, "--> MAIN::Iniciando el programa. <--\n" );
      Q_INIT_RESOURCE ( bulmages );

      /// Leemos la configuracion que luego podremos usar siempre.
      //        g_confpr = new BlConfiguration ( "bulmafact" );
      initConfiguration ("bulmafact");

      /// Inicializa el sistema de traducciones 'gettext'.
      setlocale(LC_ALL, "");
      bindtextdomain ("bulmafact", g_confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());
      textdomain ("bulmafact");

      /// Iniciamos la clase QApplication para el uso de las Qt.
      g_theApp = new BlApplication ( argc, argv );

      /// Preparamos el sistema de plugins.
      //        g_plugins = new BlPlugins();
      initPlugins();

      /// Definimos la codificacion a Unicode.
      QTextCodec::setCodecForCStrings ( QTextCodec::codecForName ( "UTF-8" ) );
      QTextCodec::setCodecForLocale ( QTextCodec::codecForName ( "UTF-8" ) );

      g_theApp->setFont ( QFont ( g_confpr->valor ( CONF_FONTFAMILY_BULMAGES ).toAscii().constData(), atoi ( g_confpr->valor ( CONF_FONTSIZE_BULMAGES ).toAscii().constData() ) ) );

      /// Interpretar tomar los valores pasados por l&iacute;nea de comandos.
      BlArgParser *argParser = new BlArgParser( g_theApp->argc(), g_theApp->argv() );
      //   QTextStream(stdout)<<"DbName ["<<argParser->DbName()<<"] Host ["<<argParser->Host()<<"] Port ["<<argParser->Port()<<"] UserName ["<<argParser->UserName()<<"] Version ["<<argParser->ShowVersion()<<"] AskPassword ["<<argParser->AskPassword()<<"] ShowHelp ["<<argParser->ShowHelp()<<"]"<<endl;
      if( ! argParser->Host().isEmpty() ) {
         g_confpr->setValor( CONF_SERVIDOR, argParser->Host() );
      } // end if
      if( ! argParser->Port().isEmpty() ) {
         g_confpr->setValor( CONF_PUERTO, argParser->Port() );
      } // end if

      /// Mostrar las opciones de l&iacute;nea de comandos.
      if( argParser->ShowHelp() ) {
         QTextStream(stdout)
               <<"Usage: bulmafact [OPTION]"<<endl
               <<"-d, --dbname NAME     Database name"<<endl
               <<"-h, --host ADDRESS    Server name or IP"<<endl
               <<"-p, --port PORT       Port number"<<endl
               <<"-U, --username NAME   User name"<<endl
               <<"-W, --password        Force password asking"<<endl
               <<"-V, --version         Show current version number, then exit"<<endl
               <<"-?, --help            Show this help, then exit"<<endl;
      } // end if

      /// Salir ordenadamente del programa si s&oacute;lo se ha pedido ver la ayuda o la versi&oacute;n.
      if( argParser->ShowHelp() || argParser->ShowVersion() ) {
         QTextStream(stdout)<<"BulmaGes "<<CONFIG_VERSION<<endl;
         delete argParser;
         delete g_theApp;
         delete g_confpr;
         return 0;
      } // end if

      /// Cargamos el BlSplashScreen.
      BlSplashScreen *splashScr = new BlSplashScreen ( g_confpr->valor ( CONF_SPLASH_BULMAFACT ), "Iglues/BulmaFact", CONFIG_VERSION );
      splashScr->mensaje ( _( "Iniciando clases" ) );
      splashScr->setBarraProgreso ( 1 );

      /// Preguntar el nombre de usuario y/o contrase&ntilde;a en caso necesario.
      BlDbLoginDialog *login1 = new BlDbLoginDialog ( 0, "" );
      if ( !login1->authOK() || argParser->AskPassword() ) {
         if( !argParser->UserName().isEmpty() ) {
            login1->m_login->setText( argParser->UserName() );
         } // end if
         login1->exec();
      } // end if
      /// Si la autentificacion falla una segunda vez abortamos el programa.
      if ( !login1->authOK() ) {
         exit ( 1 );
      } // end if
      delete login1;

      bges = new BfBulmaFact ( argParser->DbName() );
      bges->hide();
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
         g_confpr->leeconfig ( confEsp );
      } // end if

      // Pone el color de fondo del workspace si esta definido y es un color valido.
      if ( QColor(g_confpr->valor ( CONF_BACKGROUND_COLOR )).isValid() ) {
         bges->workspace()->setBackground(QBrush(QColor( g_confpr->valor ( CONF_BACKGROUND_COLOR ) )));
      } // end if


      // Pone la imagen de fondo del workspace si esta definido y es una imagen valida.
      if ( !QImage(g_confpr->valor ( CONF_BACKGROUND_IMAGE )).isNull() ) {
         bges->workspace()->setBackground(QBrush( QImage(g_confpr->valor ( CONF_BACKGROUND_IMAGE )) ));
      } // end if

      /// Hacemos la carga de las hojas de estilo.
      QFile arch(g_confpr->valor(CONF_STYLESHEET));
      if (arch.open(QIODevice::ReadOnly | QIODevice::Text)) {
         QString style = arch.readAll();
         g_theApp->setStyleSheet(style);
      } // end if

      splashScr->mensaje ( _( "Cargando plugins" ) );
      splashScr->setBarraProgreso ( 10 );

      /// Hacemos la carga de las librerias que contienen los plugins.
      g_plugins->cargaLibs ( g_confpr->valor ( CONF_PLUGINS_BULMAFACT ) );

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
   delete g_confpr;

   return 0;
}

