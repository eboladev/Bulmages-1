/***************************************************************************
 *   Copyright (C) 2011 by Tomeu Borras Riera                              *
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


#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtWidgets/QMenu>

#include <QWebView>

#include "pluginbl_feeds.h"
#include "visorhtml.h"
#include "bfcompany.h"
#include "bldockwidget.h"


BlDockWidget *g_docFeedsWidget;
QAction *g_actionFeedsWidget;


///
/**
\param bcont
**/
int entryPoint ( BlMainWindow *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_feeds", g_confpr->value ( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// Vamos a probar con un docwindow.
    g_docFeedsWidget = new BlDockWidget ( "Info", bcont );
    g_docFeedsWidget->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    g_docFeedsWidget->setGeometry ( 100, 100, 100, 500 );
    g_docFeedsWidget->resize ( 330, 400 );
    bcont->addDockWidget ( Qt::RightDockWidgetArea, g_docFeedsWidget );
    g_docFeedsWidget->show();


    visorhtml *corr = new visorhtml ( g_docFeedsWidget );
    corr->dock = g_docFeedsWidget;

    g_docFeedsWidget->setWidget ( corr );
    
    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    g_actionFeedsWidget = new QAction ( "&Info", 0 );
    g_actionFeedsWidget->setCheckable ( true );
    g_actionFeedsWidget->setStatusTip ( "Muestra/oculta las notificaciones" );
    g_actionFeedsWidget->setWhatsThis ( "Feeds.\n\nMuestra/oculta notificaciones" );

    g_docFeedsWidget->show();
    g_actionFeedsWidget->setChecked ( true );

    return 0;
}

