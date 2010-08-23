/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *   Copyright (C) 2006 by Fco. Javier M. C.                               *
 *   fcojavmc@todo-redes.com                                               *
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

#include "pluginbl_template2mail.h"
#include "eqtoolbutton.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    blDebug ( "entryPoint", 0, "Punto de Entrada del plugin de PluginImPersMail" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    bindtextdomain ( "pluginbl_template2mail", g_confpr->valor ( CONF_DIR_TRADUCCION ).toAscii().constData() );

    blDebug ( "END entryPoint", 0, "Punto de Entrada del plugin de PluginImPersMail" );
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    blDebug ( "BlForm_BlForm", 0 );
    new EQToolButtonMail ( l );
    blDebug ( "END BlForm_BlForm", 0 );
    return 0;
}



