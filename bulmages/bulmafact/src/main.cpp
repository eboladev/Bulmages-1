#include <qapplication.h>
#include <qtranslator.h>
#include "configuracion.h"
#include "bulmafact.h"
#include "splashscreen.h"

#include "qtextcodec.h"
#include "logpass.h"

#include "fixed.h"

QApplication *theApp;
bulmafact *bges;
QTranslator * traductor;

int main( int argc, char ** argv ) {
    /// Definimos la codificación a Unicode.
    // Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();
    theApp = new QApplication( argc, argv );
    QString db= argv[2];
    QString us=argv[3];
    QString pass=argv[4];
    Splash *splashScr = new Splash();
    delete splashScr;

    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8"));

    theApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).ascii(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).ascii())));

    traductor = new QTranslator ( 0 );
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load( QString("bulmalib_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
    } else {
        QString archivo = "bulmalib_"+confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo,confpr->valor(CONF_DIR_TRADUCCION).ascii());
    }// end if
    theApp->installTranslator( traductor );

    traductor = new QTranslator ( 0 );
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load( QString("bulmafact_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
    } else {
        QString archivo = "bulmafact_"+confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo.ascii(),confpr->valor(CONF_DIR_TRADUCCION).ascii());
    }// end if
    theApp->installTranslator( traductor );


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

    theApp->setMainWidget(bges);
    theApp->exec();
    delete bges;
    delete confpr;
    return 1;
}// end main
