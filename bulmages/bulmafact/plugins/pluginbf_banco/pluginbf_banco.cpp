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

#include "pluginbf_banco.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "bancoview.h"




///
/**
**/
MyPlugBanc::MyPlugBanc()
{
    _depura ( "MyPlugBanc::MyPlugBanc", 0 );
    _depura ( "END MyPlugBanc::MyPlugBanc", 0 );
}


///
/**
**/
MyPlugBanc::~MyPlugBanc()
{
    _depura ( "MyPlugBanc::~MyPlugBanc", 0 );
    _depura ( "END MyPlugBanc::~MyPlugBanc", 0 );
}


///
/**
**/
void MyPlugBanc::elslot1()
{
    _depura ( "MyPlugBanc::elslot1", 0 );
        BancoView * bud = new BancoView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugBanc::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugBanc::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugBanc::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "banco", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->menuMaestro;
	pPluginMenu->addSeparator();

	/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Bancos" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/bank.svg" ) ));
	planCuentas->setStatusTip ( _( "Bancos" ) );
	planCuentas->setWhatsThis ( _( "Bancos" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    _depura ( "END MyPlugBanc::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Punto de Entrada del plugin de bancos\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_banco", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugBanc *plug = new MyPlugBanc();
    plug->inicializa ( bges );
    return 0;
}



/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewBancoView (BfCompany *v)
{
	BancoView *h = new BancoView(v, 0);
	g_plugParams = h;
	return 1;
}
