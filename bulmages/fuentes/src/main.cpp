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


/// Estas son las variables globales de la aplicación.
/// El puntero de la aplicación
QApplication * theApp;
/// El traductor.
QTranslator * traductor;
/// Faltan el configurador de parametros confpr y el sistema de log ctlog

    typedef void (*MyPrototype)(Bulmages01 *);
    MyPrototype myFunction;
    
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
    /// Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();

    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("latin1"));

    //    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("latin1"));
    QApplication * mainApp = new QApplication (argc, argv);
    theApp = mainApp;
    mainApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).ascii(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).ascii())));

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
    ctllog->add(LOG_SEG, 1,"MaiMai003", "---- Iniciación del programa ----");


    Splash *splashScr = new Splash();
    delete splashScr;

    if (argc == 5) {
        confpr->setValor(CONF_LOGIN_USER, us);
        confpr->setValor(CONF_PASSWORD_USER, pass);
        bges = new Bulmages01(NULL, "bulmages",0, &db);
    } else if (argc == 3) {
        QString db= argv[2];
        bges = new Bulmages01(NULL, "bulmages",0, &db);
    } else {
        logpass *login1 = new logpass(0,"");
        if (!login1->authOK())
            login1->exec();
        bges = new Bulmages01(NULL, "bulmages",0, NULL);
    }// end if




    /// Hacemos la carga de los plugins.
    QString cad= confpr->valor(CONF_PLUGINS_BULMACONT);
    fprintf(stderr,"Carga de plugins: %s\n",cad.ascii());
    QStringList plugins = QStringList::split( ";", cad );
    for ( QStringList::Iterator it = plugins.begin(); it != plugins.end(); ++it ) {
	QLibrary *lib= new QLibrary(*it);
	if (!lib->load()) {
		fprintf(stderr,"No se ha podido cargar la libreria\n");
	} else {
		fprintf(stderr,"Resolviendo la libreria: %s\n", (*it).ascii());
		myFunction = (MyPrototype) lib->resolve( "entryPoint" );
		if ( myFunction ) {
			myFunction(bges);
		} else {
			fprintf(stderr,"No ha entrado la libreria\n");
		}// end if    
	}// end if
    }// end for

    mainApp->setMainWidget(bges);
    valorsalida = mainApp->exec();
    delete confpr;
    delete bges;
    return valorsalida;
}// end main
