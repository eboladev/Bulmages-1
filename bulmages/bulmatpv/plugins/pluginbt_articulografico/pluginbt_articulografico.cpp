/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

#include "pluginbt_articulografico.h"
#include "btcompany.h"
#include "artgraficos.h"
#include "blplugins.h"
#include "btticket.h"
#include "bldockwidget.h"


BlDockWidget *g_pantallas;
ArtGraficos *g_graf;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_articulografico", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );


    /// Ponemos el widget de pantallas.
    g_pantallas = new BlDockWidget ( _("Pantallas"), tpv, "pantallasdock" );
    g_pantallas->setObjectName ( "pantallasdock" );
    g_pantallas->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    tpv->addDockWidget ( Qt::RightDockWidgetArea, g_pantallas );
    g_pantallas->show();
    g_pantallas->loadConfig();

    
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    g_graf = new ArtGraficos ( etpv, NULL );
    ((BtBulmaTPV *)g_main)->setCentralBtWidget ( g_graf );
    return 0;
}
