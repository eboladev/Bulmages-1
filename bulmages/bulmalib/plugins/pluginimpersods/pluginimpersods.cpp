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

#include "pluginimpersods.h"
#include "funcaux.h"
#include "eqtoolbutton.h"


///
/**
\return
**/
int entryPoint ( QApplication * )
{
    _depura ( "entryPoint", 0, "Punto de Entrada del plugin de PluginImPersODS" );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginimpersods", confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    _depura ( "END entryPoint", 0, "Punto de Entrada del plugin de PluginImPersODS" );
    return 0;
}


///
/**
\param l
\return
**/
int BlForm_BlForm ( BlForm *l )
{
    _depura ( "BlForm_BlForm", 0 );

    EQToolButton *mui_imp = new EQToolButton ( l );

    _depura ( "END BlForm_BlForm", 0 );
    return 0;
}



