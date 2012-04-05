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
#include <QFile>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>
#include <QHBoxLayout>
#include <QDesktopServices>

#include "local_blI18n.h"
#include "pluginbl_attachdocument.h"
#include "blform.h"

#include "archmenu.h"
#include "bltoolbutton.h"
#include "blaction.h"
#include "archivo.h"

int entryPoint ( QMainWindow *bcont )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbl_attachdocument", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );


    
    return ( 0 );
}


int BlMainCompany_init(BlMainCompany * main) {
    BL_FUNC_DEBUG
    main->dbPatchVersionCheck("PluginBl_AttachDocument", "0.11.1-0001");
    return 0;
}



int BlForm_load ( BlForm *ficha )
{
    BL_FUNC_DEBUG
    ADQToolButton *eq = ficha->findChild<ADQToolButton *>("BotonArchDoc");
    
    /// Comprueba la existencia del objeto.
    if (eq != 0) {
        eq->hazMenu();
    } // end if
    
    return 0;
    
}


int BlForm_DesFicha ( BlForm *ficha )
{
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    BL_FUNC_DEBUG
    return 0;
}



///
/**
\param l
\return
**/
int BlForm_loadSpecs ( BlForm *l )
{
    BL_FUNC_DEBUG
    new ADQToolButton ( l );
    
    return 0;
}



