/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include "pluginbf_profesor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "profesorview.h"
#include "profesoreslist.h"
#include "aboutfapacview.h"

ProfesoresList *g_profesoresList = NULL;

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
    
    if ( g_profesoresList ) {
        g_profesoresList->hide();
        g_profesoresList->show();
    } // end if
    
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
    
    ProfesorView * bud = new ProfesorView ( ( BfCompany * ) mainCompany(), NULL );
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
    
    AboutFapacView *afv = new AboutFapacView();
    afv->show();
    
    _depura ( "END MyPlugProf::elslot2", 0 );
}

///
/**
\param bges
**/
void MyPlugProf::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugProf::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "profesor", "SELECT" ) ) {

        /// Miramos si existe un menu Docencia
        QMenu *pPluginMenu = bges->newMenu ( "&Docencia", "menuDocencia", "menuMaestro" );

        /// Agregamos un separador
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Monitores" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/profesor.gif" ) ) );
        planCuentas->setStatusTip ( _ ( "Monitores" ) );
        planCuentas->setWhatsThis ( _ ( "Monitores" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

        QAction *npago = new QAction ( _ ( "&Nuevo monitor" ), 0 );
        npago->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/profesor_add.png" ) ) );
        npago->setStatusTip ( _ ( "Nuevo profesor" ) );
        npago->setWhatsThis ( _ ( "Nuevo profesor" ) );
        pPluginMenu->addAction ( npago );
        bges->Fichas->addAction ( npago );
        connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

        QAction *nfapac = new QAction ( _ ( "About FAPAC" ), 0 );
        nfapac->setStatusTip ( _ ( "About FAPAC" ) );
        nfapac->setWhatsThis ( _ ( "About FAPAC" ) );
        bges->menuAcerca_de->addAction ( nfapac );
        connect ( nfapac, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

    } // end if
    
    _depura ( "END MyPlugProf::inicializa", 0 );
}

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de Monitores", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_profesor", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    
    _depura ( "END Punto de Entrada del plugin de Monitores", 0 );
    
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    _depura ( "BfCompany_createMainWindows_Post", 0 );

    if ( comp->hasTablePrivilege ( "profesor", "SELECT" ) ) {
        g_profesoresList = new ProfesoresList ( comp, NULL );
        comp->m_pWorkspace->addWindow ( g_profesoresList );
        g_profesoresList->hide();
    } // end if
    
    _depura ( "END BfCompany_createMainWindows_Post", 0 );
    
    return 0;
}

int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    _depura ( "Busqueda_on_mui_buscar_clicked", 0 );

    if ( busq->tableName() == "profesor" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        centrarEnPantalla ( diag );

        ProfesoresList *clients = new ProfesoresList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( clients );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( clients->windowTitle() );

        diag->exec();
        
        if ( clients->idprofesor() != "" ) {
            busq->setId ( clients->idprofesor() );
        } // end if
        
        delete diag;

        return 1;
        
    } // end if
    
    _depura ( "END Busqueda_on_mui_buscar_clicked", 0 );
    
    return 0;

}
