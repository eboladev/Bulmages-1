/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Wed Nov 27 17:16:28 CET 2002
    copyright            : (C) 2002 by tborras@conetxia.com, josep@burcion.com
    email                : 
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include <stdlib.h>
#include <qapplication.h>
#include <qfont.h>
#include <qstring.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qlibrary.h>


#ifndef WIN32
#include <unistd.h>
#endif

#include "bulmages01.h"
#include "splashscreen.h"
#include "configuracion.h"
#include "qtextcodec.h"
#include "log.h"
#include "logpass.h"
#include "plugins.h"
#include "qapplication2.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages_"
#else
#define CONFGLOBAL "/etc/bulmages_"
#endif



/// Estas son las variables globales de la aplicación.
/// El puntero de la aplicación
QApplication2 * theApp;
/// El traductor.
QTranslator * traductor;

/// Faltan el configurador de parametros confpr y el sistema de log ctlog


/** \brief los datos de ejecución del programa son sencillos
  * La ejecución primero crea e inicializa los objetos configuración, idioma, splash, etc
  * luego intenta entrar en el sistema de base de datos
  * Y por último crea el objeto del tipo \ref Bulmages01 que es la aplicación de ventanas.
  */
int main(int argc, char *argv[]) {
    Bulmages01 *bges;
    int valorsalida=0;
    QString db= argv[2];
    QString us=argv[3];
    QString pass=argv[4];
    try {
        /// Leemos la configuracion que luego podremos usar siempre
        confpr = new configuracion();
        /// Inicializamos el objeto global para uso de plugins
        g_plugins = new Plugins();

        /// Definimos la codificacion a Unicode.
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1252"));

        /// Creamos la aplicacion principal
        theApp = new QApplication2 (argc, argv);
        theApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).ascii(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).ascii())));

        /// Cargamos las primeras traducciones para bulmalib y para bulmacont
        traductor = new QTranslator ( 0 );
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load( QString("bulmalib_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
        } else {
            QString archivo = "bulmalib_"+confpr->valor(CONF_TRADUCCION);
            traductor->load(archivo,confpr->valor(CONF_DIR_TRADUCCION).ascii());
        }// end if
        theApp->installTranslator( traductor );

        traductor = new QTranslator ( 0 );
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load( QString("bulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
        } else {
            QString archivo = "bulmages_"+confpr->valor(CONF_TRADUCCION);
            traductor->load(archivo.ascii(),confpr->valor(CONF_DIR_TRADUCCION).ascii());
        }// end if
        theApp->installTranslator( traductor );

        /// Iniciamos el sistema de log del programa para que se pueda seguir la ejecución del mismo.
        ctllog = new bitacora();
        ctllog->add
        (LOG_SEG, 1,"MaiMai003", "---- Iniciacion del programa ----");

        /// Cargamos el SplashScreen de BulmaCont
        Splash *splashScr = new Splash();
        delete splashScr;

        /// Miramos en los parametros pasados al programa por si ya viene indicada la empresa y no hay que mostrar selector
        if (argc == 5) {
            confpr->setValor(CONF_LOGIN_USER, us);
            confpr->setValor(CONF_PASSWORD_USER, pass);
            bges = new Bulmages01(NULL, "bulmages",0, db);
        } else if (argc == 3) {
            QString db= argv[2];
            bges = new Bulmages01(NULL, "bulmages",0, db);
        } else {
            logpass *login1 = new logpass(0,"");
            if (!login1->authOK())
                login1->exec();
            if (!login1->authOK())
                exit(1);
            delete login1;
            bges = new Bulmages01(NULL, "bulmages",0, "");
        }// end if

        /// Leemos la configuración específica de la base de datos que se ha abierto.
        QString confesp = CONFGLOBAL+bges->empresaactual()->nameDB()+".conf";
        confpr->leeconfig(confesp);

        /// cargamos las librerias de g_plugins
        g_plugins->cargaLibs(confpr->valor(CONF_PLUGINS_BULMACONT));

        g_plugins->lanza("entryPoint", bges);

        theApp->setMainWidget(bges);
        g_main = bges;
        valorsalida = theApp->exec();


    } catch(...) {
        mensajeInfo( "Error inesperado en BulmaCont, el programa se cerrará");
    } // end try

    /// Liberamos memoria.
    delete confpr;
    delete theApp;
    delete traductor;

    return valorsalida;
}
