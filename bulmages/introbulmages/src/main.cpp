#include <qapplication.h>
#include "bselector.h"
#include "configuracion.h"


QApplication *theApp;

int main( int argc, char ** argv ) {

    confpr = new configuracion();
    
    QApplication a( argc, argv );
    BSelector *bw = new BSelector();
    theApp = &a;
    bw->setCaption( "introbulmages" );
    bw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
