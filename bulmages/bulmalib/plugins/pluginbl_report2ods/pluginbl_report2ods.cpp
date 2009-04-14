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

#include "pluginbl_report2ods.h"
#include "bfbulmafact.h"
#include "bulmacont.h"
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
    _depura ( "MyPluginInformesODS::MyPluginInformesODS", 0 );
    _depura ( "END MyPluginInformesODS::MyPluginInformesODS", 0 );
}

///
/**
**/
MyPluginInformesODS::~MyPluginInformesODS()
{
    _depura ( "MyPluginInformesODS::~MyPluginInformesODS", 0 );
    _depura ( "END MyPluginInformesODS::~MyPluginInformesODS", 0 );
}




///
/**
**/
void MyPluginInformesODS::elslot1( )
{
    _depura ( "MyPluginInformesODS::elslot", 0 );

    BlForm *ficha = new BlForm ( g_emp, 0 );

    if ( !ficha->generaRML ( sender()->objectName() ) ) return;

    QString archivod = sender()->objectName();
    QString archivo =  sender()->objectName().left ( sender()->objectName().size() - 4 );
    invocaPYS ( archivo );

    /*
        QString cadena = "rm " + g_confpr->valor ( CONF_DIR_USER ) + archivo + ".ods";
        system ( cadena.toAscii() );

        cadena = " cd " + g_confpr->valor ( CONF_DIR_USER ) + "; python " + archivod;
        system ( cadena.toAscii() );
        cadena = "oocalc " + g_confpr->valor ( CONF_DIR_USER ) + archivo + ".ods &";
        system ( cadena.toAscii() );
    */

    delete ficha;
    _depura ( "END MyPluginInformesODS::elslot", 0 );
}


int entryPoint ( QMainWindow *bges )
{
    _depura ( "Entrada del plugin Docket", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbl_report2ods", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_bges = bges;

    MyPluginInformesODS *mcont = new MyPluginInformesODS;

    QMenu *pPluginMenu = NULL;
    /// Miramos si existe un menu Informes
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuInformesODS" );

    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        QMenu *pPluginVer = bges->menuBar()->findChild<QMenu *> ( "menuVentana" );
        pPluginMenu = new QMenu ( _ ( "Informes &ODS" ), bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "menuInformesODS" ) );
        bges->menuBar()->insertMenu ( pPluginVer->menuAction(), pPluginMenu );
    } // end if

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
        QRegExp rx1 ( "title\\s*=\\s*\"(.*)\"" );
        rx1.setMinimal ( TRUE );
        if ( rx1.indexIn ( buff, 0 )  != -1 ) {
            titulo = rx1.cap ( 1 );
        } // end while

        /// Creamos el men&uacute;.
        QAction *accion = new QAction ( titulo, 0 );
        accion->setObjectName ( fileInfo.fileName() );
        accion->setStatusTip ( _ ( "Informe" ) );
        accion->setWhatsThis ( _ ( "Informe" ) );
        mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );
        pPluginMenu->addAction ( accion );
    } // end for

    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
//    bges->menuBar() ->insertMenu ( 0, pPluginMenu );
    _depura ( "Iniciado correctamente el plugin dock", 10 );
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    g_emp = cmp;
    return 0;
}


int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_emp = cmp;
    return 0;
}

