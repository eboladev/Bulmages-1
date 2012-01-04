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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>

#include "pluginbc_corrector.h"
#include "correctorwidget.h"
#include "bccompany.h"
#include "bldockwidget.h"


BlDockWidget *doc1;
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
    blBindTextDomain ( "pluginbc_corrector", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    BcCompany *emp = bcont->company();
    /// Vamos a probar con un docwindow.
    doc1 = new BlDockWidget ( _ ( "Corrector" ), bcont );
    doc1->setObjectName("mui_corrector");
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 400 );
    bcont->addDockWidget ( Qt::RightDockWidgetArea, doc1 );
    doc1->show();

    correctorwidget *corr = new correctorwidget ( doc1 );
    corr->setCompany  ( emp );
    corr->dock = doc1;

    doc1->setWidget ( corr );
    
    QMenu *pPluginMenu = bcont->newMenu(_("&Ver"), "menuVer", "menuMaestro");

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewCorrector = new QAction ( _ ( "&Corrector" ), 0 );
    viewCorrector->setCheckable ( TRUE );

    viewCorrector->setStatusTip ( _ ( "Muestra/oculta el corrector" ) );
    viewCorrector->setWhatsThis ( _ ( "Corrector.\n\nMuestra/oculta el corrector" ) );

    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), corr, SLOT ( cambia ( bool ) ) );
    QObject::connect ( doc1, SIGNAL ( visibilityStateChanged ( bool ) ), corr, SLOT ( cambia ( bool ) ) );

    pPluginMenu ->addSeparator();
    pPluginMenu ->addAction ( viewCorrector );

    /// Mientras no haya icono para el corrector lo mantengo quitado.
//    bcont->toolBar->addAction ( viewCorrector );

    corr->m_viewCorrector = viewCorrector;

    QFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginbc_corrector_" + emp->dbName() + ".cfn" );
    if ( file.exists () ) {
        doc1->show();
        viewCorrector->setChecked ( TRUE );
    } else {
        doc1->hide();
        viewCorrector->setChecked ( FALSE );
    } // end if

    
    return 0;
}

///
/**
\param bcont
**/
int BcBulmaCont_closeEvent ( BcBulmaCont *bcont )
{
    BcCompany * emp = bcont->company();
    QFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginbc_corrector_" + emp->dbName() + ".cfn" );
    if ( !viewCorrector->isChecked() ) {
        file.remove();
    } else {
        file.open ( QIODevice::WriteOnly );
        file.close();
    } // end if
    return 0;
}
