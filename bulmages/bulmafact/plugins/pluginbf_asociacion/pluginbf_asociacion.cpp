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



// RecibosList *g_recibosList=NULL;

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
    _depura ( "END MyPlugProf::elslot", 0 );
}

///
/**
**/
void MyPlugProf::elslot1()
{
    _depura ( "MyPlugProf::elslot1", 0 );
    _depura ( "END MyPlugProf::elslot1", 0 );
}


///
/**
**/
void MyPlugProf::elslot2()
{
    _depura ( "MyPlugProf::elslot2", 0 );
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
		QMenu *pPluginMenu = bges->newMenu("&Asociacion", "menuAsociacion", "menuMaestro");
		pPluginMenu->addSeparator();

		/// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
		m_bges = bges;
		setMainCompany ( bges->getcompany() );
		QAction *planCuentas = new QAction ( _( "&Asociacion" ), 0 );
		planCuentas->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ));
		planCuentas->setStatusTip ( _( "Asociacion" ) );
		planCuentas->setWhatsThis ( _( "Asociacion" ) );
		pPluginMenu->addAction ( planCuentas );
		bges->Listados->addAction (planCuentas);
		connect ( planCuentas, SIGNAL ( activated() ), this, SLOT ( elslot() ) );
	
		QAction *npago = new QAction ( _( "&Junta Directiva" ), 0 );
		npago->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ));
		npago->setStatusTip ( _( "Junta Directiva" ) );
		npago->setWhatsThis ( _( "Junta Directiva" ) );
		pPluginMenu->addAction ( npago );
		bges->Fichas->addAction (npago);
		connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot1() ) );


      QAction *npago1 = new QAction ( _( "&Comisiones" ), 0 );
      npago1->setIcon(QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/actividad.svg" ) ));
      npago1->setStatusTip ( _( "Comisiones" ) );
      npago1->setWhatsThis ( _( "Comisiones" ) );
      pPluginMenu->addAction ( npago1 );
      bges->Fichas->addAction (npago1);
      connect ( npago, SIGNAL ( activated() ), this, SLOT ( elslot2() ) );

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
    _depura ( "Punto de entrada del plugin de asociaciones\n", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbf_asociacion", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    MyPlugProf *plug = new MyPlugProf();
    plug->inicializa ( bges );
    return 0;
}


int BfCompany_createMainWindows_Post(BfCompany *comp) {
    return 0;
}