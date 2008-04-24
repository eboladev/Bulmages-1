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

#include "plugininformes.h"
#include "funcaux.h"
#include "bulmafact.h"
#include "bulmacont.h"
#include "myplugininformes.h"
#include "qworkspace2.h"
#include "ficha.h"

QMainWindow *g_bges = NULL;
EmpresaBase *g_emp = NULL;
///
/**
**/
MyPluginInformes::MyPluginInformes()
{
    _depura ( "MyPluginInformes::MyPluginInformes", 0 );
    _depura ( "END MyPluginInformes::MyPluginInformes", 0 );
}

///
/**
**/
MyPluginInformes::~MyPluginInformes()
{
    _depura ( "MyPluginInformes::~MyPluginInformes", 0 );
    _depura ( "END MyPluginInformes::~MyPluginInformes", 0 );
}




///
/**
**/
void MyPluginInformes::elslot1( )
{
    _depura ( "MyPluginInformes::elslot", 0 );

	    Ficha *ficha = new Ficha(g_emp, 0);
            ficha->generaRML ( sender()->objectName() );
            invocaPDF ( sender()->objectName().left ( sender()->objectName().size() - 4 ) );

    _depura ( "END MyPluginInformes::elslot", 0 );
}

void entryPoint ( QMainWindow *bges )
{
    _depura ( "Entrada del plugin Docket", 0 );

    g_bges = bges;

    MyPluginInformes *mcont = new MyPluginInformes;

    QMenu *pPluginMenu = NULL;
    /// Miramos si existe un menu Herramientas
    pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuInformes" );
    /// En BulmaCont el menu de Herramientas tiene otro nombre.
    if ( !pPluginMenu ) {
	pPluginMenu = bges->menuBar() ->findChild<QMenu *> ( "menuInformes" );
    } // end if
	
    /// Creamos el men&uacute;.
    if ( !pPluginMenu ) {
        pPluginMenu = new QMenu ( "&Informes", bges->menuBar() );
        pPluginMenu->setObjectName ( QString::fromUtf8 ( "Informes" ) );
    } // end if

/*
    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    pPluginMenu->addAction ( accion1 );
*/

//    QMenu *ajust = menu->addMenu ( tr ( "Informes Personales" ) );

    /// Buscamos ficheros que tengan el nombre de la tabla
    QDir dir ( confpr->valor ( CONF_DIR_OPENREPORTS ) );
    dir.setFilter ( QDir::Files | QDir::NoSymLinks );
    dir.setSorting ( QDir::Size | QDir::Reversed );
    /// Hacemos un filtrado de busqueda
    QStringList filters;
    filters << "inf_*.rml";
    dir.setNameFilters ( filters );


    QFileInfoList list = dir.entryInfoList();
    for ( int i = 0; i < list.size(); ++i ) {
        QFileInfo fileInfo = list.at ( i );
	/// Creamos el men&uacute;.
    	QAction *accion = new QAction ( fileInfo.fileName(), 0 );
        accion->setObjectName ( fileInfo.fileName() );
    	accion->setStatusTip ( "Informe" );
    	accion->setWhatsThis ( "Informe" );
	mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );
        pPluginMenu->addAction ( accion );
    } // end for



    /// A&ntilde;adimos la nueva opci&oacute;n al men&uacute; principal del programa.
    bges->menuBar() ->insertMenu ( 0, pPluginMenu );

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

