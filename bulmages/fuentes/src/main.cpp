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

#include "qtextcodec.h"


QApplication * theApp;
QTranslator * traductor;

int main(int argc, char *argv[]) {
   int valorsalida=0;
   // Leemos la configuracion que luego podremos usar siempre
  confpr = new configuracion();

  QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8"));
    
  
  QApplication * mainApp = new QApplication (argc, argv);
  theApp = mainApp;
  //mainApp->setFont(QFont("helvetica", 11));
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
  
  Splash *splashScr = new Splash();
  delete splashScr;
  
  BSelector * SelectorAplicaciones = new BSelector(0,"SelectorAplicaciones");
  mainApp->setMainWidget(SelectorAplicaciones);
  
  valorsalida = mainApp->exec();
  
  delete confpr;
  return valorsalida;
}
