/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>

#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>
#include <QTextCodec>
#include <QLocale>

#include "bfcompany.h"
#include "blfunctions.h"
#include "pluginbf_comercial.h"
#include "comercialclienteview.h"
#include "rutacomerciallist.h"
#include "listzonacomercialview.h"

BfBulmaFact *g_bges;

///
/**
**/
PluginBf_Comercial::PluginBf_Comercial() : QObject()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
PluginBf_Comercial::~PluginBf_Comercial()
{
    blDebug ( Q_FUNC_INFO, 0 );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void PluginBf_Comercial::elslot()
{
    blDebug ( Q_FUNC_INFO, 0 );
    RutaComercialList *list  = new RutaComercialList ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( list );
    list->show();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
**/
void PluginBf_Comercial::elslot1()
{
    blDebug ( Q_FUNC_INFO, 0 );
    ListZonaComercialView *list = new ListZonaComercialView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( list );
    list->show();
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}


///
/**
\param bges
**/
void PluginBf_Comercial::inicializa ( BfBulmaFact *bges )
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// El men&uacute; de empresa.
    m_bges = bges;
    /// Cogemos la 'company' para tener acceso a todo y sobretodo a BD.
    setMainCompany ( bges->company() );
    /// Insertamos un separador en el men&uacute;.
    bges->menuMaestro->addSeparator();
    /// Creamos la opci&oacute;n para Zonas Comerciales que dispare el m&eacute;todo adecuado.
    QAction *planCuentas1 = new QAction ( _ ( "&Zonas comerciales" ), 0 );
    planCuentas1->setStatusTip ( _ ( "Zonas comerciales" ) );
    planCuentas1->setWhatsThis ( _ ( "Zonas comerciales" ) );
    bges->menuMaestro->addAction ( planCuentas1 );
    connect ( planCuentas1, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );
    /// Creamos la opci&oacute;n para Rutas Comerciales que dispare el m&eacute;todo adecuado.
    QAction *planCuentas = new QAction ( _ ( "&Rutas comerciales" ), 0 );
    planCuentas->setStatusTip ( _ ( "Rutas comerciales" ) );
    planCuentas->setWhatsThis ( _ ( "Rutas comerciales" ) );
    bges->menuMaestro->addAction ( planCuentas );
    connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
}



///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( Q_FUNC_INFO, 0 );

    g_bges = bges;
    
    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Comercial", "0.5.9");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_comercial", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );


    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "rutacomercial", "SELECT" ) ) {
      PluginBf_Comercial *plug = new PluginBf_Comercial();
      plug->inicializa ( g_bges );
    }// end if
    return 0;
}


///
/**
\param cli
\return
**/
int ClienteView_ClienteView_Post ( ClienteView *cli )
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Agregamos los campos de registro que hacen falta.
    cli->addDbField ( "idzonacomercial", BlDbField::DbInt, BlDbField::DbNothing, _ ( "Zona comercial" ) );
    cli->addDbField ( "fechabasecomercialcliente", BlDbField::DbDate, BlDbField::DbNothing, _ ( "Fecha base" ) );
    cli->addDbField ( "periodocomercialcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Periodo" ) );
    cli->addDbField ( "comentcomercialcliente", BlDbField::DbVarChar, BlDbField::DbNothing, _ ( "Comentarios para el comercial" ) );
    /// Creamos la pesta&ntilde;a de comercial y la tratamos.
    ComercialClienteView *l = new ComercialClienteView ( cli );
    l->setMainCompany ( cli->mainCompany() );
    l->setObjectName ( QString::fromUtf8 ( "lcomercial" ) );
    cli->mui_tab->addTab ( l, _ ( "&Comercial" ) );
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}


///
/**
\param cli
\return
**/
int ClienteView_Des_ClienteView_Post ( ClienteView *cli )
{
    blDebug ( Q_FUNC_INFO, 0 );
    /// Creamos la pesta&ntilde;a de comercial y la tratamos.
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *> ( "lcomercial" );
    delete bus;
    blDebug ( ("END ", Q_FUNC_INFO), 0 );
    return 0;
}

/*
int ClienteView_saveClient(ClienteView *cli) {
    blDebug ( Q_FUNC_INFO, 0 );
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *>("lcomercial");
    cli->setDbValue("idzonacomercial", bus->mui_zonacomercial->idzonacomercial());
    cli->setDbValue("fechabasecomercialcliente", bus->mui_fechabasecomercialcliente->fecha());
    cli->setDbValue("periodocomercialcliente", bus->mui_periodocomercialcliente->periodo());
    cli->setDbValue("comentcomercialcliente", bus->mui_comentcomercialcliente->toPlainText());

    blDebug("END ClienteView_saveClient", 0);
    return 0;
}


int Cliente_pintaCliente(Cliente *cli) {
    blDebug ( Q_FUNC_INFO, 0 );
    ClienteView *clo = (ClienteView *) cli;
    ComercialClienteView *bus = clo->findChild<ComercialClienteView *>("lcomercial");
    bus->mui_zonacomercial->setidzonacomercial(clo->dbValue("idzonacomercial"));
    bus->mui_fechabasecomercialcliente->setDate(clo->dbValue("fechabasecomercialcliente"));
    bus->mui_periodocomercialcliente->setperiodo(clo->dbValue("periodocomercialcliente"));
    bus->mui_comentcomercialcliente->setPlainText(clo->dbValue("comentcomercialcliente"));
    blDebug("END Cliente_pintaCliente", 0);
    return 0;
}
*/
