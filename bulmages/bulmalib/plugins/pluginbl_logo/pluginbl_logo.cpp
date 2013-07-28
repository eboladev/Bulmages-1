/*******************************************f********************************
 *   Copyright (C) 2010 by Tomeu Borras Riera                              *
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
#include <QtCore/QProcess>
#include <QtCore/QTextStream>

#include "local_blI18n.h"
#include "pluginbl_logo.h"
#include "bldockwidget.h"
#include "blworkspace.h"
#include "blform.h"


int entryPoint ( BlMainWindow *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_logo", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    /// Vamos a probar con un docwindow.
    LogoDockWidget *doc1 = new LogoDockWidget ( _("Logotipo"), g_main );
    doc1->setWindowIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_lo32-app-bulmages.png" ) ) );
    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 100 );
    doc1->resize ( 330, 400 );
    g_main->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );
/*
    QLabel *label = new QLabel();
    label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    label->setAlignment(Qt::AlignBottom | Qt::AlignRight);
    label->setPixmap(QPixmap(g_confpr->value(CONF_DIR_OPENREPORTS) + "logo.jpg"));
    label->setScaledContents(true);
    label->setMinimumSize(100, 100);

    doc1->setWidget ( label );
    */


    doc1->setPixmap(QPixmap(g_confpr->value(CONF_DIR_OPENREPORTS) + "logo.jpg"));


    doc1->show();
    doc1->loadConfig();

    

    return 0;
}

