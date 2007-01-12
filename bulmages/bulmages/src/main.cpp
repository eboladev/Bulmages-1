#include "comun/bselector.h"


#include "qapplication2.h"
//Added by qt3to4:
#include <QTranslator>
#include "configuracion.h"
#include "logpass.h"
#include "postgresiface2.h"
#include <qtextcodec.h>
#include "funcaux.h"


/// Instancia de la aplicacion. Usada en algunos casos para acceder a determinadas funcionalidades como la traducci�.
QApplication2 *theApp;
/// Instancia de la traduccion que se carga en tiempo de ejecuci� y que se usa de forma global.
QTranslator * traductor;
/** \author Tomeu Borras Riera
  * \fn main
  * \brief Funcion principal del programa BulmaGes
  *
  * Crea el objeto base y lo lanza, tambien comprueba si se ha lanzado la
  * aplicaci� con parametros o sin ellos
  */


int main( int argc, char ** argv ) {
    confpr = new configuracion("bulmages");
    QApplication2 a( argc, argv );
    theApp = &a;



    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("latin1"));
    theApp->setFont(QFont(confpr->valor(CONF_FONTFAMILY_BULMAGES).ascii(),atoi(confpr->valor(CONF_FONTSIZE_BULMAGES).ascii())));

    // set the location where your .qm files are in load() below as the last parameter instead of "."
    // for development, use "/" to use the english original as
    // .qm files are stored in the base project directory.
    traductor = new QTranslator ( 0 );
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load( QString("bulmalib_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
    } else {
        QString archivo = "bulmalib_"+confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo,confpr->valor(CONF_DIR_TRADUCCION).ascii());
    }// end if
    a.installTranslator( traductor );

    traductor = new QTranslator ( 0 );
    if (confpr->valor(CONF_TRADUCCION) == "locales") {
        traductor->load( QString("ibulmages_") + QTextCodec::locale(), confpr->valor(CONF_DIR_TRADUCCION).ascii() );
    } else {
        QString archivo = "ibulmages_"+confpr->valor(CONF_TRADUCCION);
        traductor->load(archivo.ascii(),confpr->valor(CONF_DIR_TRADUCCION).ascii());
    }// end if
    a.installTranslator( traductor );




    BSelector *bw = new BSelector();
    bw->setCaption( theApp->translate("main","BulmaGes Selector" ));
    a.setMainWidget(bw);

    /// Lo primero que hacemos es comprobar el sistema de autentificaci� de Postgres para pedir un loggin y un password en caso de que sea necesario para entrar en el sistema.
    logpass *login1 = new logpass(0,"");
    if (!login1->authOK())
        login1->exec();
    if (!login1->authOK())
        exit(1);
delete login1;

    /// Una vez que hemos pasado por el sistema de loggin y password y ya somos autenticos podemos mostrar el selector de acciones.
    bw->show();
    return a.exec();
}// end main
