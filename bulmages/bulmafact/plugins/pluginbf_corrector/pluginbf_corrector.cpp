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
#include <QFile>

#include "pluginbf_corrector.h"
#include "correctorwidget.h"
#include "bfcompany.h"
#include "bldockwidget.h"


BlDockWidget *doc1;
QAction *viewCorrector;


///
/**
\param bcont
**/
int entryPoint ( BfBulmaFact *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_corrector", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    BlMainCompany *emp = bcont->company();
    /// Vamos a probar con un docwindow.
    doc1 = new BlDockWidget ( "Corrector", bcont );
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 400 );
    bcont->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );
    doc1->show();

    correctorwidget *corr = new correctorwidget ( doc1 );
    corr->setMainCompany ( emp );
    corr->dock = doc1;

    doc1->setWidget ( corr );

    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewCorrector = new QAction ( _("&Corrector"), 0 );
    viewCorrector->setCheckable ( TRUE );
    viewCorrector->setStatusTip ( _("Muestra/oculta el corrector") );
    viewCorrector->setWhatsThis ( _("Corrector.\n\nMuestra/oculta el corrector") );

    QObject::connect ( viewCorrector, SIGNAL ( toggled ( bool ) ), doc1, SLOT ( setVisibilityState ( bool ) ) );
    QObject::connect ( doc1, SIGNAL ( visibilityStateChanged ( bool ) ), viewCorrector, SLOT ( setChecked ( bool ) ) );
    bcont->menuVentana->addSeparator();
    bcont->menuVentana->addAction ( viewCorrector );

    QFile file ( g_confpr->value( CONF_DIR_USER ) + "plugincorrectorbf_" + emp->dbName() + ".cfn" );
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
int BfBulmaFact_closeEvent ( BfBulmaFact *bcont )
{
    BlMainCompany * emp = bcont->company();
    QFile file ( g_confpr->value( CONF_DIR_USER ) + "plugincorrectorbf_" + emp->dbName() + ".cfn" );
    if ( !viewCorrector->isChecked() ) {
        file.remove();
    } else {
        file.open ( QIODevice::WriteOnly );
        file.close();
    } // end if
    return 0;
}
