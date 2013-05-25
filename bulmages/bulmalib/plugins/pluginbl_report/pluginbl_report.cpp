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
#include <QtCore/QProcess>
#include <QtCore/QTextStream>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>

#include "local_blI18n.h"
#include "pluginbl_report.h"
#include "plblreport.h"
#include "blworkspace.h"
#include "blform.h"


QMainWindow *g_pluginbl_report = NULL;
BlMainCompany *g_pluginbl_report_emp = NULL;


///
/**
**/
PluginBl_Report::PluginBl_Report()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
PluginBl_Report::~PluginBl_Report()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void PluginBl_Report::elslot2( )
{
    BL_FUNC_DEBUG

    BlForm *ficha = new BlForm ( g_pluginbl_report_emp, 0 );
    if ( !ficha->generateRML ( sender()->objectName() ) ) return;
    
    if (!g_confpr->value( CONF_TICKET_PRINTER_FILE).isEmpty() && g_confpr->value( CONF_TICKET_PRINTER_FILE) != "/dev/null") {
	QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + sender()->objectName() + "  > " + g_confpr->value( CONF_TICKET_PRINTER_FILE );
	system ( comando.toLatin1().data() );
    } else if (!g_confpr->value( CONF_CASHBOX_FILE).isEmpty() && g_confpr->value( CONF_CASHBOX_FILE) != "/dev/null") {
	QString comando = "cat " + g_confpr->value(CONF_DIR_USER) + sender()->objectName() + "  > " + g_confpr->value( CONF_CASHBOX_FILE );
	system ( comando.toLatin1().data() );
    } else if (!g_confpr->value(CONF_CUPS_TICKET_PRINTER).isEmpty() && g_confpr->value(CONF_CUPS_TICKET_PRINTER) != "None") {
		blRawPrint( sender()->objectName(), true, g_confpr->value( CONF_TICKET_PRINTER_FILE));
    } else if (g_confpr->value(CONF_CUPS_DEFAULT_PRINTER).isEmpty() || g_confpr->value(CONF_CUPS_DEFAULT_PRINTER) == "None") {
	blMsgError(_("Debe establecer el parametro 'CONF_CUPS_DEFAULT_PRINTER' o 'CONF_CASHBOX_FILE' para abrir el cajon."));
    } else {
		blRawPrint( sender()->objectName());
    } // end if  
}


///
/**
**/
void PluginBl_Report::elslot1( )
{
    BL_FUNC_DEBUG

    BlForm *ficha = new BlForm ( g_pluginbl_report_emp, 0 );
    if ( !ficha->generateRML ( sender()->objectName() ) ) return;
    blCreateAndLoadPDF ( sender()->objectName().left ( sender()->objectName().size() - 4 ) );
}


int entryPoint ( QMainWindow *bges )
{
    g_pluginbl_report = bges;
    return 0;
}


