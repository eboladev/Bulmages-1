#include <qapplication.h>
#include "configuracion.h"
#include "bulmafact.h"
#include "splashscreen.h"

#include "qtextcodec.h"
#include "logpass.h"

QApplication *theApp;
bulmafact * mainClass;

int main( int argc, char ** argv ) {
    // Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();
    theApp = new QApplication( argc, argv );  
    QString db= argv[2];
    QString us=argv[3];
    QString pass=argv[4];
    Splash *splashScr = new Splash();
    delete splashScr;
    bulmafact *bges;
    if (argc == 5) {
        confpr->setValor(CONF_LOGIN_USER, us);
        confpr->setValor(CONF_PASSWORD_USER, pass);
        bges = new bulmafact( db);
    } else if (argc == 3) {
        bges = new bulmafact( db);
    } else {
        logpass *login1 = new logpass(0,"");
        if (!login1->authOK())
            login1->exec();
        bges = new bulmafact("");
    }// end if    
    
    
//    mainClass = new bulmafact();
    bges->setCaption( "BulmaFact" );
//    bges->show();
    theApp->connect( theApp, SIGNAL(lastWindowClosed()), theApp, SLOT(quit()) );
    return theApp->exec();
}// end main
