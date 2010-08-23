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
#include "myplugin.h"
#include "pluginbf_comercial.h"
#include "comercialclienteview.h"


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "entryPoint", 0, "Punto de Entrada del plugin ComercialBF" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_comercial", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    mypluginbf *plug = new mypluginbf();
    plug->inicializa ( bges );
    blDebug ( "END entryPoint", 0, "Punto de Entrada del plugin ComercialBF" );
    return 0;
}


///
/**
\param cli
\return
**/
int ClienteView_ClienteView_Post ( ClienteView *cli )
{
    blDebug ( "ClienteView_ClienteView_Post", 0 );
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
    blDebug ( "END ClienteView_ClienteView_Post", 0 );
    return 0;
}


///
/**
\param cli
\return
**/
int ClienteView_Des_ClienteView_Post ( ClienteView *cli )
{
    blDebug ( "ClienteView_Des_ClienteView", 0 );
    /// Creamos la pesta&ntilde;a de comercial y la tratamos.
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *> ( "lcomercial" );
    delete bus;
    blDebug ( "END ClienteView_Des_ClienteView", 0 );
    return 0;
}

/*
int ClienteView_saveClient(ClienteView *cli) {
    blDebug("ClienteView_saveClient", 0);
    ComercialClienteView *bus = cli->findChild<ComercialClienteView *>("lcomercial");
    cli->setDbValue("idzonacomercial", bus->mui_zonacomercial->idzonacomercial());
    cli->setDbValue("fechabasecomercialcliente", bus->mui_fechabasecomercialcliente->fecha());
    cli->setDbValue("periodocomercialcliente", bus->mui_periodocomercialcliente->periodo());
    cli->setDbValue("comentcomercialcliente", bus->mui_comentcomercialcliente->toPlainText());

    blDebug("END ClienteView_saveClient", 0);
    return 0;
}


int Cliente_pintaCliente(Cliente *cli) {
    blDebug("Cliente_pintaCliente", 0);
    ClienteView *clo = (ClienteView *) cli;
    ComercialClienteView *bus = clo->findChild<ComercialClienteView *>("lcomercial");
    bus->mui_zonacomercial->setidzonacomercial(clo->dbValue("idzonacomercial"));
    bus->mui_fechabasecomercialcliente->setfecha(clo->dbValue("fechabasecomercialcliente"));
    bus->mui_periodocomercialcliente->setperiodo(clo->dbValue("periodocomercialcliente"));
    bus->mui_comentcomercialcliente->setPlainText(clo->dbValue("comentcomercialcliente"));
    blDebug("END Cliente_pintaCliente", 0);
    return 0;
}
*/
