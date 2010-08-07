/***************************************************************************
 *   Copyright (C) 2010 by Fco. Javier M. C.                               *
 *   comercial@todo-redes.com                                              *
 *   http://www.todo-redes.com                                             *
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

#include "pluginbt_serverlight.h"
#include "btcompany.h"
#include "serverlight.h"

ServerLight *g_sl = NULL;

///
/**
\return
**/
int entryPoint ( BtBulmaTPV *tpv )
{
    _depura ( "pluginbt_ServerLight::entryPoint", 0 );

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale(LC_ALL, "");
    bindtextdomain ("pluginbt_ServerLight", g_confpr->valor(CONF_DIR_TRADUCCION).toAscii().constData());

    _depura ( "END pluginbt_ServerLight::entryPoint", 0 );
    
    return 0;
}


int BtCompany_createMainWindows_Post ( BtCompany *etpv )
{
    _depura ( "pluginbt_ServerLight::BtCompany_createMainWindows_Post", 0 );

    g_sl = new ServerLight ( etpv );

    _depura ( "END pluginbt_ServerLight::BtCompany_createMainWindows_Post", 0 );

    return 0;
}


