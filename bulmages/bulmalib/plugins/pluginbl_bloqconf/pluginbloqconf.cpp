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
    blDebug ( "MyPluginBloqConf::MyPluginBloqConf", 0 );
    blDebug ( "END MyPluginBloqConf::MyPluginBloqConf", 0 );
}

///
/**
**/
MyPluginBloqConf::~MyPluginBloqConf()
{
    blDebug ( "MyPluginBloqConf::~MyPluginBloqConf", 0 );
    blDebug ( "END MyPluginBloqConf::~MyPluginBloqConf", 0 );
}




///
/**
**/
void MyPluginBloqConf::elslot( )
{
    blDebug ( "MyPluginBloqConf::elslot", 0 );

    QString cad = "chmod a-w " + g_confpr->valor(CONF_DIR_USER) + "*.cfn";
    system ( cad.toAscii().constData() );
    blMsgInfo("Configuraciones Bloqueadas. Se mantendra la configuracion establecida al cargar el programa. Cualquier configuracion posterior a la carga del programa se perdera.");
    blDebug ( "END MyPluginBloqConf::elslot", 0 );
}

///
/**
**/
void MyPluginBloqConf::elslot1( )
{
    blDebug ( "MyPluginBloqConf::elslot", 0 );

    QString cad = "chmod a+w " + g_confpr->valor(CONF_DIR_USER) + "*.cfn";
    system ( cad.toAscii().constData() );
    blMsgInfo("Configuraciones Desbloqueadas. Las configuraciones se guardaran al cerrar el programa.");

    blDebug ( "END MyPluginBloqConf::elslot", 0 );
}

///
/**
**/
void MyPluginBloqConf::elslot2( )
{
    blDebug ( "MyPluginBloqConf::elslot2", 0 );

    QString cad = "rm" + g_confpr->valor(CONF_DIR_USER) + "*.cfn";
    system ( cad.toAscii().constData() );
    blMsgInfo("Configuraciones Borradas. Las configuraciones se guardaran al cerrar el programa.");

    blDebug ( "END MyPluginBloqConf::elslot2", 0 );
}

///
/**
\param menu
**/
void MyPluginBloqConf::s_pintaMenu ( QMenu *menu )
{
    blDebug ( "MyPluginBloqConf::s_pintaMenu", 0 );
    
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
    blDebug ( "END MyPluginBloqConf::s_pintaMenu", 0 );
}


///
/**
\param action
**/
void MyPluginBloqConf::s_trataMenu ( QAction *action )
{
    blDebug ( "MyPluginBloqConf::s_trataMenu", 0 );
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
    blDebug ( "END MyPluginBloqConf::s_trataMenu", 0 );
}



int entryPoint ( BlMainWindow *bges )
{
    blDebug ( "Entrada del plugin Bloqconf", 0 );

    g_bges = bges;


    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_bloqconf", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );


    MyPluginBloqConf *mcont = new MyPluginBloqConf;



    /// Creamos el men&uacute;.
    QAction *accion = new QAction ( _("&Bloquear Configuraciones"), 0 );
    accion->setStatusTip ( _("Bloquear Configuraciones") );
    accion->setWhatsThis ( _("Bloquear Configuraciones") );

    /// Creamos el men&uacute;.
    QAction *accion1 = new QAction ( _("&Desbloquear Configuraciones"), 0 );
    accion1->setStatusTip ( _("Desbloquear Configuraciones") );
    accion1->setWhatsThis ( _("Desbloquear Configuraciones") );


    /// Creamos el men&uacute;.
    QAction *accion2 = new QAction ( _("&Borrar Configuraciones"), 0 );
    accion2->setStatusTip ( _("Borrar Configuraciones") );
    accion2->setWhatsThis ( _("Borrar Configuraciones") );


    mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot() ) );
    mcont->connect ( accion1, SIGNAL ( activated() ), mcont, SLOT ( elslot1() ) );
    mcont->connect ( accion2, SIGNAL ( activated() ), mcont, SLOT ( elslot2() ) );

    /// Miramos si existe un menu Herramientas
	QMenu *pPluginMenu = bges->newMenu(_("&Herramientas"), "menuHerramientas", "menuAcerca_de");

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );
    pPluginMenu->addAction ( accion1 );
    pPluginMenu->addAction ( accion2 );

    blDebug ( "Iniciado correctamente el plugin dock", 10 );
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
    blDebug ( "BlSubForm_BlSubForm_Post", 0 );
    MyPluginBloqConf *subformods = new MyPluginBloqConf (  );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    blDebug ( "END BlSubForm_BlSubForm_Post", 0 );
    return 0;
}

