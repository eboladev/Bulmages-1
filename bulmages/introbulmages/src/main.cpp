#include <qapplication.h>
#include "configuracion.h"
#include "logpass.h"
#include "postgresiface2.h"

QApplication *theApp;

int main( int argc, char ** argv ) {
    bool authOK;

    confpr = new configuracion();
    QApplication a( argc, argv );
   
    logpass *login1 = new logpass(0,"");
    login1->show();
    
    a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
    return a.exec();   //QLabel *lblAuthError;

    
}// end main
