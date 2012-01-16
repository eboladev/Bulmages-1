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

#include "pluginbf_asociacion.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "jdirectivalist.h"
#include "jdirectivaview.h"
#include "convreunionlist.h"
#include "convreunionview.h"
#include "comisioneslist.h"
#include "comisionview.h"


BfBulmaFact *g_pluginbf_asociacion = NULL;

///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_asociacion", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_asociacion = bges;

    if ( bges->company()->hasTablePrivilege ( "reunion", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Asociacion"), "menuAsociacion", "menuMaestro" );
        pPluginMenu->addSeparator();



        BlAction *accionA = new BlAction ( _ ( "&Juntas directivas" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/jdirectiva-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Juntas directivas" ) );
        accionA->setWhatsThis ( _ ( "Juntas directivas" ) );
        accionA->setObjectName("mui_actionJuntasDirectivas");
        pPluginMenu->addAction ( accionA );
        bges->Fichas->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Convocar junta directiva" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/jdirectiva-new.png" ) ) );
        accionB->setStatusTip ( _ ( "Convocar junta directiva" ) );
        accionB->setWhatsThis ( _ ( "Convocar junta directiva" ) );
        accionB->setObjectName("mui_actionJuntaDirectivaConvocar");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );
        
        pPluginMenu->addSeparator();
        BlAction *accionC = new BlAction ( _ ( "&Reuniones" ), 0 );
        accionC->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/reunion-list.png" ) ) );
        accionC->setStatusTip ( _ ( "Reuniones" ) );
        accionC->setWhatsThis ( _ ( "Reuniones" ) );
        accionC->setObjectName("mui_actionReunion");
        pPluginMenu->addAction ( accionC );
        bges->Fichas->addAction ( accionC );
        
        BlAction *accionD = new BlAction ( _ ( "&Convocar reunion" ), 0 );
        accionD->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/reunion-new.png" ) ) );
        accionD->setStatusTip ( _ ( "Convocar reunion" ) );
        accionD->setWhatsThis ( _ ( "Convocar reunion" ) );
        accionD->setObjectName("mui_actionReunionConvocar");
        pPluginMenu->addAction ( accionD );
        bges->Fichas->addAction ( accionD );

        BlAction *accionE = new BlAction ( _ ( "&Comisiones" ), 0 );
        accionE->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/comision-list.png" ) ) );
        accionE->setStatusTip ( _ ( "Comisiones" ) );
        accionE->setWhatsThis ( _ ( "Comisiones" ) );
        accionE->setObjectName("mui_actionComisiones");
        pPluginMenu->addAction ( accionE );
        bges->Fichas->addAction ( accionE );
                
        BlAction *accionF = new BlAction ( _ ( "&Convocar comision" ), 0 );
        accionF->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/comision-new.png" ) ) );
        accionF->setStatusTip ( _ ( "Convocar comision" ) );
        accionF->setWhatsThis ( _ ( "Convocar comision" ) );
        accionF->setObjectName("mui_actionComisionConvocar");
        pPluginMenu->addAction ( accionF );
        bges->Fichas->addAction ( accionF );
        
        pPluginMenu->addSeparator();        

    } // end if

    return 0;
}

int BlAction_actionTriggered(BlAction *accion)
{
   BL_FUNC_DEBUG
   if (accion->objectName() == "mui_actionJuntasDirectivas") {
       JDirectivaList * bud = new JDirectivaList ( g_pluginbf_asociacion->company(), NULL );
       g_pluginbf_asociacion->company()->m_pWorkspace->addSubWindow ( bud );
       bud->show();
    }// end if

   if (accion->objectName() == "mui_actionJuntaDirectivaConvocar") {
       JDirectivaView *bud = new JDirectivaView ( g_pluginbf_asociacion->company(), 0 );
       g_pluginbf_asociacion->company()->m_pWorkspace->addSubWindow ( bud );
       bud->show();
       bud->pintar();
    }// end if

   if (accion->objectName() == "mui_actionReunion") {
       ConvReunionList * bud = new ConvReunionList ( g_pluginbf_asociacion->company(), NULL );
       g_pluginbf_asociacion->company() ->m_pWorkspace->addSubWindow ( bud );
       bud->show();
    }// end if

   if (accion->objectName() == "mui_actionReunionConvocar") {
       ConvReunionView *bud = new ConvReunionView ( g_pluginbf_asociacion->company(), 0 );
       g_pluginbf_asociacion->company() ->m_pWorkspace->addSubWindow ( bud );
       bud->show();
       bud->pintar();
    }// end if

    if (accion->objectName() == "mui_actionComisiones") {
        ComisionesList * bud = new ComisionesList ( g_pluginbf_asociacion->company(), NULL );
        g_pluginbf_asociacion->company() ->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    }// end if

    if (accion->objectName() == "mui_actionComisionConvocar") {
        ComisionView *bud = new ComisionView ( g_pluginbf_asociacion->company(), 0 );
        g_pluginbf_asociacion->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
        bud->pintar();
    }// end if
    
    return 0;
}

int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    return 0;
}