int init (  )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_report", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    PluginBl_Report *mcont = new PluginBl_Report;

    QMenu *pPluginMenu = NULL;
    /// Miramos si existe un menu Informes
    pPluginMenu = g_pluginbl_report->menuBar()->findChild<QMenu *> ( "menuInformes" );
    QMenu *pPluginVer = g_pluginbl_report->menuBar()->findChild<QMenu *> ( "menuVentana" );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "inf_*.rml";
    dir.setNameFilters ( filters );

    QFileInfoList list = dir.entryInfoList();

    
    for ( int i = 0; i < list.size(); ++i ) {
      
        QFileInfo fileInfo = list.at ( i );

        QFile file;
        file.setFileName ( g_confpr->value( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos el titulo
        QString titulo = fileInfo.fileName();
        QRegExp rx3 ( " title\\s*=\\s*\"(.*)\"" );
        rx3.setMinimal ( true );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
        } // end while

        QString pathtitulo = fileInfo.fileName();
        QRegExp rx1 ( "pathtitle\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( true );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            pathtitulo = rx1.cap ( 1 );
        } else {
	    pathtitulo = titulo;
	} // end while

        /// Buscamos el icono
        QString icon = ":/Images/template2rml.png";
        QRegExp rx6 ( " icon\\s*=\\s*\"(.*)\"" );
        rx6.setMinimal ( true );
        if ( rx6.indexIn ( buff, 0 )  != -1 ) {
            icon = rx6.cap ( 1 );
        } // end while


	QMenuBar *menubar =g_pluginbl_report->menuBar();
	QMenu *menu = NULL;
	QStringList path = pathtitulo.split("\\");


	if (path.size() > 1) {
		    QList<QMenu *> allPButtons = menubar->findChildren<QMenu *>();
		    bool encontrado = false;
		    for (int j = 0; j < allPButtons.size(); ++j) {
			if (allPButtons.at(j)->title() == path[0]) {
			    encontrado = true;
			    menu = allPButtons.at(j);
			} // end if
		    } // end for

		    if (!encontrado) {
			QMenu *pPluginMenu1 = new QMenu (path[0] , menubar );
                        menubar->insertMenu ( pPluginVer->menuAction(), pPluginMenu1 );
			menu = pPluginMenu1;
		    } // end if
	} else {

		    if (!pPluginMenu) {
			    pPluginMenu = new QMenu ( _ ( "Informes" ), g_pluginbl_report->menuBar() );
			    pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuInformes" ) );
			    g_pluginbl_report->menuBar()->insertMenu ( pPluginVer->menuAction(), pPluginMenu );
		    } // end if
		    menu = pPluginMenu;
	} // end if
	


	for (int i = 1; i < path.size()-1; ++i) {
	    QList<QMenu *> allPButtons = menu->findChildren<QMenu *>();
	    bool encontrado = false;
	    for (int j = 0; j < allPButtons.size(); ++j) {
		if (allPButtons.at(j)->title() == path[i]) {
		    encontrado = true;
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

    init1();
    
    return 0;
}


// ========================================================
int init1 (  )
{
    BL_FUNC_DEBUG


    PluginBl_Report *mcont = new PluginBl_Report;

    QMenu *pPluginMenu = NULL;
    /// Miramos si existe un menu Informes
    pPluginMenu = g_pluginbl_report->menuBar()->findChild<QMenu *> ( "menuInformes" );
    QMenu *pPluginVer = g_pluginbl_report->menuBar()->findChild<QMenu *> ( "menuVentana" );

    /// Buscamos ficheros adecuados
    QDir dir ( g_confpr->value( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "inf_*.txt";
    dir.setNameFilters ( filters );

    QFileInfoList list = dir.entryInfoList();

    
    for ( int i = 0; i < list.size(); ++i ) {
      
        QFileInfo fileInfo = list.at ( i );

        QFile file;
        file.setFileName ( g_confpr->value( CONF_DIR_OPENREPORTS ) + fileInfo.fileName() );
        file.open ( QIODevice::ReadOnly );
        QTextStream stream ( &file );
        QString buff = stream.readAll();
        file.close();

        /// Buscamos el titulo
        QString titulo = fileInfo.fileName();
        QRegExp rx3 ( " title\\s*=\\s*\"(.*)\"" );
        rx3.setMinimal ( true );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
        } // end while

        QString pathtitulo = fileInfo.fileName();
        QRegExp rx1 ( "pathtitle\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( true );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            pathtitulo = rx1.cap ( 1 );
        } else {
	    pathtitulo = titulo;
	} // end while

        /// Buscamos el icono
        QString icon = ":/Images/template2rml.png";
        QRegExp rx6 ( " icon\\s*=\\s*\"(.*)\"" );
        rx6.setMinimal ( true );
        if ( rx6.indexIn ( buff, 0 )  != -1 ) {
            icon = rx6.cap ( 1 );
        } // end while


	QMenuBar *menubar =g_pluginbl_report->menuBar();
	QMenu *menu = NULL;
	QStringList path = pathtitulo.split("\\");


	if (path.size() > 1) {
		    QList<QMenu *> allPButtons = menubar->findChildren<QMenu *>();
		    bool encontrado = false;
		    for (int j = 0; j < allPButtons.size(); ++j) {
			if (allPButtons.at(j)->title() == path[0]) {
			    encontrado = true;
			    menu = allPButtons.at(j);
			} // end if
		    } // end for

		    if (!encontrado) {
			QMenu *pPluginMenu1 = new QMenu (path[0] , menubar );
                        menubar->insertMenu ( pPluginVer->menuAction(), pPluginMenu1 );
			menu = pPluginMenu1;
		    } // end if
	} else {

		    if (!pPluginMenu) {
			    pPluginMenu = new QMenu ( _ ( "Informes" ), g_pluginbl_report->menuBar() );
			    pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuInformes" ) );
			    g_pluginbl_report->menuBar()->insertMenu ( pPluginVer->menuAction(), pPluginMenu );
		    } // end if
		    menu = pPluginMenu;
	} // end if
	


	for (int i = 1; i < path.size()-1; ++i) {
	    QList<QMenu *> allPButtons = menu->findChildren<QMenu *>();
	    bool encontrado = false;
	    for (int j = 0; j < allPButtons.size(); ++j) {
		if (allPButtons.at(j)->title() == path[i]) {
		    encontrado = true;
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
        mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot2() ) );
        menu->addAction ( accion );
    } // end for

    return 0;
}
// ========================================================

int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    g_pluginbl_report_emp = cmp;
    init();
    return 0;
}


int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_pluginbl_report_emp = cmp;
    init();
    return 0;
}
