/***************************************************************************
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
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

#include <QPushButton>
#include <QVBoxLayout>

#include "pluginbt_articulograficodb.h"
#include "btcompany.h"
#include "artgraficosdb.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"

BlDockWidget *g_pantallas;
ArtGraficosDb *g_graf;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    _depura ( "entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbt_articulograficodb", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    /// Ponemos el widget de pantallas.
    g_pantallas = new BlDockWidget ( "Pantallas", tpv, "pantallasdock" );
    g_pantallas->setObjectName ( "pantallasdock" );
    g_pantallas->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    tpv->addDockWidget ( Qt::RightDockWidgetArea, g_pantallas );
    g_pantallas->show();
    g_pantallas->cargaconf();

    _depura ( "END entryPoint", 0 );
    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    _depura ( "BtCompany_createMainWindows_Post", 0 );

    g_graf = new ArtGraficosDb ( etpv, NULL );
    g_main->setCentralWidget ( g_graf );
    
    _depura ( "BtCompany_createMainWindows_Post", 0 );
    
    return 0;
}