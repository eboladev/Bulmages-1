#include <qapplication.h>
#include "configuracion.h"
#include "bulmafact.h"

QApplication *theApp;

int main( int argc, char ** argv ) {
    // Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();
    theApp = new QApplication( argc, argv );
    bulmafact * mw = new bulmafact();
    mw->setCaption( "bulmafact" );
    mw->show();
    theApp->connect( theApp, SIGNAL(lastWindowClosed()), theApp, SLOT(quit()) );
    return theApp->exec();
}
