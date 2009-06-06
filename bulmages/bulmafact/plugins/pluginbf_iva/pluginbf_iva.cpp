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

#include "pluginbf_iva.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "listtipoivaview.h"
#include "listtasaivaview.h"




///
/**
**/
MyPlugIVA::MyPlugIVA()
{
    _depura ( "MyPlugIVA::MyPlugIVA", 0 );
    _depura ( "END MyPlugIVA::MyPlugIVA", 0 );
}


///
/**
**/
MyPlugIVA::~MyPlugIVA()
{
    _depura ( "MyPlugIVA::~MyPlugIVA", 0 );
    _depura ( "END MyPlugIVA::~MyPlugIVA", 0 );
}


///
/**
**/
void MyPlugIVA::elslot()
{
    _depura ( "MyPlugIVA::elslot", 0 );
    ListTasaIVAView *pag = new ListTasaIVAView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    /*        TrabajadorView * bud = new TrabajadorView((BfCompany *)mainCompany(), NULL);
            mainCompany() ->m_pWorkspace->addWindow ( bud );
            bud->show();
    */
    _depura ( "END MyPlugIVA::elslot", 0 );
}


///
/**
**/
void MyPlugIVA::elslot1()
{
    _depura ( "MyPlugIVA::elslot1", 0 );
    ListTipoIVAView *pag = new ListTipoIVAView ( ( BfCompany * ) mainCompany(), NULL );
    mainCompany() ->m_pWorkspace->addWindow ( pag );
    pag->show();
    /*        TrabajadorView * bud = new TrabajadorView((BfCompany *)mainCompany(), NULL);
            mainCompany() ->m_pWorkspace->addWindow ( bud );
            bud->show();
    */
    _depura ( "END MyPlugIVA::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugIVA::inicializa ( BfBulmaFact *bges )
{
    _depura ( "MyPlugIVA::inicializa", 0 );

    if ( bges->company()->hasTablePrivilege ( "tipo_iva", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->menuMaestro;
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tipos de IVA en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas = new QAction ( _ ( "&Tipos de IVA" ), 0 );
        planCuentas->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
        planCuentas->setStatusTip ( _ ( "Tipos de IVA" ) );
        planCuentas->setWhatsThis ( _ ( "Tipos de IVA" ) );
        pPluginMenu->addAction ( planCuentas );
        bges->Listados->addAction ( planCuentas );
        connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


        /// El men&uacute; de Tasa de IVA en la secci&oacute;n de art&iacute;culos.
        m_bges = bges;
        setMainCompany ( bges->company() );
        QAction *planCuentas1 = new QAction ( _ ( "&Tasas de IVA" ), 0 );
        planCuentas1->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/vat.png" ) ) );
        planCuentas1->setStatusTip ( _ ( "Tasas de IVA" ) );
        planCuentas1->setWhatsThis ( _ ( "Tasas de IVA" ) );
        pPluginMenu->addAction ( planCuentas1 );
        bges->Listados->addAction ( planCuentas1 );
        connect ( planCuentas1, SIGNAL ( activated() ), this, SLOT ( elslot() ) );

    }// end if
    _depura ( "END MyPlugIVA::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    _depura ( "Punto de Entrada del plugin de IVA\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_iva", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    MyPlugIVA *plug = new MyPlugIVA();
    plug->inicializa ( bges );
    return 0;
}


