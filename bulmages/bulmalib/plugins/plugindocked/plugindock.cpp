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
#include <QProcess>
#include <QX11EmbedContainer>

#include "plugindock.h"
#include "bulmafact.h"
#include "bulmacont.h"
#include "myplugindock.h"
#include "bldockwidget.h"
#include "blworkspace.h"
#include "bx11embedcontainer.h"


QMainWindow *g_bges = NULL;
EmpresaBase *g_emp = NULL;


///
/**
**/
myplugincont::myplugincont()
{
    _depura ( "myplugincont::myplugincont", 0 );
    _depura ( "END myplugincont::myplugincont", 0 );
}

///
/**
**/
myplugincont::~myplugincont()
{
    _depura ( "myplugincont::~myplugincont", 0 );
    _depura ( "END myplugincont::~myplugincont", 0 );
}


///
/**
**/
void myplugincont::elslot()
{
    _depura ( "myplugincont::elslot", 0 );

    /// Vamos a probar con un docwindow.
    BlDockWidget *doc1 = new BlDockWidget ( "Aplicacion Externa", g_main );
    doc1->setWindowIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_lo32-app-bulmages.png" ) ) );

    doc1->setFeatures ( QDockWidget::AllDockWidgetFeatures );

    doc1->setGeometry ( 100, 100, 100, 500 );
    doc1->resize ( 330, 400 );
    g_main->addDockWidget ( Qt::LeftDockWidgetArea, doc1 );
    doc1->show();

    QString winId = "";
    while ( winId == "" ) winId = windowID ( "" );

    QX11EmbedContainer *container = new QX11EmbedContainer ( doc1 );
    container->embedClient ( winId.toInt() );
    doc1->setWidget ( container );

    _depura ( "END myplugincont::elslot", 0 );
}

///
/**
**/
void myplugincont::elslot1()
{
    _depura ( "myplugincont::elslot", 0 );
    QString winId = "";
    while ( winId == "" ) winId = windowID ( "" );
    BlWorkspace *work = g_bges ->findChild<BlWorkspace *> (  );
    if ( work ) {
        BX11EmbedContainer * container = new BX11EmbedContainer ( g_emp, work );
        container->setAttribute ( Qt::WA_DeleteOnClose );
        container->setWindowIcon ( QIcon ( QString::fromUtf8 ( ":/BulmaCont32x32/images/png/i_lo32-app-bulmages.png" ) ) );
        work->addWindow ( container );
        if ( g_emp )
            g_emp->meteWindow ( "Aplicacion Externa", container, FALSE );
        container->embedClient ( winId.toInt() );
        container->show();
    } // end if

    _depura ( "END myplugincont::elslot", 0 );
}



void entryPoint ( BlMainWindow *bges )
{
    _depura ( "Entrada del plugin Docket", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("plugindocked", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    g_bges = bges;

    myplugincont *mcont = new myplugincont;

    /// Creamos el men&uacute;.
    QAction *accion = new QAction ( _("&Dock Aplicacion"), 0 );
    accion->setStatusTip ( _("Dockear Aplicacion") );
    accion->setWhatsThis ( _("Dockear Aplicacion") );

    /// Creamos el men&uacute;.
    QAction *accion1 = new QAction ( _("&Embeber Aplicacion"), 0 );
    accion1->setStatusTip ( _("Embeber Aplicacion") );
    accion1->setWhatsThis ( _("Embeber Aplicacion") );

    mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot() ) );
    mcont->connect ( accion1, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );

    /// Miramos si existe un menu Herramientas
	QMenu *pPluginMenu = bges->newMenu("&Herramientas", "menuHerramientas", "menuAcerca_de");

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    pPluginMenu->addAction ( accion1 );

    _depura ( "Iniciado correctamente el plugin dock", 10 );
}




int Company_createMainWindows_Post ( Company *cmp )
{
    g_emp = cmp;
    return 0;
}

int Empresa_createMainWindows_Post ( Empresa *cmp )
{
    g_emp = cmp;
    return 0;
}

