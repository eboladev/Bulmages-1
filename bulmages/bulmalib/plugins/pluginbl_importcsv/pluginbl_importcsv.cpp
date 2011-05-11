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


#include <QAction>
#include <QMessageBox>
#include <QStringList>
#include <QWidget>
#include <QIcon>
#include <QApplication>
#include <QObject>
#include <QProcess>
#include <QMenu>

#include "local_blI18n.h"
#include "pluginbl_importcsv.h"
#include "plblimportcsv.h"
#include "importcsv.h"
#include "blmaincompany.h"


QMainWindow *g_bges_importcsv = NULL;
BlMainCompany *g_emp_importcsv = NULL;


///
/**
**/
PluginBl_ImportCSV::PluginBl_ImportCSV()
{
    blDebug ( "PluginBl_ImportCSV::PluginBl_ImportCSV", 0 );
    blDebug ( "END PluginBl_ImportCSV::PluginBl_ImportCSV", 0 );
}

///
/**
**/
PluginBl_ImportCSV::~PluginBl_ImportCSV()
{
    blDebug ( "PluginBl_ImportCSV::~PluginBl_ImportCSV", 0 );
    blDebug ( "END PluginBl_ImportCSV::~PluginBl_ImportCSV", 0 );
}


///
/**
**/
void PluginBl_ImportCSV::elslot()
{
    blDebug ( "PluginBl_ImportCSV::elslot", 0 );

    ImportCSV *imp = new ImportCSV ( g_emp_importcsv, 0 );
    g_emp_importcsv->pWorkspace() ->addSubWindow ( imp );
    imp->show();

    blDebug ( "END PluginBl_ImportCSV::elslot", 0 );
}




int entryPoint ( BlMainWindow *bges )
{
    blDebug ( "Entrada del plugin ImportCSV", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_importcsv", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_bges_importcsv = bges;

    PluginBl_ImportCSV *mcont = new PluginBl_ImportCSV;

    /// Creamos el men&uacute;.
    QAction *accion = new QAction ( _ ( "&Importacion CSV" ), 0 );
    accion->setStatusTip ( _ ( "Importacion CSV" ) );
    accion->setWhatsThis ( _ ( "Importacion CSV" ) );

    mcont->connect ( accion, SIGNAL ( activated() ), mcont, SLOT ( elslot() ) );
    
    /// Miramos si existe un menu Herramientas
    QMenu *pPluginMenu = bges->newMenu ( _("&Herramientas"), "menuHerramientas", "menuAcerca_de" );

    pPluginMenu->addSeparator();
    pPluginMenu->addAction ( accion );


    blDebug ( "Iniciado correctamente el plugin imoprtcsv", 10 );
    return 0;
}




int BfCompany_createMainWindows_Post ( BfCompany *cmp )
{
    g_emp_importcsv = cmp;
    return 0;
}

int BcCompany_createMainWindows_Post ( BcCompany *cmp )
{
    g_emp_importcsv = cmp;
    return 0;
}



