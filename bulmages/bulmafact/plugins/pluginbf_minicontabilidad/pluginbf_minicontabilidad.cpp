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
#include "acontableview.h"

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
void MyPlugArt::elslot2()
{
    _depura ( "MyPlugArt::elslot3", 0 );
    ApunteContableView *pag = new ApunteContableView ( ( BfCompany * ) mainCompany(), 0 );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    _depura ( "END MyPlugArt::elslot3", 0 );
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
        tfam->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/account_plan.png" ) ) );
        tfam->setStatusTip ( _ ( "Partidas contables" ) );
        tfam->setWhatsThis ( _ ( "Partidas contables" ) );
        pPluginMenu->addAction ( tfam );
        m_bges->Fichas->addAction ( tfam );
        connect ( tfam, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

        QAction *tfam1 = new QAction ( _ ( "&Anotaciones contables" ), 0 );
        tfam1->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/balance-sheet.png" ) ) );
        tfam1->setStatusTip ( _ ( "Anotaciones contables" ) );
        tfam1->setWhatsThis ( _ ( "Anotaciones contables" ) );
        pPluginMenu->addAction ( tfam1 );
        m_bges->Fichas->addAction ( tfam1 );
        connect ( tfam1, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

        QAction *tfam2 = new QAction ( _ ( "&Previsiones" ), 0 );
        tfam2->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/book-list.png" ) ) );
        tfam2->setStatusTip ( _ ( "Previsiones" ) );
        tfam2->setWhatsThis ( _ ( "Previsiones" ) );
        pPluginMenu->addAction ( tfam2 );
        m_bges->Fichas->addAction ( tfam2 );
        connect ( tfam2, SIGNAL ( activated() ), this, SLOT ( elslot3() ) );

	
	
	
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



int BfSubForm_pressedAsterisk ( BfSubForm *sub )
{
    _depura ( "BfSubForm_pressedAsterisk" );

    if ( sub->m_campoactual->nomcampo() != "codigocompletopartida" ) {
        _depura ( "END BfSubForm::pressedAsterisk", 0 );
        return 0;
    } // end if

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

        PartidasView *arts = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, TRUE );

        diag->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

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

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM partida WHERE idpartida = " + arts->idPartida() );
    if ( !cur->eof() ) {
        sub->m_registrolinea->setDbValue ( "idpartida", arts->idPartida() );
        sub->m_registrolinea->setDbValue ( "codigocompletopartida", cur->valor ( "codigocompletopartida" ));
        sub->m_registrolinea->setDbValue ( "nombrepartida", cur->valor ( "nombrepartida" ));
    } // end if
    
    delete cur;
	  
	  
        } // end if
        delete diag;

    _depura ( "END BfSubForm_pressedAsterisk" );

    return 1;
}




/// --------------------------------------------------------------
/// --------- Implemento la edicion de articulos -----------------
/// Con esta funcionalidad creamos menus contextuales en todos los subformularios donde
/// Aparezca el identificador de articulo como elemento y permite de forma sencilla
/// La creacion, la edicion, y la seleccion.
///


/**
\param parent
**/
MyPlugArt1::MyPlugArt1 ( BlSubForm *parent ) : QObject ( parent )
{
    _depura ( "MyPlugArt1::MyPlugArt1", 0 );
    _depura ( "END MyPlugArt1::MyPlugArt1", 0 );
}

///
/**
**/
MyPlugArt1::~MyPlugArt1()
{
    _depura ( "MyPlugArt1::~MyPlugArt1", 0 );
    _depura ( "END MyPlugArt1::~MyPlugArt1", 0 );
}


///
/**
\param menu
**/
void MyPlugArt1::s_pintaMenu ( QMenu *menu )
{
    _depura ( "MyPlugArt1::s_pintaMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    BlSubFormHeader *header = sub->header ( "codigocompletopartida" );
    if ( header ) {
        menu->addSeparator();
        menu->addAction ( _ ( "Gestionar partidas" ) );
	menu->addAction ( _ ( "Seleccionar partida" ) );
    } // end if
    _depura ( "END MyPlugArt1::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPlugArt1::s_trataMenu ( QAction *action )
{
    _depura ( "MyPlugArt1::s_trataMenu", 0 );
    BfSubForm *sub = ( BfSubForm * ) parent();
    if ( action->text() == _ ( "Gestionar partidas" ) ) {
            gestionarPartidas ( sub);
    } else if ( action->text() == _ ( "Seleccionar partida" ) ) {
        seleccionarPartida ( sub );
    } // end if

    _depura ( "END MyPlugArt1::s_trataMenu", 0 );
}


///
/**
**/
void MyPlugArt1::gestionarPartidas (  BfSubForm *sub )
{
    _depura ( "MyPlugArt1::editarArticulo", 0 );
    PartidasView *pag = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, FALSE );
    sub->mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    _depura ( "END MyPlugArt1::editarArticulo", 0 );
}



///
/**
**/
void MyPlugArt1::seleccionarPartida ( BfSubForm *sub )
{
    _depura ( "MyPlugArt1::editarArticulo", 0 );

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

        PartidasView *arts = new PartidasView ( ( BfCompany * ) sub->mainCompany(), 0, TRUE );

        diag->connect ( arts, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

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

    BlDbRecordSet *cur = sub->mainCompany() ->loadQuery ( "SELECT * FROM partida WHERE idpartida = " + arts->idPartida() );

    if ( !cur->eof() ) {
        sub->lineaact()->setDbValue ( "idpartida", arts->idPartida() );
        sub->lineaact()->setDbValue ( "codigocompletopartida", cur->valor ( "codigocompletopartida" ) );
        sub->lineaact()->setDbValue ( "nombrepartida", cur->valor ( "nombrepartida" ) );
    } // end if
    delete cur;
	} // end if
    delete diag;
    
    _depura ( "END MyPlugArt1::editarArticulo", 0 );
}


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    MyPlugArt1 *subformods = new MyPlugArt1 ( sub );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}





