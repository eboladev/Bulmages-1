#include <stdio.h>
#include "pluginestadisticas.h"
#include "empresa.h"
#include "estadisticasview.h"

#include "estadisticas.xpm"

#include "postgresiface2.h"

#include <qobject.h>



void entryPoint(Bulmages01 *bges) {
    fprintf(stderr,"Estoy dentro del plugin\n");

    QPixmap *estadisticasIcon = new QPixmap(estadisticas);
    myplugin *plug= new myplugin();

    plug->empresaactual = &(bges->empresaactual);
    plug->conexionbase = plug->empresaactual->bdempresa();
    //El menu de empresa
    QPopupMenu *pPluginMenu = new QPopupMenu();
    pPluginMenu->setCheckable(true);
    bges->menuBar()->insertItem("&PyChart",pPluginMenu,-1,3);

    QAction *planCuentas1 = new QAction("Balance en Tarta", "&Balance Tarta", 0, 0);
    planCuentas1->setStatusTip("Balance en Tarta");
    planCuentas1->setWhatsThis("Balance en Tarta");
    planCuentas1->addTo(pPluginMenu);
    QObject::connect(planCuentas1, SIGNAL(activated()), plug, SLOT(BalanceGrafico()));

    QAction *planCuentas2 = new QAction("Balance en Barras", "&Balance Barras", 0, 0);
    planCuentas2->setStatusTip("Balance Barras");
    planCuentas2->setWhatsThis("Balance Barras");
    planCuentas2->addTo(pPluginMenu);
    QObject::connect(planCuentas2, SIGNAL(activated()), plug, SLOT(BalanceBarras()));

}


