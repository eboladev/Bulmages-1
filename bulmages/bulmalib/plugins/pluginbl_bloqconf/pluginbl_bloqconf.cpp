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

#include "pluginbl_bloqconf.h"
#include "plblbloqconf.h"
#include "blworkspace.h"
#include "blform.h"

QMainWindow *g_pluginbl_bloqconf = NULL;
BlMainCompany *g_emp = NULL;


///
/**
**/
PluginBl_BloqConf::PluginBl_BloqConf()
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
PluginBl_BloqConf::~PluginBl_BloqConf()
{
    BL_FUNC_DEBUG
    
}


///
/**
\param menu
**/
void PluginBl_BloqConf::s_pintaMenu ( QMenu *menu )
{
    BL_FUNC_DEBUG
    
    BlSubForm *sub = (BlSubForm *) sender();
    int modo = !sub->editMode();
    
    
    menu->addSeparator();
    QAction * ac = new QAction("Bloqueo Configuracion" , menu);
    ac->setObjectName("actionBloqueoConfiguracion");
    ac -> setCheckable(TRUE);
            QString archivo = g_confpr->value( CONF_DIR_USER ) + sub->fileConfig() + "_" + sub->mainCompany()->dbName() + "_" + QString::number ( modo ) + "_tablecfn.cfn";
            QFile file ( archivo );
            if (file.permissions() & QFile::WriteOwner) {
              ac->setChecked(FALSE);
            } else {
              ac->setChecked(TRUE);
            } // end if
    
    
    menu->addAction ( ac );
    
}


///
/**
\param action
**/
void PluginBl_BloqConf::s_trataMenu ( QAction *action )
{
    BL_FUNC_DEBUG
    if ( action->objectName() ==  "actionBloqueoConfiguracion" ) {
            BlSubForm *sub = (BlSubForm *) sender();
            int modo = !sub->editMode();
            QString archivo = g_confpr->value( CONF_DIR_USER ) + sub->fileConfig() + "_" + sub->mainCompany()->dbName() + "_" + QString::number ( modo ) + "_tablecfn.cfn";
            QFile file ( archivo );
            if (file.permissions() & QFile::WriteOwner) {
              sub->saveConfig();
              QString cad = "chmod a-w " + archivo;
              system ( cad.toAscii().constData() );
            } else {
              sub->saveConfig();
              QString cad = "chmod a+w " + archivo;
              system ( cad.toAscii().constData() );
            } // end if
    } // end if
    
}



int entryPoint ( BlMainWindow *bges )
{
    BL_FUNC_DEBUG

    g_pluginbl_bloqconf = bges;


    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_bloqconf", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );


    PluginBl_BloqConf *mcont = new PluginBl_BloqConf;

    /// Miramos si existe un menu Herramientas

	QMenu *pPluginMenu = bges->newMenu(_("&Herramientas"), "menuHerramientas", "menuAcerca_de");

    pPluginMenu->addSeparator();

    /// Creamos el men&uacute;.
    BlAction *accionA = new BlAction ( _("&Bloquear Configuraciones"), 0 );
    accionA->setStatusTip ( _("Bloquear Configuraciones") );
    accionA->setWhatsThis ( _("Bloquear Configuraciones") );
    accionA->setObjectName("mui_actionBloqConf");
    pPluginMenu->addAction ( accionA );

    /// Creamos el men&uacute;.
    BlAction *accionB = new BlAction ( _("&Desbloquear Configuraciones"), 0 );
    accionB->setStatusTip ( _("Desbloquear Configuraciones") );
    accionB->setWhatsThis ( _("Desbloquear Configuraciones") );
    accionB->setObjectName("mui_actionUnlockConf");
    pPluginMenu->addAction ( accionB );

    /// Creamos el men&uacute;.
    BlAction *accionC = new BlAction ( _("&Borrar Configuraciones"), 0 );
    accionC->setStatusTip ( _("Borrar Configuraciones") );
    accionC->setWhatsThis ( _("Borrar Configuraciones") );
    accionC->setObjectName("mui_actionDeleteConf");
    pPluginMenu->addAction ( accionC );


    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG

    if (accion->objectName() == "mui_actionBloqConf") {
        QString cad = "chmod a-w " + g_confpr->value(CONF_DIR_USER) + "*.cfn";
        system ( cad.toAscii().constData() );
        blMsgInfo("Configuraciones Bloqueadas. Se mantendra la configuracion establecida al cargar el programa. Cualquier configuracion posterior a la carga del programa se perdera.");
    }
    if (accion->objectName() == "mui_actionUnlockConf") {
        QString cad = "chmod a+w " + g_confpr->value(CONF_DIR_USER) + "*.cfn";
        system ( cad.toAscii().constData() );
        blMsgInfo("Configuraciones Desbloqueadas. Las configuraciones se guardaran al cerrar el programa.");
    }
    if (accion->objectName() == "mui_actionDeleteConf") {
        QString cad = "rm" + g_confpr->value(CONF_DIR_USER) + "*.cfn";
        system ( cad.toAscii().constData() );
        blMsgInfo("Configuraciones Borradas. Las configuraciones se guardaran al cerrar el programa.");
    }

    
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
    BL_FUNC_DEBUG
    PluginBl_BloqConf *subformods = new PluginBl_BloqConf (  );
    sub->QObject::connect ( sub, SIGNAL ( pintaMenu ( QMenu * ) ), subformods, SLOT ( s_pintaMenu ( QMenu * ) ) );
    sub->QObject::connect ( sub, SIGNAL ( trataMenu ( QAction * ) ), subformods, SLOT ( s_trataMenu ( QAction * ) ) );
    
    return 0;
}

