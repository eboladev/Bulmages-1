/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2012 by Fco. Javier M. C.                               *
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

#include <stdio.h>

#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>

#include "pluginbc_corrector.h"
#include "correctorwidget.h"
#include "bccompany.h"
#include "bldockwidget.h"


BlDockWidget *dockWidget;
QAction *viewCorrector;


///
/**
\param bcont
**/
int entryPoint ( BcBulmaCont *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_corrector", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    BcCompany *company = bcont->company();
    /// Vamos a probar con un docwindow.
    dockWidget = new BlDockWidget ( _ ( "Corrector" ), bcont );
    dockWidget->setObjectName("mui_corrector");
    dockWidget->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    dockWidget->setGeometry ( 100, 100, 100, 500 );
    dockWidget->resize ( 330, 400 );
    bcont->addDockWidget ( Qt::RightDockWidgetArea, dockWidget );
    dockWidget->show();

    CorrectorWidget *corrector = new CorrectorWidget ( dockWidget );
    corrector->setCompany  ( company );
    corrector->m_dockWidget = dockWidget;

    dockWidget->setWidget ( corrector );
    
    QMenu *pPluginMenu = bcont->newMenu(_("&Ver"), "menuVer", "menuMaestro");

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewCorrector = new QAction ( _ ( "&Corrector" ), 0 );
    viewCorrector->setCheckable ( true );

    viewCorrector->setStatusTip ( _ ( "Muestra/oculta el corrector" ) );
    viewCorrector->setWhatsThis ( _ ( "Corrector.\n\nMuestra/oculta el corrector" ) );

    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), corrector, SLOT ( cambia ( bool ) ) );
    QObject::connect ( dockWidget, SIGNAL ( visibilityStateChanged ( bool ) ), corrector, SLOT ( cambia ( bool ) ) );

    pPluginMenu ->addSeparator();
    pPluginMenu ->addAction ( viewCorrector );

    /// Mientras no haya icono para el corrector lo mantengo quitado.
//    bcont->toolBar->addAction ( viewCorrector );

    corrector->m_viewCorrector = viewCorrector;

    QFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginbc_corrector_" + company->dbName() + ".cfn" );
    if ( file.exists () ) {
        dockWidget->show();
        viewCorrector->setChecked ( true );
    } else {
        dockWidget->hide();
        viewCorrector->setChecked ( false );
    } // end if

    
    return 0;
}

///
/**
\param bcont
**/
int BcBulmaCont_closeEvent ( BcBulmaCont *bcont )
{
    BcCompany *company = bcont->company();
    QFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginbc_corrector_" + company->dbName() + ".cfn" );
    if ( !viewCorrector->isChecked() ) {
        file.remove();
    } else {
        file.open ( QIODevice::WriteOnly );
        file.close();
    } // end if
    return 0;
}
