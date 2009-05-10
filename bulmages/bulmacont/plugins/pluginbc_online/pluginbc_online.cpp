#include <stdio.h>
#include "pluginbc_online.h"
#include "bccompany.h"

#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>


myplugin:: myplugin()
{}
myplugin::~myplugin()
{}


void myplugin::elslot()
{
    char * args[] = {"http://www.iglues.org/documentacion.php", "http://www.iglues.org/documentacion.php", NULL};
    int pid;
    int error;
#ifndef WIN32
    if ( ( pid = fork() ) < 0 ) {
        perror ( "Fork failed" );
        exit ( errno );
    }// end if
    if ( !pid ) {
        error = execvp ( g_confpr->valor ( CONF_NAVEGADOR ).ascii(), args );
    }// end if
#endif
}// end elslot

void myplugin::soporte()
{
    char * args[] = {"http://www.iglues.org/soporte.php", "http://www.iglues.org/soporte.php", NULL};
    int pid;
    int error;
#ifndef WIN32
    if ( ( pid = fork() ) < 0 ) {
        perror ( "Fork failed" );
        exit ( errno );
    }// end if
    if ( !pid ) {
        error = execvp ( g_confpr->valor ( CONF_NAVEGADOR ).ascii(), args );
    }// end if
#endif
}// end elslot


void myplugin::inicializa ( Bulmages01 *bges )
{
    //El menu de empresa
//    QAction *planCuentas = new QAction("Ayuda On-Line", "&Ayuda On-Line", 0, 0);
    QAction * planCuentas = new QAction ( "&Ayuda On-Line", 0 );
    planCuentas->setStatusTip ( "Ayuda a traves de Internet" );
    planCuentas->setWhatsThis ( "Ayuda a traves de Internet" );
    //El menu de empresa
//    QAction *eSoporte = new QAction("Soporte Profesional", "&Soporte Profesional", 0, 0);
    QAction *eSoporte = new QAction ( "&Soporte Profesional",  0 );
    eSoporte->setStatusTip ( "Encontrar empresas que dan soporte al programa" );
    eSoporte->setWhatsThis ( "Encontrar empresas que dan soporte al programa" );
    bges->pAyudaMenu() ->insertSeparator();
    planCuentas->addTo ( bges->pAyudaMenu() );
    eSoporte->addTo ( bges->pAyudaMenu() );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    connect ( eSoporte, SIGNAL ( activated() ), this, SLOT ( soporte() ) );
}// end inicializa


int entryPoint ( Bulmages01 *bges )
{
    fprintf ( stderr, "Estoy dentro del plugin\n" );
    myplugin *plug = new myplugin( );
    plug->inicializa ( bges );
    return 0;
}

// Q_EXPORT_PLUGIN(myplugin)


