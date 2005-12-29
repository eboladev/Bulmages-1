#include <qapplication.h>
#include "bulmatpv.h"
#include "tpvview.h"


int main( int argc, char ** argv ) {
    QApplication a( argc, argv );
/*
    BulmaTPV * mw = new BulmaTPV();
    mw->setCaption( "BulmaTPV" );
    mw->show();
*/
   tpvview *mw = new tpvview();
   mw->setCaption ( "BulmaTPV");
   mw->show();
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();
}
