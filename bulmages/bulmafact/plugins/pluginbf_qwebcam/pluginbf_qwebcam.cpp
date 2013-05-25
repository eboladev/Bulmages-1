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

#include <QtCore/QObject>
#include "pluginbf_qwebcamp.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "qwebcamview.h"
//#include "busquedatarifa.h"
// #include "listltarifaview.h"
// #include "tarifalistview.h"


/*
myplugin1::myplugin1() {}


myplugin1::~myplugin1() {}


void myplugin1::elslot() {

//    TarifaListView *tar = new TarifaListView(m_companyact, NULL);
//    m_companyact->m_pWorkspace->addSubWindow(tar);
//    tar->show();
}


void myplugin1::inicializa(bulmafact *bges) {
    /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
    m_companyact = bges->company();
    QAction *planCuentas = new QAction("&QWebCams", 0);
    planCuentas->setStatusTip("QWebCams");
    planCuentas->setWhatsThis("QWebCams");
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
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_qwebcam", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

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
    BL_FUNC_DEBUG
    QWebCamView *l = new QWebCamView ( art->mainCompany(), art );
    l->setObjectName ( QString::fromUtf8 ( "lpromedios" ) );
    art->mui_tab->addTab ( l, _("Webcams") );
    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_load ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    QWebCamView *l = art->findChild<QWebCamView *> ( "lpromedios" );
    l->load ( art->dbValue ( "idarticulo" ) );
    
    return 0;
}


