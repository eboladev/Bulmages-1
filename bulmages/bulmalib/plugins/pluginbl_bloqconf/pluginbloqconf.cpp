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
#include <fstream>
#include <iostream>

#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QProcess>
#include <QTextStream>

#include "pluginbloqconf.h"
#include "bfbulmafact.h"
#include "bcbulmacont.h"
#include "myplugingbloqconf.h"
#include "blworkspace.h"
#include "blform.h"

QMainWindow *g_bges = NULL;
BlMainCompany *g_emp = NULL;


///
/**
**/
MyPluginBloqConf::MyPluginBloqConf()
{
    _depura ( "MyPluginBloqConf::MyPluginBloqConf", 0 );
    _depura ( "END MyPluginBloqConf::MyPluginBloqConf", 0 );
}

///
/**
**/
MyPluginBloqConf::~MyPluginBloqConf()
{
    _depura ( "MyPluginBloqConf::~MyPluginBloqConf", 0 );
    _depura ( "END MyPluginBloqConf::~MyPluginBloqConf", 0 );
}




///
/**
**/
void MyPluginBloqConf::elslot( )
{
    _depura ( "MyPluginBloqConf::elslot", 0 );

    QString cad = "chmod a-w " + g_confpr->valor(CONF_DIR_USER) + "*.cfn";
    system ( cad.toAscii().constData() );
    mensajeInfo("Configuraciones Bloqueadas. Se mantendra la configuracion establecida al cargar el programa. Cualquier configuracion posterior a la carga del programa se perdera.");
    _depura ( "END MyPluginBloqConf::elslot", 0 );
}

///
/**
**/
void MyPluginBloqConf::elslot1( )
{
    _depura ( "MyPluginBloqConf::elslot", 0 );

    QString cad = "chmod a+w " + g_confpr->valor(CONF_DIR_USER) + "*.cfn";
    system ( cad.toAscii().constData() );
    mensajeInfo("Configuraciones Desbloqueadas. Las configuraciones se guardaran al cerrar el programa.");

    _depura ( "END MyPluginBloqConf::elslot", 0 );
}



///
/**
\param menu
**/
void MyPluginBloqConf::s_pintaMenu ( QMenu *menu )
{
    _depura ( "MyPluginBloqConf::s_pintaMenu", 0 );
    
    BlSubForm *sub = (BlSubForm *) sender();
    int modo = !sub->modoEdicion();
    
    
    menu->addSeparator();
    QAction * ac = new QAction("Bloqueo Configuracion" , menu);
    ac -> setCheckable(TRUE);
            QString archivo = g_confpr->valor ( CONF_DIR_USER ) + sub->fileConfig() + "_" + sub->mainCompany()->dbName() + "_" + QString::number ( modo ) + "_tablecfn.cfn";
            QFile file ( archivo );
            if (file.permissions() & QFile::WriteOwner) {
              ac->setChecked(FALSE);
            } else {
              ac->setChecked(TRUE);
            } // end if
    
    
    menu->addAction ( ac );
    _depura ( "END MyPluginBloqConf::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPluginBloqConf::s_trataMenu ( QAction *action )
{
    _depura ( "MyPluginBloqConf::s_trataMenu", 0 );
    if ( action->text() == _ ( "Bloqueo Configuracion" ) ) {
            BlSubForm *sub = (BlSubForm *) sender();
            int modo = !sub->modoEdicion();
            QString archivo = g_confpr->valor ( CONF_DIR_USER ) + sub->fileConfig() + "_" + sub->mainCompany()->dbName() + "_" + QString::number ( modo ) + "_tablecfn.cfn";
            QFile file ( archivo );
            if (file.permissions() & QFile::WriteOwner) {
              sub->guardaconfig();
              QString cad = "chmod a-w " + archivo;
              system ( cad.toAscii().constData() );
            } else {
              sub->guardaconfig();
              QString cad = "chmod a+w " + archivo;
              system ( cad.toAscii().constData() );
            } // end if
    } // end if
    _depura ( "END MyPluginBloqConf::s_trataMenu", 0 );
}



int entryPoint ( BlMainWindow *bges )
{
    _depura ( "Entrada del plugin Bloqconf", 0 );

    g_bges = bges;

    MyPluginBloqConf *mcont = new MyPluginBloqConf;



    /// Creamos el men&uacute;.
    QAction *accion = new QAction ( "&Bloquear Configuraciones", 0 );
    accion->setStatusTip ( "Bloquear Configuraciones" );
    accion->setWhatsThis ( "Bloquear Configuraciones" );

    /// Creamos el men&uacute;.
    QAction *accion1 = new QAction ( "&Desbloquear Configuraciones", 0 );
    accion1->setStatusTip ( "Desbloquear Configuraciones" );
    accion1->setWhatsThis ( "Desbloquear Configuraciones" );

    mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot() ) );
    mcont->connect ( accion1, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );

    /// Miramos si existe un menu Herramientas
	QMenu *pPluginMenu = bges->newMenu("&Herramientas", "menuHerramientas", "menuAcerca_de");

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    pPluginMenu->addAction ( accion1 );


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


///
/**
\param sub
\return
**/
int BlSubForm_BlSubForm_Post ( BlSubForm *sub )
{
    _depura ( "BlSubForm_BlSubForm_Post", 0 );
    MyPluginBloqConf *subformods = new MyPluginBloqConf (  );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    _depura ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}

