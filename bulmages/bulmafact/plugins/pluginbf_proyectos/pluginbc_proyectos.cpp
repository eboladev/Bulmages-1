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

#include "pluginbc_proyectos.h"
#include "blformlist.h"
#include "listproyectosview.h"

BcBulmaCont *g_pluginbc_proyectos = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BcBulmaCont *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBc_Proyectos", "0.10.1-0001");

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbc_proyectos", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    g_pluginbc_proyectos = bges;
    BlAction *accionA = new BlAction ( _ ( "&Proyectos" ), 0 );
    accionA->setStatusTip ( _ ( "Proyectos" ) );
    accionA->setWhatsThis ( _ ( "Proyectos" ) );
    accionA->setObjectName("mui_actionProyectos");
    bges->menuMaestro->insertAction ( bges->actionPaises, accionA );

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionProyectos") {
        /// Agregamos el subformulario de proyectos.
        ListProyectosView *l = new ListProyectosView ( g_pluginbc_proyectos->company(), 0, 0, BL_EDIT_MODE );
        l->setObjectName ( QString::fromUtf8 ( "mui_proyectos" ) );
        //m_bulmacont->company()->insertWindow(l);
        g_pluginbc_proyectos->workspace() ->addSubWindow ( l );
        l->show();    
    } // end if
    return 0;
}

///
/**
\param ccoste
\return
**/
int BcCentroCosteView_BcCentroCosteView ( BcCentroCosteView *ccoste )
{
    BL_FUNC_DEBUG

    /// Agregamos el subformulario de proyectos.
    ListProyectosView *l = new ListProyectosView ( ccoste->mainCompany(), ccoste, 0, BL_SELECT_MODE );
    l->setObjectName ( QString::fromUtf8 ( "mui_proyectos" ) );
    l->mui_actualizar->setDisabled ( TRUE );
//    l->setEditMode();
    ccoste->tabWidget->addTab ( l, _ ( "Proyectos asociados" ) );
    return 0;
}


///
/**
\param ficha
\return
**/
int BlForm_load ( BlForm *ficha )
{
    ListProyectosView * l = ficha->findChild<ListProyectosView *> ( "mui_proyectos" );
    if ( l != 0)
        l->mui_listado->load ( "SELECT * FROM presupuestoc WHERE idc_coste = " + ficha->dbValue ( "idc_coste" ) );
    return 0;
}

