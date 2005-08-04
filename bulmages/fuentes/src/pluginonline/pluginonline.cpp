#include <stdio.h>
#include "pluginonline.h"
#include "empresa.h"

#include <qpopupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>


myplugin:: myplugin() {}
myplugin::~myplugin() {}


void myplugin::elslot() {
   char *args[]={"http://www.conetxia.com/bulmages/manualindex.php?version=0.5.1","http://www.conetxia.com/bulmages/manualindex.php?version=0.5.1",NULL};
   int pid;
   int error;
#ifndef WIN32
   if ((pid=fork()) < 0) {
       perror ("Fork failed");
       exit(errno);
   }// end if
   if (!pid) {
      error = execvp(confpr->valor(CONF_NAVEGADOR).ascii(),args);
   }// end if
#endif
}// end elslot


void myplugin::inicializa(Bulmages01 *bges) {
    //El menu de empresa
    QAction *planCuentas = new QAction("Ayuda On-Line", "&Ayuda On-Line", 0, 0);
    planCuentas->setStatusTip("Ayuda a trav� de Internet");
    planCuentas->setWhatsThis("Ayuda a trav� de Internet");
    bges->pAyudaMenu()->insertSeparator();
    planCuentas->addTo(bges->pAyudaMenu());
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}// end inicializa


void entryPoint(Bulmages01 *bges) {
    fprintf(stderr,"Estoy dentro del plugin\n");
    myplugin *plug= new myplugin( );
    plug->inicializa(bges);
    bges->setCaption("Prueba de plugin.");
}


