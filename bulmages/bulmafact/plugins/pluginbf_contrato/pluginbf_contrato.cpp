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

#include <QAction>
#include <QObject>
#include <QMessageBox>
#include <QLineEdit>
#include <QTabWidget>

#include "bfcompany.h"
#include "pluginbf_contrato.h"
#include "blfunctions.h"
#include "contratoslist.h"
#include "blform.h"

BfBulmaFact *g_pluginbf_contrato = NULL;




///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Contrato", "0.11.1-0001");

    g_pluginbf_contrato = bges;
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_contrato", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );

    
    /// Miramos si existe un menu Ventas
    QMenu *pPluginMenu = bges->newMenu ( _("&Ventas"), "menuVentas", "menuMaestro" );

    BlAction *accion = new BlAction ( _("&Contratos"), 0 );
    accion->setStatusTip ( _("Listado de contratos") );
    accion->setWhatsThis ( _("Listado de contratos") );
    accion->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/contract-list.png" ) ) );
    accion->setObjectName("mui_actionListadoContratos");

    pPluginMenu->addAction(accion);
    
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    if (accion->objectName() == "mui_actionListadoContratos") {
	ContratosList *contratoslist = new ContratosList ( g_pluginbf_contrato->company() );
	g_pluginbf_contrato->company()->m_pWorkspace->addSubWindow ( contratoslist );
	contratoslist->show();
    } // end if
    
    return 0;
} // end if


///
/**
\param art
\return
**/
int ClienteView_ClienteView ( ClienteView *art )
{
    BL_FUNC_DEBUG
    /// Para que funcione bien debemos iniciar con BL_SELECT_MODE y luego pasar a BL_EDIT_MODE ya que si no se hace un insertWindow y no es deseable.
    ContratosList *l = new ContratosList ( ( ( BfCompany * ) art->mainCompany() ), art, 0, BL_SELECT_MODE );
    l->setObjectName ( QString::fromUtf8 ( "ccontratoslist" ) );
    art->mui_tab->addTab ( l, _("Contratos") );
    l->setEditMode();
    
    return 0;
}


///
/**
\param fich
\return
**/
int BlForm_load ( BlForm *fich )
{
    BL_FUNC_DEBUG
    ContratosList *l = fich->findChild<ContratosList *> ( "ccontratoslist" );
    if ( l ) {
        l->mui_idcliente->setId ( fich->dbValue ( "idcliente" ) );
        l->on_mui_actualizar_clicked();
        return 0;
    } // end if
    return 0;
}
