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
#include <QToolButton>
#include <QPushButton>

#include "local_BlI18n.h"
#include "pluginbl_attachdocument.h"
#include "blform.h"
#include "blmaincompany.h"
#include "archmenu.h"


int entryPoint ( QMainWindow *bcont )
{
    _depura ( "Entrada del plugin Bloqueos", 0 );
    _depura ( "Iniciado correctamente el plugin Bloqueos", 10 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbl_attachdocument", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    return ( 0 );
}

int BlForm_cargar ( BlForm *ficha )
{
    _depura ( "PluginBl_AttachDocument::BlForm_cargar", 0 );
    return 0;
    _depura ( "END PluginBl_AttachDocument::BlForm_cargar", 0 );
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
    _depura ( "PluginBl_AttacgDocument::BlForm_BlForm", 0 );

    ArchMenu *mui_imp = new ArchMenu ( l );

    _depura ( "END PluginBl_AttachDocument::BlForm_BlForm", 0 );
    return 0;
}
