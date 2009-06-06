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

#include "pluginbf_minicontabilidad.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "partidasview.h"


///
/**
**/
MyPlugArt::MyPlugArt( BfBulmaFact *bges) 
{
    _depura ( "MyPlugArt::MyPlugArt", 0 );
    setMainCompany(bges->company());
    m_bges = bges;
    _depura ( "END MyPlugArt::MyPlugArt", 0 );
}


///
/**
**/
MyPlugArt::~MyPlugArt()
{
    _depura ( "MyPlugArt::~MyPlugArt", 0 );
    _depura ( "END MyPlugArt::~MyPlugArt", 0 );
}






///
/**
**/
void MyPlugArt::elslot3()
{
    _depura ( "MyPlugArt::elslot3", 0 );
    PartidasView *pag = new PartidasView ( ( BfCompany * ) mainCompany(), 0, FALSE );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    _depura ( "END MyPlugArt::elslot3", 0 );
}



///
/**
\param bges
**/
void MyPlugArt::inicializa ( ) 
{
    _depura ( "MyPlugArt::inicializa", 0 );

    if ( m_bges->company()->hasTablePrivilege ( "partida", "SELECT" ) ) {

        /// Miramos si existe un menu Articulos
        QMenu *pPluginMenu = m_bges->newMenu ( _("Gestion"), "menuGestion", "menuMaestro" );
        pPluginMenu->addSeparator();


        pPluginMenu->addSeparator();
        QAction *tfam = new QAction ( _ ( "&Partidas contables" ), 0 );
        tfam->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/product-family.png" ) ) );
        tfam->setStatusTip ( _ ( "Partidas contables" ) );
        tfam->setWhatsThis ( _ ( "Partidas contables" ) );
        pPluginMenu->addAction ( tfam );
        m_bges->Fichas->addAction ( tfam );
        connect ( tfam, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

	    }// end if
    _depura ( "END MyPlugArt::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de MiniContabilidad\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_minicontabilidad", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugArt *plug = new MyPlugArt(bges);
    plug->inicializa ( );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "partida", "SELECT" ) ) {
/*
        g_articulosList = new ArticuloList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_articulosList );
        g_articulosList->hide();
*/
    }// end if
    return 0;
}


int Busqueda_on_mui_buscar_released ( BlSearchWidget *busq )
{


    if ( busq->tableName() == "partida" ) {



        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

        PartidasView *arts = new PartidasView ( ( BfCompany * ) busq->mainCompany(), 0, TRUE );

        busq->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( arts );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( arts->windowTitle() );

        diag->exec();
        if ( arts->idPartida() != "" ) {
            busq->setId ( arts->idPartida() );
        } // end if
        delete diag;

        return 1;
    } // end if


    return 0;
}






