#include <stdio.h>
#include "pluginregistroiva.h"
#include "registroivaview.h"
#include "empresa.h"
#include "asiento1.h"

#include <q3popupmenu.h>
#include <qaction.h>
#include <qobject.h>
#include <qmessagebox.h>
#include <q3dockwindow.h>
#include <q3mainwindow.h>


//#include <qwidgetplugin.h>
#include <qstringlist.h>
#include <qwidget.h>
#include <qicon.h>
#include <qapplication.h>
#include <qobject.h>
#include <q3dockwindow.h>



myplugin:: myplugin() {}
myplugin::~myplugin() {}


void myplugin::elslot() {
    _depura("SE ha activado el slot\n",0);
	RegistroIvaView * reg = new RegistroIvaView(companyact,0,0);
	reg->exec();
	delete reg;
}// end elslot


void myplugin::inicializa(Bulmages01 *bges) {
    //El menu de empresa
	companyact = bges->empresaactual();
    Q3PopupMenu *pRIVAMenu = new Q3PopupMenu();
    pRIVAMenu->setCheckable(true);
    bges->menuBar()->insertItem("&RegistroIVA",pRIVAMenu);
    QAction *planCuentas = new QAction("&Prueba de Plugin", 0);
    planCuentas->setStatusTip("Muestra el plan contable");
    planCuentas->setWhatsThis("Muestra el plan contable");
    planCuentas->addTo(pRIVAMenu);

    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}// end inicializa





void entryPoint(Bulmages01 *bges) {
    _depura("Estoy dentro del plugin\n",0);
  // Vamos a probar con un docwindow.
    myplugin *plug= new myplugin( );
    plug->inicializa(bges);
    bges->setCaption("Prueba de plugin.");


/*	
    //El menu de empresa
  QAction  *viewCorrector = new QAction( "&RegistroIvaView", 0);
  QObject::connect(viewCorrector, SIGNAL(toggled(bool)), corr, SLOT(cambia(bool)));
  viewCorrector->addTo(bges->pVerMenu());
*/
   _depura("END entryPoint\n",0);
}



/**
  * Buscamos en el asiento si hay indicios de una factura y actuamos en consecuencia.
  */
int Asiento1_guardaAsiento1_post(Asiento1 *as) {
	_depura("Asiento1_guardaAsiento1_post",0);
	empresa *companyact = as->companyact();
    QString cuentas="";
    QString query = "SELECT valor FROM configuracion WHERE nombre='RegistroEmitida' OR nombre='RegistroSoportada'";
    cursor2 *curvalor = companyact->cargacursor(query);
    while (!curvalor->eof()) {
        cuentas += curvalor->valor("valor")+"%|"; // Preparamos una expresión regular para usar en la consulta
        curvalor->siguienteregistro();
    }// end while
    delete curvalor;
    cuentas.truncate(cuentas.length()-1); // Le quitamos el último '|' que nos sobra

    /// Recorremos la tabla en busca de entradas de factura no introducidas y las preguntamos antes de cerrar nada.
    QString SQLQuery = "SELECT bcontrapartidaborr(idborrador) AS contra FROM borrador LEFT JOIN cuenta ON borrador.idcuenta=cuenta.idcuenta WHERE idasiento="+as->DBvalue("idasiento")+" AND codigo SIMILAR TO '"+companyact->sanearCadena(cuentas.ascii())+"' GROUP BY contra";

    cursor2 *cursborr= companyact->cargacursor(SQLQuery);
    while (!cursborr->eof()) {
        int idborrador = cursborr->valor("contra").toInt();
	RegistroIvaView * reg = new RegistroIvaView(companyact,0,0);
	reg->inicializa1( idborrador);
	reg->exec();
	delete reg;
        cursborr->siguienteregistro();
    }// end while
    delete cursborr;
    return 0;
}// end buscaFactura

