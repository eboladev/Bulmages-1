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

#include <unistd.h>

#include "bselector.h"
#include "splashscreen.h"
#include "configuracion.h"

QApplication * theApp;

int main(int argc, char *argv[]) {
   int valorsalida=0;
   // Leemos la configuracion que luego podremos usar siempre
  confpr = new configuracion();

  QApplication * mainApp = new QApplication (argc, argv);
  theApp = mainApp;
  mainApp->setFont(QFont("helvetica", 11));
  QTranslator tor( 0 );
  // set the location where your .qm files are in load() below as the last parameter instead of "."
  // for development, use "/" to use the english original as
  // .qm files are stored in the base project directory.
if (confpr->valor(CONF_TRADUCCION) == "locales") {
  tor.load( QString("bulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).c_str() );
} else {
   string archivo = "bulmages_"+confpr->valor(CONF_TRADUCCION);
   tor.load(archivo.c_str(),confpr->valor(CONF_DIR_TRADUCCION).c_str());
}// end if
  mainApp->installTranslator( &tor );
  
  Splash *splashScr = new Splash();
  delete splashScr;
  
  BSelector * SelectorAplicaciones = new BSelector(0,"SelectorAplicaciones");
  mainApp->setMainWidget(SelectorAplicaciones);
  
  valorsalida = mainApp->exec();
  
  delete confpr;
  return valorsalida;
}
