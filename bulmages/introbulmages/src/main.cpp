#include <qapplication.h>
#include "bselector.h"
#include "configuracion.h"
#include "postgresiface2.h"

QApplication *theApp;

int main( int argc, char ** argv ) {
    confpr = new configuracion();
    QApplication a( argc, argv );
   
    BSelector *bw = new BSelector();
    theApp = &a;
    bw->setCaption( "BulmaGés Selector" );
    bw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}// end main
