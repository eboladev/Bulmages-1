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

#include <QtCore/QProcess>

#include "pluginbt_accesobotones.h"
#include "btcompany.h"
#include "abrevs.h"
#include "blplugins.h"
#include "blapplication.h"
#include "bldockwidget.h"


Abrevs *g_tot;
BlDockWidget *g_doc1;


///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginabrevs", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// Vamos a probar con un docwindow.
    g_doc1 = new BlDockWidget ( _("Abreviaturas"), tpv, "abrevsdock" );
    g_doc1->setObjectName ( "abrevsdock" );
    g_doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );
    tpv->addDockWidget ( Qt::BottomDockWidgetArea, g_doc1 );
    g_doc1->show();
    g_doc1->loadConfig();
    
    return 0;
}

///
/**
\return
**/
int exitPoint ( BtBulmaTPV *tpv )
{
    BL_FUNC_DEBUG

    /// Vamos a probar con un docwindow.
    delete g_doc1;
    
    return 0;
}

int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    g_tot = new Abrevs ( etpv, g_doc1 );
    g_tot->setObjectName ( "abrevs" );
    g_doc1->setWidget ( g_tot );
    return 0;
}



