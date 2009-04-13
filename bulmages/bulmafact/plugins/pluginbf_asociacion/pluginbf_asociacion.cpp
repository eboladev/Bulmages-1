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

#include "pluginbf_asociacion.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "jdirectivalist.h"
#include "jdirectivaview.h"

#include "convreunionlist.h"
#include "convreunionview.h"
///
/**
**/
MyPlugAsoc::MyPlugAsoc()
{
    _depura ( "MyPlugAsoc::MyPlugAsoc", 0 );
    _depura ( "END MyPlugAsoc::MyPlugAsoc", 0 );
}


///
/**
**/
MyPlugAsoc::~MyPlugAsoc()
{
    _depura ( "MyPlugAsoc::~MyPlugAsoc", 0 );
    _depura ( "END MyPlugAsoc::~MyPlugAsoc", 0 );
}


///
/**
**/
void MyPlugAsoc::elslot()
{
    _depura ( "MyPlugAsoc::elslot", 0 );
    _depura ( "END MyPlugAsoc::elslot", 0 );
}

///
/**
**/
void MyPlugAsoc::elslot1()
{
    _depura ( "MyPlugAsoc::elslot1", 0 );
    JDirectivaList * bud = new JDirectivaList ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugAsoc::elslot1", 0 );
}


///
/**
**/
void MyPlugAsoc::elslot2()
{
    _depura ( "MyPlugAsoc::elslot2", 0 );
    ConvReunionList * bud = new ConvReunionList ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( bud );
    bud->show();
    _depura ( "END MyPlugAsoc::elslot2", 0 );
}


///
/**
\param bges
**/
void MyPlugAsoc::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugAsoc::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "cobro", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( "&Asociacion", "menuAsociacion", "menuMaestro" );
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->getcompany() );
        QAction *asociacion = new QAction ( _ ( "&Asociacion" ), 0 );
        asociacion->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        asociacion->setStatusTip ( _ ( "Asociacion" ) );
        asociacion->setWhatsThis ( _ ( "Asociacion" ) );
        pPluginMenu->addAction ( asociacion );
        bges->Listados->addAction ( asociacion );
        connect ( asociacion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *jdirectiva = new QAction ( _ ( "&Junta Directiva" ), 0 );
        jdirectiva->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        jdirectiva->setStatusTip ( _ ( "Junta Directiva" ) );
        jdirectiva->setWhatsThis ( _ ( "Junta Directiva" ) );
        pPluginMenu->addAction ( jdirectiva );
        bges->Fichas->addAction ( jdirectiva );
        connect ( jdirectiva, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


        QAction *comision = new QAction ( _ ( "&Comisiones" ), 0 );
        comision->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        comision->setStatusTip ( _ ( "Comisiones" ) );
        comision->setWhatsThis ( _ ( "Comisiones" ) );
        pPluginMenu->addAction ( comision );
        bges->Fichas->addAction ( comision );
        connect ( comision, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
        
        pPluginMenu->addSeparator();

        QAction *convjd = new QAction ( _ ( "&Convocar Junta" ), 0 );
        convjd->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        convjd->setStatusTip ( _ ( "Convocar Junta" ) );
        convjd->setWhatsThis ( _ ( "Convocar Junta" ) );
        pPluginMenu->addAction ( convjd );
        bges->Fichas->addAction ( convjd );
        connect ( convjd, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
        
        QAction *convas = new QAction ( _ ( "&Convocar Asamblea" ), 0 );
        convas->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        convas->setStatusTip ( _ ( "Convocar Asamblea" ) );
        convas->setWhatsThis ( _ ( "Convocar Asamblea" ) );
        pPluginMenu->addAction ( convas );
        bges->Fichas->addAction ( convas );
        connect ( convas, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
        
        QAction *convreunion = new QAction ( _ ( "&Convocar Reunion" ), 0 );
        convreunion->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ) );
        convreunion->setStatusTip ( _ ( "Convocar Reunion" ) );
        convreunion->setWhatsThis ( _ ( "Convocar Reunion" ) );
        pPluginMenu->addAction ( convreunion );
        bges->Fichas->addAction ( convreunion );
        connect ( convreunion, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );
        
    }// end if
    _depura ( "END MyPlugAsoc::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de entrada del plugin de asociaciones\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_asociacion", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugAsoc *plug = new MyPlugAsoc();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    return 0;
}
