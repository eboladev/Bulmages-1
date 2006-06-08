#include "myplugin.h"
#include "rutacomerciallist.h"
#include "listzonacomercialview.h"



mypluginbf::mypluginbf() : QObject() {}
mypluginbf::~mypluginbf() {}

void mypluginbf::elslot() {
    RutaComercialList *list  = new RutaComercialList(m_companyact, NULL);
    m_companyact->m_pWorkspace->addWindow(list);
    list->show();
}

void mypluginbf::elslot1() {
    ListZonaComercialView *list  = new ListZonaComercialView(m_companyact,NULL, NULL);
    list->exec();
}


void mypluginbf::inicializa(bulmafact *bges) {

    //El menu de empresa
    m_bges = bges;
    // cogemos la company para tener acceso a todo y sobretodo a BD
    m_companyact = bges->getcompany();

    // insertamos un separador en el menu.
    bges->menuMaestro->addSeparator();

    // Cremaos la opcion para Zonas Comerciales que dispare al metodo adecuado
    QAction *planCuentas1 = new QAction("&Zonas Comerciales", 0);
    planCuentas1->setStatusTip("Zonas Comerciales");
    planCuentas1->setWhatsThis("Zonas Comerciales");
    planCuentas1->addTo(bges->menuMaestro);
    connect(planCuentas1, SIGNAL(activated()), this, SLOT(elslot1()));

    // Cremaos la opcion para Rutas Comerciales que dispare al metodo adecuado
    QAction *planCuentas = new QAction("&Rutas Comerciales", 0);
    planCuentas->setStatusTip("Rutas Comerciales");
    planCuentas->setWhatsThis("Rutas Comerciales");
    planCuentas->addTo(bges->menuMaestro);
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));

}


