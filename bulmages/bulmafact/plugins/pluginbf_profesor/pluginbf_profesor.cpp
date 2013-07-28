/***************************************************************************
 *   Copyright (C) 2009 by Tomeu Borras Riera                              *
 *   tborras@conetxia.com                                                  *
 *                                                                         *
 *   Copyright (C) 2009 by Arturo Martin Llado                             *
 *   amartin@conetxia.com                                                  *
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

#include "pluginbf_profesor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "profesorview.h"
#include "profesoreslist.h"
#include "aboutfapacview.h"

ProfesoresList *g_profesoresList = NULL;
BfBulmaFact *g_pluginbf_profesor = NULL;


///
/**
\param bges
\return
**/
int entryPoint ( BfBulmaFact *bges )
{
    BL_FUNC_DEBUG

    /// El plugin necesita un parche en la base de datos para funcionar.
    bges->company()->dbPatchVersionCheck("PluginBf_Profesor", "0.12.1-0003");
    
    /// Inicializa el sistema de traducciones 'gettext'.
    setlocale ( LC_ALL, "" );
    blBindTextDomain ( "pluginbf_profesor", g_confpr->value( CONF_DIR_TRADUCCION ).toLatin1().constData() );
    g_pluginbf_profesor = bges;

    if ( bges->company()->hasTablePrivilege ( "profesor", "SELECT" ) ) {

        /// Miramos si existe un menu Docencia
        QMenu *pPluginMenu = bges->newMenu ( _("&Activitats"), "menuActivitats", "menuMaestro" );

        /// Agregamos un separador
        pPluginMenu->addSeparator();

        /// El men&uacute; de Tarifas en la secci&oacute;n de art&iacute;culos.
        BlAction *accionA = new BlAction ( _ ( "&Profesores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/profesor.png" ) ) );
        accionA->setStatusTip ( _ ( "Profesores" ) );
        accionA->setWhatsThis ( _ ( "Profesores" ) );
        accionA->setObjectName("mui_actionProfesores");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo profesor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/ImgGestionAula/icons/profesor_add.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo profesor" ) );
        accionB->setWhatsThis ( _ ( "Nuevo profesor" ) );
        accionB->setObjectName("mui_actionProfesorNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );

        BlAction *accionC = new BlAction ( _ ( "About FAPAC" ), 0 );
        accionC->setStatusTip ( _ ( "About FAPAC" ) );
        accionC->setWhatsThis ( _ ( "About FAPAC" ) );
        accionC->setObjectName("mui_actionAboutFapac");
        bges->menuAcerca_de->addAction ( accionC );

    } // end if

    
    
    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    if (accion->objectName() == "mui_actionProfesores") {
        if ( g_profesoresList ) {
            g_profesoresList->hide();
            g_profesoresList->show();
        } // end if
    } // end if
    if (accion->objectName() == "mui_actionProfesorNuevo") {
        ProfesorView * bud = new ProfesorView ( g_pluginbf_profesor->company(), NULL );
        g_pluginbf_profesor->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    if (accion->objectName() == "mui_actionAboutFapac") {
        AboutFapacView *afv = new AboutFapacView();
        afv->show();
    } // end if
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    BL_FUNC_DEBUG

    if ( comp->hasTablePrivilege ( "profesor", "SELECT" ) ) {
        g_profesoresList = new ProfesoresList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_profesoresList );
        g_profesoresList->hide();
    } // end if
    
    return 0;
}

int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    BL_FUNC_DEBUG

    if ( busq->tableName() == "profesor" ) {

        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        ProfesoresList *clients = new ProfesoresList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
        busq->connect ( clients, SIGNAL ( selected ( QString ) ), diag, SLOT ( accept() ) );

        /// Creamos un layout donde estara el contenido de la ventana y la ajustamos al QDialog
        /// para que sea redimensionable y aparezca el titulo de la ventana.
        QHBoxLayout *layout = new QHBoxLayout;
        layout->addWidget ( clients );
        layout->setMargin ( 0 );
        layout->setSpacing ( 0 );
        diag->setLayout ( layout );
        diag->setWindowTitle ( clients->windowTitle() );

        diag->exec();
        
        if ( clients->idprofesor() != "" ) {
            busq->setId ( clients->idprofesor() );
        } // end if
        
        delete diag;

        return 1;
        
    } // end if
    
    
    
    return 0;

}
