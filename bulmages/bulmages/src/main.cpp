/***************************************************************************
 *   Copyright (C) 2003 by Tomeu Borras Riera                              *
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

#include "comun/bselector.h"
#include "qapplication2.h"
#include "configuracion.h"
#include "logpass.h"
#include "postgresiface2.h"
#include "funcaux.h"


/// Instancia de la aplicacion. Usada en algunos casos para acceder a determinadas
/// funcionalidades como la traduccion.
QApplication2 *theApp;
/// Instancia de la traduccion que se carga en tiempo de ejecucion y que se usa
/// de forma global.
QTranslator *traductor;


/// Crea el objeto base y lo lanza, tambien comprueba si se ha lanzado la
/// aplicacion con parametros o sin ellos.
int main(int argc, char **argv) {
    confpr = new configuracion("bulmages");
    QApplication2 a(argc, argv);
    theApp = &a;

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("latin1"));
    theApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).ascii(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).ascii())));

    /// Set the location where your .qm files are in load() below as the last parameter
    /// instead of "." for development, use "/" to use the english original as
    /// .qm files are stored in the base project directory.
    traductor = new QTranslator(0);
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load(QString("bulmalib_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii());
    } else {
        QString archivo = "bulmalib_" + confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo, confpr->valor(CONF_DIR_TRADUCCION).ascii());
    } // end if
    a.installTranslator(traductor);

    traductor = new QTranslator(0);
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load(QString("ibulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii());
    } else {
        QString archivo = "ibulmages_" + confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo.ascii(), confpr->valor(CONF_DIR_TRADUCCION).ascii());
    } // end if
    a.installTranslator(traductor);

    BSelector *bw = new BSelector();
    bw->setCaption(theApp->translate("main", "Selector de BulmaGes"));
    a.setMainWidget(bw);

    /// Lo primero que hacemos es comprobar el sistema de autentificacion de
    /// Postgres para pedir un 'login' y un 'password' en caso de que sea necesario para
    /// entrar en el sistema.
    logpass *login1 = new logpass(0, "");
    if (!login1->authOK()) {
        login1->exec();
    } // end if
    if (!login1->authOK()) {
        exit(1);
    } // end if
    delete login1;

    /// Una vez que hemos pasado por el sistema de 'login' y 'password' y ya somos
    /// autenticos podemos mostrar el selector de acciones.
    bw->show();
    return a.exec();
}

