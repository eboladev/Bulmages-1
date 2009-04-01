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

#include "pluginbf_formasdepago.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "fpagoview.h"




///
/**
**/
MyPlugFPago::MyPlugFPago()
{
    _depura ( "MyPlugFPago::MyPlugFPago", 0 );
    _depura ( "END MyPlugFPago::MyPlugFPago", 0 );
}


///
/**
**/
MyPlugFPago::~MyPlugFPago()
{
    _depura ( "MyPlugFPago::~MyPlugFPago", 0 );
    _depura ( "END MyPlugFPago::~MyPlugFPago", 0 );
}


///
/**
**/
void MyPlugFPago::elslot1()
{
    _depura ( "MyPlugFPago::elslot1", 0 );
        FPagoView * bud = new FPagoView((BfCompany *)mainCompany(), NULL);
        mainCompany() ->m_pWorkspace->addWindow ( bud );
        bud->show();
    _depura ( "END MyPlugFPago::elslot1", 0 );
}



///
/**
\param bges
**/
void MyPlugFPago::inicializa ( Bulmafact *bges )
{
    _depura ( "MyPlugFPago::inicializa", 0 );

    if ( bges->getcompany()->hasTablePrivilege ( "forma_pago", "SELECT" ) ) {

    /// Miramos si existe un menu Ventas
	QMenu *pPluginMenu = bges->menuMaestro;
	pPluginMenu->addSeparator();

	m_bges = bges;
	setMainCompany ( bges->getcompany() );
	QAction *planCuentas = new QAction ( _( "&Formas de pago" ), 0 );
	planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/Images/payment-method.svg" ) ));
	planCuentas->setStatusTip ( _( "Formas de pago" ) );
	planCuentas->setWhatsThis ( _( "Formas de pago" ) );
	pPluginMenu->addAction ( planCuentas );
	bges->Listados->addAction (planCuentas);
	connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );

    }// end if
    _depura ( "END MyPlugFPago::inicializa", 0 );
}





///
/**
\param bges
\return
**/
int entryPoint ( Bulmafact *bges )
{
    _depura ( "Entra plugin pluginbf_formasdepago\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_formasdepago", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugFPago *plug = new MyPlugFPago();
    plug->inicializa ( bges );
    return 0;
}



/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewFPagoView (BfCompany *v)
{
	FPagoView *h = new FPagoView(v, 0);
	g_plugParams = h;
	return 1;
}
