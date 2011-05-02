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
#include "comisioneslist.h"

///
/**
**/
PluginBf_Asociacion::PluginBf_Asociacion()
{
    blDebug ( "PluginBf_Asociacion::PluginBf_Asociacion", 0 );
    blDebug ( "END PluginBf_Asociacion::PluginBf_Asociacion", 0 );
}


///
/**
**/
PluginBf_Asociacion::~PluginBf_Asociacion()
{
    blDebug ( "PluginBf_Asociacion::~PluginBf_Asociacion", 0 );
    blDebug ( "END PluginBf_Asociacion::~PluginBf_Asociacion", 0 );
}


///
/**
**/
void PluginBf_Asociacion::elslot()
{
    blDebug ( "PluginBf_Asociacion::elslot", 0 );
    blDebug ( "END PluginBf_Asociacion::elslot", 0 );
}

///
/**
**/
void PluginBf_Asociacion::elslot1()
{
    blDebug ( "PluginBf_Asociacion::elslot1", 0 );
    JDirectivaList * bud = new JDirectivaList ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END PluginBf_Asociacion::elslot1", 0 );
}




///
/**
**/
void PluginBf_Asociacion::comision()
{
    blDebug ( "PluginBf_Asociacion::comision", 0 );
    ComisionesList * bud = new ComisionesList ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END PluginBf_Asociacion::comision", 0 );
}




///
/** Convocar Asamblea
**/
void PluginBf_Asociacion::convjunta()
{
    blDebug ( "PluginBf_Asociacion::convjunta", 0 );
    JDirectivaView *bud = new JDirectivaView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->pintar();
//     bud->junta();
    blDebug ( "END PluginBf_Asociacion::convjunta", 0 );
}

///
/** Convocar Asamblea
**/
void PluginBf_Asociacion::elslot2()
{
    blDebug ( "PluginBf_Asociacion::elslot2", 0 );
    ConvReunionView *bud = new ConvReunionView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    bud->pintar();
    blDebug ( "END PluginBf_Asociacion::elslot2", 0 );
}


///
/** Lista de Reuniones
**/
void PluginBf_Asociacion::elslot3()
{
    blDebug ( "PluginBf_Asociacion::elslot2", 0 );
    ConvReunionList * bud = new ConvReunionList ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addSubWindow ( bud );
    bud->show();
    blDebug ( "END PluginBf_Asociacion::elslot2", 0 );
}

///
/**
\param bges
**/
void PluginBf_Asociacion::inicializa ( BfBulmaFact *bges )
{
    blDebug ( "PluginBf_Asociacion::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "reunion", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Asociacion"), "menuAsociacion", "menuMaestro" );
        pPluginMenu->addSeparator();


        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
	
/*
        QAction *asociacion = new QAction ( _ ( "&Asociacion" ), 0 );
        asociacion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/employee-list.png" ) ) );
        asociacion->setStatusTip ( _ ( "Asociacion" ) );
        asociacion->setWhatsThis ( _ ( "Asociacion" ) );
        pPluginMenu->addAction ( asociacion );
        bges->Listados->addAction ( asociacion );
        connect ( asociacion, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
*/

        QAction *jdirectiva = new QAction ( _ ( "&Juntas Directivas" ), 0 );
        jdirectiva->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/jdirectiva-list.png" ) ) );
        jdirectiva->setStatusTip ( _ ( "Juntas Directivas" ) );
        jdirectiva->setWhatsThis ( _ ( "Juntas Directivas" ) );
        pPluginMenu->addAction ( jdirectiva );
        bges->Fichas->addAction ( jdirectiva );
        connect ( jdirectiva, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        QAction *convjd = new QAction ( _ ( "&Convocar Junta Directiva" ), 0 );
        convjd->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/jdirectiva-new.png" ) ) );
        convjd->setStatusTip ( _ ( "Convocar Junta Directiva" ) );
        convjd->setWhatsThis ( _ ( "Convocar Junta Directiva" ) );
        pPluginMenu->addAction ( convjd );
        bges->Fichas->addAction ( convjd );
        connect ( convjd, SIGNAL ( activated() ), this, SLOT ( convjunta() ) );
        
        pPluginMenu->addSeparator();
        QAction *reuniones = new QAction ( _ ( "&Reuniones" ), 0 );
        reuniones->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/reunion-list.png" ) ) );
        reuniones->setStatusTip ( _ ( "Reuniones" ) );
        reuniones->setWhatsThis ( _ ( "Reuniones" ) );
        pPluginMenu->addAction ( reuniones );
        bges->Fichas->addAction ( reuniones );
        connect ( reuniones, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );
        
        QAction *convreunion = new QAction ( _ ( "&Convocar Reunion" ), 0 );
        convreunion->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/reunion-new.png" ) ) );
        convreunion->setStatusTip ( _ ( "Convocar Reunion" ) );
        convreunion->setWhatsThis ( _ ( "Convocar Reunion" ) );
        pPluginMenu->addAction ( convreunion );
        bges->Fichas->addAction ( convreunion );
        connect ( convreunion, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

        QAction *comision = new QAction ( _ ( "&Comisiones" ), 0 );
        comision->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/comision-list.png" ) ) );
        comision->setStatusTip ( _ ( "Comisiones" ) );
        comision->setWhatsThis ( _ ( "Comisiones" ) );
        pPluginMenu->addAction ( comision );
        bges->Fichas->addAction ( comision );
        connect ( comision, SIGNAL ( activated() ), this, SLOT ( comision() ) );
                
        QAction *convas = new QAction ( _ ( "&Convocar Asamblea" ), 0 );
        convas->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/asamblea-new.png" ) ) );
        convas->setStatusTip ( _ ( "Convocar Asamblea" ) );
        convas->setWhatsThis ( _ ( "Convocar Asamblea" ) );
        pPluginMenu->addAction ( convas );
        bges->Fichas->addAction ( convas );
        connect ( convas, SIGNAL ( activated() ), this, SLOT ( convjunta() ) );
        
        pPluginMenu->addSeparator();        

    } // end if
    
    blDebug ( "END PluginBf_Asociacion::inicializa", 0 );
}


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    blDebug ( "Punto de entrada del plugin de asociaciones\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_asociacion", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    PluginBf_Asociacion *plug = new PluginBf_Asociacion();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    return 0;
}
