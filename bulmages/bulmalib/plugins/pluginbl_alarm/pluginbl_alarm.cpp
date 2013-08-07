/***************************************************************************
 *   Copyright (C) 2008 by Tomeu Borras Riera                              *
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
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QDesktopServices>

#include "local_blI18n.h"
#include "pluginbl_alarm.h"
#include "blform.h"
#include "blmainwindow.h"
#include "alarmamenu.h"
#include "bltoolbutton.h"
#include "blaction.h"
#include "alarma.h"
#include "visoralarm.h"
#include "bldockwidget.h"
#include "blfile.h"


QAction *viewAlarm;


int entryPoint ( BlMainWindow *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_alarm", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData());
    
    BlMainCompany *main = bcont->company();


    main->dbPatchVersionCheck("PluginBl_Alarm", "1.5.1-0001");
    
    /// Vamos a probar con un docwindow.
    BlDockWidget *doc1 = new BlDockWidget ( "Alarmas", bcont );
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 400 );
    bcont->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );
    doc1->show();

    visoralarm *corr = new visoralarm ( doc1 );
    corr->setMainCompany ( main );
    corr->presentar();
    corr->dock = doc1;

    doc1->setWidget ( corr );
    
    
    /// A&ntilde;ade en el men&uacute; del programa la opci&oacuteMn para
    /// acceder al corrector.
    viewAlarm = new QAction ( _("&Alarmas"), 0 );
    viewAlarm->setCheckable ( true );
    viewAlarm->setStatusTip ( _("Muestra/oculta las alarmas") );
    viewAlarm->setWhatsThis ( _("Alarmas.\n\nMuestra/oculta las alarmas") );

    QObject::connect ( viewAlarm, SIGNAL ( toggled ( bool ) ), doc1, SLOT ( setVisibilityState ( bool ) ) );
    QObject::connect ( doc1, SIGNAL ( visibilityStateChanged ( bool ) ), viewAlarm, SLOT ( setChecked ( bool ) ) );
    
    QMenu *pPluginMenu = bcont->newMenu(_("&Herramientas"), "menuHerramientas", "menuAcerca_de");
    pPluginMenu->addSeparator();    
    pPluginMenu->addAction ( viewAlarm );

    BlFile file ( g_confpr->value( CONF_DIR_USER ) + "pluginalarmbl_" + main->dbName() + ".cfn" );
    if ( file.exists () ) {
        doc1->show();
        viewAlarm->setChecked ( true );
    } else {
        doc1->hide();
        viewAlarm->setChecked ( false );
    } // end if
    
    return 0;
}



int BlForm_load ( BlForm *ficha )
{
    BL_FUNC_DEBUG
    AlQToolButton *eq = ficha->findChild<AlQToolButton *>("BotonAlarm");
    
    /// Comprueba la existencia del objeto.
    if (eq != 0) {
        eq->hazMenu();
    } // end if
    
    return 0;
    
}


int BlForm_DesFicha ( BlForm *ficha )
{
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    BL_FUNC_DEBUG
    return 0;
}



///
/**
\param l
\return
**/
int BlForm_loadSpecs ( BlForm *l )
{
    BL_FUNC_DEBUG
    new AlQToolButton ( l );
    
    return 0;
}



