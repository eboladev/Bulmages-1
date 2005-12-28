#include <qapplication.h>
#include <qtranslator.h>
#include "configuracion.h"
#include "plugins.h"

#include "bulmafact.h"
#include "splashscreen.h"

#include "qtextcodec.h"
#include "logpass.h"
#include "funcaux.h"

#include "fixed.h"

QApplication *theApp;
bulmafact *bges;
QTranslator * traductor;
Q3MainWindow *g_main=NULL;


int main( int argc, char ** argv ) {
    /// Leemos la configuracion que luego podremos usar siempre
    confpr = new configuracion();
    /// Preparamos el sistema de plugins.
    g_plugins = new Plugins();

    /// Iniciamos la clase QApplication para el uso de las Qt
    theApp = new QApplication( argc, argv );

    /// Definimos la codificacion a Unicode.
    //    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("utf8"));


    theApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).ascii(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).ascii())));


    /// Hacemos la carga de las librerias que contienen los plugins.
    g_plugins->cargaLibs(confpr->valor(CONF_PLUGINS_BULMAFACT));

    /// Cargamos el sistema de traducciones
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

    /// Cargamos el splashScreen
    Splash *splashScr = new Splash();
    delete splashScr;


    /// Leemos los argumentos pasados por la linea de comandos.
    QString db= argv[2];
    QString us=argv[3];
    QString pass=argv[4];

    /// Dependiendo de los argumentos pasados se lanza con unas opciones u otras para la
    /// conexion con la base de datos.
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
        /// Si la autentificaci� falla una segunda vez abortamos el programa.
        if (!login1->authOK())
            exit(1);
        delete login1;
        bges = new bulmafact("");
    }// end if

    g_main = bges;
    theApp->setMainWidget(bges);

    theApp->exec();

    _depura("Se acabo la ejecucion en main",1);

    // Liberamos memora, aunque no sea necesario ya que va a acabar el programa.
    delete theApp;
    delete traductor;
    delete confpr;
    return 0;
}// end main
