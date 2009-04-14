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

#include <QObject>
#include "pluginbf_promedios.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "promedioview.h"
//#include "busquedatarifa.h"
// #include "listltarifaview.h"
// #include "tarifalistview.h"


/*
myplugin1::myplugin1() {}


myplugin1::~myplugin1() {}


void myplugin1::elslot() {

//    TarifaListView *tar = new TarifaListView(m_companyact, NULL);
//    m_companyact->m_pWorkspace->addWindow(tar);
//    tar->show();
}


void myplugin1::inicializa(bulmafact *bges) {
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_companyact = bges->getcompany();
    QAction *planCuentas = new QAction("&Promedios", 0);
    planCuentas->setStatusTip("Promedios");
    planCuentas->setWhatsThis("Promedios");
    bges->menuArticulos->addSeparator();
    bges->menuArticulos->addAction(planCuentas);
    connect(planCuentas, SIGNAL(activated()), this, SLOT(elslot()));
}


*/


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    _depura ( "Punto de Entrada del plugin de Promedios", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbf_promedios", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

//    myplugin1 *plug = new myplugin1();
//    plug->inicializa(bges);
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    _depura ( "ArticuloView_ArticuloView", 0 );
    PromedioView *l = new PromedioView ( art->mainCompany(), art );
    l->setObjectName ( QString::fromUtf8 ( "lpromedios" ) );
    art->mui_tab->addTab ( l, "Promedios" );
    _depura ( "END ArticuloView_ArticuloView", 0 );
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_cargar ( ArticuloView *art )
{
    _depura ( "ArticuloView_cargar", 0 );
    PromedioView *l = art->findChild<PromedioView *> ( "lpromedios" );
    l->cargar ( art->dbValue ( "idarticulo" ) );
    _depura ( "END ArticuloView_cargar", 0 );
    return 0;
}



