#include <qapplication.h>
#include "configuracion.h"
#include "bulmafact.h"
#include "splashscreen.h"

QApplication *theApp;
bulmafact * mainClass;

int main( int argc, char ** argv ) {
    // Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();
    theApp = new QApplication( argc, argv );
    
  Splash *splashScr = new Splash();
  delete splashScr;
  
    mainClass = new bulmafact();
    mainClass->setCaption( "bulmafact" );
    mainClass->show();
    theApp->connect( theApp, SIGNAL(lastWindowClosed()), theApp, SLOT(quit()) );
    return theApp->exec();
}// end main
