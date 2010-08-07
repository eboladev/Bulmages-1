/*******************************************f********************************
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

#ifndef WIN32


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
#include <QTextStream>

#include "local_BlI18n.h"
#include "pluginbl_embed.h"
#include "bfbulmafact.h"
#include "bcbulmacont.h"
#include "myplugindock.h"
#include "bldockwidget.h"
#include "blworkspace.h"
#include "bx11embedcontainer.h"
#include "blform.h"

QMainWindow *g_bges = NULL;
BlMainCompany *g_emp = NULL;


///
/**
**/

/*
MyPluginEmbedODS::MyPluginEmbedODS()
{
    _depura ( "MyPluginEmbedODS::MyPluginEmbedODS", 0 );
    _depura ( "END MyPluginEmbedODS::MyPluginEmbedODS", 0 );
}
*/


///
/**
**/

/*
MyPluginEmbedODS::~MyPluginEmbedODS()
{
    _depura ( "MyPluginEmbedODS::~MyPluginEmbedODS", 0 );
    _depura ( "END MyPluginEmbedODS::~MyPluginEmbedODS", 0 );
}
*/



///
/**
**/

void myplugincont::embedPYS( )
{
    _depura ( "MyPluginEmbedODS::elslot", 0 );

    BlForm *ficha = new BlForm ( g_emp, 0 );

    if ( !ficha->generaRML ( sender()->objectName() ) ) return;

    QString archivod = sender()->objectName();
    QString archivo =  sender()->objectName().left ( sender()->objectName().size() - 4 );

    delete ficha;


        QFile file;
        file.setFileName ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) + archivod );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos el titulo
        QString titulo = archivod;
        QRegExp rx3 ( " title\\s*=\\s*\"(.*)\"" );
        rx3.setMinimal ( TRUE );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
        } // end if
    
        /// Buscamos el icono
        QString icon = ":/Images/externalapp.png" ;
        QRegExp rx4 ( "icon\\s*=\\s*\"(.*)\"" );
        rx4.setMinimal ( TRUE );
        if ( rx4.indexIn ( buff, 0 )  != -1 ) {
            icon = rx4.cap ( 1 );
        } // end if
    
    
    
    QDir::setCurrent ( g_confpr->valor ( CONF_DIR_USER ) );
    QString cadsys;

    QString cadena = " python " + archivo + ".pys";
    
    
    QProcess *m_proc = new QProcess();

    m_proc->start ( cadena );
    if ( !m_proc->waitForStarted() ) {
        return;
    } // end if
    m_proc->waitForReadyRead();
    QString winId = "";
    winId = m_proc->readAllStandardOutput();

    BlWorkspace *work = g_bges ->findChild<BlWorkspace *> (  );
    if ( work ) {
        BX11EmbedContainer * container = new BX11EmbedContainer ( g_emp, work );
        container->setAttribute ( Qt::WA_DeleteOnClose );
        container->setWindowIcon ( QIcon ( icon ) );
        work->addWindow ( container );
        if ( g_emp )
            g_emp->meteWindow ( titulo, container, FALSE );
        container->embedClient ( winId.toInt() );
        container->show();
    } // end if
    
    _depura ( "END MyPluginEmbedODS::elslot", 0 );
}


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



int entryPoint ( BlMainWindow *bges )
{
    _depura ( "Entrada del plugin Docket", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbl_embed", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_bges = bges;

    myplugincont *mcont = new myplugincont;

    /// Creamos el men&uacute;.
    QAction *accion = new QAction ( _ ( "&Dock Aplicacion" ), 0 );
    accion->setStatusTip ( _ ( "Dockear Aplicacion" ) );
    accion->setWhatsThis ( _ ( "Dockear Aplicacion" ) );

    /// Creamos el men&uacute;.
    QAction *accion1 = new QAction ( _ ( "&Embeber Aplicacion" ), 0 );
    accion1->setStatusTip ( _ ( "Embeber Aplicacion" ) );
    accion1->setWhatsThis ( _ ( "Embeber Aplicacion" ) );

    mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot() ) );
    mcont->connect ( accion1, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );

    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu ( "&Herramientas", "menuHerramientas", "menuAcerca_de" );

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    pPluginMenu->addAction ( accion1 );

    _depura ( "Iniciado correctamente el plugin dock", 10 );
    return 0;
}


