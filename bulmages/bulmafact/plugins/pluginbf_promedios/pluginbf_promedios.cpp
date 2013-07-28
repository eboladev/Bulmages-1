/***************************************************************************
 *   Copyright (C) 2005 by Tomeu Borras Riera                              *
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

#include <QtCore/QObject>
#include "pluginbf_promedios.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "promedioview.h"

///
/**
\return
**/
int entryPoint ( BfBulmaFact * )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_promedios", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );

    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_ArticuloView ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    PromedioView *l = new PromedioView ( art->mainCompany(), art );
    l->setObjectName ( QString::fromUtf8 ( "lpromedios" ) );
    art->mui_tab->addTab ( l, "Promedios" );
    
    return 0;
}


///
/**
\param art
\return
**/
int ArticuloView_load ( ArticuloView *art )
{
    BL_FUNC_DEBUG
    PromedioView *l = art->findChild<PromedioView *> ( "lpromedios" );
    l->load ( art->dbValue ( "idarticulo" ) );
    
    return 0;
}



