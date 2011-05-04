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

#include "bfcompany.h"
#include "pluginbf_contrato.h"
#include "blfunctions.h"
#include "contratoslist.h"
#include "blform.h"

///
/**
**/
PluginBf_Contrato::PluginBf_Contrato()
{
    blDebug ( "PluginBf_Contrato::PluginBf_Contrato", 0 );
    blDebug ( "END PluginBf_Contrato::PluginBf_Contrato", 0 );
}

///
/**
**/
PluginBf_Contrato::~PluginBf_Contrato()
{
    blDebug ( "PluginBf_Contrato::~PluginBf_Contrato", 0 );
    blDebug ( "END PluginBf_Contrato::~PluginBf_Contrato", 0 );
}


///
/**
**/
void PluginBf_Contrato::elslot()
{
    blDebug ( "PluginBf_Contrato::elslot", 0 );
    ContratosList *vehiculoview = new ContratosList ( ( BfCompany * ) m_dbConnection );
    m_bulmafact->workspace() ->addSubWindow ( vehiculoview );
    vehiculoview->show();
    blDebug ( "END PluginBf_Contrato::elslot", 0 );
}


///
/**
\param bges
**/
void PluginBf_Contrato::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "PluginBf_Contrato::inicializa", 0 );
    /// Creamos el men&uacute;.
    m_dbConnection = bges->company();
    m_bulmafact = bges;

    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );

    QAction *accion = new QAction ( "&Contratos", 0 );
    accion->setStatusTip ( "Listado de Contratos" );
    accion->setWhatsThis ( "Listado de Contratos" );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/contract-list.png" ) ) );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    bges->Listados->addAction ( accion );
    blDebug ( "END PluginBf_Contrato::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "entryPoint", 0, "Punto de Entrada del plugin PluginBf_Contrato" );

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Contrato", "0.11.1-0001");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_contrato", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBf_Contrato *plug = new PluginBf_Contrato();
    plug->inicializa ( bges );
    blDebug ( "END entryPoint", 0, "Punto de Entrada del plugin PluginBf_Contrato" );
    return 0;
}


///
/**
\param art
\return
**/
int ClienteView_ClienteView ( ClienteView *art )
{
    blDebug ( "ClienteView_ClienteView", 0 );
    /// Para que funcione bien debemos iniciar con BL_SELECT_MODE y luego pasar a BL_EDIT_MODE ya que si no se hace un meteWindow y no es deseable.
    ContratosList *l = new ContratosList ( ( ( BfCompany * ) art->mainCompany() ), art, 0, BL_SELECT_MODE );
    l->setObjectName ( QString::fromUtf8 ( "ccontratoslist" ) );
    art->mui_tab->addTab ( l, "Contratos" );
    l->setModoEdicion();
    blDebug ( "END ClienteView_ClienteView", 0 );
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_cargar ( BlForm *fich )
{
    blDebug ( "BlForm_cargar", 0 );
    ContratosList *l = fich->findChild<ContratosList *> ( "ccontratoslist" );
    if ( l ) {
        l->mui_idcliente->setId ( fich->dbValue ( "idcliente" ) );
        l->on_mui_actualizar_clicked();
        return 0;
    } // end if
    return 0;
}