int init67 (  )
{


    myplugincont *mcont = new myplugincont;
    

    QMenu *pPluginMenu = NULL;
    /// Miramos si existe un menu Informes
    pPluginMenu = g_bges->menuBar() ->findChild<QMenu *> ( "menuInformesODS" );
    QMenu *pPluginVer = g_bges->menuBar()->findChild<QMenu *> ( "menuVentana" );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "embed_*.pys";
    dir.setNameFilters ( filters );

    QFileInfoList list = dir.entryInfoList();

    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );

        QFile file;
        file.setFileName ( g_confpr->valor ( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos el titulo
        QString titulo = fileInfo.fileName();
        QRegExp rx3 ( " title\\s*=\\s*\"(.*)\"" );
        rx3.setMinimal ( TRUE );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
	    
        } // end if


        /// Buscamos el titulo
        QString icon = ":/Images/externalapp.png";
        QRegExp rx4 ( " icon\\s*=\\s*\"(.*)\"" );
        rx4.setMinimal ( TRUE );
        if ( rx4.indexIn ( buff, 0 )  != -1 ) {
            icon = rx4.cap ( 1 );
            
        } // end if

        QString pathtitulo = fileInfo.fileName();
        QRegExp rx1 ( "pathtitle\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( TRUE );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            pathtitulo = rx1.cap ( 1 );
	} else {
	    pathtitulo = titulo;
        } // end if


	QMenuBar *menubar =g_bges->menuBar();
	QMenu *menu = NULL;
	QStringList path = pathtitulo.split("\\");


	if (path.size() > 1) {
		    QList<QMenu *> allPButtons = menubar->findChildren<QMenu *>();
		    bool encontrado = FALSE;
		    for (int j = 0; j < allPButtons.size(); ++j) {
			if (allPButtons.at(j)->title() == path[0]) {
			    encontrado = TRUE;
			    menu = allPButtons.at(j);
			} // end if
		    } // end for

		    if (!encontrado) {
			QMenu *pPluginMenu1 = new QMenu (  path[0] , menubar );
			menubar->insertMenu ( pPluginVer->menuAction(), pPluginMenu1 );
			menu = pPluginMenu1;
		    } // end if
	} else {

		    if (!pPluginMenu) {
			    pPluginMenu = new QMenu ( _ ( "Informes &ODS" ), g_bges->menuBar() );
			    pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuInformesODS" ) );
			    g_bges->menuBar()->insertMenu ( pPluginVer->menuAction(), pPluginMenu );
		    } // end if
		    menu = pPluginMenu;
	} // end if
	


	for (int i = 1; i < path.size()-1; ++i) {
	    QList<QMenu *> allPButtons = menu->findChildren<QMenu *>();
	    bool encontrado = FALSE;
	    for (int j = 0; j < allPButtons.size(); ++j) {
		if (allPButtons.at(j)->title() == path[i]) {
		    encontrado = TRUE;
		    menu = allPButtons.at(j);
		} // end if
	    } // end for

	    if (!encontrado) {
		QMenu *pPluginMenu1 = new QMenu ( path[i] , menu );
		menu->addMenu (  pPluginMenu1 );
		menu = pPluginMenu1;
	    } // end if

	} // end for

        /// Creamos el men&uacute;.
        QAction *accion = new QAction ( path[path.size()-1], 0 );
        accion->setIcon(QIcon(icon));
        accion->setObjectName ( fileInfo.fileName() );
        accion->setStatusTip ( titulo);
        accion->setWhatsThis ( titulo );
        mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( embedPYS() ) );
        menu->addAction ( accion );
    } // end for

    _depura ( "Iniciado correctamente el plugin", 10 );
    

    return 0;
    
}
   
   
   
int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    g_emp = cmp;
    init67();
    return 0;
}

int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_emp = cmp;
    init67();
    return 0;
}

#endif

