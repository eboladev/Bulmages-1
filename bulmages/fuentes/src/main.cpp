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

#ifndef WIN32
#include <unistd.h>
#endif

#include "bulmages01.h"
#include "splashscreen.h"
#include "configuracion.h"

#include "qtextcodec.h"
#include "log.h"
#include "logpass.h"

#ifdef REPORTS
#include "rtkinputbges.h"
using namespace RTK;
Input *InputBGesCreator(const String &name, const String &driver) {
    return new InputBGes(name, driver);
}

void registraInputBGes() {
    RTK::Report::registerInput("sqlbulmages", &InputBGesCreator);
}
#endif

/// Estas son las variables globales de la aplicación.
/// El puntero de la aplicación
QApplication * theApp;
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
    QString ej="";
    /// Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();

#ifdef REPORTS
    // Esta llamada que se hace aqui sólo hay que hacerla una vez al principio de la aplicacion
    registraInputBGes();
#endif

    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8"));
    QApplication * mainApp = new QApplication (argc, argv);
    theApp = mainApp;
    mainApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).c_str(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).c_str())));
    traductor = new QTranslator ( 0 );
    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load( QString("bulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).c_str() );
    } else {
        string archivo = "bulmages_"+confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
    }// end if
    mainApp->installTranslator( traductor );

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
        QString ej="";
        bges = new Bulmages01(NULL, "bulmages",0, &db);
    } else {
        logpass *login1 = new logpass(0,"");
        if (!login1->authOK())
            login1->exec();
        bges = new Bulmages01(NULL, "bulmages",0, NULL);
    }// end if
    mainApp->setMainWidget(bges);
    valorsalida = mainApp->exec();
    delete confpr;
    return valorsalida;
}// end main
