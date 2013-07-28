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
#include <QtWidgets/QToolButton>

#include "pluginbf_almacenamplia.h"
#include "bfcompany.h"
#include "almaceneslistview.h"


///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_almacenamplia", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}


///
/**
\param bges
\return
**/
int BfCompany_s_almacenes ( BfCompany *bges )
{
    BL_FUNC_DEBUG
    AlmacenesListView *lser = new AlmacenesListView ( bges, 0 );
    bges->pWorkspace() ->addSubWindow ( lser );
    lser->show();
    
    /// Este plugin reemplaza completamente la funcion llamante y por eso devuelve un valor distinto de 0
    return 1;
}

