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
#include <QMenu>
#include <QMenuBar>
#include <QSqlDatabase>
#include <QSqlRecord>

#include "local_blI18n.h"
#include "pluginbl_exaro.h"
#include "blworkspace.h"
#include "blform.h"


/// Usaremos exaro
#include "reportengine.h"


QMainWindow *g_pluginbl_report = NULL;
BlMainCompany *g_pluginbl_report_emp = NULL;


#ifdef NO_COMPILAR_EXARO

class PluginBl_Exaro : public QObject
{
    Q_OBJECT

public:
    PluginBl_Exaro();
    virtual ~PluginBl_Exaro();

public slots:
    virtual void elslot1();
};


///
/**
**/
PluginBl_Exaro::PluginBl_Exaro()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
PluginBl_Exaro::~PluginBl_Exaro()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void PluginBl_Exaro::elslot1( )
{
    BL_FUNC_DEBUG
    BlForm *ficha = new BlForm ( g_pluginbl_report_emp, 0 );
    if ( !ficha->generateRML ( sender()->objectName() ) ) return;
    blCreateAndLoadPDF ( sender()->objectName().left ( sender()->objectName().size() - 4 ) );
}

#endif

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
    blBindTextDomain ( "pluginbl_exaro", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

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
    filters << "inf_*.bdrt";
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
        rx3.setMinimal ( TRUE );
        if ( rx3.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx3.cap ( 1 );
        } // end while

        QString pathtitulo = fileInfo.fileName();
        QRegExp rx1 ( "pathtitle\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( TRUE );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            pathtitulo = rx1.cap ( 1 );
        } else {
	    pathtitulo = titulo;
	} // end while


        /// Buscamos el icono
        QString icon = ":/Images/template2rml.png";
        QRegExp rx6 ( " icon\\s*=\\s*\"(.*)\"" );
        rx6.setMinimal ( TRUE );
        if ( rx6.indexIn ( buff, 0 )  != -1 ) {
            icon = rx6.cap ( 1 );
        } // end while


	QMenuBar *menubar =g_pluginbl_report->menuBar();
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
        BlAction *accion = new BlAction ( path[path.size()-1], 0 );
        accion->setIcon(QIcon(icon));
        accion->setObjectName ( "exaro_" + fileInfo.fileName() );
        accion->setStatusTip ( titulo);
        accion->setWhatsThis ( titulo );
        menu->addAction ( accion );
    } // end for

    
    return 0;
}

int BlAction_actionTriggered(BlAction *accion) 
{
    BL_FUNC_DEBUG
    if (accion->objectName().startsWith("exaro_") ) {
	QString filename = accion->objectName().right(accion->objectName().size()-6);
	
        QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	db.setDatabaseName(g_pluginbl_report_emp->dbName()); // setup a database
	db.setHostName(g_confpr->value( CONF_SERVIDOR ));
	if ( g_confpr->value( CONF_SERVIDOR ) == "--" ) {
	  db.setHostName("localhost");
	} // end if
	db.setPort(g_confpr->value( CONF_PUERTO ).toInt());
	db.setUserName(g_pluginbl_report_emp->currentUser()); // setup a database
	db.setPassword(g_confpr->value( CONF_PASSWORD_USER ));
	
        if (!db.open())
	{
		qCritical()<<"Error: Can't open database";
		return -1;
	}

        Report::ReportInterface* m_report=0;
        Report::ReportEngine m_reportEngine;
	m_report = m_reportEngine.loadReport(g_confpr->value(CONF_DIR_OPENREPORTS) + filename); // open report

        if (!m_report)
	{
		qCritical()<<"Error: Can't open the report";
		return -1;
	}

        m_report->setDatabase(db); // sets the report database

        if (!m_report->exec()) // and finaly, exec report
	{
		qCritical()<<"Error: Can't exec the report";
                delete m_report;
                return -1;
        }
        delete m_report;

	
	
    } // end if
    
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    BL_FUNC_DEBUG
    g_pluginbl_report_emp = cmp;
    init();
    
    return 0;
}


int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    BL_FUNC_DEBUG
    g_pluginbl_report_emp = cmp;
    init();
    return 0;
}
