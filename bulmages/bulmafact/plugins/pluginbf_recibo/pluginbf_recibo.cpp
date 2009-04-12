/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include "pluginbf_recibo.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "reciboview.h"
#include "reciboslist.h"
#include "emitirrecibosview.h"


RecibosList *g_recibosList = NULL;

///
/**
**/
MyPlugProf::MyPlugProf()
{
    _depura ( "MyPlugProf::MyPlugProf", 0 );
    _depura ( "END MyPlugProf::MyPlugProf", 0 );
}


///
/**
**/
MyPlugProf::~MyPlugProf()
{
    _depura ( "MyPlugProf::~MyPlugProf", 0 );
    _depura ( "END MyPlugProf::~MyPlugProf", 0 );
}


///
/**
**/
void MyPlugProf::elslot()
{
    _depura ( "MyPlugProf::elslot", 0 );
    if ( g_recibosList ) {
        g_recibosList->hide();
        g_recibosList->show();
    }// end if
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
    ReciboView * bud = new ReciboView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugProf::elslot1", 0 );
}


///
/**
**/
void MyPlugProf::elslot2()
{
    _depura ( "MyPlugProf::elslot2", 0 );
    EmitirRecibosView * bud = new EmitirRecibosView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugProf::elslot2", 0 );
}


///
/**
\param bges
**/
void MyPlugProf::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Gestion", "menuGestion", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->getcompany() );
        QAction *listRecibos = new QAction ( _ ( "&Recibos" ), 0 );
        listRecibos->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        listRecibos->setStatusTip ( _ ( "Recibos" ) );
        listRecibos->setWhatsThis ( _ ( "Recibos" ) );
        pPluginMenu->addAction ( listRecibos );
        bges->Listados->addAction ( listRecibos );
        connect ( listRecibos, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *nuevoRecibo = new QAction ( _ ( "&Nuevo recibo" ), 0 );
        nuevoRecibo->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        nuevoRecibo->setStatusTip ( _ ( "Nuevo recibo" ) );
        nuevoRecibo->setWhatsThis ( _ ( "Nuevo recibo" ) );
        pPluginMenu->addAction ( nuevoRecibo );
        bges->Fichas->addAction ( nuevoRecibo );
        connect ( nuevoRecibo, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


        QAction *emitirRecibos = new QAction ( _ ( "&Emitir Recibos Periodo" ), 0 );
        emitirRecibos->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        emitirRecibos->setStatusTip ( _ ( "Emitir recibos" ) );
        emitirRecibos->setWhatsThis ( _ ( "Emitir recibos" ) );
        pPluginMenu->addAction ( emitirRecibos );
        bges->Fichas->addAction ( emitirRecibos );
        connect ( emitirRecibos, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

    }// end if
    _depura ( "END MyPlugProf::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de entrada del plugin de recibos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_recibo", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "recibo", "SELECT" ) ) {
        g_recibosList = new RecibosList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_recibosList );
        g_recibosList->hide();
    }// end if
    return 0;
}
