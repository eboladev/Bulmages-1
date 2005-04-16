#include <stdio.h>
#include "pluginestadisticas.h"
#include "empresa.h"
#include "resmensualview.h"
#include "estadisticasview.h"

#include "estadisticas.xpm"

#include <qpopupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>


myplugin:: myplugin() {}
myplugin::~myplugin() {}


void myplugin::SegCuentas() {
// Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
   resmensualview *rmen = new resmensualview(empresaactual,0,0);
   rmen->inicializa();
   rmen->exec();
   delete rmen;   
}// end SegCuentas

void myplugin::BalanceGrafico() {
   // Aprovechamos para empezar a trabajar con estadisticas a ver que tal van.
   estadisticasview *est = new estadisticasview(0,0);
   est->inicializa(conexionbase);
   est->exec();
   delete est;   
}// end BalanceGrafico

void myplugin::inicializa(Bulmages01 *bges) {

  QPixmap *estadisticasIcon = new QPixmap(estadisticas); 
   
    empresaactual = &(bges->empresaactual);
    conexionbase = empresaactual->bdempresa();
    //El menu de empresa
    QPopupMenu *pPluginMenu = new QPopupMenu();
    pPluginMenu->setCheckable(true);
    bges->menuBar()->insertItem("&Estadisticas",pPluginMenu);
    QAction *planCuentas = new QAction("Resumen Mensual", *estadisticasIcon, "&Resumen Mensual", 0, 0);
    planCuentas->setStatusTip("Resumen Mensual");
    planCuentas->setWhatsThis("Resumen Mensual");
    planCuentas->addTo(pPluginMenu);

    connect(planCuentas, SIGNAL(activated()), this, SLOT(SegCuentas()));

    QAction *planCuentas1 = new QAction("Balance Gráfico", "&Balance Gráfico", 0, 0);
    planCuentas1->setStatusTip("Balance Gráfico");
    planCuentas1->setWhatsThis("Balance Gráfico");
    planCuentas1->addTo(pPluginMenu);
    
    connect(planCuentas1, SIGNAL(activated()), this, SLOT(BalanceGrafico()));
}// end inicializa


void entryPoint(Bulmages01 *bges) {
    fprintf(stderr,"Estoy dentro del plugin\n");
    myplugin *plug= new myplugin( );
    plug->inicializa(bges);
    bges->setCaption("Prueba de plugin.");

}


