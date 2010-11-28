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

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QProcess>
#include <QTextStream>

#include "local_blI18n.h"
#include "pluginbl_report2ods.h"
#include "bfbulmafact.h"
#include "bcbulmacont.h"
#include "myplugininformesods.h"
#include "blworkspace.h"
#include "blform.h"


QMainWindow *g_bges = NULL;
BlMainCompany *g_emp = NULL;


///
/**
**/
MyPluginInformesODS::MyPluginInformesODS()
{
    blDebug ( "MyPluginInformesODS::MyPluginInformesODS", 0 );
    blDebug ( "END MyPluginInformesODS::MyPluginInformesODS", 0 );
}

///
/**
**/
MyPluginInformesODS::~MyPluginInformesODS()
{
    blDebug ( "MyPluginInformesODS::~MyPluginInformesODS", 0 );
    blDebug ( "END MyPluginInformesODS::~MyPluginInformesODS", 0 );
}




///
/**
**/
void MyPluginInformesODS::elslot1( )
{
    blDebug ( "MyPluginInformesODS::elslot", 0 );

    BlForm *ficha = new BlForm ( g_emp, 0 );

    if ( !ficha->generaRML ( sender()->objectName() ) ) return;

    QString archivod = sender()->objectName();
    QString archivo =  sender()->objectName().left ( sender()->objectName().size() - 4 );
    blCreateAndLoadODS ( archivo );

    delete ficha;
    blDebug ( "END MyPluginInformesODS::elslot", 0 );
}


int entryPoint ( QMainWindow *bges )
{
    g_bges = bges;
    return 0;
}


int init (  )
{
    blDebug ( "Entrada del plugin ", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_report2ods", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );



    MyPluginInformesODS *mcont = new MyPluginInformesODS;

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
    filters << "inf_*.pys";
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
        QRegExp rx3 ( "title\\s*=\\s*\"(.*)\"" );
        rx3.setMinimal ( TRUE );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
        } // end if

        QString pathtitulo = fileInfo.fileName();
        QRegExp rx1 ( "pathtitle\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( TRUE );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            pathtitulo = rx1.cap ( 1 );
	} else {
	    pathtitulo = titulo;
        } // end if

        /// Buscamos el icono
        QString icon = ":/Images/template2ods.png";
        QRegExp rx4 ( " icon\\s*=\\s*\"(.*)\"" );
        rx4.setMinimal ( TRUE );
        if ( rx4.indexIn ( buff, 0 )  != -1 ) {
            icon = rx4.cap ( 1 );
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
        mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );
        menu->addAction ( accion );
    } // end for

    blDebug ( "Iniciado correctamente el plugin", 10 );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    g_emp = cmp;
    init();
    return 0;
}


int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_emp = cmp;
    init();
    return 0;
}

