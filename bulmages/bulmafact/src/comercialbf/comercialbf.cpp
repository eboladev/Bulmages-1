#include <stdio.h>
#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "company.h"
#include "funcaux.h"
#include "myplugin.h"
#include "comercialbf.h"
#include "comercialclienteview.h"


int entryPoint(bulmafact *bges) {
    _depura("Punto de Entrada del plugin ComercialBF", 0);
    mypluginbf *plug= new mypluginbf( );
    plug->inicializa(bges);
    _depura("END Punto de Entrada del plugin ComercialBF", 0);
    return 0;
}

int ClienteView_ClienteView_Post(ClienteView *cli) {
    _depura("ClienteView_ClienteView_Post",0);
	/// Agregamos los campos de registro que hacen falta.
    cli->addDBCampo("idzonacomercial", DBCampo::DBint, DBCampo::DBNothing, "Zona Comercial");
    cli->addDBCampo("fechabasecomercialcliente", DBCampo::DBdate, DBCampo::DBNothing, "Fecha Base");
    cli->addDBCampo("periodocomercialcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Periodo");
    cli->addDBCampo("comentcomercialcliente", DBCampo::DBvarchar, DBCampo::DBNothing, "Comentarios para Comercial");


	/// Creamos la pestaña de comercial y la tratamos.
    ComercialClienteView *l = new ComercialClienteView(cli);
    l->setcompany(cli->companyact());
    l->setObjectName(QString::fromUtf8("lcomercial"));
    cli->mui_tab->addTab(l, "&Comercial");
    cli->mui_tab->removeTab(8);
    cli->mui_tab->removeTab(7);
    cli->mui_tab->removeTab(6);
    _depura("END ClienteView_ClienteView_Post",0);
    return 0;
}


int ClienteView_Des_ClienteView_Post(ClienteView *cli) {
    _depura("ClienteView_Des_ClienteView",0);
	/// Creamos la pestaña de comercial y la tratamos.
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *>("lcomercial");
	delete bus;
    _depura("END ClienteView_Des_ClienteView",0);
    return 0;
}


int ClienteView_saveClient(ClienteView *cli) {
    _depura("ClienteView_saveClient",0);
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *>("lcomercial");
    cli->setDBvalue("idzonacomercial",bus->mui_zonacomercial->idzonacomercial());
    cli->setDBvalue("fechabasecomercialcliente",bus->mui_fechabasecomercialcliente->fecha());
    cli->setDBvalue("periodocomercialcliente",bus->mui_periodocomercialcliente->periodo());
    cli->setDBvalue("comentcomercialcliente",bus->mui_comentcomercialcliente->text());

    _depura("END ClienteView_saveClient",0);
    return 0;
}



int Cliente_pintaCliente(Cliente *cli) {
    _depura("Cliente_pintaCliente",0);
    ClienteView *clo = (ClienteView *) cli;
    ComercialClienteView *bus = clo->findChild<ComercialClienteView *>("lcomercial");
    bus->mui_zonacomercial->setidzonacomercial( clo->DBvalue("idzonacomercial"));
    bus->mui_fechabasecomercialcliente->setfecha(clo->DBvalue("fechabasecomercialcliente"));
    bus->mui_periodocomercialcliente->setperiodo(clo->DBvalue("periodocomercialcliente"));
    bus->mui_comentcomercialcliente->setText(clo->DBvalue("comentcomercialcliente"));
    _depura("END Cliente_pintaCliente",0);
    return 0;
}
 
