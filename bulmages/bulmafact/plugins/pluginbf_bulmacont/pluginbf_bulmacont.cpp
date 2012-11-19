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

#include <QToolButton>

#include "pluginbf_bulmacont.h"
#include "blfunctions.h"
// #include "bcplancontablelistview.h"
#include "bfcompany.h"


BcCompany *g_empresaactual;

BfBulmaFact *g_pluginbf_bulmacont;

///
/**
\return
**/
int entryPoint ( BfBulmaFact *bf )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_bulmacont", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_pluginbf_bulmacont = bf;
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{

/*
    g_empresaactual = new BcCompany ( NULL );
    g_empresaactual->setProgressBar ( g_pluginbf_bulmacont->m_pb );
    g_empresaactual->init ( "csoncatiu", "BulmaCont" );
    g_empresaactual->setWorkspace ( comp->pWorkspace() );
    g_empresaactual->setListVentanas ( comp->listVentanas() );


    BcPlanContableListView *listcuentas = new BcPlanContableListView ( g_empresaactual, 0 );
    listcuentas->inicializa();
    g_pluginbf_bulmacont->workspace()->addSubWindow ( listcuentas );
*/
    return 0;
}

