/***************************************************************************
                          main.cpp  -  description
                             -------------------
    begin                : Julio 2004
    copyright            : (C) 2004 by Josep Burción
    email                : josep@burcion.com
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//ruta a la libreria bulmafact para las versiones liberadas.
//#define libFileName "/usr/local/lib/bulmages/libbulmafact.so"

//ruta a la libreria bulmafact para la versión en desarrolo.
#define libFileName "./lib/libbulmafact.so"

#include <qapplication.h>
#include <qworkspace.h>
#include <qmainwindow.h>
#include <qlibrary.h>


typedef QMainWindow* (*BgFuncPtr)(QWorkspace *, int);


int main(int argc, char *argv[])
{
  BgFuncPtr FuncRef;
  int valorsalida=0;
  QMainWindow *BulmaFact=0;
  QApplication * mainApp;
  mainApp = new QApplication (argc, argv);
    
 
  QLibrary ptr(libFileName);
  if (! ptr.load()) {
      fprintf(stderr,"Error al cargar la libreria: " libFileName  "\n");
      valorsalida++;
  }
  else {
      FuncRef = (BgFuncPtr) ptr.resolve("enlace");
      if (FuncRef) BulmaFact=FuncRef(0,0);
      else { 
          fprintf(stderr,"Error al ejecutar el enlace.\n"); 
          valorsalida++;
      }
  }
  
  if (!valorsalida) {
      mainApp->setMainWidget(BulmaFact);
      BulmaFact->showMaximized();
      valorsalida = mainApp->exec();
  }

  return valorsalida;
}
