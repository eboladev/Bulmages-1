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


#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QWidget>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtCore/QObject>
#include <QtCore/QProcess>
#include <QtWidgets/QMenu>

#include "local_blI18n.h"
#include "pluginbl_importcsv.h"
#include "plblimportcsv.h"
#include "importcsv.h"
#include "blmaincompany.h"


QMainWindow *g_pluginbl_importcsv = NULL;
BlMainCompany *g_pluginbl_importcsv_emp = NULL;


///
/**
**/
PluginBl_ImportCSV::PluginBl_ImportCSV()
{
    BL_FUNC_DEBUG
    
}

///
/**
**/
PluginBl_ImportCSV::~PluginBl_ImportCSV()
{
    BL_FUNC_DEBUG
    
}


///
/**
**/
void PluginBl_ImportCSV::elslot()
{
    BL_FUNC_DEBUG

    ImportCSV *imp = new ImportCSV ( g_pluginbl_importcsv_emp, 0 );
    g_pluginbl_importcsv_emp->pWorkspace() ->addSubWindow ( imp );
    imp->show();

    
}




int entryPoint ( BlMainWindow *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_importcsv", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    g_pluginbl_importcsv = bges;

    PluginBl_ImportCSV *mcont = new PluginBl_ImportCSV;

    /// Creamos el men&uacute;.
    QAction *accion = new QAction ( _ ( "&Importacion CSV" ), 0 );
    accion->setStatusTip ( _ ( "Importacion CSV" ) );
    accion->setWhatsThis ( _ ( "Importacion CSV" ) );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/document-import.png" ) ) );
	
    mcont->connect ( accion, SIGNAL ( triggered(bool) ), mcont, SLOT ( elslot() ) );
    
    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu ( _("&Herramientas"), "menuHerramientas", "menuAcerca_de" );

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );


    
    return 0;
}




int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    g_pluginbl_importcsv_emp = cmp;
    return 0;
}

int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_pluginbl_importcsv_emp = cmp;
    return 0;
}



