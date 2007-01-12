
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

#include <stdlib.h>

#include <QApplication>
#include <QFont>
#include <QString>
#include <QTextCodec>
#include <QTranslator>
#include <QLibrary>
#include <QLocale>

#ifndef WIN32
#include <unistd.h>
#endif

#include "bulmacont.h"
#include "splashscreen.h"
#include "configuracion.h"
#include "qtextcodec.h"
#include "logpass.h"
#include "plugins.h"
#include "qapplication2.h"

#ifdef WIN32
#define CONFGLOBAL "C:\\bulmages_"
#else
#define CONFGLOBAL "/etc/bulmages_"
#endif


/// Estas son las variables globales de la aplicaci&oacute;n.
/// El puntero de la aplicaci&oacute;n.
QApplication2 *theApp;
/// El traductor.
QTranslator *traductor;
/// Faltan el configurador de par&aacute;metros confpr y el sistema de log ctlog.


/// Los datos de ejecuci&oacute;n del programa son sencillos.
/** La ejecuci&oacute;n primero crea e inicializa los objetos configuraci&oacute;n,
    idioma, splash, etc.
    Luego intenta entrar en el sistema de base de datos.
    Y por &uacute;ltimo crea el objeto del tipo \ref Bulmacont que es la aplicaci&oacute;n
    de ventanas. */
int main(int argc, char *argv[]) {
    /// Leemos la configuraci&oacute;n que luego podremos usar siempre.
    confpr = new configuracion("bulmacont");
    Bulmacont *bges;
    int valorsalida = 0;
    QString db = argv[2];
    QString us = argv[3];
    QString pass = argv[4];
    try {
        /// Inicializamos el objeto global para uso de plugins.
        g_plugins = new Plugins();
        /// Definimos la codificaci&oacute;n a Unicode.
        QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));
        QTextCodec::setCodecForLocale(QTextCodec::codecForName("CP1252"));
        /// Creamos la aplicaci&oacute;n principal.
        theApp = new QApplication2(argc, argv);
        theApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).toAscii(), atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).toAscii())));
        /// Cargamos las primeras traducciones para bulmalib y para bulmacont.
        traductor = new QTranslator(0);
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load(QString("bulmalib_") + QLocale::system().name(),
                            confpr->valor(CONF_DIR_TRADUCCION).toAscii());
        } else {
            QString archivo = "bulmalib_" + confpr->valor(CONF_TRADUCCION);
            traductor->load(archivo, confpr->valor(CONF_DIR_TRADUCCION).toAscii());
        } // end if
        theApp->installTranslator(traductor);

        traductor = new QTranslator(0);
        if (confpr->valor(CONF_TRADUCCION) == "locales") {
            traductor->load(QString("bulmages_") + QLocale::system().name(),
                            confpr->valor(CONF_DIR_TRADUCCION).toAscii());
        } else {
            QString archivo = "bulmages_" + confpr->valor(CONF_TRADUCCION);
            traductor->load(archivo.toAscii(), confpr->valor(CONF_DIR_TRADUCCION).toAscii());
        } // end if
        theApp->installTranslator(traductor);

        /// Cargamos el SplashScreen de BulmaCont.
        Splash *splashScr = new Splash();
        delete splashScr;

        /// Miramos en los par&aacute;metros pasados al programa por si ya viene
        /// indicada la empresa y no hay que mostrar selector.
        if (argc == 5) {
            confpr->setValor(CONF_LOGIN_USER, us);
            confpr->setValor(CONF_PASSWORD_USER, pass);
            bges = new Bulmacont(NULL, 0, db);
        } else if (argc == 3) {
            QString db= argv[2];
            bges = new Bulmacont(NULL, 0, db);
        } else {
            logpass *login1 = new logpass(0, "");
            if (!login1->authOK())
                login1->exec();
            if (!login1->authOK())
                exit(1);
            delete login1;
            bges = new Bulmacont(NULL, 0, "");
        } // end if

        /// Leemos la configuraci&oacute;n específica de la base de datos que se ha abierto.
        QString confesp = CONFGLOBAL + bges->empresaactual()->nameDB() + ".conf";
        confpr->leeconfig(confesp);

        /// Cargamos las librerias de g_plugins.
        g_plugins->cargaLibs(confpr->valor(CONF_PLUGINS_BULMACONT));
        g_plugins->lanza("entryPoint", bges);

        g_main = bges;
        valorsalida = theApp->exec();
    } catch (...) {
        mensajeInfo("Error inesperado en BulmaCont, el programa se cerrara.");
    } // end try

    /// Liberamos memoria.
    delete confpr;
    delete theApp;
    delete traductor;

    return valorsalida;
}

