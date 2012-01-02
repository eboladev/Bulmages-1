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

#include "pluginbf_proveedor.h"
#include "bfcompany.h"
#include "blfunctions.h"
#include "provedit.h"
#include "providerslist.h"



ProveedorList *g_providersList = NULL;
BfBulmaFact *g_pluginbf_proveedor = NULL;



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
    blBindTextDomain ( "pluginbf_proveedor", g_confpr->value( CONF_DIR_TRADUCCION ).toAscii().constData() );
    g_pluginbf_proveedor = bges;

    if ( bges->company()->hasTablePrivilege ( "proveedor", "SELECT" ) ) {

        /// Miramos si existe un menu Ventas
        QMenu *pPluginMenu = bges->newMenu ( _("&Compras"), "menuCompras", "menuMaestro" );
        pPluginMenu->addSeparator();
        
        /// El men&uacute; de proveedores
        BlAction *accionA = new BlAction ( _ ( "&Proveedores" ), 0 );
        accionA->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier-list.png" ) ) );
        accionA->setStatusTip ( _ ( "Proveedores" ) );
        accionA->setWhatsThis ( _ ( "Proveedores" ) );
        accionA->setObjectName("mui_actionProveedores");
        pPluginMenu->addAction ( accionA );
        bges->Listados->addAction ( accionA );

        BlAction *accionB = new BlAction ( _ ( "&Nuevo proveedor" ), 0 );
        accionB->setIcon ( QIcon ( QString::fromUtf8 ( ":/Images/supplier.png" ) ) );
        accionB->setStatusTip ( _ ( "Nuevo proveedor" ) );
        accionB->setWhatsThis ( _ ( "Nuevo proveedor" ) );
        accionB->setObjectName("mui_actionProveedorNuevo");
        pPluginMenu->addAction ( accionB );
        bges->Fichas->addAction ( accionB );

    } // end if

    return 0;
}


int BlAction_actionTriggered(BlAction *accion) {
    BL_FUNC_DEBUG
    BlDebug::blDebug ( Q_FUNC_INFO, 0, "PluginBf_Proveedor" );
    if (accion->objectName() == "mui_actionProveedores") {
        if ( g_providersList ) {
            g_providersList->hide();
            g_providersList->show();
        } // end if
    } // end if

    if (accion->objectName() == "mui_actionProveedorNuevo") {
        ProveedorView * bud = new ProveedorView ( g_pluginbf_proveedor->company(), NULL );
        g_pluginbf_proveedor->company()->m_pWorkspace->addSubWindow ( bud );
        bud->show();
    } // end if
    return 0;
}


int BfCompany_createMainWindows_Post ( BfCompany *comp )
{
    if ( comp->hasTablePrivilege ( "proveedor", "SELECT" ) ) {
        g_providersList = new ProveedorList ( comp, NULL );
        comp->m_pWorkspace->addSubWindow ( g_providersList );
        g_providersList->hide();
    } // end if
    return 0;
}


int Busqueda_on_mui_buscar_clicked ( BlSearchWidget *busq )
{
    if ( busq->tableName() == "proveedor" ) {


        QDialog *diag = new QDialog ( 0 );
        diag->setModal ( true );
        diag->setGeometry ( QRect ( 0, 0, 750, 550 ) );
        blCenterOnScreen ( diag );

        ProveedorList *clients = new ProveedorList ( ( BfCompany * ) busq->mainCompany(), diag, 0, BL_SELECT_MODE );
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
        if ( clients->idprovider() != "" ) {
            busq->setId ( clients->idprovider() );
        } // end if
        delete diag;


        return 1;
    } // end if
    return 0;

}


/// Esta llamada de plugin es bastante novedosa ya es una llamada que no responde a una funcion
/// Sino que se llama desde multiples partes del sistema.
int SNewProveedorView ( BfCompany *v )
{
    ProveedorView *h = new ProveedorView ( v, 0 );
    g_plugParams = h;
    return 1;
}
