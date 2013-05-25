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

///Dependencias de QT
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtCore/QObject>
#include <QtWidgets/QMessageBox>

///Dependencias de C++
#include <cstdio>


///Dependencias de BulmaGes
#include "pluginbt_example.h"
#include "btcompany.h"
#include "blfunctions.h"
#include "example.h"
#include "bldockwidget.h"


///Declaracion de globales
ExampleView *g_example;
BlDockWidget *g_dock;

///
/**
\param btpv
**/
int entryPoint ( BtBulmaTPV *btpv )
{
    BlDebug::blDebug ( Q_FUNC_INFO, 0, _("Punto de entrada de PluginBt_Example") );
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbt_example", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    
}


///
/**
\param btpv
**/

int BtCompany_createMainWindows_Post ( BtCompany *btpv )
{
    BL_FUNC_DEBUG
    /// Creamos un widget
    g_example = new ExampleView ( btpv, g_dock );
    QDockWidget *dockExample = new QDockWidget( _("Ejemplo"), g_main);
    
    ///Definimos su geometria y localizacion
    dockExample->setFeatures(QDockWidget::AllDockWidgetFeatures);
    dockExample->setGeometry ( 100, 100, 100, 100 );
    g_main->addDockWidget(Qt::RightDockWidgetArea, dockExample);
    dockExample->setWidget(g_example);
    
    
    
    return 0;
}
