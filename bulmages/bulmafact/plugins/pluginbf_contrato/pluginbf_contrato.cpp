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
myplugincont::myplugincont()
{
    _depura ( "myplugincont::myplugincont", 0 );
    _depura ( "END myplugincont::myplugincont", 0 );
}

///
/**
**/
myplugincont::~myplugincont()
{
    _depura ( "myplugincont::~myplugincont", 0 );
    _depura ( "END myplugincont::~myplugincont", 0 );
}


///
/**
**/
void myplugincont::elslot()
{
    _depura ( "myplugincont::elslot", 0 );
    ContratosList *vehiculoview = new ContratosList ( ( BfCompany * ) m_dbConnection );
    m_bulmafact->workspace() ->addWindow ( vehiculoview );
    vehiculoview->show();
    _depura ( "END myplugincont::elslot", 0 );
}


///
/**
\param bges
**/
void myplugincont::inicializa ( Bulmafact *bges )
{
    _depura ( "myplugincont::inicializa", 0 );
    /// Creamos el men&uacute;.
    m_dbConnection = bges->getcompany();
    m_bulmafact = bges;

    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = bges->newMenu ( "&Ventas", "menuVentas", "menuMaestro" );

    QAction *accion = new QAction ( "&Contratos", 0 );
    accion->setStatusTip ( "Listado de Contratos" );
    accion->setWhatsThis ( "Listado de Contratos" );
    connect ( accion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    bges->Listados->addAction ( accion );
    _depura ( "END myplugincont::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "entryPoint", 0, "Punto de Entrada del plugin PluginBf_Contrato" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_contrato", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    myplugincont *plug = new myplugincont();
    plug->inicializa ( bges );
    _depura ( "END entryPoint", 0, "Punto de Entrada del plugin PluginBf_Contrato" );
    return 0;
}


///
/**
\param art
\return
**/
int ClienteView_ClienteView ( ClienteView *art )
{
    _depura ( "ClienteView_ClienteView", 0 );
    /// Para que funcione bien debemos iniciar con BL_SELECT_MODE y luego pasar a BL_EDIT_MODE ya que si no se hace un meteWindow y no es deseable.
    ContratosList *l = new ContratosList ( ( ( BfCompany * ) art->mainCompany() ), art, 0, BL_SELECT_MODE );
    l->setObjectName ( QString::fromUtf8 ( "ccontratoslist" ) );
    art->mui_tab->addTab ( l, "Contratos" );
    l->editMode();
    _depura ( "END ClienteView_ClienteView", 0 );
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_cargar ( BlForm *fich )
{
    _depura ( "BlForm_cargar", 0 );
    ContratosList *l = fich->findChild<ContratosList *> ( "ccontratoslist" );
    if ( l ) {
        l->mui_idcliente->setId ( fich->dbValue ( "idcliente" ) );
        l->on_mui_actualizar_clicked();
        return 0;
    } // end if
    return 0;
}
