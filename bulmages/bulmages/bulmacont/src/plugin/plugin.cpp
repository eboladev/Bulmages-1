#include <stdio.h>
#include "plugin.h"
#include "empresa.h"

#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>



myplugin:: myplugin() {}
myplugin::~myplugin() {}


void myplugin::elslot() {
    fprintf(stderr,"SE ha activado el slot\n");
    QMessageBox::warning( 0, "Guardar Familia",
		"Desea guardar los cambios.",
		QMessageBox::Ok ,
		QMessageBox::Cancel );
}// end elslot


void myplugin::inicializa(Bulmages01 *bges) {
    //El menu de empresa
    Q3PopupMenu *pPluginMenu = new Q3PopupMenu();
    pPluginMenu->setCheckable(true);
    bges->menuBar()->insertItem("&Plugin",pPluginMenu);
    QAction *planCuentas = new QAction("&Prueba de Plugin", 0);
    planCuentas->setStatusTip("Muestra el plan contable");
    planCuentas->setWhatsThis("Muestra el plan contable");
    planCuentas->addTo(pPluginMenu);

    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}// end inicializa


void entryPoint(Bulmages01 *bges) {
    fprintf(stderr,"Estoy dentro del plugin\n");
    myplugin *plug= new myplugin( );
    plug->inicializa(bges);
    bges->setCaption("Prueba de plugin.");
}


